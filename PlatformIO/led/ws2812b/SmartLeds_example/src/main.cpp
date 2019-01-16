#include <Arduino.h>
#include <SmartLeds.h>

// CONSTANTS
const int LED_COUNT = 100;
const gpio_num_t DATA_PIN = GPIO_NUM_33;
const int CHANNEL = 0;
const int DLD_PIN_DIN = 23;
const int DLD_PIN_CS = 5;
const int DLD_PIN_CLK = 18;

// PROTOTIPOS
void showGradient();
void show30(uint8_t intensidade);
void show130(uint8_t intensidade);
void show140(uint8_t intensidade);

// VARIAVEIS
uint8_t hue;
int ledCount;
SmartLed leds(LED_WS2812B, LED_COUNT, DATA_PIN, CHANNEL, DoubleBuffer);

void setup()
{
    Serial.begin(115200);
    ledCount = 0;
}

void loop()
{
    Serial.println("Teste dos Leds");

    for (int i = 0; i < 2; i++)
    {
        show30(200);
        delay(50);
    }

    //delay(5000);
    //  if (millis() % 10000 < 5000)
    //      showGradient();
    //  else
    //      showRgb(10);
    while (true);
    //delay(5000);
}

void showGradient()
{
    hue++;
    // Use HSV to create nice gradient
    for (int i = 0; i != LED_COUNT; i++)
        leds[i] = Hsv{static_cast<uint8_t>(hue + 30 * i), 255, 255};
    leds.show();
    // Show is asynchronous; if we need to wait for the end of transmission,
    // we can use leds.wait(); however we use double buffered mode, so we
    // can start drawing right after showing.
}

