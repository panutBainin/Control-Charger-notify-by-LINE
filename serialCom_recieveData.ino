#include <EasyScheduler.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
SoftwareSerial NodeSerial(D2, D3); // RX | TX


#include <TridentTD_LineNotify.h>

#define SSID        "EPE 2.4 G"
#define PASSWORD    "EPE313131"
#define LINE_TOKEN  "gYqVZ81ze5wEibcH1J6Orhofmy5BNAvjyDAF8bPlzFT"
float data;
Schedular Task1;


void setup() {
    pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  Serial.begin(9600);
  NodeSerial.begin(230400);
  Task1.start();
  Serial.println(LINE.getVersion());
   WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while(WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(400); }
  Serial.printf("\nWiFi connected\nIP : ");   // จะหลุดออกจาก while ก็ต่อเมือ Connected เรียบร้อย
  Serial.println(WiFi.localIP());  
  LINE.setToken(LINE_TOKEN);

  }

  
  void loop() {
    while (NodeSerial.available() > 0){
      Serial.println("recieved");
    data = NodeSerial.parseFloat();
   if(data > 3 &&  data < 26 ){
  Serial.print("Battery(V) : ");
  Serial.println(data);
  LINE.notify("แบตเตอรี่เท่ากับ : "+String(data)+" V");
  data = 0;
    

    }
    }
  }

  
