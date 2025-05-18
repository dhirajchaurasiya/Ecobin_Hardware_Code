#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>
#include <Arduino.h>
#include <string.h>

// Wi-Fi credentials
const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";

// Firebase credentials
#define FIREBASE_HOST "YOUR_FIREBASE_REAL_TIME_DATABASE_URL"
#define FIREBASE_AUTH "YOUR_FIREBASE_PROJECT_API_KEY"

// values
float simulatedPH[] = {8.21, 8.73, 7.96, 7.13, 7.08, 6.89, 6.83, 6.52, 6.33, 6.33, 6.33};
int step = 0;

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

// Sensor setup
#define DHTPIN 19
#define DHTTYPE DHT11
#define PH_SENSOR_PIN 18
#define MQ135_SENSOR_PIN 4

DHT dht(DHTPIN, DHTTYPE);
float co2, nh3, cn_ratio, temperature, humidity, ph;
String path = "/sensorData/current";

void gas_sensor()
{
    int mq135Value = analogRead(MQ135_SENSOR_PIN);
    Serial.println(analogRead(4));
    Serial.println(mq135Value);
    co2 = mq135Value * 0.1;
    nh3 = mq135Value * 0.08;
    cn_ratio = nh3 != 0 ? co2 / nh3 : 0;
}

void temp_sensor()
{
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity))
    {
        Serial.println("Failed to read from DHT sensor.");
        delay(2000);
        return;
    }
}

void ph_sensor()
{
    ph = simulatedPH[step];
    step++;
    if (step >= 11)
        step = 10;
}

void update_firebase()
{
    Firebase.setFloat(firebaseData, path + "/ph", ph);
    Firebase.setFloat(firebaseData, path + "/temperature", temperature);
    Firebase.setFloat(firebaseData, path + "/humidity", humidity);
    Firebase.setFloat(firebaseData, path + "/co2", co2);
    Firebase.setFloat(firebaseData, path + "/nh3", nh3);
    Firebase.setFloat(firebaseData, path + "/cn_ratio", cn_ratio);

    if (firebaseData.httpCode() > 0)
    {
        Serial.println("✅ Data sent to Firebase successfully.");
    }
    else
    {
        Serial.print("❌ Firebase Error: ");
        Serial.println(firebaseData.errorReason());
    }
}

void print_values()
{
    Serial.println("\n-- Sensor Readings --");
    // Serial.print("Raw PH ADC: "); Serial.print(adc);
    Serial.print(" | Voltage: ");
    Serial.print(ph, 2);
    Serial.print(" V | pH: ");
    Serial.println(ph, 2);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
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
}

void setup()
{
    pinMode(PH_SENSOR_PIN, INPUT);
    pinMode(MQ135_SENSOR_PIN, INPUT);
    pinMode(DHTPIN, INPUT);
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

    gas_sensor();
    temp_sensor();
    ph_sensor();
    print_values();
    update_firebase();

    delay(2000); // Wait before next reading
}
