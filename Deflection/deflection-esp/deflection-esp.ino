//ThatsEngineering
//Sending Data from Arduino to NodeMCU Via Serial Communication
//NodeMCU code

//Include Lib for Arduino to Nodemcu
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //This library you can add via Include Library > Manage Library > 
LiquidCrystal_I2C lcd(0x27, 20, 4);

//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D5, D6);


void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  nodemcu.begin(9600);
  while (!Serial) continue;
  
  Wire.begin(2,0);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 

  
  lcd.print(" Hello Engineers "); // Start Printing
}

void loop() {
  
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu);

  if (data == JsonObject::invalid()) {
    //Serial.println("Invalid Json Object");
    jsonBuffer.clear();
    return;
  }

  Serial.println("JSON Object Recieved");
  Serial.print("Recieved mass:  ");
  float num1 = data["mass"];
  Serial.println(num1);
  
  Serial.print("Recieved weight:  ");
  float num2 = data["weight"];
  Serial.println(num2);

  Serial.print("Recieved deflection:  ");
  float num3 = data["deflection"];
  Serial.println(num3);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Deflection");
  lcd.setCursor(0, 1);
  lcd.print(num3);
  lcd.print(" mm");
  
  Serial.println("-----------------------------------------");
}
