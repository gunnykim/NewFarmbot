#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>
//#include <ESP32Servo.h>

//Servo servo1;
//#define SERVO_1 14
const char* WIFI_SSID = "meta";
const char* WIFI_PASS = "smartfarm";

//httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

const char pageMain[] PROGMEM = R"=====(
<!doctype html>
<html>
<head>
   <title>Main Page</title>
</head>
<body>
   <h1>ESP32CAM TEST</h1>
  <p><a href="/">Main Page</a></p>
  <p><a href="/ledBlink">LED TEST</a></p>
   <p><a href="/low.jpg">low Img</a></p>
   <p><a href="/high.jpg">high Img</a></p>
  
</body>
</html> 
)=====";


WebServer server(80);
static auto loRes = esp32cam::Resolution::find(320, 240);
static auto hiRes = esp32cam::Resolution::find(800, 600);

void serveJpg()
{
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));

  WiFiClient client = server.client();
  frame->writeTo(client);

  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
}
 
void handleJpgLo()
{
  if (!esp32cam::Camera.changeResolution(loRes)) {
    Serial.println("SET-LO-RES FAIL");
  }
  serveJpg();
}
 
void handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}
 
void handleMain() {
   String html = pageMain;
   server.send(200, "text/html", html);
}
 
void forServo(){
  digitalWrite(4, HIGH);
  delay(2000);
  digitalWrite(4, LOW);
  delay(2000);
}


void  setup(){
  Serial.begin(115200);
  pinMode(4, OUTPUT); 

  Serial.println();
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(1);
    cfg.setJpeg(80);
 
    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("  /low.jpg");
  Serial.println("  /high.jpg");

  //server.enableCORS(true);
  server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
  server.sendHeader(F("Access-Control-Max-Age"), F("600"));
  server.sendHeader(F("Access-Control-Allow-Methods"), F("PUT,POST,GET,OPTIONS"));
  server.sendHeader(F("Access-Control-Allow-Headers"), F("*"));
 
  server.on("/", handleMain);
  server.on("/low.jpg", handleJpgLo);
  server.on("/high.jpg", handleJpgHi);
  server.on("/ledBlink",forServo);

  server.begin();
}
 
void loop()
{
  server.handleClient();
}