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

bool leftButtonPressed = false, rightButtonPressed = false;
unsigned long lastUpdate = 0;
const unsigned long interval = 25; // update every 50ms //20fps

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
        if (leftButtonPressed && X > 0) {
            X -= speed;
        }
        if (rightButtonPressed && X < SCREEN_WIDTH - width) {
            X += speed;
        }
        draw();
    }
};

class PongGame
{
    public:
    Ball ball;
    Paddle paddle;
    float ball_speed, paddle_speed;
    int score; // here i defined the score variable

    PongGame(float b_v=5, float p_v=5): 
            ball_speed(b_v),
            paddle_speed(p_v),
            ball(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, b_v, b_v, 7),
            paddle()
    {
        score = 0; // this i give start value 0 to score
    }

    void reset()
    {
        score=0;
        showScore();
        ball.erase();
        ball.X = SCREEN_WIDTH/2;
        ball.Y = SCREEN_HEIGHT/2;
        ball.speedX = -ball.speedX;
        ball.speedY = -ball.speedY;
        ball.update();
        tft.fillRect(0, 0, 100, 30, ILI9341_BLACK); // extra line to clear score area
    }

    void showScore() {
        tft.setCursor(5, 5);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);  
        tft.setTextSize(2);
        tft.print("Score: ");
        tft.print(score);
    }

    void update()
    {
        // Wall collisions
        if (ball.X <= ball.size || ball.X >= SCREEN_WIDTH - ball.size) ball.speedX = -ball.speedX;
        if (ball.Y <= ball.size) ball.speedY = -ball.speedY;

        //Paddle Collision
        
        if (ball.Y + ball.size >= paddle.Y && ball.X > paddle.X && ball.X < paddle.X + paddle.width) 
        {
            ball.erase();

            ball.Y = paddle.Y - ball.size - 1; 

            ball.speedY = -fabs(ball.speedY); 

            float paddleCenter = paddle.X + (paddle.width / 2.0);
            float collisionPoint = ball.X - paddleCenter;

            ball.speedX = collisionPoint * 0.3; 
    
            score++;
            showScore();
        }
        
        //Paddle missed
        if (ball.Y > SCREEN_HEIGHT)
            reset();
        
        ball.update();
        paddle.update();

        // reprinting scoreboard
        if (ball.X < 120 && ball.Y < 30) {
            showScore();
        }
    }
};

PongGame pong(7,5);

void setup()
{
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
    pong.showScore(); // calling upon the fxn in setup itself
}

void loop()
{
    if (!digitalRead(BTN_LEFT))
        leftButtonPressed = true;
    if (!digitalRead(BTN_RIGHT))
        rightButtonPressed = true;
    
    unsigned long now = millis();

    if (now - lastUpdate >= interval)
    {
        pong.update();
        leftButtonPressed = false;
        rightButtonPressed = false;
        lastUpdate = now;
    }
}