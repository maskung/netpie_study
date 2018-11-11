#include <WiFi.h>
#include <MicroGear.h>
#define MAX_OUT_CHARS 16

const char* ssid     = "kidbright";
const char* password = "12345678";

#define APPID   "srisaketmaad2018"
#define KEY     "gMYm2ynPXCMbRdk"
#define SECRET  "O2qsa8buACgPuWxyk9n7O7T0E"
#define ALIAS   "kidbrightsuphanut"

#define WIFI_LED 2
#define NETPIE_LED 12

#define LIGHTTOPIC "/lightdata/" ALIAS 

WiFiClient client;
MicroGear microgear(client);

int timer = 0;

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
   
}

void loop() {
  /* To check if the microgear is still connected */
    static int i =0 ;
    static char   buffer[MAX_OUT_CHARS + 1];
    if (microgear.connected()) {
        Serial.println("connected");
        digitalWrite(NETPIE_LED, LOW);

        /* Call this method regularly otherwise the connection may be lost */
        microgear.loop();

        if (timer >= 1000) {
            Serial.println("Publish...");

            /* Chat with the microgear named ALIAS which is myself */
            sprintf(buffer,"%d", 1024-analogRead(A0) );   
            microgear.chat(ALIAS,buffer );
            microgear.publish(LIGHTTOPIC, buffer);
            timer = 0;
        } 
        else timer += 100;
    }
    else {
        Serial.println("connection lost, reconnect...");
        digitalWrite(NETPIE_LED, HIGH);
        if (timer >= 5000) {
            microgear.connect(APPID);
            timer = 0;
        }
        else timer += 100;
    }
    delay(100);
}
