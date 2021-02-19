#include "PololuLedStrip_XP.h"
#include <EEPROM.h>

// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<11> ledStrip11; //pin 11

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT 100
//WARNING: avoid 5 to 30 (Hertz), this is the range most likely to trigger photosensitive epilepsy.
int hertz = 60; //############## Set frequency here ################
unsigned long timeUnit(){
  return micros(); //############## Set time intervals here #############
}

bool lightsOn;
unsigned long nextchangeTime;
unsigned long gUnitsPerSecond;
unsigned long gChangeInterval;
unsigned long timeInUnits;

rgb_color strip[LED_COUNT];
int counter = 0;
unsigned long startTime = 0;
unsigned long stopTime = 0;



void setup()
{
  Serial.begin(9600);
  nextchangeTime = timeUnit();
  lightsOn = false;
  //Time Units Per Second
  startTime = timeUnit();
  unsigned long oneMillion = 1000000;
  delay(1000);
  //realDelayMicroseconds(oneMillion);
  stopTime = timeUnit();
  gUnitsPerSecond = stopTime - startTime;
  
  gChangeInterval = gUnitsPerSecond/(hertz*2); //*2 because flipping off and on is 1hz
  Serial.print("\nTime units per second: "); 
  Serial.print(gUnitsPerSecond);
  Serial.print(" | Change interval: "); 
  Serial.print(gChangeInterval);
  Serial.print("\n###########\n");
  Serial.print(startTime);
  Serial.print("\n");
  Serial.print(stopTime);
  Serial.print("\n###########\n");
  startTime = timeUnit();
}

void loop()
{  
  timeInUnits = timeUnit();
  if(timeInUnits >= nextchangeTime || timeInUnits <= gChangeInterval){ //in case clock gets reset
    nextchangeTime = timeInUnits + gChangeInterval;
    for (uint16_t i = 0; i < LED_COUNT; i++)  {
      //if((counter >> i) & 1){
      if(lightsOn){
        strip[i] = (rgb_color) {150, 150, 0};
      }
      else{
        strip[i] = (rgb_color) {0, 0, 0};
      }
    }
    ledStrip11.write(strip, LED_COUNT);
  
    lightsOn = not(lightsOn);
    counter++;
  
    if(counter % 10 == 0 && counter <= 50){
      stopTime = timeUnit();
      //Serial.print("\n passed 100\n");
      
      Serial.print("\nChange interval: "); 
      Serial.print(gChangeInterval);
      Serial.print(" | Delay: ");
      Serial.print(stopTime - startTime);
      Serial.print("| Stopped at: "); Serial.print(stopTime);
      
      startTime = timeUnit();
    }
  }
  
  
}

void realDelayMicroseconds(unsigned long pTime){
  unsigned long counter;
  while(counter < pTime){
    delayMicroseconds(100);
    counter += 100;
  }
}

int TimeUnitsPerExecution(){
  startTime = timeUnit();
  lightsOn = not(lightsOn);
  nextchangeTime = timeInUnits + gChangeInterval;
  for (uint16_t i = 0; i < LED_COUNT; i++)  {
      //if((counter >> i) & 1){
      if(lightsOn){
        strip[i] = (rgb_color) {150, 150, 0};
      }
      else{
        strip[i] = (rgb_color) {0, 0, 0};
      }
    }
    ledStrip11.write(strip, LED_COUNT);
  stopTime = timeUnit();
  return stopTime - startTime;
}

void printMicros(){
    bool test;
    Serial.print("\nmicros are: ");
    Serial.print(stopTime-startTime);
}

void HandleCounter(int counter){
  if(counter == 1){
    Serial.print("\nbeginning:");
  }
  else if(counter == 1000){
    startTime = millis();
  }
  else if(counter == 1100){
    stopTime = millis();
    Serial.print("Millis beween are: ");
    Serial.print(stopTime-startTime);
    Serial.print("\ncounter: ");
    Serial.print(counter);
    Serial.print("\n");
    String myString = "Binary: ";
    myString.concat((counter >> 4) & 1);
    myString.concat((counter >> 3) & 1);
    myString.concat((counter >> 2) & 1);
    myString.concat((counter >> 1) & 1);
    myString.concat((counter >> 0) & 1);
   Serial.print(myString);
    counter = 0;
  }
}
