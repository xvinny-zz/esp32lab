#include <Arduino.h>
#include <string.h>
#include "espcurl.h"

#define SIMULATE_FS "simulate"

void testGET();
void print_response(char *hd, char *bdy, int res);
int check_file(char *fname);

char Get_testURL[] = "http://loboris.eu/ESP32/test.php?par1=765&par2=test";
char Get_file_testURL[] = "http://loboris.eu/ESP32/tiger.bmp";
char Get_bigfile_testURL[] = "http://loboris.eu/ESP32/bigimg.jpg";
static uint8_t print_header = 0;
static uint8_t print_body = 0;
static int last_error = 0;
static int num_errors = 0;
static uint8_t has_fs = 0;

void setup()
{
    // put your setup code here, to run once:
}

void loop()
{
    // put your main code here, to run repeatedly:
}

void testGET()
{
    int res = 0;
    int exists = 0;
    uint16_t tmo = 0;
    uint32_t maxb = 0;

    char *hdrbuf = (char *)calloc(1024, 1);
    assert(hdrbuf);

    char *bodybuf = (char *)calloc(4096, 1);
    assert(bodybuf);

    printf("\r\n\r\n#### HTTP GET\r\n");
    printf("     Send GET request with parameters\r\n");

    res = Curl_GET(Get_testURL, NULL, hdrbuf, bodybuf, 1024, 4096);

    print_response(hdrbuf, bodybuf, res);
    if (res)
        goto exit;

    vTaskDelay(2000 / portTICK_RATE_MS);

#ifndef USE_GSM
    printf("\r\n\r\n#### HTTP GET FILE\r\n");
    printf("     Get file (~225 KB) from http server");

    exists = check_file("/spiflash/tiger.bmp");
    if (exists == 0)
    {
        printf(" and save it to file system\r\n");
        res = Curl_GET(Get_file_testURL, "/spiflash/tiger.bmp", hdrbuf, bodybuf, 1024, 4096);
    }
    else
    {
        printf(", simulate saving to file system\r\n");
        res = Curl_GET(Get_file_testURL, SIMULATE_FS, hdrbuf, bodybuf, 1024, 4096);
    }

    print_response(hdrbuf, bodybuf, res);
    if (res)
        goto exit;
    vTaskDelay(1000 / portTICK_RATE_MS);

    printf("\r\n\r\n#### HTTP GET BIG FILE\r\n");
    printf("     Get big file (~2.4 MB), simulate saving to file system\r\n");

    tmo = curl_timeout;
    maxb = curl_maxbytes;

    curl_timeout = 90;
    curl_maxbytes = 3000000;

    res = Curl_GET(Get_bigfile_testURL, SIMULATE_FS, hdrbuf, bodybuf, 1024, 4096);
    print_response(hdrbuf, bodybuf, res);

    curl_timeout = tmo;
    curl_maxbytes = maxb;
#endif

exit:
    free(bodybuf);
    free(hdrbuf);
    vTaskDelay(1000 / portTICK_RATE_MS);
}

void print_response(char *hd, char *bdy, int res)
{
    if (res)
    {
        printf("     ERROR: %d [%s]\r\n", res, bdy);
    }
    else
    {
        if (print_header)
        {
            printf("\r\n____________ Response HEADER: ____________\r\n%s\r\n^^^^^^^^^^^^ Response HEADER: ^^^^^^^^^^^^\r\n", hd);
        }
        if (print_body)
        {
            printf("\r\n____________ Response BODY: ____________\r\n%s\r\n^^^^^^^^^^^^ Response BODY: ^^^^^^^^^^^^\r\n", bdy);
        }
        if ((!print_header) && (!print_body))
            printf("     OK.");
    }
    last_error = res;
    if (res)
        num_errors++;
    else
        num_errors = 0;
}

int check_file(char *fname)
{
    if (!has_fs)
        return -1;

    struct stat sb;

    if ((stat(fname, &sb) == 0) && (sb.st_size > 0))
        return 1;

    return 0;
}