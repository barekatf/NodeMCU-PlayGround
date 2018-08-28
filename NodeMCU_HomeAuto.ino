/* 
 *  This is a secure web based control station for home automation
 *  It is based on ESP3266 (in this case NodeMCU) 
 *  
 *  The ESP3266 is setup as a standalone access point 
 *  and does not need to connect to an existing rounter with WiFi
 *  
 *  You can connect to it with any WiFi enabled device 
 *  In this case the SSID for the WiFi Network is: NodeMCU 
 *  and Password is: mcunodeWF
 *  After connecting the control center page is on: 
 *  192.168.7.1
 *  *** All these settings can be changed below.
 *  
 *  At this point there is only 2 separate on and off command 
 *  sent from the webserver: 
 *  One for the Built in LED and 
 *  One for an extra port which is connected to D6
 *  
 *  You can add/remove/change number of ports and their function
 *  See below for instructions on how to do it. 
 *  
 *  Date: Aug 18, 2018
 *  Authur: Fardin B 
 *  
 *  Enjoy and have fun automating your home
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "NodeMCU";  // Enter SSID here
const char* password = "mcunodeWF";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,7,1);
IPAddress gateway(192,168,7,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

uint8_t LEDpin = LED_BUILTIN;
bool LEDstatus = false;
uint8_t extra = D6; 
bool extraStatus = false; 
// Add a pin description for your output pin
// Example ---> uint8_t motor = D7; 
// Add status for your next button here 
// Example ---> bool motorStatus = false; 

void setup() {
  Serial.begin(9600);
  pinMode(LEDpin, OUTPUT);
  pinMode(extra, OUTPUT); 
  // Set your pinmode for your output pin
  // Example ---> pinMode(motor, OUTPUT); 

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  
  // Copy the section below for your handler functions 
  // change the flags and the handler function names 
  server.on("/extraON", handle_extraON); 
  server.on("/extraOFF", handle_extraOFF); 
  // Copy until here 
  
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("*********************************************************"); 
  Serial.println("**************** NodeMCU Control Station ****************"); 
  Serial.println("*********************************************************"); 
  Serial.println(); 
  Serial.println("**** HTTP server started");
  Serial.println("**** Connect to WiFi Network with"); 
  Serial.println(); 
  Serial.println("**** SSID: NodeMCU"); 
  Serial.println("**** Password: mcunodeWF"); 
  Serial.println(); 
  Serial.println("*********************************************************"); 
}

void loop() {
  server.handleClient();
  if(LEDstatus)
    digitalWrite(LEDpin, HIGH);
  else
    digitalWrite(LEDpin, LOW);

  // Copy the section below for assigning On or Off values to your outputs 
  // change the if condition status with the status you created above 
  // Change the pin description with the one you created above
  if(extraStatus)
    digitalWrite(extra, HIGH);
  else
    digitalWrite(extra, LOW);
  // Copy until here 
}

void handle_OnConnect() {
  LEDstatus = false;
  extraStatus = false; 
  server.send(200, "text/html", SendHTML()); 
}

void handle_ledon() {
  LEDstatus = true;
  server.send(200, "text/html", SendHTML()); 
}

void handle_ledoff() {
  LEDstatus = false;
  server.send(200, "text/html", SendHTML()); 
}
// Copy the handler functions below and modify to match your own 
// chnage the handler function names you made above 
// change the status description 
void handle_extraON() { 
  extraStatus = true; 
  server.send(200, "text/html", SendHTML()); 
}

void handle_extraOFF() { 
  extraStatus = false; 
  server.send(200, "text/html", SendHTML()); 
}
// Copy until here

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(){
  String ptr = "<!DOCTYPE html>\n";
  ptr +="<html>\n";
  ptr +="<head>\n";
  ptr +="<title>Home Automation Console</title>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>LED</h1>\n";
  ptr +="<p>Click to switch LED on and off.</p>\n";
  ptr +="<form method=\"get\">\n";
  
  if(LEDstatus)
    ptr +="<input type=\"button\" value=\"LED OFF\" onclick=\"window.location.href='/ledoff'\">\n";
  else
    ptr +="<input type=\"button\" value=\"LED ON\" onclick=\"window.location.href='/ledon'\">\n";
  
  // ******** Copy this section for additional buttons 
  // Remember to chnage the following 
  // the header ---> "<h1>YOUR_HEADER</h1>\n";
  // the if condition ---> if(YOUR_CONDITION)
  // the values shown for the button ---> value=\"YOUR_VALUE\", 
  // and the flag for the button ---> "window.location.href='/YOUR_FLAG'\"
  
  ptr +="<h1>Extra</h1>\n";
  ptr +="<p>Click to switch EXTRA on and off.</p>\n";
  ptr +="<form method=\"get\">\n";

  if(extraStatus)
    ptr +="<input type=\"button\" value=\"Extra OFF\" onclick=\"window.location.href='/extraOFF'\">\n";
  else
    ptr +="<input type=\"button\" value=\"Extra ON\" onclick=\"window.location.href='/extraON'\">\n";
  
  // ******* Copy until here 
  
  ptr +="</form>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
