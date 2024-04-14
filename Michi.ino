#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BOTON  5 //boton
#define RST_PIN         22
#define SS_PIN          21//lcd
#define RELAY_PIN_1     19//bomba
#define RELAY_PIN_2     18//foco
#define RELAY_PIN_3     23 
#define SENSOR_HUMEDAD 34 
#define HUMEDAD_BAJA 50 // Ajusta este valor según tu sensor y las necesidades de tus rábanos
#define HUMEDAD_ALTA 65 // Ajusta este valor según tu sensor y las necesidades de tus rábanos
const char* ssid = "michi";
const char* password = "1949michi";

WebServer server(80);
Adafruit_SSD1306 display(128, 64, &Wire, -1);


int leerHumedadSuelo() {
  return map(analogRead(SENSOR_HUMEDAD), 0, 4095, 100, 0);
}

// Función para encender la bomba
void encenderBomba() {
  digitalWrite(RELAY_PIN_3, HIGH);
}

// Función para apagar la bomba
void apagarBomba() {
  digitalWrite(RELAY_PIN_3, LOW);
}

void handleRoot() {
  int humedad = leerHumedadSuelo();

  String page= "<html><head><meta http-equiv=\"refresh\" content=\"5\"><title>ESP32 SENSOR DATA</title>";
  page += "<style>";
  page += "body { font-family: Arial, sans-serif; background-color: #92c37f; text-align: center; }"; // Centrar todo en el medio
  page += "h1 { color: #333; background-color: #d2b48c; padding: 10px; border-radius: 5px; margin: auto; width: 50%; }"; // Encabezado café claro centrado
  page += "p { color: #666; text-align: center; }"; // Párrafos centrados
  page += ".button {";
  page += "  display: inline-block;";
  page += "  background-color: #007bff;";
  page += "  color: #fff;";
  page += "  text-decoration: none;";
  page += "  padding: 10px 20px;";
  page += "  border-radius: 5px;";
  page += "  margin: 5px;";
  page += "  cursor: pointer;";
  page += "  border: none;";
  page += "}";
  page += "</style>";
  page += "</head><body>";
  page += "<h1>Control de Humedad y Foco</h1>";
  page += "<p>Control del Foco:</p>";
  page += "<button onclick=\"location.href='/encender'\" class=\"button\">Encender Foco</button>";
  page += "<button onclick=\"location.href='/apagar'\" class=\"button\">Apagar Foco</button>";
  page += "<h1>Control de Rabanos</h1>";
  page += "<p>Humedad del suelo: " + String(humedad) + "</p>";
  page += "<p>Estado de la bomba: ";
  page += (digitalRead(RELAY_PIN_3) == HIGH) ? "Encendida" : "Apagada";
  page += "</p>";
  page+="</body></html>";

  server.send(200, "text/html", page);
}



void handleEncenderFoco() {
  digitalWrite(RELAY_PIN_2, HIGH); // Activa el relé 2 para encender el foco
  server.send(200, "text/html", "Foco encendido");
}

void handleApagarFoco() {
  digitalWrite(RELAY_PIN_2, LOW); // Desactiva el relé 2 para apagar el foco
  server.send(200, "text/html", "Foco apagado");
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);
  pinMode(BOTON, INPUT);
  pinMode(SENSOR_HUMEDAD, INPUT);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red WiFi...");
  }

  
  Serial.println("Conexión exitosa! Dirección IP: " + WiFi.localIP().toString());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/encender", HTTP_GET, handleEncenderFoco);
  server.on("/apagar", HTTP_GET, handleApagarFoco);
  server.begin();
}

void loop() {
server.handleClient(); // Maneja las solicitudes de la página web
Serial.println(analogRead(SENSOR_HUMEDAD));
int humedad = leerHumedadSuelo();
Serial.print("Humedad: ");
Serial.print(humedad);
Serial.println("%");
delay(1000);

  // Comprobar si la humedad es baja y encender la bomba
  if (humedad < HUMEDAD_BAJA) {
    encenderBomba();
  }
  
  // Comprobar si la humedad es alta y apagar la bomba
  if (humedad > HUMEDAD_ALTA) {
    apagarBomba();
  }
  delay(1000); // Puedes ajustar este valor según la frecuencia de lectura deseada


}