#include <Arduino.h>
#include <Adafruit_CAP1188.h>
#include "wifi_setup.h"
#include "server.h"
// Optional LED display
#include <TFT_eSPI.h>

#define LED_PIN 2
#define LIGHT_INPUT 33  // Don't have the photoresistor
#define PWM_CHANNEL 0
#define BUTTON 26

const int pwm_freq = 5000;
const int resolution = 8;

// State 0 for TOUCH
// State 1 for Auto-brightness
int state = 0;  

//I2C Connection
Adafruit_CAP1188 cap = Adafruit_CAP1188();

// LED Display object
TFT_eSPI tft = TFT_eSPI();

// Number of touch sensors touched
int light_level = 0;
int last_duty = 0;


void setup() {
  // Set Baudrate
  Serial.begin(9600);

  tft.init();
  tft.fillScreen(0xF81F);
  tft.setTextColor(0x07FF);
  tft.setTextSize(10);
  Serial.printf("TTGO Initialized\n");
  

  // Setup LED PWM functionalities
  ledcSetup(PWM_CHANNEL, pwm_freq, resolution);
  // Connect LED Pin to the PWM Channel
  ledcAttachPin(LED_PIN, PWM_CHANNEL);
  
  // Attach interrupt
  pinMode(BUTTON, INPUT);
  //attachInterrupt(BUTTON, ISR, FALLING);
  
  delay(500);
  // Error check 
  if(!cap.begin())
  {
    Serial.println("CAP1188 not found");
    while(1);
  }
  delay(500);
  enable_wifi();
}

void loop() 
{
  
  tft.setTextColor(0x07FF);
  tft.setTextSize(20);
  tft.setCursor(50, 50);

  int current_duty = light_level*31;
  if (current_duty != 0 && current_duty != last_duty)
  {
    last_duty = current_duty;
  }
  
  light_level = 0;
  
  // Touch value
  if (state == 0)
  {
    // Serial.printf("last_duty: %d, current_duty: %d\n", last_duty, current_duty);
    // For I2C
    uint8_t touched = cap.touched();

    for (uint8_t i =0; i < 8; i++)
    {
      if (touched & (1 << i))
      {
        light_level = i+1;  // +1 to start from 1 not 0 
      }
    }
  }
  // Serial.printf("Number of sensors touched: %d\n", counter);
  ledcWrite(PWM_CHANNEL, last_duty);
  
  tft.printf("%d", last_duty/31);
  // Auto brightness
  if (state == 1)
  {
    int pr_val = analogRead(LIGHT_INPUT);
    //Serial.printf("Photoresistor value: %d\n", pr_val);
    light_level = 8 - (pr_val / 512);
  }
  //Serial.printf("light level (number of sensors touched): %d\n", light_level);
  //delay(100);
  tft.fillScreen(0xF81F);
  

if (digitalRead(BUTTON))
{
  Serial.println("Button Pressed");
  if (state == 1)
    state = 0;
  else
    state = 1;
  Serial.println("State: " + String(state));
}
readESP(last_duty/31, state);
}

