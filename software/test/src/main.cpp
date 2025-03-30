#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 80
CRGB leds[NUM_LEDS];

int adcval;

#define eFuse_SHDN    32
#define DATA_ESP      14
#define Button3       33
#define eFuse_Fault   27
#define LED_PIN1      19
#define LED_PIN2      21
#define LVL_GATE      12
#define V_SNS         34
#define I_SNS         25

void setup() {
  // Serial UART
  Serial.begin(115200);
  Serial.println("test");

  // LED library
  FastLED.addLeds<NEOPIXEL, DATA_ESP>(leds, NUM_LEDS);

  // generic gpio
  pinMode(eFuse_SHDN,OUTPUT);
  pinMode(eFuse_Fault, INPUT);
  pinMode(Button3, INPUT);
  pinMode(LED_PIN1, OUTPUT); // make led pin output mode
  pinMode(LED_PIN2, OUTPUT); // make led pin output mode
  pinMode(LVL_GATE, OUTPUT); // make led pin output mode
  pinMode(V_SNS, ANALOG);
  pinMode(I_SNS, ANALOG);

  //always high
  digitalWrite(eFuse_SHDN, 0);
  digitalWrite(LED_PIN1, 1);
  digitalWrite(LED_PIN2, 1);
  digitalWrite(LVL_GATE, 1);

}


void loop() {
for (int i = 0; i<NUM_LEDS;i++)
{
  leds[i] = CRGB::Orange; 
  FastLED.show(); 
  delay(500);
}

for (int i = (NUM_LEDS-1); i>=0;i--)
{
  leds[i] = CRGB::DarkCyan; 
  FastLED.show(); 
  delay(500);
}

adcval = analogRead(V_SNS);

Serial.print("ADC Value = ");
Serial.print(adcval);

  
}
