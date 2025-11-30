// #include <Adafruit_GFX.h>
// #include <Adafruit_ILI9341.h>

// // ---------------- Display Pins (Wokwi Standard) ----------------
// #define TFT_DC 2
// #define TFT_CS 15
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// const int SCREEN_WIDTH = 320;
// const int SCREEN_HEIGHT = 240;

// // Buttons
// #define BTN_LEFT 5
// #define BTN_RIGHT 17


// class Paddle
// {
//     public:
//     float speed, pos, width, posY, thickness;
//     Paddle(float p = SCREEN_WIDTH/2, float w = 80)
//     {
//         speed = 0.5;
//         pos = p;
//         width = w;
//         thickness = 10;
//         posY = SCREEN_HEIGHT - thickness;
//     }
//     void draw()
//     {
//         tft.fillRect(pos, posY, width, thickness, ILI9341_GREEN);
//     }
//     void erase()
//     {
//         tft.fillRect(pos, posY, width, thickness, ILI9341_BLACK);
//     }
//     void update() 
//     {
//         erase();
//         if (!digitalRead(BTN_LEFT) && pos > 0) {
//             pos -= speed;
//         }
//         if (!digitalRead(BTN_RIGHT) && pos < SCREEN_WIDTH - width) {
//             pos += speed;
//         }
//         draw();
//     }
// };

// Paddle paddle;

// void setup()
// {
//     tft.begin();
//     tft.setRotation(1);
//     tft.fillScreen(ILI9341_BLACK);

//     pinMode(BTN_LEFT, INPUT_PULLUP);
//     pinMode(BTN_RIGHT, INPUT_PULLUP);
// }

// void loop()
// {
//     paddle.update();
//     delay(15);
// }