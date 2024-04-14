#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>

const char *ssid = "ALUMNOS";
const char *password = "123456789";

AsyncWebServer server(80);
Servo myServo;

const int servoPin = 26;
const int laserPin = 12;

const char *html = "<!DOCTYPE html><html><head><style>body{text-align:center;}button{width:80px;height:40px;margin:10px;}</style></head><body><h1>Control de Servo y Laser</h1><button onClick='setPosition(0)'>0 Grados</button><button onClick='setPosition(90)'>90 Grados</button><button onClick='setPosition(180)'>180 Grados</button><br><br><button onClick='toggleLaser()'>Toggle Laser</button><script>function setPosition(degrees){fetch('/position?deg='+degrees);}function toggleLaser(){fetch('/toggleLaser');}</script></body></html>";

void handleRoot(AsyncWebServerRequest *request)
{
  request->send(200, "text/html", html);
}

void handlePosition(AsyncWebServerRequest *request)
{
  if (request->hasParam("deg"))
  {
    int degrees = request->getParam("deg")->value().toInt();
    myServo.write(degrees);
    request->send(200, "text/plain", "Posicionando el servo a" + String(degrees) + " grados");
  }
  else
  {
    request->send(400, "text/plain", "Parametro 'deg' faltante");
  }
}

void handleToggleLaser(AsyncWebServerRequest *request)
{
  digitalWrite(laserPin, !digitalRead(laserPin));
  request->send(200, "text/plain", "Toggle laser");
}

void setup()
{
  Serial.begin(115200);
  myServo.attach(servoPin);
  pinMode(laserPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi.....");
  }

  Serial.println("WiFi connected.....");
  Serial.println("IP Address:");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/position", HTTP_GET, handlePosition);
  server.on("/toggleLaser", HTTP_GET, handleToggleLaser);

  server.begin();
}

void loop(){
}
