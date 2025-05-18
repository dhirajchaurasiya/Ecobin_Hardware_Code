#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>

// Wi-Fi credentials
const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";

// Firebase credentials
#define FIREBASE_HOST "YOUR_FIREBASE_REAL_TIME_DATABASE_URL" // without https:// and trailing slash
#define FIREBASE_AUTH "YOUR_FIREBASE_PROJECT_API_KEY"

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

// Sensor setup
#define DHTPIN 4
#define DHTTYPE DHT11
#define PH_SENSOR_PIN 33
#define MQ135_SENSOR_PIN 32

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(115200);
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.println(WiFi.localIP());

  // Firebase initialization
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    WiFi.begin(ssid, password);
    delay(3000);
    return;
  }

  // Read DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("Failed to read from DHT sensor.");
    delay(2000);
    return;
  }

  // Read pH
  int val = analogRead(PH_SENSOR_PIN);
  float voltage = val * (3.3 / 4095.0);
  float ph = 3.5 * voltage; // tweak 3.5 after calibration

  // Read MQ135
  int mq135Value = analogRead(MQ135_SENSOR_PIN);
  float co2 = mq135Value * 0.1;
  float nh3 = mq135Value * 0.08;
  float cn_ratio = nh3 != 0 ? co2 / nh3 : 0;

  Serial.println("\n-- Sensor Readings --");
  Serial.print("pH: ");
  Serial.println(ph);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" Â°C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("CO2: ");
  Serial.print(co2);
  Serial.println(" ppm");
  Serial.print("NH3: ");
  Serial.print(nh3);
  Serial.println(" ppm");
  Serial.print("C:N Ratio: ");
  Serial.println(cn_ratio);
  Serial.println("----------------------");

  String path = "/sensorData/current"; // simple timestamp substitute

  // Push only valid values
  if (!isnan(ph))
  {
    Firebase.setFloat(firebaseData, path + "/ph", ph);
  }
  if (!isnan(temperature))
  {
    Firebase.setFloat(firebaseData, path + "/temperature", temperature);
  }
  if (!isnan(humidity))
  {
    Firebase.setFloat(firebaseData, path + "/humidity", humidity);
  }
  if (co2 > 0)
  {
    Firebase.setFloat(firebaseData, path + "/co2", co2);
  }
  if (nh3 > 0)
  {
    Firebase.setFloat(firebaseData, path + "/nh3", nh3);
  }
  if (cn_ratio > 0)
  {
    Firebase.setFloat(firebaseData, path + "/cn_ratio", cn_ratio);
  }

  if (firebaseData.httpCode() > 0)
  {
    Serial.println("Data sent to Firebase successfully.");
  }
  else
  {
    Serial.print("Firebase Error: ");
    Serial.println(firebaseData.errorReason());
  }

  delay(5000); // Wait before next reading
}