void show30(uint8_t intensidade)
{
    //if (intensidade > 71)
    //    return;
    //    RED GR  BL
    // 0 a 255
    leds[0] = Rgb{0, 0, 0};
    leds[1] = Rgb{0, 0, 0};
    leds[2] = Rgb{0, 0, 0};
    leds[3] = Rgb{0, 0, 0};
    leds[4] = Rgb{0, 0, 0};
    leds[5] = Rgb{0, 0, 0};
    leds[6] = Rgb{0, 0, 0};
    leds[7] = Rgb{0, 0, 0};
    leds[8] = Rgb{0, 0, 0};
    leds[9] = Rgb{0, 0, 0};
    leds[10] = Rgb{0, 0, 0};
    leds[11] = Rgb{0, 0, 0};
    leds[12] = Rgb{0, 0, 0};
    leds[13] = Rgb{0, 255 - intensidade, 0};
    leds[14] = Rgb{0, 255 - intensidade, 0};
    leds[15] = Rgb{0, 255 - intensidade, 0};
    leds[16] = Rgb{0, 0, 0};
    leds[17] = Rgb{0, 255 - intensidade, 0};
    leds[18] = Rgb{0, 255 - intensidade, 0};
    leds[19] = Rgb{0, 255 - intensidade, 0};
    leds[20] = Rgb{0, 0, 0};
    leds[21] = Rgb{0, 0, 0};
    leds[22] = Rgb{0, 0, 0};
    leds[23] = Rgb{0, 0, 0};
    leds[24] = Rgb{0, 0, 0};
    leds[25] = Rgb{0, 255 - intensidade, 0};
    leds[26] = Rgb{0, 0, 0};
    leds[27] = Rgb{0, 255 - intensidade, 0};
    leds[28] = Rgb{0, 0, 0};
    leds[29] = Rgb{0, 255 - intensidade, 0};
    leds[30] = Rgb{0, 0, 0};
    leds[31] = Rgb{0, 0, 0};
    leds[32] = Rgb{0, 0, 0};
    leds[33] = Rgb{0, 0, 0};
    leds[34] = Rgb{0, 0, 0};
    leds[35] = Rgb{0, 255 - intensidade, 0};
    leds[36] = Rgb{0, 0, 0};
    leds[37] = Rgb{0, 255 - intensidade, 0};
    leds[38] = Rgb{0, 0, 0};
    leds[39] = Rgb{0, 255 - intensidade, 0};
    leds[40] = Rgb{0, 0, 0};
    leds[41] = Rgb{0, 0, 0};
    leds[42] = Rgb{0, 0, 0};
    leds[43] = Rgb{0, 255 - intensidade, 0};
    leds[44] = Rgb{0, 255 - intensidade, 0};
    leds[45] = Rgb{0, 255 - intensidade, 0};
    leds[46] = Rgb{0, 0, 0};
    leds[47] = Rgb{0, 255 - intensidade, 0};
    leds[48] = Rgb{0, 0, 0};
    leds[49] = Rgb{0, 255 - intensidade, 0};
    leds[50] = Rgb{0, 0, 0};
    leds[51] = Rgb{0, 0, 0};
    leds[52] = Rgb{0, 0, 0};
    leds[53] = Rgb{0, 0, 0};
    leds[54] = Rgb{0, 0, 0};
    leds[55] = Rgb{0, 255 - intensidade, 0};
    leds[56] = Rgb{0, 0, 0};
    leds[57] = Rgb{0, 255 - intensidade, 0};
    leds[58] = Rgb{0, 0, 0};
    leds[59] = Rgb{0, 255 - intensidade, 0};
    leds[60] = Rgb{0, 0, 0};
    leds[61] = Rgb{0, 0, 0};
    leds[62] = Rgb{0, 0, 0};
    leds[63] = Rgb{0, 0, 0};
    leds[64] = Rgb{0, 0, 0};
    leds[65] = Rgb{0, 255 - intensidade, 0};
    leds[66] = Rgb{0, 0, 0};
    leds[67] = Rgb{0, 255 - intensidade, 0};
    leds[68] = Rgb{0, 0, 0};
    leds[69] = Rgb{0, 255 - intensidade, 0};
    leds[70] = Rgb{0, 0, 0};
    leds[71] = Rgb{0, 0, 0};
    leds[72] = Rgb{0, 0, 0};
    leds[73] = Rgb{0, 255 - intensidade, 0};
    leds[74] = Rgb{0, 255 - intensidade, 0};
    leds[75] = Rgb{0, 255 - intensidade, 0};
    leds[76] = Rgb{0, 0, 0};
    leds[77] = Rgb{0, 255 - intensidade, 0};
    leds[78] = Rgb{0, 255 - intensidade, 0};
    leds[79] = Rgb{0, 255 - intensidade, 0};
    leds[80] = Rgb{0, 0, 0};
    leds[81] = Rgb{0, 0, 0};
    leds[82] = Rgb{0, 0, 0};
    leds[83] = Rgb{0, 0, 0};
    leds[84] = Rgb{0, 0, 0};
    leds[85] = Rgb{0, 0, 0};
    leds[86] = Rgb{0, 0, 0};
    leds[87] = Rgb{0, 0, 0};
    leds[88] = Rgb{0, 0, 0};
    leds[89] = Rgb{0, 0, 0};
    leds[90] = Rgb{0, 255 - intensidade, 0};
    leds[91] = Rgb{0, 255 - intensidade, 0};
    leds[92] = Rgb{0, 0, 0};
    leds[93] = Rgb{0, 0, 0};
    leds[94] = Rgb{0, 0, 0};
    leds[95] = Rgb{0, 0, 0};
    leds[96] = Rgb{0, 0, 0};
    leds[97] = Rgb{0, 0, 0};
    leds[98] = Rgb{0, 0, 0};
    leds[99] = Rgb{0, 0, 0};

    leds.show();
}

