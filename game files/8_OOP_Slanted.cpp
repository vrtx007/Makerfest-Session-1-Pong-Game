#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <cmath>

// ---------------- Display Pins ----------------
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
const unsigned long interval = 25; 

class Ball
{
    public:
    float X, Y, speedX, speedY;
    int color, size;

    Ball(float x=0, float y=SCREEN_HEIGHT/2, float s_x=2, float s_y=0, int sz=6, int c = ILI9341_RED)
    {
        X = x; Y = y; speedX = s_x; speedY = s_y; size = sz; color = c;
    }

    void erase() { tft.fillCircle(X, Y, size, ILI9341_BLACK); }
    void draw()  { tft.fillCircle(X, Y, size, color); }
    void update() {
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
        speed = 5; X = p; width = w; thickness = 10; Y = SCREEN_HEIGHT - thickness;
    }
    void draw()  { tft.fillRect(X, Y, width, thickness, ILI9341_GREEN); }
    void erase() { tft.fillRect(X, Y, width, thickness, ILI9341_BLACK); }
    void update() 
    {
        erase();
        if (leftButtonPressed && X > 0) X -= speed;
        if (rightButtonPressed && X < SCREEN_WIDTH - width) X += speed;
        draw();
    }
};

class SlantedObstacle {
    public:
    float cx, cy; // Center
    float w, h;   // Width, Height
    float angle;  // Rotation in Radians
    int color;
    
    // Store corners for drawing lines easily
    float c1x, c1y, c2x, c2y, c3x, c3y, c4x, c4y;

    SlantedObstacle(float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, int _c = ILI9341_MAGENTA) {
        color = _c;
        
        // Save raw corners for the draw() function
        c1x = x1; c1y = y1;
        c2x = x2; c2y = y2;
        c3x = x3; c3y = y3;
        c4x = x4; c4y = y4;

        cx = (x1 + x2 + x3 + x4)/4.0;
        cy = (y1 + y2 + y3 + y4)/4.0;
        
        w = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); 
        
        h = sqrt(pow(x4 - x1, 2) + pow(y4 - y1, 2));

        angle = atan2(y2 - y1, x2 - x1);
    }

    void draw() {
     
        tft.drawLine(c1x, c1y, c2x, c2y, color);
        tft.drawLine(c2x, c2y, c3x, c3y, color);
        tft.drawLine(c3x, c3y, c4x, c4y, color);
        tft.drawLine(c4x, c4y, c1x, c1y, color);
    }
};

class PongGame
{
    public:
    Ball ball;
    Paddle paddle;
    SlantedObstacle obstacle; 
    float ball_speed, paddle_speed;
    int score; 

    PongGame(float b_v=5, float p_v=5): 
            ball_speed(b_v),
            paddle_speed(p_v),
            ball(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, b_v, b_v, 7),
            paddle(),
            obstacle(160,120,138,178, 90,60,36,66) 
    {
        score = 0; 
    }

    void reset()
    {
        ball.erase(); 
        
        score = 0;
        ball.X = SCREEN_WIDTH/2; 
        ball.Y = SCREEN_HEIGHT/2;
        ball.speedX = -ball.speedX; 
        ball.speedY = -ball.speedY;
        
        tft.fillRect(0, 0, 100, 30, ILI9341_BLACK); 
        showScore();
        obstacle.draw(); 
        ball.draw(); 
    }

