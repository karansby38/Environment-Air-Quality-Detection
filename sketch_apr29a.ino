// C++ code
//
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

void playSuccessTone() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
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
      
      if (temperatureC <= 40 && ppm <= 100) {
        digitalWrite(GREEN_LED, HIGH);   // Turn on green LED
        buzzerState = true;
      } else if ((temperatureC > 40 && temperatureC <= 50) || (ppm > 100 && ppm <= 300)) {
        digitalWrite(GREEN_LED, LOW);    // Turn off green LED
        digitalWrite(YELLOW_LED, HIGH);  // Turn on yellow LED
        delay(500);                       // Delay for 0.5 seconds
        digitalWrite(YELLOW_LED, LOW);   // Turn off yellow LED
        delay(500);                       // Delay for 0.5 seconds
        buzzerState = true;
      } else {
        digitalWrite(GREEN_LED, LOW);    // Turn off green LED
        digitalWrite(YELLOW_LED, LOW);   // Turn off yellow LED
        digitalWrite(RED_LED, HIGH);     // Turn on red LED
        buzzerState = !buzzerState;
      }
      
      // Check buzzer state and adjust interval for rapid buzzing
      if (buzzerState && currentMillis - buzzerPreviousMillis >= (buzzerInterval / 10)) {
        digitalWrite(BUZZER, HIGH);  // Turn on buzzer
        buzzerPreviousMillis = currentMillis;
      } else {
        digitalWrite(BUZZER, LOW);   // Turn off buzzer
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
}
