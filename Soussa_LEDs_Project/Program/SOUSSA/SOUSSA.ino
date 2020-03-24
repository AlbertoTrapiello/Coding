#include <FastLED.h>
#include <Streaming.h>

#define LED_PIN     2
//#define CLOCK_PIN 4
#define NUM_LEDS    1
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define BRIGHTNESS  255

#define pinHue A0
#define pinSat A1
#define pinVal A2


CRGB leds[NUM_LEDS];



void setup() {
    Serial.begin(9600);
    
    delay(3000);
    LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    //FastLED.setTemperature(Candle);
    FastLED.setTemperature(0xFF7029);
    //Candle 0xFF9329 seems OK
    //Tungsten40W 0xFFC58F way to blue
    //Tungsten100W 0xFFD6AA way to blue
    //Halogen 0xFFF1E0 way to blue
    //CarbonArc 0xFFFAF4 way to blue
    //HighNoonSun 0xFFFFFB way to blue
    //DirectSunlight 0xFFFFFF way to blue
    // OvercastSky 0xC9E2FF way to blue
    //ClearBlueSky 0x409CFF way to blue
    // WarmFluorescent 0xFFF4E5 way to blue
    // StandardFluorescent 0xF4FFFA way to blue
    // MercuryVapor 0xD8F7FF way to blue
    //SodiumVapor 0xFFD1B2 way to blue
    // MetalHalide 0xF2FCFF way to blue
    // HighPressureSodium 0xFFB74C better but a a bit greenish blue
    // UncorrectedTemperature 0xFFFFFF  way to blue
    // Custom 0xFF8029 getting there but still a bit greenish > reduced green(?) in Candle 0xFF9329 : Candle 0x FF 93 29 > Candle 0x FF 80 29
    // Custom 0xFF6029 nice white with a very slight pink touch
    // Custom 0xFF5029 getting more pink, no good
    // Custom 0xFF7029 Yup, looks like a nice white for my leds !
    FastLED.setBrightness(BRIGHTNESS);
 
    pinMode(pinHue, INPUT);
    pinMode(pinSat, INPUT);
    pinMode(pinVal, INPUT);
}

void loop()
{
  byte byteHue = map(analogRead(pinHue), 0, 1023, 0, 255);
  byte byteSat = map(analogRead(pinSat), 0, 1023, 0, 255);
  byte byteVal = map(analogRead(pinVal), 0, 1023, 0, 255);

  Serial << byteHue << "  " <<  byteSat << "  " << byteVal << endl;
  
  CHSV color = CHSV(byteHue, byteSat, byteVal);

  fill_solid(leds, NUM_LEDS, color);
  
  FastLED.show();
  
}
