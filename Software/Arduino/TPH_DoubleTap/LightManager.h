///////////////////////////////////////////////////////////////////
// Class controlling the light and its effects
//
//
// This code is under A Creative Commons Attribution/Share-Alike License
// http://creativecommons.org/licenses/by-sa/4.0/
// 2018, Imanol Gomez
///////////////////////////////////////////////////////////////////

#pragma once
#include "Arduino.h"
#include "FastLED.h"

#define NUM_LEDS 20
#define DATA_PIN 12


class LightManager{

  public:
    
    LightManager();
    
    void setup();
    void update();

    void setColor(int red, int green, int blue);
    void setColorHSV(int h, int s, int v);
    void setColorHue(int hue);

    void setFading(bool fading);

    void toggleOnOff();
    
    void turnOn();
    void turnOff();
   
  private:

    void HSV_to_RGB(int h, int s, int v, int* colors);
    void fadeColor();

    CRGB leds[NUM_LEDS];
    
    bool isFading;
    bool fadeDirection;
    float h, s, v;

    bool lightOn;

};

LightManager::LightManager()
{
   isFading= false;
   fadeDirection = true;
   v = 255;
   s = 255;
   h = 0;
   lightOn =true;
}

void LightManager::setup()
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    this->setColor(255,255,255);

    pinMode(LED_BUILTIN, OUTPUT);
    //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
}

void LightManager::update()
{
    fadeColor();
}


void LightManager::toggleOnOff()
{
    if(lightOn){
      turnOff();
    }
    else{
       turnOn();
    }

    Serial.print("LightManager::toggleOnOff -> ");  Serial.println(lightOn);
}
void LightManager::turnOn()
{
   lightOn = true;
   this->setColor(255,255,255);
   digitalWrite(LED_BUILTIN, HIGH);
}

void LightManager::turnOff()
{
   this->setColor(0,0,0);
   lightOn = false;
   digitalWrite(LED_BUILTIN, LOW);
}

void LightManager::fadeColor()
{
    if(!isFading){
       return;
    }

    
    if(fadeDirection)
    {
        v = v - 1;
        setColorHSV(h, s, v);

        if(v <= 0){
         // v = 0;
          fadeDirection = !fadeDirection;
        }
    }
    
    else
    {
        v = v + 1;
        setColorHSV(h, s, v);

        if(v >= 255){
         // v = 100;
          fadeDirection = !fadeDirection;
        }
    }

    // Serial.print("fadeDirection -> ");  Serial.println(fadeDirection);
     Serial.print("fadeColor -> ");  Serial.println(v);
}


void LightManager::setFading(bool fading) 
{
    isFading = fading;
    //Serial.print("LightManager::isFading -> ");  Serial.println(isFading);
}


void LightManager::setColor(int red, int green, int blue)
{

  if(!lightOn){
    return;
  }
  
  for(int i=0; i<NUM_LEDS; i++){
    leds[i].setRGB(red, green, blue);
  }
  
  FastLED.show();
  
  Serial.print("LightManager::setColor -> r: ");  Serial.print(red);
  Serial.print(",g: ");  Serial.print(green);
  Serial.print(",b : ");  Serial.println(blue);
}



void LightManager::setColorHue(int hue)
{
    if(!lightOn){
      return;
    }
    
    h = hue;    
    
    for(int i=0; i<NUM_LEDS; i++){
      leds[i].setHSV(h, s, v);
    }
    
    FastLED.show();
    
    Serial.print("LightManager::setColor -> h: ");  Serial.print(h);
    Serial.print(",s: ");  Serial.print(s);
    Serial.print(",v : ");  Serial.println(v);
}
void LightManager::setColorHSV(int h_, int s_, int v_)
{

   if(!lightOn){
    return;
  }

  h = h_;
  s = s_;
  v = v_;


  for(int i=0; i<NUM_LEDS; i++){
    leds[i].setHSV(h, s, v);
  }
  
  FastLED.show();
  
  Serial.print("LightManager::setColor -> h: ");  Serial.print(h);
  Serial.print(",s: ");  Serial.print(s);
  Serial.print(",v : ");  Serial.println(v);
  
}



void LightManager::HSV_to_RGB(int hue, int sat, int bright, int* colors)
{
  // constrain all input variables to expected range
    hue = constrain(hue, 0, 360);
    sat = constrain(sat, 0, 100);
    bright = constrain(bright, 0, 100);

  // define maximum value for RGB array elements
  float max_rgb_val = 255;

  // convert saturation and brightness value to decimals and init r, g, b variables
    float sat_f = float(sat) / 100.0;
    float bright_f = float(bright) / 100.0;        
    int r, g, b;
    
    // If brightness is 0 then color is black (achromatic)
    // therefore, R, G and B values will all equal to 0
    if (bright <= 0) {      
        colors[0] = 0;
        colors[1] = 0;
        colors[2] = 0;
    } 
  
  // If saturation is 0 then color is gray (achromatic)
    // therefore, R, G and B values will all equal the current brightness
    if (sat <= 0) {      
        colors[0] = bright_f * max_rgb_val;
        colors[1] = bright_f * max_rgb_val;
        colors[2] = bright_f * max_rgb_val;
    } 
    
    // if saturation and brightness are greater than 0 then calculate 
  // R, G and B values based on the current hue and brightness
    else {
        
        if (hue >= 0 && hue < 120) {
      float hue_primary = 1.0 - (float(hue) / 120.0);
      float hue_secondary = float(hue) / 120.0;
      float sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
      float sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
      float sat_tertiary = 1.0 - sat_f;
      r = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
      g = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
      b = (bright_f * max_rgb_val) * sat_tertiary;  
        }

        else if (hue >= 120 && hue < 240) {
      float hue_primary = 1.0 - ((float(hue)-120.0) / 120.0);
      float hue_secondary = (float(hue)-120.0) / 120.0;
      float sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
      float sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
      float sat_tertiary = 1.0 - sat_f;
      r = (bright_f * max_rgb_val) * sat_tertiary;  
      g = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
      b = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
        }

        else if (hue >= 240 && hue <= 360) {
      float hue_primary = 1.0 - ((float(hue)-240.0) / 120.0);
      float hue_secondary = (float(hue)-240.0) / 120.0;
      float sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
      float sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
      float sat_tertiary = 1.0 - sat_f;
      r = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
      g = (bright_f * max_rgb_val) * sat_tertiary;  
      b = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
        }
        
        colors[0]=r;
        colors[1]=g;
        colors[2]=b;
    
    }

//  Serial.print("LightManager::setColor -> r: ");  Serial.print( colors[0]);
//  Serial.print(",g: ");  Serial.print( colors[1]);
//  Serial.print(",b : ");  Serial.println( colors[2]);
}


