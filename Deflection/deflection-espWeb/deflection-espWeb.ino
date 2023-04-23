//NODE MCU

//WIFI setup
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
const char *ssid = "Enter your WiFi Name"; 
const char *pass = "Enter your WiFi Password";

WiFiServer server(80);

//Include Lib for Arduino to Nodemcu
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //This library you can add via Include Library > Manage Library > 
LiquidCrystal_I2C lcd(0x27, 20, 4);

//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D5, D6);

float mass;
float weight;
float deflection;

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  nodemcu.begin(9600);
  while (!Serial) continue;

  Wire.begin(2,0);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 

  
  lcd.print(" Hello Engineers "); // Start Printing

  delay(10);
  Serial.println("Connecting to ");

  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(550);
    Serial.print(".");

  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
//  delay(500);

  ReceiveData();
  //// put your main code here, to run repeatedly:

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Deflection");
  lcd.setCursor(0, 1);
  lcd.print(deflection);
  lcd.print(" mm");

  WiFiClient client = server.available();
  if ( client.connected()) { // loop while the client's connected
    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println("Refresh: 1"); // update the page after 1 sec
    client.println();


    // Display the HTML web page
    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<link rel=\"icon\" href=\"data:,\">");

    // Web Page Heading
    client.println("<style>html{background-image: url('https://i.pinimg.com/564x/72/91/b2/7291b24ed113b554b1cd4fc73c6a54cc.jpg'); background-color: background-image; background-repeat: no-repeat; background-attachment: fixed; background-size: cover; font-family: Century Gothic; color: white; display: inline-block; margin: 0px auto; text-align: center;}");
    client.println("font-family: Century Gothic;text-shadow: 2px 2px DodgerBlue; font-size: 30px; margin: 0px; cursor: pointer;}</style></head>");

    // Web Page Heading // School, Title, Members
    client.println("<body><p>Notre Dame of Marbel University<br>City of Koronadal, South Cotabato<br>College of Engineering and Technology</p>");
    client.println("<body><h1><p align=\"Center\">Strain Gauge-Based Beam Monitoring System using IoT Technology</h1></p>");
    client.println("<body><p align=\"Center\">Erum, K., Fernandez, B., Sumagaysay, F.</p>");


    client.println("<body><p style=\"width:100%; border-style: solid; background-color: 5c2a04; border-radius: 10px; text-align: center; font-size: 30px; color:white;\"> Mass: " + String(mass) + " Kg " + " </p>");

    client.println("<body><p style=\"width:100%; border-style: solid; background-color: 5c2a04; border-radius: 10px; text-align: center; font-size: 30px; color:white;\"> Weight: " + String(weight) + " N " + " </p>");

    client.println("<body><p style=\"width:100%; border-style: solid; background-color: 5c2a04; border-radius: 10px; text-align: center; font-size: 30px; color:white;\"> Deflection " + String(deflection) + " mm " + " </p>");

    if (mass <6 ) {
      client.println("<body><h1 style=\"color:green;\"> NORMAL </h1>");
    } else if (mass > 12) {
      client.println("<body><h1 style=\"color:red;\"> MODERATE! </h1>");
    } else if (mass > 18) {
      client.println("<body><h1 style=\"color:red;\"> DANGER!! </h1>");
    } 


    client.println("</body></html>");
//    delay(500);

  }
}

void ReceiveData(){
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu);

  if (data == JsonObject::invalid()) {
    //Serial.println("Invalid Json Object");
    jsonBuffer.clear();
    return;

  }

  Serial.println("JSON Object Recieved");
  Serial.print("Recieved mass:  ");
  mass = data["mass"];
  Serial.println(mass);

  Serial.print("Recieved weight:  ");
  weight = data["weight"];
  Serial.println(weight);

  Serial.print("Recieved deflection:  ");
  deflection = data["deflection"];
  Serial.println(deflection);

  Serial.println("-----------------------------------------");
}
