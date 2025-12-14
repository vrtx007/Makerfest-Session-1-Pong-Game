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

Ball ball(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 5, 5, 7);
Paddle paddle;

void reset_game()
{
    ball.erase();
    ball.X = SCREEN_WIDTH/2;
    ball.Y = SCREEN_HEIGHT/2;
    ball.speedX = -ball.speedX;
    ball.speedY = -ball.speedY;
    ball.update();
}

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
    // Wall collisions
    if (ball.X <= ball.size || ball.X >= SCREEN_WIDTH - ball.size) ball.speedX = -ball.speedX;
    if (ball.Y <= ball.size) ball.speedY = -ball.speedY;
    //Paddle Collision
    if (ball.Y + ball.size >= paddle.Y && ball.X > paddle.X && ball.X < paddle.X + paddle.width) {
        ball.speedY = -ball.speedY;
    }
    //Paddle missed
    if (ball.Y > SCREEN_HEIGHT)
        reset_game();
    
    ball.update();
    paddle.update();
    delay(15);
}