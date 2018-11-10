#include <WiFi.h>
#include <MicroGear.h>

const char* ssid     = "kidbright";
const char* password = "12345678";

#define APPID   "srisaketmaad2018"
#define KEY     "gMYm2ynPXCMbRdk"
#define SECRET  "O2qsa8buACgPuWxyk9n7O7T0E"
#define ALIAS   "kidbrightsuphanut"

#define WIFI_LED 2
#define NETPIE_LED 12

WiFiClient client;
MicroGear microgear(client);

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    microgear.setAlias(ALIAS);
}

void setup() {
  pinMode(WIFI_LED, OUTPUT);
  pinMode(NETPIE_LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("Starting...");
  if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
   }
   digitalWrite(WIFI_LED, LOW);
   Serial.println("WiFi connected");  
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
   microgear.on(CONNECTED,onConnected);
   microgear.init(KEY,SECRET,ALIAS);
   microgear.connect(APPID);
   digitalWrite(NETPIE_LED, LOW);
}

void loop() {
  
}
