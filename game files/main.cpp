// #include <Adafruit_GFX.h>
// #include <Adafruit_ILI9341.h>

// // ---------------- Display Pins (Wokwi Standard) ----------------
// #define TFT_DC 2
// #define TFT_CS 15
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// // #define TFT_CS   15
// // #define TFT_DC    2
// // #define TFT_RST   4
// // Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// // ---------------- Game Constants ----------------
// const int SCREEN_WIDTH = 320;
// const int SCREEN_HEIGHT = 240;

// // Ball properties
// int ballX = SCREEN_WIDTH / 2;
// int ballY = SCREEN_HEIGHT / 2;
// int ballSize = 6;
// int ballSpeedX = 3;
// int ballSpeedY = 3;

// // Paddle properties
// int paddleWidth = 60;
// int paddleHeight = 10;
// int paddleX = SCREEN_WIDTH / 2 - paddleWidth / 2;
// int paddleY = SCREEN_HEIGHT - 30;
// int paddleSpeed = 5;

// // Buttons (Change pins if needed in Wokwi)
// #define BTN_LEFT 5
// #define BTN_RIGHT 17


// void updatePaddle();
// void drawPaddle();
// void updateBall();
// void drawBall();
// void resetBall();
// void showScore();



// int score = 0;

// // ---------------- Setup ----------------
// void setup() {
//   Serial.begin(115200);

//   pinMode(BTN_LEFT, INPUT_PULLUP);
//   pinMode(BTN_RIGHT, INPUT_PULLUP);

//   tft.begin();
//   tft.setRotation(1);
//   tft.fillScreen(ILI9341_BLACK);

//   drawPaddle();
//   drawBall();
//   showScore();
// }

// // ---------------- Main Game Loop ----------------
// void loop() {
//   updatePaddle();
//   updateBall();
//   delay(15);
// }

// // ---------------- Paddle Control & Drawing ----------------
// void updatePaddle() {
//   tft.fillRect(paddleX, paddleY, paddleWidth, paddleHeight, ILI9341_BLACK);

//   if (!digitalRead(BTN_LEFT) && paddleX > 0) {
//     paddleX -= paddleSpeed;
//   }
//   if (!digitalRead(BTN_RIGHT) && paddleX < SCREEN_WIDTH - paddleWidth) {
//     paddleX += paddleSpeed;
//   }

//   drawPaddle();
// }

// void drawPaddle() {
//   tft.fillRect(paddleX, paddleY, paddleWidth, paddleHeight, ILI9341_GREEN);
// }

// // ---------------- Ball Movement & Collision ----------------
// void updateBall() {
//   tft.fillCircle(ballX, ballY, ballSize, ILI9341_BLACK);

//   ballX += ballSpeedX;
//   ballY += ballSpeedY;

//   // Wall collisions
//   if (ballX <= ballSize || ballX >= SCREEN_WIDTH - ballSize) ballSpeedX = -ballSpeedX;
//   if (ballY <= ballSize) ballSpeedY = -ballSpeedY;

//   // Paddle collision
//   if (ballY + ballSize >= paddleY &&
//       ballX > paddleX && ballX < paddleX + paddleWidth) {
//     ballSpeedY = -ballSpeedY;
//     score++;
//     showScore();
//   }

//   // Missed
//   if (ballY > SCREEN_HEIGHT) {
//     score = 0;
//     resetBall();
//     showScore();
//   }

//   drawBall();
// }

// void drawBall() {
//   tft.fillCircle(ballX, ballY, ballSize, ILI9341_WHITE);
// }

// void resetBall() {
//   ballX = SCREEN_WIDTH / 2;
//   ballY = SCREEN_HEIGHT / 2;
//   ballSpeedY = -ballSpeedY;
// }

// // ---------------- Score Display ----------------
// void showScore() {
//   tft.fillRect(0, 0, SCREEN_WIDTH, 20, ILI9341_BLACK);
//   tft.setCursor(5, 5);
//   tft.setTextColor(ILI9341_YELLOW);
//   tft.setTextSize(2);
//   tft.print("Score: ");
//   tft.print(score);
// }

// //*/