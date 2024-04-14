#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);

#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT, &Wire,-1 );

const char *ssid = "Frijolito";
const char *password = "conquesito12";

WebServer server(80);

void handleRoot() {
float humidity=dht.readHumidity();
float temperature=dht.readTemperature();

String webpage= "<html><head><title>ESP32 SENSOR DATA</title></head></body>";
webpage+="<h1>Sensor Data</h1>";
webpage +="<p>Temperature:" +String(temperature)+"C</p>";
webpage+="<p>Humedity:"+String(humidity)+"%</p>";
webpage+="</body></html>";

server.send(200,"text/html",webpage);
}

void setup(){
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC,SCREEN_ADDRESS)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();

  dht.begin();

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

  float humidity=dht.readHumidity();
  float temperature=dht.readTemperature();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Temperatura:"+String(temperature)+"C");
  display.println("Humedad:"+String(humidity)+"%");
  display.display();
}
