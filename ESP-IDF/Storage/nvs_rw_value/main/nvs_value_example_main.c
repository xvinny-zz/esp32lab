/* Non-Volatile Storage (NVS) Read and Write a Value - Example

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

void app_main()
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    // Open
    printf("\nOpening Non-Volatile Storage (NVS) handle... ");
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    nvs_handle my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        printf("\nError (%d) opening NVS handle!", err);
    }
    else
    {
        printf("Done\n");

        // Read
        printf("\nReading restart counter from NVS ... ");
        int32_t restart_counter = 0; // value will default to 0, if not set yet in NVS
        err = nvs_get_i32(my_handle, "restart_counter", &restart_counter);
        switch (err) {
            case ESP_OK:
                printf("\nDone");
                printf("\nRestart counter = %d\n", restart_counter);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("\nThe value is not initialized yet!");
                break;
            default :
                printf("\nError (%d) reading!", err);
        }

        // Write
        printf("\nUpdating restart counter in NVS ... ");
        restart_counter++;
        err = nvs_set_i32(my_handle, "restart_counter", restart_counter);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");


        printf("\nReading WIFI SSID..." );
        size_t ssidSize = 0; // value will default to 0, if not set yet in NVS
        err = nvs_get_str( my_handle, "ssid", NULL, &ssidSize );
        switch (err)
        {
			case ESP_OK:
				printf("\nDone");
				printf("\nSSID size = %d\n", ssidSize );
				break;
			case ESP_ERR_NVS_NOT_FOUND:
				printf("\nThe value is not initialized yet!");
				break;
			default :
				printf("\nError (%d) reading!", err);
        }

        if ( ssidSize > 0 )
        {
        	char * ssid = malloc( ssidSize );
        	nvs_get_str( my_handle, "ssid", ssid, &ssidSize );
        	printf("\nSSID= %s\n", ssid );
        }
        else
        {
        	printf( "\nSSID not found.. initializing.. " );
        	char * ssid = "qflux";
        	nvs_set_str( my_handle , "ssid", ssid );
        }

        printf("\nReading WIFI pwd..." );
		size_t ssidPwdSize = 0; // value will default to 0, if not set yet in NVS
		err = nvs_get_str( my_handle, "ssidpwd", NULL, &ssidPwdSize );
		switch (err)
		{
			case ESP_OK:
				printf("\nDone");
				printf("\nSSID PWD size = %d\n", ssidPwdSize );
				break;
			case ESP_ERR_NVS_NOT_FOUND:
				printf("\nThe value is not initialized yet!");
				break;
			default :
				printf("\nError (%d) reading!", err);
		}

		if ( ssidPwdSize > 0 )
		{
			char * ssidpwd = malloc( ssidPwdSize );
			nvs_get_str( my_handle, "ssidpwd", ssidpwd, &ssidPwdSize );
			printf("\nSSID PWD= %s\n", ssidpwd );
		}
		else
		{
			printf( "\nSSID PWD not found.. initializing.. " );
			char * ssidpwd = "33285901ctba";
			nvs_set_str( my_handle , "ssidpwd", ssidpwd );
		}

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        printf("\nCommitting updates in NVS ... ");
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "\nFailed!" : "\nDone");

        // Close
        nvs_close(my_handle);
    }

    printf("\n");

    // Restart module
    for (int i = 10; i >= 0; i--) {
        printf("\nRestarting in %d seconds...", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("\nRestarting now.");
    fflush(stdout);
    esp_restart();
}
