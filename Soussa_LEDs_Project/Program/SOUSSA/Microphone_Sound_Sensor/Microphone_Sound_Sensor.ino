#include <Filters.h>
#include <math.h>

#define OFFSET 2.50 
#define OFFSET_dB 38

int AnalogPin = A0;
int DigitalPin = 3;
int Buzzer = 9;

void setup()
{
  // put your setup code here, to run once:
  pinMode(AnalogPin, INPUT);
  pinMode(DigitalPin, INPUT);
  pinMode(Buzzer, OUTPUT);
  
  Serial.begin(2000000);
}

float actual_time = 0;
float previous_time = 0;
float TIME_INC = 2000;
int freq = 1;

void loop()
{
  // put your main code here, to run repeatedly:
  // filters out changes faster that 5 Hz.
  float LowfilterFrequency = 20000;
  float HighfilterFrequency = 20;

// create a one pole (RC) lowpass filter
//  FilterOnePole lowpassFilter( LOWPASS, LowfilterFrequency );
//  FilterOnePole highpassFilter( HIGHPASS, HighfilterFrequency );
  FilterTwoPole TwoPoleFilter( 20, 1, OFFSET);
  
  //float FilteredSignal = TwoPoleFilter.input(analogRead(AnalogPin));
  
  float VoltageRead = analogRead(AnalogPin) * (5.0/1023.0); 
  // do something else
  
 /*
  tone(Buzzer, freq);  
  
  if((millis() - previous_time) >= TIME_INC)  
  {
    freq = (freq + 100)%1000; 
    previous_time = millis();
    Serial.print("--------");
    Serial.print(freq);
    Serial.print("--------");
    delay(500);
  } 
*/
//  if(digitalRead(DigitalPin))
//   Serial.println("Extreme value reached");

/*      
  float Radius = 0.009;  
  float Surface = 2 * 3.141592 * Radius * Radius;
  float Resistor = 24000;  
  float Power = VoltageRead*VoltageRead/Resistor;
  float Intensity = Power/Surface;
  float dB = 10*log10(Intensity/pow(10, -12));
*/
  // 2.55 V equals (aproximately) to 38 dB;
  // float IncdB =  10*log((5 - VoltageRead)/OFFSET);
  // float dB = OFFSET_dB + IncdB;
//  Serial.print("Voltage: ");
//  if((VoltageRead - OFFSET)*100 > 5)
//  {
    //Serial.println(dB);
    
  Serial.print(millis());
  Serial.print(";");
  Serial.println(VoltageRead - OFFSET);
//  }
//  else
//   Serial.println(0);

}
