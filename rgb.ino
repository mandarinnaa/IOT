#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "michi";
const char *password = "1949michi";

WebServer server(80);

int redPin = 12;
int greenPin = 13;
int bluePin = 14;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

const char* html = 
  "<!DOCTYPE html>"
  "<html>"
  "<head>"
  "<style>body {text-align:center;} button {width:80px;height:80px;margin:10px;}</style>"
  "</head>"
  "<body>"
  "<h1>Control de LED RGB</h1>"
  "<button onclick='setColor(255,0,0)'>Rojo</button>"
  "<button onClick='setColor(0,255,0)'>Verde</button>"
  "<button onClick='setColor(0,0,255)'>Azul</button>"
  "<button onClick='setColor(255,182,193)'>Rosa</button>"
  "<button onClick='setColor(255,255,0)'>Amarillo</button>"
  "<button onClick='setColor(255,165,0)'>Naranja</button>"
  "<script>"
  "function setColor(r, g, b) { fetch('/color?r=' + r + '&g=' + g + '&b=' + b); }"
  "</script>"
  "</body>"
  "</html>";

void handleRoot() {
  server.send(200, "text/html", html);
}

void handleColor() {
  if (server.hasArg("r")) {
    redValue = server.arg("r").toInt();
  }
  if (server.hasArg("g")) {
    greenValue = server.arg("g").toInt();
  }
  if (server.hasArg("b")) {
    blueValue = server.arg("b").toInt();
  }

  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);

  server.send(200, "text/plain", "Color configurado");
}

void setup() {
  Serial.begin(115200);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi.....");
  }

  Serial.println("WiFi connected.....");
  Serial.println("IP Address:");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/color", HTTP_GET, handleColor);

  server.begin();
}

void loop() {
  server.handleClient();
}
