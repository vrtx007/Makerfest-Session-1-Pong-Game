#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// ---------------- Display Pins (Wokwi Standard) ----------------
#define TFT_DC 2
#define TFT_CS 15
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;

class Ball
{
    public:
    float X, Y, speedX, speedY;
    int color, size;

    Ball(float x=0, float y=SCREEN_HEIGHT/2, float s_x=2, float s_y=0, int sz=6, int c = ILI9341_RED)
    {
        X = x;
        Y = y;
        speedX = s_x;
        speedY = s_y;
        size = sz;
        color = c;
    }

    void erase()
    {
        tft.fillCircle(X, Y, size, ILI9341_BLACK);
    }

    void draw()
    {
       tft.fillCircle(X, Y, size, color);
    }

    void update()
    {
        erase();
        X += speedX;
        Y += speedY;
        draw();
    }
};


Ball ball1;
Ball ball2(SCREEN_WIDTH, SCREEN_HEIGHT, -0.5, -0.5, 7, ILI9341_DARKGREEN);


void setup()
{
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);
}

void loop()
{
    ball1.update();
    ball2.update();
    delay(15);
}