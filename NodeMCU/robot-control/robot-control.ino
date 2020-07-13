#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
SoftwareSerial __uart(D6,D5); // (Rx, Tx)

const char* ssid = "ssid";
const char* password = "password";

ESP8266WebServer server(80);
int data;
void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleCommand() {
  String command = server.arg(0);
  if (command == "stopped") 
    __uart.write('0');
  else if (command == "forward") 
    __uart.write('1');  
   else if (command == "backward") 
    __uart.write('2');   
   else if (command == "right") 
    __uart.write('3');
    else if (command == "left") 
    __uart.write('4');    
   server.send(200, "text/plain", "Moving Status: " + command);
   Serial.println("Moving Status: " + command);
}

void setup(void){
  Serial.begin(9600);
  Serial.println();
  __uart.begin(9600);

  WiFi.begin(ssid, password);
  WiFi.setAutoConnect(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.on("/", handleRoot);
  server.on("/command", handleCommand);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
