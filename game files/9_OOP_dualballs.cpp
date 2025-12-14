#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <cmath>


#define TFT_DC 2
#define TFT_CS 15
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;


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

    Ball(float x, float y, float s_x, float s_y, int c)
    {
        X = x; Y = y; speedX = s_x; speedY = s_y; size = 6; color = c;
    }

    
    void checkWalls() {
        if (X <= size || X >= SCREEN_WIDTH - size) speedX = -speedX;
        if (Y <= size) speedY = -speedY;
    }

    void move() {
        X += speedX;
        Y += speedY;
    }
};

class Paddle
{
    public:
    float speed, X, width, Y, thickness;
    Paddle(float p = SCREEN_WIDTH/2, float w = 80)
    {
        speed = 7; X = p; width = w; thickness = 10; Y = SCREEN_HEIGHT - thickness;
    }
    void draw()  { tft.fillRect(X, Y, width, thickness, ILI9341_GREEN); }
    
    bool update() 
    {
        float oldX = X;
        if (leftButtonPressed && X > 0) X -= speed;
        if (rightButtonPressed && X < SCREEN_WIDTH - width) X += speed;
        return (X != oldX);
    }
};

class Obstacle {
    public:
    float x, y, w, h;
    int color;

    Obstacle(float _x, float _y, float _w, float _h, int _c = ILI9341_MAGENTA) {
        x = _x; y = _y; w = _w; h = _h; color = _c;
    }
    void draw() {
        tft.fillRect(x, y, w, h, color);
    }
};

class PongGame
{
    public:
    Ball ball1;
    Ball ball2;
    Paddle paddle;
    Obstacle obstacle; 
    int score; 

    PongGame(): 
            ball1(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 4, 4, ILI9341_RED),
            ball2(SCREEN_WIDTH/2 - 30, SCREEN_HEIGHT/2 - 30, -3, 5, ILI9341_CYAN),
            paddle(),
            obstacle(120, 80, 80, 40)
    {
        score = 0; 
    }

    void reset()
    {

        tft.fillScreen(ILI9341_BLACK);
        
        score = 0;
        
        ball1.X = SCREEN_WIDTH/2; ball1.Y = SCREEN_HEIGHT/2;
        ball1.speedX = 4; ball1.speedY = 4;
        
        ball2.X = SCREEN_WIDTH/2 - 30; ball2.Y = SCREEN_HEIGHT/2 - 30;
        ball2.speedX = -3; ball2.speedY = 5;

        showScore();
        obstacle.draw(); 
        tft.fillCircle(ball1.X, ball1.Y, ball1.size, ball1.color);
        tft.fillCircle(ball2.X, ball2.Y, ball2.size, ball2.color);
        paddle.draw();
    }

    void showScore() {

        tft.setCursor(5, 5);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);  
        tft.setTextSize(2);
        tft.print("Score: "); tft.print(score);
    }

    void checkPaddle(Ball &b) {
        if (b.Y + b.size >= paddle.Y && b.X > paddle.X && b.X < paddle.X + paddle.width) {
             b.Y = paddle.Y - b.size - 1; 
             b.speedY = -fabs(b.speedY); 
             float paddleCenter = paddle.X + (paddle.width / 2.0);
             b.speedX = (b.X - paddleCenter) * 0.3; 
             score++; 
             showScore();
        }
    }

    void checkObstacle(Ball &b) {
        float closestX = constrain(b.X, obstacle.x, obstacle.x + obstacle.w);
        float closestY = constrain(b.Y, obstacle.y, obstacle.y + obstacle.h);
        float distX = b.X - closestX;
        float distY = b.Y - closestY;
        float distance = sqrt((distX*distX) + (distY*distY));

        if (distance < b.size) {
            if (abs(distX) > abs(distY)) b.speedX = -b.speedX;
            else b.speedY = -b.speedY;
            
            if (distance > 0) {
                b.X += (distX/distance) * (b.size - distance + 1);
                b.Y += (distY/distance) * (b.size - distance + 1);
            }
        }
    }

    void update()
    {
        
        float oldB1x = ball1.X; float oldB1y = ball1.Y;
        float oldB2x = ball2.X; float oldB2y = ball2.Y;
        float oldPadX = paddle.X;

        bool paddleMoved = paddle.update(); 

        ball1.move();
        ball2.move();
        ball1.checkWalls();
        ball2.checkWalls();
        checkPaddle(ball1);
        checkPaddle(ball2);
        checkObstacle(ball1);
        checkObstacle(ball2);

        float dx = ball2.X - ball1.X;
        float dy = ball2.Y - ball1.Y;
        float distance = sqrt(dx*dx + dy*dy);
        if (distance < ball1.size + ball2.size) {
            float nx = dx / distance; float ny = dy / distance;
            float overlap = (ball1.size + ball2.size - distance) / 2.0;
            ball1.X -= overlap * nx; ball1.Y -= overlap * ny;
            ball2.X += overlap * nx; ball2.Y += overlap * ny;

            float tx = -ny; float ty = nx;
            float dpTan1 = ball1.speedX * tx + ball1.speedY * ty;
            float dpTan2 = ball2.speedX * tx + ball2.speedY * ty;
            float dpNorm1 = ball1.speedX * nx + ball1.speedY * ny;
            float dpNorm2 = ball2.speedX * nx + ball2.speedY * ny;
            
            float m1 = 1.0; float m2 = 1.0;
            float p1 = (dpNorm1 * (m1 - m2) + 2.0 * m2 * dpNorm2) / (m1 + m2);
            float p2 = (dpNorm2 * (m2 - m1) + 2.0 * m1 * dpNorm1) / (m1 + m2);
            
            ball1.speedX = tx * dpTan1 + nx * p2; ball1.speedY = ty * dpTan1 + ny * p2;
            ball2.speedX = tx * dpTan2 + nx * p1; ball2.speedY = ty * dpTan2 + ny * p1;
        }

        if (ball1.Y > SCREEN_HEIGHT || ball2.Y > SCREEN_HEIGHT) reset();


       
        obstacle.draw();

        if (paddleMoved) {
            tft.fillRect(oldPadX, paddle.Y, paddle.width, paddle.thickness, ILI9341_BLACK);
            paddle.draw();
        }

        tft.fillCircle(oldB1x, oldB1y, ball1.size, ILI9341_BLACK);
        tft.fillCircle(ball1.X, ball1.Y, ball1.size, ball1.color);

        tft.fillCircle(oldB2x, oldB2y, ball2.size, ILI9341_BLACK);
        tft.fillCircle(ball2.X, ball2.Y, ball2.size, ball2.color);

        bool scoreNeedsRefresh = false;
        if (oldB1x < 120 && oldB1y < 40) scoreNeedsRefresh = true;
        if (ball1.X < 120 && ball1.Y < 40) scoreNeedsRefresh = true;
        if (oldB2x < 120 && oldB2y < 40) scoreNeedsRefresh = true;
        if (ball2.X < 120 && ball2.Y < 40) scoreNeedsRefresh = true;

        if (scoreNeedsRefresh) {
            showScore();
        }
    }
};

PongGame pong;

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