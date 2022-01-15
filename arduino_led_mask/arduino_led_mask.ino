#include <FastLED.h>

// init LEDs
#define LED_PIN 8
#define NUM_LEDS 84
#define LED_TYPE WS2812B
#define BRIGHTNESS 20

// init SOUND
#define SOUND_PIN 2
bool SoundInput;
unsigned long LEDon;
unsigned long LEDdelay;
unsigned long LEDoff;

CRGB leds[NUM_LEDS];

void setup()
{
  FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(4.5, 500);
  FastLED.show();

// baud rate
  Serial.begin(9600);
}

// one static mask design
void loop()
{
  uint_least8_t circlePattern[NUM_LEDS] = {
     3, 4, 0, 1, 1, 1, 1, 0, 4, 3,
    3, 4, 0, 1, 2, 2, 2, 1, 0, 4, 3,
     4, 0, 1, 2, 3, 3, 2, 1, 0, 4,
    4, 0, 1, 2, 3, 4, 3, 2, 1, 0, 4,
     4, 0, 1, 2, 3, 3, 2, 1, 0, 4,
    3, 4, 0, 1, 2, 2, 2, 1, 0, 4, 3,
     3, 4, 0, 1, 1, 1, 1, 0, 4, 3,
    2, 3, 4, 0, 0, 0, 0, 0, 4, 3, 2
  };

uint_least8_t circleColors[5]= { 0, 0, 0, 8, 8 }; // green and yellow

	// set LEDs depending on input from sound sensor
	SoundInput = digitalRead(SOUND_PIN);
	if (SoundInput == true){
		pattern(circlePattern, circleColors, false, 3, 5);
	}else{
		FastLED.clear();
		for (uint_least8_t i = 0; i < NUM_LEDS; i++) {
			leds[i] = CRGB::Black;
		}
	FastLED.show();
	}

}

void pattern(uint_least8_t pattern[NUM_LEDS], uint_least8_t rgbColors[5], bool reverse, float speed, uint_least8_t max) {
  uint_least8_t colors[11][3] = {
    {95, 0, 10},
    {130, 170, 23},
    {5, 100, 150},
    {120, 35, 190},
    {5, 160, 60},
    {60, 35, 150},
    {5, 100, 150},
    {0, 30, 170},
    {140, 165, 10},
    {130, 130, 130},
    {10, 100, 00}
  };
  for (uint_least8_t x = 0; x < max; x++) {
    for (uint_least8_t z = 0; z < (4 * speed); z++) {
      for (uint_least8_t i = 0; i < NUM_LEDS; i++) {
        uint_least8_t colorA;
        uint_least8_t colorB;
        if (reverse) {
          colorB = (pattern[i] + (4 - x)) % max;
          colorA = (colorB + 1) % max;
        } else {
          colorA = (pattern[i] + x) % max;
          colorB = (colorA + 1) % max;
        }
        leds[i] = CRGB(
          getColorFade(colors[rgbColors[colorA]][0], colors[rgbColors[colorB]][0], z, (4 * speed), i),
          getColorFade(colors[rgbColors[colorA]][1], colors[rgbColors[colorB]][1], z, (4 * speed), i),
          getColorFade(colors[rgbColors[colorA]][2], colors[rgbColors[colorB]][2], z, (4 * speed), i)
        );
      }
      FastLED.show();
    }
  }
}

float getColorFade(uint_least8_t a, uint_least8_t b, uint_least8_t index, uint_least8_t range, uint_least8_t i) {
  if (a == b)
  {
    return a;
  }
  uint_least8_t dif = abs(a - b);
  float change = (float)dif / range * (index + 1);
  if (a > b)
  {
    return a - change;
  }
  return a + change;
}
