#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#define TEMPPIN 32
#define VOLTPIN 33

TFT_eSPI tft = TFT_eSPI();

const double adcMax = 4095.0;
const double Vs = 3.3;

double volt[100];
double temp[100];

void schieben(double neuTemp, double neuVolt){
  for (size_t i = 0; i < 99; i++)
  {
    volt[i]=volt[i+1];
    temp[i]=temp[i+1];
  }
  volt[99]=neuVolt;
  temp[99]=neuTemp;
}

void drawUI(){
  tft.fillScreen(0x0);
#pragma region Temp
  //Scala
  tft.drawNumber(200,0,0);
  tft.drawNumber(150,0,15);
  tft.drawNumber(100,0,31);
  tft.drawNumber(50,6,47);
  tft.drawNumber(0,12,63);

  //Achsen
  tft.drawLine(20,3,20,66,TFT_WHITE);
  tft.drawLine(20,66,123,66,TFT_WHITE);

  //strichelchen Y
  tft.drawLine(20,3,23,3,TFT_WHITE);
  tft.drawLine(20,18,23,18,TFT_WHITE);
  tft.drawLine(20,34,23,34,TFT_WHITE);
  tft.drawLine(20,50,23,50,TFT_WHITE);

  //strichelchen X
  tft.drawLine(40,66,40,63,TFT_WHITE);
  tft.drawLine(60,66,60,63,TFT_WHITE);
  tft.drawLine(80,66,80,63,TFT_WHITE);
  tft.drawLine(100,66,100,63,TFT_WHITE);
  tft.drawLine(120,66,120,63,TFT_WHITE);
#pragma endregion
  
#pragma region Volt
  //Scala
  tft.drawNumber(18,3,80);
  tft.drawNumber(15,3,95);
  tft.drawNumber(12,3,111);
  tft.drawNumber(9,9,127);
  tft.drawNumber(6,9,143);

  //Achsen
  tft.drawLine(20,83,20,146,TFT_WHITE);
  tft.drawLine(20,146,123,146,TFT_WHITE);

  //strichelchen Y
  tft.drawLine(20,83,23,83,TFT_WHITE);
  tft.drawLine(20,98,23,98,TFT_WHITE);
  tft.drawLine(20,114,23,114,TFT_WHITE);
  tft.drawLine(20,130,23,130,TFT_WHITE);

  //strichelchen X
  tft.drawLine(40,146,40,143,TFT_WHITE);
  tft.drawLine(60,146,60,143,TFT_WHITE);
  tft.drawLine(80,146,80,143,TFT_WHITE);
  tft.drawLine(100,146,100,143,TFT_WHITE);
  tft.drawLine(120,146,120,143,TFT_WHITE);
#pragma endregion
}

void drawValues(){
  drawUI();
  for (size_t i = 0; i < 100; i++)
  {
    tft.drawPixel(20+i,66-((temp[i]/200.)*64),TFT_BLUE);
    tft.drawPixel(20+i,146-(((volt[i]-6.)/12.)*64),TFT_BLUE);
  }
}

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
  return Vin + 1.0; // +1 für Errorcorrection
}

void setup() {
  Serial.begin(115200);
  tft.begin();
  drawUI();
}

void loop() { 
  schieben(measureTemp(),measureVolt());
  drawValues();
  for (size_t i = 0; i < 12; i++){
    tft.drawNumber((int)measureTemp(),105,0);
    tft.drawFloat(measureVolt(),1,100,80);
    delay(1000);
  }
}