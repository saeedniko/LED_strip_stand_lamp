#include <Arduino.h>
#include <FastLED.h>
#include "Adafruit_VL53L0X.h"

#define NUM_LEDS 139
CRGB leds[NUM_LEDS];

// Sensor
Adafruit_VL53L0X lox = Adafruit_VL53L0X();


// Mode variable
int modeNumber = 0; // 0 = OFF, 1..4 = effects
unsigned long lastSwitchTime = 0;
const unsigned long debounceDelay = 1000; // 1s to prevent too-fast switching

// Pins
#define SCL_PIN 26
#define SDA_PIN 13
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
  Serial.println("Starting...");

  // LED library
  FastLED.addLeds<NEOPIXEL, DATA_ESP>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();

  // generic gpio
  pinMode(eFuse_SHDN, OUTPUT);
  pinMode(eFuse_Fault, INPUT);
  pinMode(Button3, INPUT);
  pinMode(LED_PIN1, OUTPUT); // make led pin output mode
  pinMode(LED_PIN2, OUTPUT); // make led pin output mode
  pinMode(LVL_GATE, OUTPUT); // make led pin output mode
  // pinMode(V_SNS, ANALOG);
  // pinMode(I_SNS, ANALOG);

  // always high
  digitalWrite(eFuse_SHDN, 0);
  digitalWrite(LED_PIN1, 1);
  digitalWrite(LED_PIN2, 1);
  digitalWrite(LVL_GATE, 1);

  // Initialize I2C with your pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // Sensor init
  if (!lox.begin())
  {
    Serial.println("Failed to boot VL53L0X");
    while (1)
      ;
  }
  Serial.println("VL53L0X ready!");
}

// ----------- Effects -----------
void modeOff()
{
  FastLED.clear();
  FastLED.show();
}

void modeRainbow()
{
  static uint8_t hue = 0;
  fill_rainbow(leds, NUM_LEDS, hue++, 7);
  FastLED.show();
  delay(20);
}

void modeBlinkRandom()
{
  CRGB color = CRGB(random(0, 256), random(0, 256), random(0, 256));
  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();
  delay(200);
  FastLED.clear();
  FastLED.show();
  delay(200);
}

void modeTheaterChase()
{
  CRGB color = CRGB::Blue;
  for (int q = 0; q < 3; q++)
  {
    for (int i = 0; i < NUM_LEDS; i += 3)
    {
      leds[i + q] = color;
    }
    FastLED.show();
    delay(50);
    for (int i = 0; i < NUM_LEDS; i += 3)
    {
      leds[i + q] = CRGB::Black;
    }
  }
}

void modeWipe()
{
  CRGB color = CRGB::Green;
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = color;
    FastLED.show();
    delay(10);
  }
  delay(500);
  FastLED.clear();
  FastLED.show();
}

void loop()
{
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4)
  {                                              // valid measurement
    int distance = measure.RangeMilliMeter / 10; // mm → cm
    Serial.print("Distance (cm): ");
    Serial.println(distance);

    // Change mode if distance is between 50 and 150 cm
    if (distance >= 50 && distance <= 150 && (millis() - lastSwitchTime > debounceDelay))
    {
      modeNumber = (modeNumber + 1) % 5; // cycle 0..4
      Serial.print("Switched to mode: ");
      Serial.println(modeNumber);
      lastSwitchTime = millis();
    }
  }

  // Run current mode
  switch (modeNumber)
  {
  case 0:
    modeOff();
    break;
  case 1:
    modeRainbow();
    break;
  case 2:
    modeBlinkRandom();
    break;
  case 3:
    modeTheaterChase();
    break;
  case 4:
    modeWipe();
    break;
  }
}