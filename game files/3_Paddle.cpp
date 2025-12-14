#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// ---------------- Display Pins (Wokwi Standard) ----------------
#define TFT_DC 2
#define TFT_CS 15
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;

// Buttons
#define BTN_LEFT 5
#define BTN_RIGHT 17


class Paddle
{
    public:
    float speed, X, width, Y, thickness;
    Paddle(float p = SCREEN_WIDTH/2, float w = 80)
    {
        speed = 5;
        X = p;
        width = w;
        thickness = 10;
        Y = SCREEN_HEIGHT - thickness;
    }
    void draw()
    {
        tft.fillRect(X, Y, width, thickness, ILI9341_GREEN);
    }
    void erase()
    {
        tft.fillRect(X, Y, width, thickness, ILI9341_BLACK);
    }
    void update() 
    {
        erase();
        if (!digitalRead(BTN_LEFT) && X > 0) {
            X -= speed;
        }
        if (!digitalRead(BTN_RIGHT) && X < SCREEN_WIDTH - width) {
            X += speed;
        }
        draw();
    }
};

Paddle paddle;

void setup()
{
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);

    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
}

void loop()
{
    paddle.update();
    delay(15);
}