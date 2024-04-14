#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "an";
const char *password = "1234567890";

WebServer server(80);

const int ledPin = 2;

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectado al WiFi...");
  }

  Serial.println("Conectado al WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](){
    String html = "<html><head><title>Control de LED</title>";
    html += "<style>body{text-align:center;font-family:Arial, sans-serif;}h1{color:#333;}a{display:inline-block;margin:10px;padding:15px 30px;font-size:20px;text-decoration:none;color:#fff;background-color:#4CAF50;border-radius:8px;}a:hover{background-color:#45a049;}</style>";
    html += "</head><body><h1>Control de LED</h1>";
    html += "<p><a href=\"/on\">Encender LED</a></p><p><a href=\"/off\">Apagar LED</a></p></body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/on", HTTP_GET, [](){
    digitalWrite(ledPin, HIGH);
    server.send(200, "text/plain", "LED Encendido");
  });

  server.on("/off", HTTP_GET, [](){
    digitalWrite(ledPin, LOW);
    server.send(200, "text/plain", "LED Apagado");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
