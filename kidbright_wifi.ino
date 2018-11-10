#include <WiFi.h>
#include <MicroGear.h>

const char* ssid     = "kidbright";
const char* password = "12345678";

WiFiClient client;
void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
   }
   Serial.println("WiFi connected");  
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
}

void loop() {
  
}
  
