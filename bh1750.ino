#include <BH1750.h>
#include <Wire.h>
#include <WiFiNINA.h>

BH1750 lightMeter;
float lux;

// WiFi credentials
char ssid[] = "M30s";
char pass[] = "vcvw3532";

WiFiClient client;

char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME = "/trigger/light/json/with/key/ofHfOy24U8Z572xy8SHdtZm97CE8nfjBDo4YKZq9o4J"; // change your EVENT-NAME and YOUR-KEY
String queryString;

void setup() {
  Serial.begin(9600);

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();

  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  lux = lightMeter.readLightLevel(); // Read light intensity
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  if (Serial.read() == 's') {
    // Get the current timestamp in the desired format
    String timestamp = "October 1, 2023 at 12:06 AM";

    // Update the queryString with the current lux value and timestamp
    queryString = "?value1=" + String(lux) + "&value2=" + timestamp;

    // Make an HTTP request to the IFTTT webhook:
    if (client.connect(HOST_NAME, 80)) {
      // Send HTTP header
      client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
      client.println("Host: " + String(HOST_NAME));
      client.println("Connection: close");
      client.println(); // End HTTP header

      while (client.connected()) {
        if (client.available()) {
          // Read an incoming byte from the server and print it to the Serial Monitor:
          char c = client.read();
          Serial.print(c);
        }
      }

      // The server's disconnected, stop the client:
      client.stop();
      Serial.println();
      Serial.println("Disconnected");
    }
  }
  delay(2000);
}
