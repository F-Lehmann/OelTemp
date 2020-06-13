#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#define TEMPPIN 32
#define VOLTPIN 33

TFT_eSPI tft = TFT_eSPI();

const double adcMax = 4095.0;
const double Vs = 3.3;

double measureTemp(){
  
  double R1 = 9860.0;   // voltage divider resistor value
  double Beta = 3912.263325;  // Beta value
  double To = 298.15;    // Temperature in Kelvin for 25 degree Celsius
  double Ro = 926.71;   // Resistance of Thermistor at 25 degree Celsius  
  int sum=0;
  for( int i = 0; i<16; i++){
    sum += analogRead(TEMPPIN);
    delay(10);
  }
  double Vout = ((sum/16)/adcMax)*Vs;
  double Rt = R1 * Vout / (Vs - Vout);
  double T = 1/(1/To + log(Rt/Ro)/Beta);  // Temperature in Kelvin
  double Tc = T - 273.15;                 // Celsius
  return Tc;
}

double measureVolt(){
  double R1 = 130000;
  double R2 = 27000;
  int sum=0;
  for( int i = 0; i<16; i++){
    sum += analogRead(VOLTPIN);
    delay(10);
  }
  double Vout = (((double)sum/16)/adcMax)*Vs;
  double Vin = (Vout*(R1+R2))/R2;
  Serial.println("Volt");
  Serial.printf("Sum=%i\n",sum);
  Serial.printf("Vout=%f\n",Vout);
  Serial.printf("Vin=%f\n",Vin);
  return Vin;
}

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.fillScreen(0x0);
}

void loop() { 
  tft.setCursor(0,0);
  tft.printf("Temp: %f\nVolt: %f",measureTemp(),measureVolt());
}