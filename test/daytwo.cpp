#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
float co2, nh3, cn_ratio, ph, temperature, humidity;
// values
float simulatedPH[] = {8.21, 8.73, 7.96, 7.13, 7.08, 6.89, 6.83, 6.52, 6.33, 6.33, 6.33};
int step = 0;
void setup()
{
    Serial.begin(115200);
    pinMode(4, INPUT);
    dht.begin();
}

void gas_sensor()
{
    int mq135Value = analogRead(4);
    Serial.println(mq135Value);
    co2 = mq135Value * 0.1;
    nh3 = mq135Value * 0.08;
    cn_ratio = nh3 != 0 ? co2 / nh3 : 0;
}

void ph_sensor()
{
    ph = simulatedPH[step];
    step++;
    if (step >= 11)
        step = 10;
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

void loop()
{
    // int mq135Value = analogRead(4);
    // Serial.println(mq135Value);
    // co2 = mq135Value * 0.1;
    // nh3 = mq135Value * 0.08;
    // cn_ratio = nh3 != 0 ? co2 / nh3 : 0;
    gas_sensor();
    ph_sensor();
    print_values();

    delay(1000);
}