void show130(uint8_t intensidade)
{
    //    RED GR  BL
    // 0 a 255
    leds[0] = Rgb{0, 0, 0};
    leds[1] = Rgb{0, 0, 0};
    leds[2] = Rgb{0, 0, 0};
    leds[3] = Rgb{0, 0, 0};
    leds[4] = Rgb{0, 0, 0};
    leds[5] = Rgb{0, 0, 0};
    leds[6] = Rgb{0, 0, 0};
    leds[7] = Rgb{0, 0, 0};
    leds[8] = Rgb{0, 0, 0};
    leds[9] = Rgb{0, 0, 0};
    leds[10] = Rgb{0, 0, 0};
    leds[11] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[12] = Rgb{0, 0, 0};
    leds[13] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[14] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[15] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[16] = Rgb{0, 0, 0};
    leds[17] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[18] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[19] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[20] = Rgb{0, 0, 0};
    leds[21] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[22] = Rgb{0, 0, 0};
    leds[23] = Rgb{0, 0, 0};
    leds[24] = Rgb{0, 0, 0};
    leds[25] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[26] = Rgb{0, 0, 0};
    leds[27] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[28] = Rgb{0, 0, 0};
    leds[29] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[30] = Rgb{0, 0, 0};
    leds[31] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[32] = Rgb{0, 0, 0};
    leds[33] = Rgb{0, 0, 0};
    leds[34] = Rgb{0, 0, 0};
    leds[35] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[36] = Rgb{0, 0, 0};
    leds[37] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[38] = Rgb{0, 0, 0};
    leds[39] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[40] = Rgb{0, 0, 0};
    leds[41] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[42] = Rgb{0, 0, 0};
    leds[43] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[44] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[45] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[46] = Rgb{0, 0, 0};
    leds[47] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[48] = Rgb{0, 0, 0};
    leds[49] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[50] = Rgb{0, 0, 0};
    leds[51] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[52] = Rgb{0, 0, 0};
    leds[53] = Rgb{0, 0, 0};
    leds[54] = Rgb{0, 0, 0};
    leds[55] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[56] = Rgb{0, 0, 0};
    leds[57] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[58] = Rgb{0, 0, 0};
    leds[59] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[60] = Rgb{0, 0, 0};
    leds[61] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[62] = Rgb{0, 0, 0};
    leds[63] = Rgb{0, 0, 0};
    leds[64] = Rgb{0, 0, 0};
    leds[65] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[66] = Rgb{0, 0, 0};
    leds[67] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[68] = Rgb{0, 0, 0};
    leds[69] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[70] = Rgb{0, 0, 0};
    leds[71] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[72] = Rgb{0, 0, 0};
    leds[73] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[74] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[75] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[76] = Rgb{0, 0, 0};
    leds[77] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[78] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[79] = Rgb{255 - intensidade, 192 - intensidade, 0};
    leds[80] = Rgb{0, 0, 0};
    leds[81] = Rgb{0, 0, 0};
    leds[82] = Rgb{0, 0, 0};
    leds[83] = Rgb{0, 0, 0};
    leds[84] = Rgb{0, 0, 0};
    leds[85] = Rgb{0, 0, 0};
    leds[86] = Rgb{0, 0, 0};
    leds[87] = Rgb{0, 0, 0};
    leds[88] = Rgb{0, 0, 0};
    leds[89] = Rgb{0, 0, 0};
    leds[90] = Rgb{112, 173, 71};
    leds[91] = Rgb{112, 173, 71};
    leds[92] = Rgb{146, 208, 80};
    leds[93] = Rgb{146, 208, 80};
    leds[94] = Rgb{182, 210, 78};
    leds[95] = Rgb{255, 255, 0};
    leds[96] = Rgb{255, 255, 0};
    leds[97] = Rgb{255, 192, 0};
    leds[98] = Rgb{0, 0, 0};
    leds[99] = Rgb{0, 0, 0};

    leds.show();
}


