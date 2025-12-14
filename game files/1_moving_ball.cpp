#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// ---------------- Display Pins (Wokwi Standard) ----------------
#define TFT_DC 2
#define TFT_CS 15

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;

int ballX = 0;
int ballY = SCREEN_HEIGHT/2;
int ballSize = 6;
auto ballColor = ILI9341_CYAN;

void setup()
{
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);
}

void loop()
{
    tft.fillCircle(ballX, ballY, ballSize, ILI9341_BLACK);
    ballX += 3;
    tft.fillCircle(ballX, ballY, ballSize, ballColor);
    delay(15);
}