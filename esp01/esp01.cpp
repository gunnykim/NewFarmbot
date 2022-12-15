/* 20221118
 * Joy Institute of Technology
 * Serial to WiFi Bridge
 * 
 * URL = http://192.168.0.88
 * 
 * - Command for direct gcode
 * API: URL/gcode?cmd=xxxx
 * 
 * - Command for functions
 * API: URL/water
 * API: URL/cam --> removed
 * API: URL/home 
 * 
 * - Command for designated moves
 * API: URL/go?num=1
 * API: URL/go?num=2
 * API: URL/go?num=3
 * API: URL/go?num=4
 * 
 * serial port with 115200
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <uri/UriBraces.h>
#include <uri/UriRegex.h>
#ifndef STASSID
#define STASSID "meta"
#define STAPSK  "smartfarm"
#endif
const char *ssid = STASSID;
const char *password = STAPSK;

String prev_num = "";
String curr_num = "";
int h_flag = 1;

// Set your Static IP address
IPAddress local_IP(192, 168, 0, 88);
// Set your Gateway IP address
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(203, 252, 96, 77);   //optional
IPAddress secondaryDNS(8, 8, 4, 4);       //optional
ESP8266WebServer server(80);

void gcode(){
  //Gcode
  String cmd = server.arg(0);
  Serial.println(cmd);
  
  //state message
  String message = "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for(uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(200, "text/html", message);
}

void water(){
  if(!h_flag){
    if(curr_num=="1"||curr_num=="3"){
      Serial.println("G0X180");
      delay(1500);
      Serial.println("M8");
      delay(1000);
      Serial.println("M9");
      delay(1500);
      Serial.println("G0X155");
    }
    else if(curr_num=="2"||curr_num=="4"){
      Serial.println("G0X70");
      delay(1500);
      Serial.println("M8");
      delay(1000);
      Serial.println("M9");
      delay(1500);
      Serial.println("G0X45");
    }
    server.send(200, "text/html", "water");
  }
  else{
    server.send(200, "text/html", "can't operate at home.");
  }
}

void go(){
  h_flag = 0;
  prev_num = curr_num;
  curr_num = server.arg(0);
  if(curr_num=="1" && prev_num!="1")
    Serial.println("G0Z150\nG0X155\nG0Y80\nG0Z30");
  else if(curr_num=="2" && prev_num!="2")
    Serial.println("G0Z150\nG0X45\nG0Y80\nG0Z30");
  else if(curr_num=="3" && prev_num!="3")
    Serial.println("G0Z150\nG0X155\nG0Y170\nG0Z30");
  else if(curr_num=="4" && prev_num!="4")
    Serial.println("G0Z150\nG0X45\nG0Y170\nG0Z30");
  server.send(200, "text/html", "go " + curr_num);
}

void home(){
  if(!h_flag){
    Serial.println("G0Z150\nG0Y0\nG0X0\nG0Z0");
    server.send(200, "text/html", "home");
    h_flag = 1;
  }
}

void setup(void){
  Serial.begin(115200);
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Configures static IP address
  if(!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)){
    Serial.println(";STA Failed to configure");
  }
  // Wait for connection
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(";");
  }
  //Serial.println("");
  Serial.print(";Connected to ");   Serial.println(ssid);
  Serial.print(";IP address: ");    Serial.println(WiFi.localIP());
  if(MDNS.begin("serial-wifi")){
    Serial.println(";MDNS responder started");
  }

  //command URL
  server.on(F("/"), []() {server.send(200, "text/plain", "hello from serial-wifi!");});
  server.on("/gcode", gcode); 
  server.on("/water", water);
  server.on("/home", home);
  //server.on("/cam", cam);   --> control at the other ESP01
  server.on("/go", go);

  server.begin();
  Serial.println(";HTTP server started");
}

void loop(void){
  server.handleClient();
}



