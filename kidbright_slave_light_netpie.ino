#include <WiFi.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <MicroGear.h>
#define MAX_OUT_CHARS 16

Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix();

const char* ssid     = "kidbright";
const char* password = "12345678";

#define APPID   "srisaketmaad2018"
#define KEY     "gMYm2ynPXCMbRdk"
#define SECRET  "O2qsa8buACgPuWxyk9n7O7T0E"
#define ALIAS   "kidbrightsuphanutdisplay"

#define WIFI_LED 2
#define NETPIE_LED 12 

WiFiClient client;
MicroGear microgear(client);

int timer = 0;

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    microgear.setAlias(ALIAS);
}

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);

    digitalWrite(NETPIE_LED, HIGH);
    delay(10);

    //if (*(char *)msg == '0') updateLED(0);
    //else if (*(char *)msg == '1') updateLED(1);
    matrix.clear();
    matrix.setTextSize(1);
    matrix.setTextWrap(false);
    matrix.setTextColor(LED_ON);
    matrix.setRotation(1);
    matrix.setCursor(0,0);
    matrix.print((char *)msg);
    matrix.writeDisplay();

}

void setup() {
  
  pinMode(WIFI_LED, OUTPUT);
  pinMode(NETPIE_LED, OUTPUT);
  digitalWrite(NETPIE_LED, HIGH);
  digitalWrite(WIFI_LED, HIGH);

  matrix.begin(0x70); 
  
  Serial.begin(115200);
  Serial.println("Starting...");
  if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            digitalWrite(WIFI_LED, LOW);
            delay(250);
            Serial.print(".");
            digitalWrite(WIFI_LED, HIGH);
            delay(250);
        }
   }
   Serial.println("WiFi connected");  
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
   microgear.on(CONNECTED,onConnected);
   microgear.on(MESSAGE,onMsghandler);
   microgear.init(KEY,SECRET,ALIAS);
   microgear.connect(APPID);
   
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {
      digitalWrite(WIFI_LED, LOW);
  } else {
      digitalWrite(WIFI_LED, HIGH);
  }
  /* To check if the microgear is still connected */
    static int i =0 ;
    static char   buffer[MAX_OUT_CHARS + 1];
    if (microgear.connected()) {
        Serial.println("connected");
        digitalWrite(NETPIE_LED, LOW);

        /* Call this method regularly otherwise the connection may be lost */
        microgear.loop();

        if (timer >= 1000) {
            //Serial.println("refresh...");
            

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
