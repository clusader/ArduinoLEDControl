#include "LEDControl.h"

LEDControl::LEDControl() {

  void LEDControl::init() {
    FastLED.clearData();
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  //leds
    //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds2, NUM_LEDS);  //leds2
  }
};