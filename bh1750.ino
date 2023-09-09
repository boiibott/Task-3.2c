#include <BH1750.h>

#include <Wire.h>

#include <WiFiNINA.h>

// Replace with your network credentials
char ssid[] = "Free virus";
char pass[] = "mask90009000";

WiFiClient client;

char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME   = "https://maker.ifttt.com/trigger/Light_intensity/json/with/key/ofHfOy24U8Z572xy8SHdtZm97CE8nfjBDo4YKZq9o4J"; // change your EVENT-NAME and YOUR-KEY

// Create an instance of the BH1750 sensor
BH1750 lightSensor;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  while (!Serial);

  // Initialize the BH1750 sensor
  if (!lightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 initialization failed!");
    while (1);
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  // Read light intensity in lux
  float lux = lightSensor.readLightLevel();

  // Define a threshold value for light intensity
  float thresholdLux = 500.0;

  // Check if light intensity exceeds the threshold
  if (lux > thresholdLux) {
    sendIFTTTNotification(lux);
  }

  // Delay before next reading
  delay(60000); // Delay for 1 minute
}

void sendIFTTTNotification(float luxValue) {
  if (client.connect(HOST_NAME, 80)) {
    // Send HTTP request
    client.print("GET " + PATH_NAME);
    client.print("?value1=");
    client.print(luxValue);
    client.println(" HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();

    Serial.println("IFTTT Notification Sent");
  } else {
    Serial.println("Unable to connect to IFTTT");
  }

  // Wait for server response
  while (client.connected()) {
    if (client.available()) {
      // Read and print the response from the server
      char c = client.read();
      Serial.print(c);
    }
  }

  // Disconnect from the server
  client.stop();
  Serial.println("\nDisconnected");
}
