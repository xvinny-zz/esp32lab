#include <Arduino.h>
#include "mbedtls/aes.h"

void encrypt(char * plainText, char * key, unsigned char * outputBuffer)
{ 
    mbedtls_aes_context aes;

    mbedtls_aes_init( &aes );
    //mbedtls_aes_setkey_enc( &aes, (const unsigned char*) key, strlen(key) * 8 );
    mbedtls_aes_setkey_enc( &aes, (const unsigned char*) key, 32 * 8 );
    mbedtls_aes_crypt_ecb( &aes, MBEDTLS_AES_ENCRYPT, (const unsigned char*)plainText, outputBuffer);
    mbedtls_aes_free( &aes );
}
 
void decrypt(unsigned char * chipherText, char * key, unsigned char * outputBuffer)
{
    mbedtls_aes_context aes;

    mbedtls_aes_init( &aes );
    //mbedtls_aes_setkey_dec( &aes, (const unsigned char*) key, strlen(key) * 8 );
    mbedtls_aes_setkey_dec( &aes, (const unsigned char*) key, 32 * 8 );
    mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_DECRYPT, (const unsigned char*)chipherText, outputBuffer);
    mbedtls_aes_free( &aes );
}

void setup() 
{
    Serial.begin(115200);

    char key[] = {0x96, 0xb8, 0x8c, 0x04, 0x9f, 0x5e, 0x9f, 0xd2, 0xcf, 0xc3, 0xb9, 
                  0xa5, 0x34, 0x4d, 0x9e, 0x3b, 0xd6, 0x34, 0x19, 0x79, 0x9f, 0xc4, 
                  0xa2, 0xbe, 0x7d, 0xc4, 0xe2, 0x78, 0xe2, 0xab, 0x8a, 0x01}; // 256-bit (32 bytes)

    // char input[] = {0xdd, 0x13, 0xcb, 0x62, 0x23, 0xc6, 0x3f, 0x7e, 0x3b, 0xc8, 0x79, 0xd7, 
    //                 0x21, 0xc6, 0x30, 0xff, 0x5a, 0x0a, 0x4a, 0xae, 0xf5, 0x88, 0x18, 0x5f, 
    //                 0xb3, 0x92, 0xb9, 0xa6, 0x1e, 0xb0, 0xf1, 0x34, 0xfa, 0x1c, 0x9d, 0xdd, 
    //                 0xed, 0x0a, 0xd9, 0x20, 0xe7, 0x78, 0xc4, 0xd0, 0x7b, 0xc9, 0x2d, 0x06, 0x00};
    
    //char * key = "abcdefghijklmnop";
 
    String plainText = "#2003466,75,2,103,159.59,-19.749168,-43.487576";
    //String plainText = "1,-19.749168,-43.487576,291,502";
    int blockSize = 16;
    int cipherTextSize = 16;
    int dechiperedTextSize = 16;
    unsigned char cipherTextOutput[cipherTextSize];
    unsigned char decipheredTextOutput[dechiperedTextSize];

    Serial.printf("\nLength: %d", plainText.length());

    for (int i = 0 ; i < plainText.length(); i+=blockSize)
    {
        int endIndex = (plainText.length() - i) < blockSize ? (i+(plainText.length() - i)) : (i + blockSize);
        encrypt((char *)plainText.substring(i, endIndex).c_str(), key, cipherTextOutput);
        decrypt(cipherTextOutput, key, decipheredTextOutput);

        Serial.printf("\n\nOriginal plain text:\n%s (i=%d, endIndex=%d)\n", plainText.substring(i, endIndex).c_str(), i, endIndex);
        
        Serial.println("\nCiphered text:");
        for (int i = 0; i < cipherTextSize; i++) 
        {
            char str[3];

            sprintf(str, "%02x", (int)cipherTextOutput[i]);
            Serial.print(str);
        }

        Serial.println("\n\nDeciphered text:");
        for (int i = 0; i < dechiperedTextSize; i++) 
        {
            Serial.print((char)decipheredTextOutput[i]);
        }
    }
    while(1);
}

void loop() 
{
}