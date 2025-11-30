// #include <Adafruit_GFX.h>
// #include <Adafruit_ILI9341.h>

// // ---------------- Display Pins (Wokwi Standard) ----------------
// #define TFT_DC 2
// #define TFT_CS 15
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// const int SCREEN_WIDTH = 320;
// const int SCREEN_HEIGHT = 240;

// class Ball
// {
//     public:
//     float ballX, ballY, ballSpeedX, ballSpeedY;
//     int ballColor, ballSize;

//     Ball(float X=0, float Y=SCREEN_HEIGHT/2, float speedX=2, float speedY=0, int ballSz=6, int ballC = ILI9341_CYAN)
//     {
//         ballX = X;
//         ballY = Y;
//         ballSpeedX = speedX;
//         ballSpeedY = speedY;
//         ballSize = ballSz;
//         ballColor = ballC;
//     }

//     void erase()
//     {
//         tft.fillCircle(ballX, ballY, ballSize, ILI9341_BLACK);
//     }

//     void draw()
//     {
//        tft.fillCircle(ballX, ballY, ballSize, ballColor);
//     }

//     void update_position()
//     {
//         ballX += ballSpeedX;
//         ballY += ballSpeedY;
//     }
// };

// Ball ball1;
// Ball ball2(SCREEN_WIDTH, SCREEN_HEIGHT, -0.5, -0.5, 7, ILI9341_DARKGREEN);


// void setup()
// {
//     tft.begin();
//     tft.setRotation(1);
//     tft.fillScreen(ILI9341_BLACK);
// }

// void loop()
// {
//     ball1.erase();
//     ball1.update_position();
//     ball1.draw();
//     ball2.erase();
//     ball2.update_position();
//     ball2.draw();
//     delay(15);
// }