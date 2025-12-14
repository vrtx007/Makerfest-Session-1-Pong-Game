#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_DC 2
#define TFT_CS 15
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;
const int BLOCK_SIZE = 10; 
const int GRID_W = SCREEN_WIDTH / BLOCK_SIZE; 
const int GRID_H = SCREEN_HEIGHT / BLOCK_SIZE; 

#define BTN_LEFT 5
#define BTN_RIGHT 17

bool leftPressed = false;
bool rightPressed = false;

struct Point {
    int x, y;
};

class Food {
public:
    Point pos; 

    void spawn() {
      
        pos.x = random(1, GRID_W - 2); 
        pos.y = random(3, GRID_H - 2); 
    }

    void draw() {
        int px = pos.x * BLOCK_SIZE;
        int py = pos.y * BLOCK_SIZE;
        tft.fillRect(px, py, BLOCK_SIZE, BLOCK_SIZE, ILI9341_RED);
        tft.fillRect(px + 4, py - 2, 2, 2, ILI9341_GREEN); 
    }
};

// ---------------- THE SNAKE CLASS ----------------
class Snake {
public:
    Point body[200]; 
    int length;
    int dirX, dirY;  
    int color;

    void init() {
        length = 3; 
        color = ILI9341_GREEN;
        
        // Start in the middle
        body[0] = {15, 12}; // Head
        body[1] = {14, 12}; // Body
        body[2] = {13, 12}; // Tail

        // Start moving Right
        dirX = 1; 
        dirY = 0;
    }

    void drawWhole() {
        for (int i = 0; i < length; i++) {
            tft.fillRect(body[i].x * BLOCK_SIZE, body[i].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, color);
        }
    }

    void turnLeft() {
        // Rotate 90 degrees CCW
        int oldX = dirX;
        dirX = dirY;
        dirY = -oldX;
    }

    void turnRight() {
        // Rotate 90 degrees CW
        int oldX = dirX;
        dirX = -dirY;
        dirY = oldX;
    }

    void move() {
        // 1. Erase Tail
        Point tail = body[length - 1];
        tft.fillRect(tail.x * BLOCK_SIZE, tail.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, ILI9341_BLACK);

        // 2. Shift Body
        for (int i = length - 1; i > 0; i--) {
            body[i] = body[i - 1];
        }

        // 3. Move Head
        body[0].x += dirX;
        body[0].y += dirY;

        // 4. Draw New Head
        tft.fillRect(body[0].x * BLOCK_SIZE, body[0].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, color);
    }

    void grow() {
        if (length < 199) {
            body[length] = body[length - 1]; // Duplicate tail
            length++;
        }
    }

    bool checkSelfCollision() {
        for (int i = 1; i < length; i++) {
            if (body[0].x == body[i].x && body[0].y == body[i].y) return true;
        }
        return false;
    }

    bool checkWallCollision() {
        if (body[0].x < 0 || body[0].x >= GRID_W) return true;
        if (body[0].y < 2 || body[0].y >= GRID_H) return true; 
        return false;
    }
};

// ---------------- THE GAME CLASS ----------------
// ---------------- THE GAME CLASS ----------------
class SnakeGame {
public:
    Snake snake;
    Food food;
    int score;
    bool gameOver;

    SnakeGame() {
        // Constructor left empty, we init in setup()
    }

    void reset() {
        tft.fillScreen(ILI9341_BLACK);
        
        // Draw Score Bar Background
        tft.fillRect(0, 0, SCREEN_WIDTH, 20, ILI9341_BLUE);
        tft.drawRect(0, 20, SCREEN_WIDTH, SCREEN_HEIGHT - 20, ILI9341_WHITE);
        
        snake.init();
        snake.drawWhole(); 
        
        food.spawn();
        food.draw();
        
        score = 0;
        gameOver = false; // We are back in action
        drawScore();
        
        // Clear any button presses that happened during the Game Over screen
        leftPressed = false;
        rightPressed = false;
    }

    void drawScore() {
        tft.setCursor(5, 5);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLUE);
        tft.setTextSize(2);
        tft.print("Score: ");
        tft.print(score);
    }

    void showGameOver() {
        // 1. Draw the static Red Screen ONCE
        tft.fillScreen(ILI9341_RED);
        tft.setCursor(60, 100);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(4);
        tft.print("GAME OVER");
        
        tft.setCursor(80, 150);
        tft.setTextSize(2);
        tft.print("Final Score: ");
        tft.print(score);

        // 3. Set the flag to stop the game loop
        gameOver = true;
    }

    void update() {
        if (gameOver) {
            if (leftPressed || rightPressed) {
                reset(); 
            }
            return; 
        }
 
        if (leftPressed) {
            snake.turnLeft();
            leftPressed = false;
        } else if (rightPressed) {
            snake.turnRight();
            rightPressed = false;
        }

        snake.move();

        if (snake.checkWallCollision() || snake.checkSelfCollision()) {
            showGameOver(); // Just show the screen and set flag
            return;
        }

        // Check Food
        if (snake.body[0].x == food.pos.x && snake.body[0].y == food.pos.y) {
            snake.grow();
            score++;
            drawScore();
            food.spawn();
            food.draw();
        }
    }
};

SnakeGame game;

// ---------------- SETUP & LOOP ----------------
void setup() {
    Serial.begin(115200); // Debugging
    
    tft.begin();
    tft.setRotation(1);
    
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
    
    randomSeed(analogRead(0));

    // Call reset AFTER display init
    game.reset(); 
}

unsigned long lastUpdate = 0;
const int GAME_SPEED = 150; // Speed of snake (lower is faster)

void loop() {
    // Read Inputs (Must be outside the time check for responsiveness)
    if (!digitalRead(BTN_LEFT)) leftPressed = true;
    if (!digitalRead(BTN_RIGHT)) rightPressed = true;

    unsigned long now = millis();
    if (now - lastUpdate > GAME_SPEED) {
        game.update();
        lastUpdate = now;
    }
}