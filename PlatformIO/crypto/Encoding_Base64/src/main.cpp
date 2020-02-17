#include <Arduino.h>
extern "C" {
    #include "crypto/base64.h"
}


void encode();
void decode();

void setup() 
{
    Serial.begin(115200);

    decode();

    while(1);
}

void loop() 
{

}

void encode()
{
    char * toEncode = "Hello World";
    size_t outputLength;
 
    unsigned char * encoded = base64_encode((const unsigned char *)toEncode, strlen(toEncode), &outputLength);
 
    Serial.print("Length of encoded message: ");
    Serial.println(outputLength);
 
    Serial.printf("%.*s", outputLength, encoded);
    free(encoded);   
}

void decode()
{
    char * toDecode = "3s66j34j8RmrHmfz3IfXOgVA6adRMh/9jNr67TLnw6rLMVR1Bm0YxyG/n01a7IfV";
    size_t outputLength;
 
    unsigned char * decoded = base64_decode((const unsigned char *)toDecode, strlen(toDecode), &outputLength);
 
    Serial.print("Length of decoded message: ");
    Serial.println(outputLength);
 
    Serial.print("Bytes: ");

    for (int i = 0 ; i < outputLength ; i++)
    {
        Serial.printf("%02X ", decoded[i]);
    }

    free(decoded);
}