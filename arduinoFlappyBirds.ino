#include <Wire.h>
#include <LiquidCrystal.h>
// #include <LiquidCrystal_I2C.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  // Initialize the LCD display

#define buttonPin 8  // Define the button pin, tells the arduino what pin the button is connected to

// The pipe object is defined as an array of bytes.
// It represents a custom character that will be displayed on an LCD screen.
// The pipe array is initialized with 8 elements, each representing a row of the character.
// Each row is defined using a binary number that represents the pattern of pixels in that row.
// The binary numbers are written in the form of 0bXXXXXXXX, where each X represents a pixel.
// For example, the first row of the pipe character is defined as 0b11111, which means that all 5 pixels in that row are turned on.
// The second row is also 0b11111, indicating that all 5 pixels in that row are turned on as well.
// The following rows have a similar pattern, forming the shape of a pipe.
// By defining the pipe character in this way, it can be easily displayed on the LCD screen by sending the corresponding byte values to the LCD library.
// The LCD library will interpret these byte values and display the custom character accordingly.

byte pipe[8] = {  // Define the pipe obstacl
  0b00000,
  0b11111,
  0b11111,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110
};

byte bird[8] = {  // Define the bird character
  0b00000,
  0b00110,
  0b10111,
  0b11110,
  0b11110,
  0b01100,
  0b00000,
  0b00000
};

byte hit[8] = {  // Define the hit marker
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

byte heart[8] = {  // Define the heart symbol
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};


int pipes[16];        // Array to store the pipe positions
int birdY = 2;        // Initial bird position on the Y-axis
int birdX = 7;        // Initial bird position on the X-axis
int score = 0;        // Current score
int speedlvl = 1400;  // Speed of pipes// byte bird[8] = { // Define the bird character

int life = 3;  // Number of lives

void setup() {
  // lcd.backlight();  only needed for I2C use
  Serial.begin(9600);  // Start the serial communication, used for debugging,
                       // not needed for the game,
                       // but can be useful to see the score in the serial monitor

  lcd.begin(16, 2);  // Initialize the LCD display with 16 columns and 2 rows

  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as an input and enable the internal pull-up resistor
}

void birdfly() {
  int buttonValue = digitalRead(buttonPin);  // Read the button value
  int charId = 0;

  if (buttonValue == LOW) {  // If the button is pressed
    birdY = 0;               // Move the bird up
    charId = 1;              // Set the character ID to display the bird
  } else {
    birdY = 1;                // Move the bird down
    charId = 1;               // Set the character ID to display the bird
    if (pipes[birdX] == 1) {  // If the bird hits a pipe, does this by checking if the pipe is at the same position as the bird
      charId = 2;             // Set the character ID to display the hit character
      life--;                 // Decrease the number of lives
      score--;                // Decrease the score
    }
  }

  lcd.setCursor(birdX, birdY);  // Set the cursor position on the LCD display
  lcd.write((byte)charId);      // Display the character
}

void movePipes() {
  int dist = random(0, 2);  // Generate a random number to determine the pipe distance
  // Move the pipes along the screen
  int i;
  for (i = 1; i < 16; i++) {
    pipes[i - 1] = pipes[i];  // Move the pipes to the left
  }

  if (dist == 1 && (pipes[16] == 2 || pipes[13] == 1)) {
    pipes[15] = 0;  // Set the pipe position to 0
  } else {
    pipes[15] = random(0, 2);
  }

  for (i = 0; i < 16; i++) {
    if (pipes[i] != 0) {
      lcd.setCursor(i, 1);  // Set the cursor position on the LCD display
      lcd.write((byte)0);   // Display the pipe character
    }
  }
}

void showLives() {
  int i;
  for (i = 0; i < life; i++) {
    lcd.setCursor(i, 0);  // Set the cursor position on the LCD display
    lcd.write((byte)3);   // Display the heart character
  }
}

void showScore() {
  lcd.setCursor(13, 0);  // Set the cursor position on the LCD display
  lcd.print(score);      // Display the score
}

bool gameStarted = false;

void loop() {
  lcd.clear();  // Clear the LCD display

  if (!gameStarted) {
    // Display the start screen
    lcd.setCursor(3, 0);            // Set the cursor position on the LCD display
    lcd.print("Flappy Bird");       // Display "Flappy Bird"
    lcd.setCursor(0, 1);            // Set the cursor position on the LCD display
    lcd.print("Press the button");  // Display "Press the button"

    // Check if the button is pressed
    if (digitalRead(buttonPin) == LOW) {
      // If the button is pressed, start the game
      gameStarted = true;
    }
  } else if (life > 0) {  // If there are lives remaining
    movePipes();          // Move the pipes
    birdfly();            // Make the bird fly
    showLives();          // Display the remaining lives
    showScore();          // Display the score
    score++;              // Increase the score
    speedlvl--;           // Decrease the speed level
  } else {
    lcd.setCursor(3, 0);     // Set the cursor position on the LCD display
    lcd.print("GAME OVER");  // Display "GAME OVER"
    lcd.setCursor(3, 1);     // Set the cursor position on the LCD display
    lcd.print("Score: ");    // Display "Score: "
    lcd.print(score);        // Display the final score
  }

  delay(speedlvl);  // Delay for the specified speed level
}