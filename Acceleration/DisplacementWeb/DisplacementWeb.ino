//Include Lib for Arduino to Nodemcu
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//Our HTML webpage contents with javascripts
//#include "readingsIndex.h" 
#include "index.h" 

SoftwareSerial nodemcu(D6, D5);
#define LED 2        //On board LED

//SSID and Password of your WiFi router
const char* ssid = "SSID HERE";
const char* password = "PASSWORD";


ESP8266WebServer server(80); //Server on port 80

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

float x, y, z;

void handleADC() {  
  String JSONdata = "{\"xVal\":\"" + String(x) + "\", \"yVal\":\"" + String(y) + "\", \"zVal\":\"" + String(z) + "\"}";

  digitalWrite(LED, !digitalRead(LED)); //Toggle LED on data request ajax
  server.send(200, "text/plane", JSONdata); //Send x,y and Z JSON to client ajax request
}

void setup()
{
  Serial.begin(115200);
  nodemcu.begin(115200);
  while (!Serial) continue;

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  //Onboard LED port Direction output
  pinMode(LED, OUTPUT);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/readADC", handleADC); //This page is called by java Script AJAX

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();          //Handle client requests

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu);

  if (data == JsonObject::invalid()) {
    //Serial.println("Invalid Json Object");
    jsonBuffer.clear();
    return;
  }

  x = data["x"];
  y = data["y"];
  z = data["z"];

  Serial.print("Xa= ");
  Serial.print(x);
  Serial.print("   Ya= ");
  Serial.print(y);
  Serial.print("   Za= ");
  Serial.println(z);

  delay(100);
}
