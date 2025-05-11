#define BLYNK_TEMPLATE_ID "TMPL3nYHMHv1y"
#define BLYNK_TEMPLATE_NAME "NodeMCU ESP8266"
#define BLYNK_AUTH_TOKEN "jOg5XImhD7SOLaXw4mCyiwOWIDpKKmAn"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <MQ135.h>

#define DHTPIN D3
#define DHTTYPE DHT11
#define MQ135PIN A0
#define GREEN_LED D4
#define YELLOW_LED D2
#define RED_LED D1
#define BUZZER D5

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "MotoG60";
char pass[] = "12345678";

DHT dht(DHTPIN, DHTTYPE);
MQ135 mq135(MQ135PIN);

unsigned long previousMillis = 0;
const long interval = 2000;  // Interval in milliseconds for sensor readings
bool buzzerState = false;
unsigned long buzzerPreviousMillis = 0;
const long buzzerInterval = 15000; // Interval in milliseconds for buzzer

// ⬇️ Added variables for non-blocking yellow LED blinking
unsigned long yellowPreviousMillis = 0;
bool yellowLedOn = false;

void playSuccessTone() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
  }
}

// ⬇️ NEW: Reconnect WiFi if disconnected
void reconnectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    Serial.print("Reconnecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWiFi reconnected");
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  digitalWrite(GREEN_LED, HIGH);
  
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  playSuccessTone();

  Blynk.begin(auth, ssid, pass);

  dht.begin();
}

void loop() {
  Blynk.run();
  reconnectWiFi();  // ⬅️ Added WiFi auto-reconnect

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float temperatureC = dht.readTemperature(); // Read temperature in Celsius
    float humidity = dht.readHumidity();        // Read humidity
    float ppm = mq135.getPPM();                 // Read ppm value from MQ-135 sensor

    if (!isnan(temperatureC) && !isnan(humidity)) {
      Blynk.virtualWrite(V1, temperatureC); // Send temperature to Blynk app
      Blynk.virtualWrite(V2, humidity);     // Send humidity to Blynk app
      Blynk.virtualWrite(V3, ppm);          // Send ppm value to Blynk app as V3

      // Reset all LEDs before logic
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(RED_LED, LOW);

      if (temperatureC < 40 && ppm <= 100) {
        digitalWrite(GREEN_LED, HIGH);   // Turn on green LED
        buzzerState = false;             // ⬅️ No buzzer needed
      } else if ((temperatureC >= 40 && temperatureC <= 50) || (ppm > 100 && ppm <= 300)) {
        // ⬇️ Replaced blocking delay blink with millis()-based blinking
        yellowLedOn = true;
        buzzerState = true;
      } else {
        digitalWrite(RED_LED, HIGH);     // Turn on red LED
        yellowLedOn = false;
        buzzerState = true;
      }

      // Print sensor readings to the console
      Serial.print("Temperature: ");
      Serial.print(temperatureC);
      Serial.print(" °C, Humidity: ");
      Serial.print(humidity);
      Serial.print(" %, PPM: ");
      Serial.println(ppm);
    }
  }

  // ⬇️ Non-blocking yellow LED blinking
  if (yellowLedOn) {
    if (millis() - yellowPreviousMillis >= 500) {
      yellowPreviousMillis = millis();
      digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
    }
  } else {
    digitalWrite(YELLOW_LED, LOW);
  }

  // ⬇️ Improved buzzer control using millis instead of toggling every loop
  if (buzzerState && currentMillis - buzzerPreviousMillis >= (buzzerInterval / 10)) {
    digitalWrite(BUZZER, HIGH);
    delay(100);  // short beep
    digitalWrite(BUZZER, LOW);
    buzzerPreviousMillis = currentMillis;
  } else if (!buzzerState) {
    digitalWrite(BUZZER, LOW);
  }
}
