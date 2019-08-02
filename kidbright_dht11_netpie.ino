#include <WiFi.h>
#include <MicroGear.h>

const char* ssid     = "kidbright";
const char* password = "12345678";

#include <MicroGear.h>
#define APPID   "srisaketmaad2018"      // APPID ที่ได้จาการสร้าง ใน netpie.io
#define KEY     "gMYm2ynPXCMbRdk" // KEY ได้จากการสร้าง ใน netpie.io
#define SECRET  "O2qsa8buACgPuWxyk9n7O7T0E" // SECRET ได้จากการสร้างใน netpio.io
#define ALIAS   "kidbright_dht11"     // ชื่อของอุปกรณ์เพื่อใช้ในการอ้างอิงการส่งข้อความกัน

WiFiClient client;


#include "DHT.h"

#define DHTPIN 4     // ขาอินพุทสำหรับเซนเซอร์

// ไลบราลีนี้รองรับเซนเซอร์หลายชนิด สามารถเลือกเป็นชนิดที่ต้องการได้โดย เอาคอมเมนต์ (//) ออก
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)


// ประกาศตัวแปร DHT ซึ่งเป็น object เพื่อเรียกใช้ฟังก์ชั่นต่างๆ ของ ไลบรารี ในโปรแกรม
DHT dht(DHTPIN, DHTTYPE);

int timer = 0;
MicroGear microgear(client);
int last_chat_time = 0;

/* การทำงานของไลบรารีไมโครเกียร์ นั้น จะเป็นลักษณะแบบ event driven ซึ่งเราสามารถที่จะแทรกการทำงานในอีเวนท์ต่างๆ ในแต่ละอีเวนท์ได้ */
/* เมื่อได้รับข้อความใหม่เข้ามา ให้พิมพ์ออกมาแสดงให้ดูใน serial monitor */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
}

/* เมื่อมีการสร้างอุปกรณ์ใหม่ๆ เชื่อมต่อเข้ามาก็พิมพ์แสดงออกมาให้ดู */
void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Found new member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();  
}

/* เมื่อมีการยกเลิกการเชื่อต่อกับอุปกรณ์ใหม่ ก็ให้พิมพ์รายการอุปกรณ์ที่เหลือให้ดู */
void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Lost member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();
}

/* เมื่อมีการเชื่อมต่อกับ netpie ก็ให้แสดงสถานะว่าเชื่อมต่อสำเร็จแล้ว */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    microgear.setAlias(ALIAS);
}

void setup() {
  Serial.begin(115200);
  Serial.println("DHT start...");

  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {    
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

 /* ทำการผูกอีเวนท์ที่เขียนฟังก์ชั่นเพิ่มเติมไว้ทั้งหมด */
    /* เรียกใช้อีเวนท์ onMsghandler() เมื่อมีข้อความใหม่เข้ามา */
    microgear.on(MESSAGE,onMsghandler);

    /* เรียกใช้อีเวนท์ onFoundgear() ถูกเรียกใช้เมื่อมีการเชื่อมต่อกับอุปกรณ์ใหม่ */
    microgear.on(PRESENT,onFoundgear);

    /* เรียกใช้อีเวนท์ onLostgear() เมื่ออุปกรณ์ถูกถอดออก */
    microgear.on(ABSENT,onLostgear);

    /* เรียกใช้อีเวนท์ onConnected() เมื่อการเชื่อมต่อกับ NETPIE สำเร็จ */
    microgear.on(CONNECTED,onConnected);

   /* Initial with KEY, SECRET and also set the ALIAS here */
    microgear.init(KEY,SECRET,ALIAS);

    /* connect to NETPIE to a specific APPID */
    microgear.connect(APPID);

}

void loop() {
  // รอประมาน 2 วินาทีระหว่างรอการวัด
  delay(2000);

// เช็คว่า connect อยู่หรือเปล่า ถ้าไม่ให้ทำการ conenct ใหม่
    if (microgear.connected()) {
        // เรียก function นี้เป็นระยะๆ ไม่เช่นนั้น connection จะหลุด
        microgear.loop();

        if (millis() - last_chat_time >= 1000) {
  // ในการอ่านค่าเซนเซอร์อุณหภูิมิและความชื้นอาจจะใช้เวลาประมาณ 250 milisec 
    // แต่ถ้าเป็นรุ่นเก่าๆ อาจะใช้เวลาถึง 2 วินาที
    float h = dht.readHumidity();
   // อ่านค่าอุณหภูมิเป็นองศาเซลเซียส (ค่าดีฟอลท์)
    float t = dht.readTemperature();
    // ถ้าต้องการให้อ่านค่าอุหภูมิเป็นฟาเรียนไฮต์ให้ true เข้าไปในพารามิเตอร์ (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // ตรวนสอบว่ามีการอ่านถูกต้องหรือไม่ถ้าไม่ให้เริ่มต้นใหม่
   if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // คำวนค่า Heat Index เป็นฟาเรนต์ไฮต์  (ค่าดีฟอลท์)
    float hif = dht.computeHeatIndex(f, h);
    // ถ้าต้องการให้คำนวณค่า Heat Index เป็นเซลเซียสให้ใส่ false เป็นพารามิตเตอร์ที่ 3 (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" *C ");
    Serial.print(hif);
    Serial.println(" *F");

  // จัดเตรียมข้อมูลเพื่อ publish
            String data = String(h)+","+String(t);
      
            char msg[132 ];
            data.toCharArray(msg,data.length()+1);
            Serial.println(msg); 

            Serial.println("Send chat message >>>");

            // chat หา device ชื่อ  ALIAS ซึ่งก็คือชื่อของตัวเอง
            microgear.chat(ALIAS,"Hello..");
            microgear.publish("/dht11",msg);
            last_chat_time = millis();
        } 
    }
    else {
        Serial.println("connection lost, reconnect...");
        if (timer >= 5000) {
            microgear.connect(APPID);
            timer = 0;
        }
        else timer += 100;
    }
    delay(100);

  }