void show140(uint8_t intensidade)
{
    //    RED GR  BL
    // 0 a 255
    leds[0] = Rgb{0, 0, 0};
    leds[1] = Rgb{0, 0, 0};
    leds[2] = Rgb{0, 0, 0};
    leds[3] = Rgb{0, 0, 0};
    leds[4] = Rgb{0, 0, 0};
    leds[5] = Rgb{0, 0, 0};
    leds[6] = Rgb{0, 0, 0};
    leds[7] = Rgb{0, 0, 0};
    leds[8] = Rgb{0, 0, 0};
    leds[9] = Rgb{0, 0, 0};
    leds[10] = Rgb{0, 0, 0};
    leds[11] = Rgb{intensidade, 0, 0};
    leds[12] = Rgb{0, 0, 0};
    leds[13] = Rgb{intensidade, 0, 0};
    leds[14] = Rgb{0, 0, 0};
    leds[15] = Rgb{intensidade, 0, 0};
    leds[16] = Rgb{0, 0, 0};
    leds[17] = Rgb{intensidade, 0, 0};
    leds[18] = Rgb{intensidade, 0, 0};
    leds[19] = Rgb{intensidade, 0, 0};
    leds[20] = Rgb{0, 0, 0};
    leds[21] = Rgb{intensidade, 0, 0};
    leds[22] = Rgb{0, 0, 0};
    leds[23] = Rgb{intensidade, 0, 0};
    leds[24] = Rgb{0, 0, 0};
    leds[25] = Rgb{intensidade, 0, 0};
    leds[26] = Rgb{0, 0, 0};
    leds[27] = Rgb{intensidade, 0, 0};
    leds[28] = Rgb{0, 0, 0};
    leds[29] = Rgb{intensidade, 0, 0};
    leds[30] = Rgb{0, 0, 0};
    leds[31] = Rgb{intensidade, 0, 0};
    leds[32] = Rgb{0, 0, 0};
    leds[33] = Rgb{intensidade, 0, 0};
    leds[34] = Rgb{0, 0, 0};
    leds[35] = Rgb{intensidade, 0, 0};
    leds[36] = Rgb{0, 0, 0};
    leds[37] = Rgb{intensidade, 0, 0};
    leds[38] = Rgb{0, 0, 0};
    leds[39] = Rgb{intensidade, 0, 0};
    leds[40] = Rgb{0, 0, 0};
    leds[41] = Rgb{intensidade, 0, 0};
    leds[42] = Rgb{0, 0, 0};
    leds[43] = Rgb{intensidade, 0, 0};
    leds[44] = Rgb{intensidade, 0, 0};
    leds[45] = Rgb{intensidade, 0, 0};
    leds[46] = Rgb{0, 0, 0};
    leds[47] = Rgb{intensidade, 0, 0};
    leds[48] = Rgb{0, 0, 0};
    leds[49] = Rgb{intensidade, 0, 0};
    leds[50] = Rgb{0, 0, 0};
    leds[51] = Rgb{intensidade, 0, 0};
    leds[52] = Rgb{0, 0, 0};
    leds[53] = Rgb{0, 0, 0};
    leds[54] = Rgb{0, 0, 0};
    leds[55] = Rgb{intensidade, 0, 0};
    leds[56] = Rgb{0, 0, 0};
    leds[57] = Rgb{intensidade, 0, 0};
    leds[58] = Rgb{0, 0, 0};
    leds[59] = Rgb{intensidade, 0, 0};
    leds[60] = Rgb{0, 0, 0};
    leds[61] = Rgb{intensidade, 0, 0};
    leds[62] = Rgb{0, 0, 0};
    leds[63] = Rgb{0, 0, 0};
    leds[64] = Rgb{0, 0, 0};
    leds[65] = Rgb{intensidade, 0, 0};
    leds[66] = Rgb{0, 0, 0};
    leds[67] = Rgb{intensidade, 0, 0};
    leds[68] = Rgb{0, 0, 0};
    leds[69] = Rgb{intensidade, 0, 0};
    leds[70] = Rgb{0, 0, 0};
    leds[71] = Rgb{intensidade, 0, 0};
    leds[72] = Rgb{0, 0, 0};
    leds[73] = Rgb{0, 0, 0};
    leds[74] = Rgb{0, 0, 0};
    leds[75] = Rgb{intensidade, 0, 0};
    leds[76] = Rgb{0, 0, 0};
    leds[77] = Rgb{intensidade, 0, 0};
    leds[78] = Rgb{intensidade, 0, 0};
    leds[79] = Rgb{intensidade, 0, 0};
    leds[80] = Rgb{0, 0, 0};
    leds[81] = Rgb{0, 0, 0};
    leds[82] = Rgb{0, 0, 0};
    leds[83] = Rgb{0, 0, 0};
    leds[84] = Rgb{0, 0, 0};
    leds[85] = Rgb{0, 0, 0};
    leds[86] = Rgb{0, 0, 0};
    leds[87] = Rgb{0, 0, 0};
    leds[88] = Rgb{0, 0, 0};
    leds[89] = Rgb{0, 0, 0};
    leds[90] = Rgb{112, 173, 71};
    leds[91] = Rgb{112, 173, 71};
    leds[92] = Rgb{146, 208, 80};
    leds[93] = Rgb{146, 208, 80};
    leds[94] = Rgb{182, 210, 78};
    leds[95] = Rgb{255, 255, 0};
    leds[96] = Rgb{255, 255, 0};
    leds[97] = Rgb{255, 192, 0};
    leds[98] = Rgb{255, 102, 0};
    leds[99] = Rgb{intensidade, 0, 0};

    leds.show();
}
