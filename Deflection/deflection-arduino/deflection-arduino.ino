
#include <Q2HX711.h>              //Downlaod from here: https://electronoobs.com/eng_arduino_hx711.php

//Arduino to NodeMCU Lib
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(5, 6);

//Pins
const byte hx711_data_pin = 3;    //Data pin from HX711
const byte hx711_clock_pin = 2;   //Clock pin from HX711
Q2HX711 hx711(hx711_data_pin, hx711_clock_pin); // prep hx711
int buzzer = 11;


//Variables
/////////Change here with your calibrated mass////////////
float y1 = 2000; // calibrated mass to be added
//////////////////////////////////////////////////////////
long x1 = 0L;
long x0 = 0L;
float avg_size = 20.0; // amount of averages for each mass measurement
float tara = 0;
bool tara_pushed = false;
float oz_conversion = 0.035274;
float Mass;
float Weight;
float D;
//////////////////////////////////////////////////////////



void setup() {

  Serial.begin(9600);                 // prepare serial port
  nodemcu.begin(9600);

  delay(1000);                        // allow load cell and hx711 to settle

  // tare procedure
  for (int ii = 0; ii < int(avg_size); ii++) {
    delay(10);
    x0 += hx711.read();
  }
  x0 /= long(avg_size);
  Serial.println("Add Calibrated Mass");

  // calibration procedure (mass should be added equal to y1)
  int ii = 1;
  while (true) {
    if (hx711.read() < x0 + 10000)
    {
      //do nothing...
    }
    else
    {
      ii++;
      delay(2000);
      for (int jj = 0; jj < int(avg_size); jj++) {
        x1 += hx711.read();
      }
      x1 /= long(avg_size);
      break;
    }
  }
  
  Serial.println("Calibration Complete");

  Serial.println("Assuming Simply Supporter Beam");
  Serial.println(" \t\t"); Serial.print(" \t"); 
  Serial.print("Mass (kg)"); Serial.print(" \t\t"); 
  Serial.print("Weight(N)"); Serial.print(" \t\t"); 
  Serial.println("Deflection (mm)");
}

void loop() {

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  // averaging reading
  long reading = 0;
  for (int jj = 0; jj < int(avg_size); jj++)
  {
    reading += hx711.read();
  }
  reading /= long(avg_size);


  // calculating mass based on calibration and linear fit
  float ratio_1 = (float) (reading - x0);
  float ratio_2 = (float) (x1 - x0);
  float ratio = ratio_1 / ratio_2;
  float mass = y1 * ratio;

  Mass = ((mass - tara) / 1000);
  Weight = Mass * 9.81;
  D = Weight * 0.4332313965;

  //Assign collected data to JSON Objectqqqq
  data["mass"] = Mass;
  data["weight"] = Weight;
  data["deflection"] = D;


  //Send data to NodeMCU
  data.printTo(nodemcu);
  jsonBuffer.clear();

  Serial.print(" \t"); Serial.print(Mass); Serial.print(" \t\t\t"); 
  Serial.print(Weight); Serial.print(" \t\t\t"); 
  Serial.println(D);

  if (Mass > 18) {
    for (int i = 500; i <= 1000; i++) {//increase frequency
      tone(buzzer, i);
      delay(5);
    }

    delay(1000);

    for (int i = 1000; i >= 500; i--) {//decrease frequency
      tone(buzzer, i);
      delay(5);
    }
  }

}
