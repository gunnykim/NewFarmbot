#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include <Stepper.h>
// this code is for Stepper Motor to capture poly angle of plant

// WiFi attribute
const char* WIFI_SSID = "meta";
const char* WIFI_PASS = "smartfarm";

const int steps = 2048; // One cycle is 2048
const int wholeCycle = 2048;
const int half = 1024;
const int quarter = 512;
const int one_eighth = 248;
const int one_sixteen = 124;


const char pageMain[] PROGMEM = R"=====(
<!doctype html>
<html>
<head>
   <title>Main Page</title>
</head>
<body>
   <h1>Stepper For Esp32Cam</h1>
  <p><a href="/">Main Page</a></p>
   <p><a href="/oneEight">1/8</a></p>
   <p><a href="/quarter">1/4</a></p>
  <p><a href="/oneCycle">1</a></p>
  
</body>
</html> 
)=====";

WebServer server(80);
Stepper myStepper(steps,33,27,32,12);

void quarterCycle()
{
  myStepper.step(quarter);
  

}
void one_eigthCycle()
{
  myStepper.step(one_eighth);
  delay(800);
}
void oneCycle()
{
  myStepper.step(wholeCycle);
  delay(800);
  
}


void handleMain() {
   String html = pageMain;
   server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(15);

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("/oneCycle");
  Serial.println("/quarter");
  Serial.println("/oneEight");

  server.on("/", handleMain);
  server.on("/oneCycle", oneCycle);
  server.on("/quarter", quarterCycle);
  server.on("/oneEight",one_eigthCycle);
  
  server.begin();
}

void loop() {
  server.handleClient();
}