    void showScore() {
        tft.setCursor(5, 5);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);  
        tft.setTextSize(2);
        tft.print("Score: "); tft.print(score);
    }

    void update()
    {
        float oldBallX = ball.X;
        float oldBallY = ball.Y;

        ball.X += ball.speedX;
        ball.Y += ball.speedY;

        // Wall collisions
        if (ball.X <= ball.size || ball.X >= SCREEN_WIDTH - ball.size) ball.speedX = -ball.speedX;
        if (ball.Y <= ball.size) ball.speedY = -ball.speedY;

        // Paddle Collision
        if (ball.Y + ball.size >= paddle.Y && ball.X > paddle.X && ball.X < paddle.X + paddle.width) 
        {
            ball.Y = paddle.Y - ball.size - 1; 
            ball.speedY = -fabs(ball.speedY); 
            float paddleCenter = paddle.X + (paddle.width / 2.0);
            float collisionPoint = ball.X - paddleCenter;
            ball.speedX = collisionPoint * 0.3; 
            score++; showScore();
        }
        
        // Obstacle Collision
        float dx = ball.X - obstacle.cx;
        float dy = ball.Y - obstacle.cy;
        float cosA = cos(-obstacle.angle);
        float sinA = sin(-obstacle.angle);

        float localBallX = dx * cosA - dy * sinA;
        float localBallY = dx * sinA + dy * cosA;

        float halfW = obstacle.w / 2.0;
        float halfH = obstacle.h / 2.0;

        // "Deep Tunneling" Check
        if (abs(localBallX) < halfW && abs(localBallY) < halfH) {
             float overlapX = halfW - abs(localBallX);
             float overlapY = halfH - abs(localBallY);

             float localVelX = ball.speedX * cosA - ball.speedY * sinA;
             float localVelY = ball.speedX * sinA + ball.speedY * cosA;

             if (overlapX < overlapY) {
                 if (localBallX > 0) localBallX = halfW + ball.size + 1;
                 else localBallX = -halfW - ball.size - 1;
                 localVelX = -localVelX;
             } else {
                 if (localBallY > 0) localBallY = halfH + ball.size + 1;
                 else localBallY = -halfH - ball.size - 1;
                 localVelY = -localVelY;
             }

             // Rotate Position Back
             float cosRev = cos(obstacle.angle);
             float sinRev = sin(obstacle.angle);
             ball.X = obstacle.cx + (localBallX * cosRev - localBallY * sinRev);
             ball.Y = obstacle.cy + (localBallX * sinRev + localBallY * cosRev);

             // Rotate Velocity Back
             ball.speedX = localVelX * cosRev - localVelY * sinRev;     // i AM able to undertand this but.......
             ball.speedY = localVelX * sinRev + localVelY * cosRev;     // is it reeaaallyyyy reqd to be this complicated damn
        } 
        else {
             // Standard Collision
             float closestX = constrain(localBallX, -halfW, halfW);
             float closestY = constrain(localBallY, -halfH, halfH);
             float distLocalX = localBallX - closestX;
             float distLocalY = localBallY - closestY;
             float distance = sqrt((distLocalX*distLocalX) + (distLocalY*distLocalY));

             if (distance < ball.size) {
                 float localVelX = ball.speedX * cosA - ball.speedY * sinA;
                 float localVelY = ball.speedX * sinA + ball.speedY * cosA;
                 
                 if (abs(distLocalX) > abs(distLocalY)) localVelX = -localVelX;
                 else localVelY = -localVelY;

                 float cosRev = cos(obstacle.angle);
                 float sinRev = sin(obstacle.angle);
                 ball.speedX = localVelX * cosRev - localVelY * sinRev;
                 ball.speedY = localVelX * sinRev + localVelY * cosRev;
                 
                 // Ejection
                 if (distance > 0) {
                     float pushAmt = ball.size - distance + 1;
                     float pushX = (distLocalX / distance) * pushAmt;
                     float pushY = (distLocalY / distance) * pushAmt;
                     ball.X += pushX * cosRev - pushY * sinRev;
                     ball.Y += pushX * sinRev + pushY * cosRev;
                 }
             }
        }
        
        if (ball.Y > SCREEN_HEIGHT) reset();
// Redraw
        tft.fillCircle(oldBallX, oldBallY, ball.size, ILI9341_BLACK); 
        
        obstacle.draw();

        ball.draw(); 
        
        paddle.update();
        if (ball.X < 120 && ball.Y < 30) showScore();
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
    pong.showScore(); 
}

void loop()
{
    if (!digitalRead(BTN_LEFT)) leftButtonPressed = true;
    if (!digitalRead(BTN_RIGHT)) rightButtonPressed = true;
    
    unsigned long now = millis();
    if (now - lastUpdate >= interval)
    {
        pong.update();
        leftButtonPressed = false;
        rightButtonPressed = false;
        lastUpdate = now;
    }
}