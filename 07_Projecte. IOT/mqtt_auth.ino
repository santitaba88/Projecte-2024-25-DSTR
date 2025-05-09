#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define DHTPIN 4  // GPIO4 (D4)
#define DHTTYPE DHT11
#define PIRPIN  5  // GPIO5 (D3)

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

const char* ssid = "santi";       
const char* password = "nicolast6";
const char* serverName = "http://10.225.92.234/guardar_datos.php";  // IP del servidor con PHP

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  dht.begin();
  pinMode(PIRPIN, INPUT);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    float temperatura = dht.readTemperature();
    float humedad = dht.readHumidity();
    int movimiento = digitalRead(PIRPIN);

    if (!isnan(temperatura) && !isnan(humedad)) {
      WiFiClient client;
      HTTPClient http;

      String postData = "{\"temperatura\":" + String(temperatura) + ", \"humedad\":" + String(humedad) + ", \"movimiento\":" + String(movimiento) + "}";

      Serial.print("Enviando datos: ");
      Serial.println(postData);

      http.begin(client, serverName);
      http.addHeader("Content-Type", "application/json");

      int httpResponseCode = http.POST(postData);
      
      if (httpResponseCode > 0) {
        Serial.print("Respuesta del servidor: ");
        Serial.println(http.getString());
      } else {
        Serial.print("Error en la solicitud HTTP: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    } else {
      Serial.println("Error al leer los datos del sensor DHT11");
    }
  } else {
    Serial.println("Error: WiFi desconectado");
  }
  
  delay(5000); // Enviar cada 5 segundos
}

