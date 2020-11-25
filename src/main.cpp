#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#define TEMPPIN 32
#define VOLTPIN 33
#define OFFSET_X 0
#define OFFSET_Y 5

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
  tft.drawNumber(200,0 + OFFSET_X,0 + OFFSET_Y);
  tft.drawNumber(150,0 + OFFSET_X,15 + OFFSET_Y);
  tft.drawNumber(100,0 + OFFSET_X,31 + OFFSET_Y);
  tft.drawNumber(50,6 + OFFSET_X,47 + OFFSET_Y);
  tft.drawNumber(0,12 + OFFSET_X,63 + OFFSET_Y);

  //Achsen
  tft.drawLine(20 + OFFSET_X,3 + OFFSET_Y,20 + OFFSET_X,66 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(20 + OFFSET_X,66 + OFFSET_Y,123 + OFFSET_X,66 + OFFSET_Y,TFT_WHITE);

  //strichelchen Y
  tft.drawLine(20 + OFFSET_X,3 + OFFSET_Y,23 + OFFSET_X,3 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(20 + OFFSET_X,18 + OFFSET_Y,23 + OFFSET_X,18 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(20 + OFFSET_X,34 + OFFSET_Y,23 + OFFSET_X,34 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(20 + OFFSET_X,50 + OFFSET_Y,23 + OFFSET_X,50 + OFFSET_Y,TFT_WHITE);

  //strichelchen X
  tft.drawLine(40 + OFFSET_X,66 + OFFSET_Y,40 + OFFSET_X,63 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(60 + OFFSET_X,66 + OFFSET_Y,60 + OFFSET_X,63 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(80 + OFFSET_X,66 + OFFSET_Y,80 + OFFSET_X,63 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(100 + OFFSET_X,66 + OFFSET_Y,100 + OFFSET_X,63 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(120 + OFFSET_X,66 + OFFSET_Y,120 + OFFSET_X,63 + OFFSET_Y,TFT_WHITE);
#pragma endregion
  
#pragma region Volt
  //Scala
  tft.drawNumber(18,3 + OFFSET_X,80 + OFFSET_Y);
  tft.drawNumber(15,3 + OFFSET_X,95 + OFFSET_Y);
  tft.drawNumber(12,3 + OFFSET_X,111 + OFFSET_Y);
  tft.drawNumber(9,9 + OFFSET_X,127 + OFFSET_Y);
  tft.drawNumber(6,9 + OFFSET_X,143 + OFFSET_Y);

  //Achsen
  tft.drawLine(20 + OFFSET_X,83 + OFFSET_Y,20 + OFFSET_X,146 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(20 + OFFSET_X,146 + OFFSET_Y,123 + OFFSET_X,146 + OFFSET_Y,TFT_WHITE);

  //strichelchen Y
  tft.drawLine(20 + OFFSET_X,83 + OFFSET_Y,23 + OFFSET_X,83 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(20 + OFFSET_X,98 + OFFSET_Y,23 + OFFSET_X,98 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(20 + OFFSET_X,114 + OFFSET_Y,23 + OFFSET_X,114 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(20 + OFFSET_X,130 + OFFSET_Y,23 + OFFSET_X,130 + OFFSET_Y,TFT_WHITE);

  //strichelchen X
  tft.drawLine(40 + OFFSET_X,146 + OFFSET_Y,40 + OFFSET_X,143 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(60 + OFFSET_X,146 + OFFSET_Y,60 + OFFSET_X,143 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(80 + OFFSET_X,146 + OFFSET_Y,80 + OFFSET_X,143 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(100 + OFFSET_X,146 + OFFSET_Y,100 + OFFSET_X,143 + OFFSET_Y,TFT_WHITE);
  tft.drawLine(120 + OFFSET_X,146 + OFFSET_Y,120 + OFFSET_X,143 + OFFSET_Y,TFT_WHITE);
#pragma endregion
}

void drawValues(){
  drawUI();
  for (size_t i = 0; i < 100; i++)
  {
    tft.drawPixel(20+i+OFFSET_X,66-((temp[i]/200.)*64)+OFFSET_Y,TFT_BLUE);
    tft.drawPixel(20+i+OFFSET_X,146-(((volt[i]-6.)/12.)*64)+OFFSET_Y,TFT_BLUE);
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
    tft.drawNumber((int)measureTemp(),105 + OFFSET_X,0+OFFSET_Y);
    tft.drawFloat(measureVolt(),1,100 + OFFSET_X,80+OFFSET_Y);
    delay(1000);
  }
}