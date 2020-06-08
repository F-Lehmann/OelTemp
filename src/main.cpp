#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#define TEMPPIN 32
TFT_eSPI tft = TFT_eSPI();

double adcMax = 4095.0;
double Vs = 3.3;
double R1 = 9860.0;   // voltage divider resistor value
double Beta = 3912.263325;  // Beta value
double To = 298.15;    // Temperature in Kelvin for 25 degree Celsius
double Ro = 926.71;   // Resistance of Thermistor at 25 degree Celsius

int measure(){
  int sum=0;
  for( int i = 0; i<16; i++){
    sum += analogRead(TEMPPIN);
    delay(10);
  }
  return sum/16;
}

void setup() {
  Serial.begin(115200);
  tft.begin();
}

void loop() { 
  double Vout = measure()/adcMax*Vs;
  double Rt = R1 * Vout / (Vs - Vout);
  double T = 1/(1/To + log(Rt/Ro)/Beta);  // Temperature in Kelvin
  double Tc = T - 273.15;                 // Celsius
  tft.setCursor(0,0);
  tft.printf("volt: %f\nRt: %f\nTk: %f\nTc: %f",Vout,Rt,T,Tc);
}