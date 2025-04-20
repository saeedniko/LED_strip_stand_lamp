#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 139
CRGB leds[NUM_LEDS];

int vpval;
int vnval;

#define eFuse_SHDN 2
#define DATA_ESP 12
#define Button3 14
#define eFuse_Fault 15
#define LED_PIN1 19
#define LED_PIN2 21
#define LVL_GATE 27
#define V_SNS 36
#define I_SNS 39

void setup()
{
  // Serial UART
  Serial.begin(115200);
  Serial.println("test");

  // LED library
  FastLED.addLeds<NEOPIXEL, DATA_ESP>(leds, NUM_LEDS);
  randomSeed(analogRead(0)); // Seed randomness

  // generic gpio
  pinMode(eFuse_SHDN, OUTPUT);
  pinMode(eFuse_Fault, INPUT);
  pinMode(Button3, INPUT);
  pinMode(LED_PIN1, OUTPUT); // make led pin output mode
  pinMode(LED_PIN2, OUTPUT); // make led pin output mode
  pinMode(LVL_GATE, OUTPUT); // make led pin output mode
  //pinMode(V_SNS, ANALOG);
  //pinMode(I_SNS, ANALOG);

  // always high
  digitalWrite(eFuse_SHDN, 0);
  digitalWrite(LED_PIN1, 1);
  digitalWrite(LED_PIN2, 1);
  digitalWrite(LVL_GATE, 1);
}

void loop() {
  // Random color
  CRGB newColor = CRGB(random(0, 256), random(0, 256), random(0, 256));

  // Random brightness between 30 and 255
  uint8_t brightness = random(30, 256);  
  FastLED.setBrightness(brightness);

  // Random direction
  bool topToBottom = random(0, 2);

  if (topToBottom) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = newColor;
      FastLED.show();
      delay(20); // animation speed
    }
  } else {
    for (int i = NUM_LEDS - 1; i >= 0; i--) {
      leds[i] = newColor;
      FastLED.show();
      delay(20);
    }
  }

  delay(2000); // Wait before next change
}