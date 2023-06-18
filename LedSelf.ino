#include <FastLED.h>
//#include <LEDControl.h>
#define NUM_LEDS 30
#define DATA_PIN 5
#define BUTTON_PIN 6
#define MAX_BRIGHT 70

CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds2;

bool power = true;
int btn_count = 1;

LEDControl strip;
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  strip.init();
  //FastLED.clearData();

  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  //leds
  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds2, NUM_LEDS);  //leds2
}



byte prevState;
int current_mode = 0;
int total_modes = 1;  //n+1, soda not working
void loop() {
  // Turn the first led red for 1 second
  // leds[0] = CRGB::Red;
  // FastLED.show();

  byte buttonState = digitalRead(BUTTON_PIN);
  int states = 2 + total_modes;

  if (prevState != 0 && buttonState != prevState) {
    if (buttonState == LOW) {
      // Serial.println("Button is pressed");
      btn_count = (btn_count + 1) % states;
      switch (btn_count) {
        case 0:
          turnOff();
          break;
        case 1:
          power = true;
          current_mode = 0;
          break;
        case 2:
          current_mode = 1;
          break;
        case 3:
          current_mode = 2;
          break;
      }
    } else {  //button at rest
    }
  }

  prevState = buttonState;
  if (!power) return;
  switch (current_mode) {
    case 0:
      rainbowSeesaw();
      break;
    case 1:
      flowHue();
      break;
      // case 2:
      //   sodaBubbles();
      //   break;
  }


  //leds2(0, NUM_LEDS).fill_gradient(start_hue,end_hue,LONGEST_HUES);
  //for (int i = 0; i < NUM_LEDS; i++) leds[i].maximizeBrightness(10);
  //FastLED.show();
}

void turnOff() {
  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS*3);  //leds
  FastLED.clear(true);
  power = false;
  // for (int i = 0; i < NUM_LEDS; i++) leds[i].setRGB(0, 0, 0);
  // FastLED.clearData();
}

int iter = 0, q1 = 1;
void rainbowSeesaw() {
  for (int i = 0; i < NUM_LEDS; i++) {
    //leds[i].setRGB(255 + (q1 > 0 ? -iter % 255 : -iter % 255), 200, 20);
    leds[i].setHue(abs(255 - iter - i * 7) % 255);
    leds[i].maximizeBrightness(MAX_BRIGHT);
  }


  FastLED.show();
  delay(25);
  if (q1 > 0) {
    iter += 2;
    if (iter >= 250) q1 *= -1;
  } else if (q1 < 0) {
    iter -= 2;
    if (iter <= 10) q1 *= -1;
  }
}

float flow_hue = 0;
void flowHue() {
  int factor = 200 / NUM_LEDS;
  flow_hue += 1;
  for (int i = 0; i < NUM_LEDS; i++) {
    //leds[i].setRGB(255 + (q1 > 0 ? -iter % 255 : -iter % 255), 200, 20);
    leds[i].setHue(flow_hue);
    leds[i].setHSV(flow_hue, 255 - i * factor, MAX_BRIGHT);
    // leds[i].maximizeBrightness(7);
  }
  FastLED.delay(20);
  FastLED.show();
  //Serial.println(flow_hue);
  //if (flow_hue >= 254) flow_hue = 0;
}

int wave_px = 10;
CHSV start_hue = CHSV(150, 255, 3);
CHSV end_hue = CHSV(150, 0, 3);
void sodaBubbles() {
  // // 110 -> 160 ~~
  // for (int i = 0; i < NUM_LEDS; i++) leds[i].setHue(150);

  // for (int i = wave_px; i < wave_px + 5; i++) {if (i < NUM_LEDS) {leds[i].setHSV(150, 150 - (wave_px - i) * 30, 7);}}
  // for (int i = wave_px; i > wave_px - 5; i--) {if (i >= 0) {leds[i].setHSV(150, 150 + (wave_px - i) * 30, 7);}}

  // for (int i = 0; i < NUM_LEDS; i++) leds[i].maximizeBrightness(7);
  leds2(0, NUM_LEDS).fill_solid(start_hue);
  leds2(wave_px, (wave_px + 5 > NUM_LEDS) ? NUM_LEDS : wave_px + 5).fill_gradient(start_hue, end_hue, LONGEST_HUES);
  leds2(wave_px, (wave_px - 5 < 0) ? 0 : wave_px - 5).fill_gradient(start_hue, end_hue, LONGEST_HUES);

  FastLED.delay(20);
  FastLED.show();

  srandom(millis());
  Serial.println(random(0, 2));
  if (random(0, 2) > 0) wave_px = (wave_px + 1) % NUM_LEDS;
}
