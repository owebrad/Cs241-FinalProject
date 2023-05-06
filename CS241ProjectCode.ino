/*
* Owen Bradley & Gus Roselius
* CS241 Project
* Before uploading and running verify that:
* Link "https://arduino.esp8266.com/stable/package_esp8266com_index.json" is entered under File > Preferences > Additional Boards Manager URLs:
* Correct drivers are installed, they should install automatically, if not they are available on Github
* Board NodeMCU 1.0 (ESP-12E Module) is selected, if not available, install esp8266 by esp8266community from Boards Manager
* A valid port is selected
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "hotspot"; //Network ssid
const char* pass = "owebrad18"; //Netword password, leave empty string "" if none

ESP8266WebServer server(80); //Creates variable for webserver server at 80 baud

bool LEDstatus = LOW; //Boolean to track the state of the led
//#define LED_PIN 2

void setup() {
  Serial.begin(9600); //Starts serial monitor can work in both 9600 or 115200 baud
  delay(100);
  pinMode(D4,OUTPUT); //Sets pin D4 on the ESP8266 to output

  Serial.println("Connecting to ");
  Serial.print(ssid); // Displays network name to serial monitor

  WiFi.begin(ssid,pass); // Connects to network with provided information

  while(WiFi.status() != WL_CONNECTED) {
    // Loops until wifi is connected
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wifi connected!");
  Serial.println("IP: ");
  Serial.print(WiFi.localIP()); // Displays the ESP8266's IP address

  server.on("/", handle_OnConnect); // Calls function when default webpage
  server.on("/ledon", handle_ledon); // Calls function when URL changes to "/ledon"
  server.on("/ledoff", handle_ledoff); // Calls function when URL changes to "/ledoff"
  server.onNotFound(handle_NotFound); // If webpage is not found, function is called

  server.begin(); // Starts nodemcu webserver
  Serial.println(" HTTP Server Started");
}

void loop() {
  server.handleClient();

  if(LEDstatus) {
    digitalWrite(D4, HIGH);
  }
  else {
    digitalWrite(D4,LOW);
  }

}

void handle_OnConnect() {
  LEDstatus = LOW; // Defaults LED to LOW
  Serial.println("LED: Off");
  server.send(200, "text/html", updateWebpage(LEDstatus)); // Sends response to webserver in html format with the string returned from updateWebpage
}

void handle_ledon() {
  LEDstatus = HIGH; // Sets LEDstatus to high
  Serial.println("LED: On");
  server.send(200, "text/html", updateWebpage(LEDstatus)); // Sends response with updated LEDstatus to turn LED on
}

void handle_ledoff() {
  LEDstatus = LOW; // Set to low
  Serial.println("LED: Off");
  server.send(200, "text/html", updateWebpage(LEDstatus)); // Sends response with updated LEDstatus to turn LED off
}

void handle_NotFound() {
  server.send(404,"text/plain", "Not found"); // Sends if webpage has an error
}

String updateWebpage(uint8_t LEDstatus) { 
  // This function creates a dynamic webpage using HTML format, changes the button state and URL based on LEDstatus
  
  String ptr = "<!DOCTYPE html> <html>\n"; // Creates string variable as a HTML document
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n"; // Resises webpage to fit device
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n"; // Sets wepage font
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n"; // Sets webpage headings color
  
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #3498db;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  //Creates and sets the button's position, size, and color when button changes

  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";

  ptr +="<h1>ESP8266 Web Server</h1>\n"; // Header One
  ptr +="<h3>Click Button to Toggle LED</h3>\n"; // Header Three

  if(LEDstatus){
    ptr +="<p>BLUE LED: OFF</p><a class=\"button button-off\" href=\"/ledoff\">ON</a>\n";
  }
  else{
    ptr +="<p>BLUE LED: ON</p><a class=\"button button-on\" href=\"/ledon\">OFF</a>\n";
  }
  // When button is pressed the website will change based on LEDstatus

  ptr +="</body>\n";
  ptr +="</html>\n";

  return ptr;
}
