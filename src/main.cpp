#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP32.h>
#include <DHTesp.h>
#include <BH1750.h>
#define LED_GREEN  4
#define LED_YELLOW 5
#define LED_RED    18
#define PUSH_BUTTON 23
#define DHT_PIN 19

#define CAYENNE_LED_RED 4
#define CAYENNE_LED_GREEN 5
#define CAYENNE_LED_YELLOW 6

DHTesp dht;
BH1750 lightMeter;

// WiFi network info.
char ssid[] = "iot2.4g";
char wifiPassword[] = "iot2.4g123";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "01e573c0-7554-11ec-bbfc-979c23804144";
char password[] = "0737f4c6b6b29e0a4d179e837f3c3df52fd3a839";
char clientID[] = "c5507fa0-d1e0-11ed-b72d-d9f6595c5b9d";

void setup() {
	Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  dht.setup(DHT_PIN, DHTesp::DHT11);
  Wire.begin();
  lightMeter.begin(); 
  Serial.println("Connecting to Cayenne...");
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
	Cayenne.loop();
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
	// Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
	Cayenne.virtualWrite(0, millis());
  float fHumidity = dht.getHumidity();
  float fTemperature = dht.getTemperature();
  float lux = lightMeter.readLightLevel();

	// Some examples of other functions you can use to send data.
	Cayenne.celsiusWrite(1, fTemperature);
	Cayenne.luxWrite(2, lux);
  Cayenne.virtualWrite(3, fHumidity, TYPE_RELATIVE_HUMIDITY, UNIT_PERCENT);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
  switch (request.channel)
  {
  case CAYENNE_LED_RED:
    digitalWrite(LED_RED, getValue.asInt());
    break;

    case CAYENNE_LED_GREEN:
    digitalWrite(LED_GREEN, getValue.asInt());
    break;

    case CAYENNE_LED_YELLOW:
    digitalWrite(LED_YELLOW, getValue.asInt());
    break;  
  }
}