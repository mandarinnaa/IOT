#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "qeuasdixo";
const char *password = "micaracuandoXD";

WebServer server(80);
int ldrPin=34;
int ledPin=13;

const char*html="<!DOCTYPE html><html><head><meta http-equiv='refresh' content='1'><style>body {text-align:center;font-size;24px;}</style></head><body><h1>Valor del LDR:</h1><p>%ldrValue%</p></body></html>";

void handleRoot(){
  int ldrValue = analogRead(ldrPin);
  String page = String(html);
  page.replace("%ldrValue%",String(ldrValue));
  server.send(200,"text/html",page);

if(ldrValue < 100){
  digitalWrite(ledPin,LOW);
}else{
  digitalWrite(ledPin,HIGH);
}
}

void setup(){
  Serial.begin(115200);
  pinMode(ledPin,OUTPUT);

 WiFi.begin(ssid,password);
while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi.....");
  }

  Serial.println("WiFi connected.....");
  Serial.println("IP Address:");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);

  server.begin();
}

void loop() {
  server.handleClient();
}