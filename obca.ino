// Creator: James Campbell
// Last edit: 5Feb2021
// OBCA: One button to control them all

// Brings in the keyboard class
#include "Keyboard.h"

// Define pins used.
#define CLK 9
#define DT 10
#define SW 11
#define green_LED 3
#define blue_LED 5
#define red_LED 6
#define builtin 13

// Definition of variables used
const int buttonPin = 2;
int previousButtonState = HIGH;
int counter = 0;        
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;
int breathing_button = 0;
int delay_time = 750;

void setup() {
  
  // Set encoder pins as inputs
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(green_LED, OUTPUT);
  pinMode(blue_LED, OUTPUT);
  pinMode(red_LED, OUTPUT);
  pinMode(builtin, OUTPUT);
  // Setup Serial Monitor
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  // initialize control over the keyboard input
  Keyboard.begin();
  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);
  // Set high because counter is initialized to 0
  digitalWrite(red_LED, HIGH);
}

void loop() {
  // Used for blinking the button without halting the program to blink
  breathing_button +=1;
  Serial.println(breathing_button);
  if (breathing_button == 1){ 
    digitalWrite(builtin, HIGH);
  }else if(breathing_button == delay_time){
    digitalWrite(builtin, LOW);
  }else if(breathing_button == delay_time*2){
    breathing_button = 0;
  }

  // Read the clock state of the rotory encoder
  currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurre
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    // Check which way the encoder is rotating
    if (digitalRead(DT) != currentStateCLK) {
      // Keep the counter out of the negatives
      if(counter>=1){
      counter --;
      }
    } else {
      // Keep the value from going any higher than 3
      if(counter<2){
      counter++;
      }
    }
    
    // Determine what option is currently selected
    switch(counter){
      case 0:
        // Red light on if end call selected
        digitalWrite(blue_LED, LOW);
        digitalWrite(green_LED, LOW);
        digitalWrite(red_LED, HIGH);
        break;
      case 1:
        // Blue light on to turn on/off the camera
        digitalWrite(blue_LED, HIGH);
        digitalWrite(green_LED, LOW);
        digitalWrite(red_LED, LOW);
        break;
      case 2:
        // Green light on to turn on/off the mic
        digitalWrite(blue_LED, LOW);
        digitalWrite(green_LED, HIGH);
        digitalWrite(red_LED, LOW);
        break;
    }
  }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;

  // Get current button state
  int buttonState = digitalRead(buttonPin);
  // if the button state has changed,
  if ((buttonState != previousButtonState)&& (buttonState == HIGH)) {
    Serial.println("Button pressed");
    // Keyboard command to execute
    switch(counter){
      case 0:
        // Turn off the meeting
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press('b');
        Serial.println("Left the teams meeting");
        break;
      case 1:
        // Toggle the camera
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press('o');
        Serial.println("Turn camera off");
        break;
      case 2:
        // Toggle the mic
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_SHIFT);
        Keyboard.press('m');
        Serial.println("Changed the mic value");
        break;
    }
    // Allows for enough time to pass that allows
    // for the computer to recognize the keyboard was pressed
    delay(50);
    Keyboard.releaseAll();

  } 
  previousButtonState = buttonState;
  delay(1);
  
}
