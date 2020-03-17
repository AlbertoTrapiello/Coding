#include <Filters.h>

void setup()
{
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  Serial.begin(19200);
}

void loop()
{
  // put your main code here, to run repeatedly:
  // filters out changes faster that 5 Hz.
  float LowfilterFrequency = 20000;
  float HighfilterFrequency = 20;
  
// create a one pole (RC) lowpass filter
//  FilterOnePole lowpassFilter( LOWPASS, LowfilterFrequency );
//  FilterOnePole highpassFilter( HIGHPASS, HighfilterFrequency );
  FilterTwoPole TwoPoleFilter( 20, 1, analogRead(A0));

  // do something else

  Serial.print("Reading: ");

//  Serial.println(highpassFilter.input(lowpassFilter.input( analogRead( A0 ) )));
  Serial.println(TwoPoleFilter.input(analogRead(A0)));

}
