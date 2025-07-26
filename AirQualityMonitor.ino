#define BLYNK_TEMPLATE_ID "TMPL32u_rLeUK"
#define BLYNK_TEMPLATE_NAME "Air Quality Monitor"
#define BLYNK_AUTH_TOKEN "s8ZwlS65VeId3-rO4eF38G9mgbipxbVd"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// ----- Pin Definitions -----
#define DHTPIN D4           // GPIO2
#define MQ135PIN A0         // Analog input for MQ135 (NH3 & CO2)
#define MQ2PIN D3           // Digital input from MQ2 (Methane)
#define DHTTYPE DHT22

// ----- WiFi Credentials -----
char ssid[] = "POCOM2";
char pass[] = "hanifirdous";

// ----- Sensor Initialization -----
DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

void sendSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int mq135_raw = analogRead(MQ135PIN);     // NH3 & CO2 estimation
  int mq2_raw = digitalRead(MQ2PIN);         // Methane detection (basic)

  // PPM estimation
  float nh3_ppm = mq135_raw / 40.0;
  float co2_ppm = mq135_raw / 5.0;
  float methane_ppm = mq2_raw * 1000.0;

  // Send to Serial Monitor
  Serial.println("==== Sensor Readings ====");
  Serial.print("Temp: "); Serial.println(temperature);
  Serial.print("Humidity: "); Serial.println(humidity);
  Serial.print("NH3: "); Serial.println(nh3_ppm);
  Serial.print("CO2: "); Serial.println(co2_ppm);
  Serial.print("Methane: "); Serial.println(methane_ppm);
  Serial.println("=========================\n");

  // Send to Blynk
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, nh3_ppm);
  Blynk.virtualWrite(V3, co2_ppm);
  Blynk.virtualWrite(V4, methane_ppm);
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Send sensor data every 5 seconds
  timer.setInterval(5000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}
