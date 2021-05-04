const int s1 = 5; //สวิตซ์s1 รออ่านจากขา5
const int s2 = 6; //สวิตซ์s2 รออ่านจากขา6
int relay = 7;  //รีเลย์ควบคุม
int mainRelay = 8; //เมนรีเลย์ขา 8
int offset =20;// เช็คค่าความถูกต้องในฟังก์ชั่นวัดแรงดันไฟตรง
//#include <EasyScheduler.h>

const int VIN = A1; // define the Arduino pin A0 as voltage input (V in)
const float VCC   = 5.01;// supply voltage
const int MODEL = 2;   // enter the model (see above list)
#include <Robojax_AllegroACS_Current_Sensor.h>
Robojax_AllegroACS_Current_Sensor robojax(MODEL,VIN);

#include <PZEM004Tv30.h>           //ประกาศเรียกใช้Library PZEM-004T
PZEM004Tv30 pzem(11, 12);          //ใช้งานขา 11,12 Tx,Rxตามลำดับ
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f,20,4);  //ประกาศเรียกใช้Library LCD ใช้งานขา scl,sda
#include <SoftwareSerial.h>        //เรียกใช้การส่งข้อมูลแบบ SoftwareSerial จากอาดูยโน่ ไปยัง ESP
SoftwareSerial UnoSerial(3, 2);    // ตั้งชื่อUnoSerial และใช้งานขา 3,2 Rx |Tx ตามลำดับ
   
//Schedular Task1;
double  volt;
void display();
//unsigned long previousMillis = 0;
//const long interval = 1000;  
//unsigned long currentMillis = millis();
//void sending();

float currentTest = robojax.getCurrent();


     int state1 = digitalRead(s1); //ประกาศตัวแปรstate1 อ่านจากs1 
     int state2 = digitalRead(s2); //ประกาศตัวแปรstate2 อ่านจากs2

     
void setup() {
  pinMode(s1, INPUT);      //ประกาศใช้งานขา s1 เป็นอินพุต เพื่อรับค่าจากการกดสวิตซ์เลือกแรงดัน 12v         
  pinMode(s2, INPUT);       //ประกาศใช้งานขา s2 เป็นอินพุต เพื่อรับค่าจากการกดสวิตซ์เลือกแรงดัน 24v
  pinMode(relay, OUTPUT);    //ประกาศใช้งานขา relay เป็นเอาท์พุต เพื่อควบคุมการชาร์จแบตเตอรี่
  pinMode(3, INPUT);          //ส่งค่าและรับค่ากับesp82666
  pinMode(2, OUTPUT);          //ส่งค่าและรับค่ากับesp82666
  pinMode(mainRelay,OUTPUT);    //ประกาศใช้งานขา mainRelay เป็นเอาท์พุตเพื่อตัดการทำงานของระบบ
  Serial.begin(9600);            //ติดต่อข้อมูลผ่านซอฟแวร์ซีเรี่ยว ใช้ความติดต่อสื่อสารที่ 9600 บัตเรท
  UnoSerial.begin(230400);         //ติดต่อข้อมูลผ่านซอฟแวร์ซีเรี่ยว ใช้ความติดต่อสื่อสารที่ 9600 บัตเรท
  lcd.begin();
  lcd.clear();                       
  lcd.backlight();
  lcd.setCursor(3, 0); lcd.print("chargingStation");
  Serial.println("chargingStation");
     int v = analogRead(A1);  // อ่านอินพุต
     volt = map(v,0,1023, 0, 3000) + offset ;// map 0-1023 to 0-2500 และเช็คความถูกต้องด้วยoffset
     volt /=100;// หารโดย 100 เพื่อเป็นเลข 2 หลัก
     volt -=0.6;

delay(1500);
}

void loop() {
  
     int state1 = digitalRead(s1); //ประกาศตัวแปรstate1 อ่านจากs1 
     int state2 = digitalRead(s2); //ประกาศตัวแปรstate2 อ่านจากs2
     delay(500);
  if(state1 == LOW || state2 == LOW ){

     lcd.begin();
     lcd.clear();                       
     lcd.backlight();
     lcd.setCursor(3, 0); lcd.print("chargingStation");
     Serial.println("chargingStation");     
     volt = 0;
     digitalWrite(relay,LOW);
     Serial.println("รีเลย์จาก");
     delay(2000);
     int v = analogRead(A1);  // อ่านอินพุต
     volt = map(v,0,1023, 0, 3000) + offset;// map 0-1023 to 0-2500 และเช็คความถูกต้องด้วยoffset
     volt /=100;// หารโดย 100 เพื่อเป็นเลข 2 หลัก
     volt -=0.6;
//     UnoSerial.flush();
//     UnoSerial.write('#');
     UnoSerial.print(volt);
     
     Serial.println("Vdc : ");
     Serial.println(volt,1);
     delay(100);
//     Serial.print("vDc : ");Serial.print(volt);Serial.println("v");
//     Task1.check(MultiTask, 10000)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
/////////////////////////////////////////ถ้าสวิตซ์ s1 ทำงาน และแรงดันต่ำกว่า 12.6 ให้ปริ้นท์ lcd : charging12v และรีเลย์ต่อการชาร์จแบตเตอรี่///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                      
  if( state1 == LOW && volt< 14 && volt >3){ 
      lcd.clear(); 
      lcd.setCursor(3, 1);lcd.print("charging12V");       
      delay(1000);
      lcd.clear(); 
      lcd.setCursor(0, 0);lcd.print("Vbat.:");lcd.print(volt,1); lcd.print("V");     
      delayMicroseconds(10000);
      Serial.println("รีเลย์หนึ่งทำ");
      Serial.println("Charging Battery"); 
      digitalWrite(relay, HIGH);
      delay(2500);
      display();
//      robojax.getCurrent() / 10;
      delay(600000);
      
      
     }

//////////////////////////////////////////เมื่อs1ทำงานพร้อมกับแรงดันมากกว่า 12v ให้ mainRelayตัดการทำงาน และปริ้นท์ขึ้น lcd : MaximumBattery//////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////     
      else if(state1 == LOW && volt > 14){            
      Serial.println("full");  
      lcd.clear();                      
      lcd.setCursor(3, 0); lcd.print("MaximumBattery");
      lcd.setCursor(4, 1); lcd.print("Pls Unplug");      
      digitalWrite(mainRelay,HIGH);
      delay(2000);
      digitalWrite(mainRelay,LOW);                     
        }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
/////////////////////////////////////////ถ้าสวิตซ์ s2 ทำงาน และแรงดันต่ำกว่า 24.60 ให้ปริ้นท์ lcd : charging24v และรีเลย์ต่อการชาร์จแบตเตอรี่///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if( state2 == LOW && volt< 28 && volt > 18 ){      
      lcd.clear(); 
      lcd.setCursor(3, 1);lcd.print("charging24V"); 
      delay(1000); 
      lcd.setCursor(0, 0);lcd.print("Vbat.:");lcd.print(volt,1); lcd.print("V");     
      delayMicroseconds(10000);
      Serial.println("รีเลย์สองทำ");
      Serial.print("Charging Battery"); 
      digitalWrite(relay, HIGH);
      delay(2500);
      display();
//      robojax.getCurrent() / 50;
      delay(600000);
      }
      
//////////////////////////////////////////เมื่อs2ทำงานพร้อมกับแรงดันมากกว่า 24v ให้ mainRelayตัดการทำงาน และปริ้นท์ขึ้น lcd : MaximumBattery//////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////     
      else if(state2 == LOW && volt > 28){
       Serial.println("full");  
      lcd.clear();                      
      lcd.setCursor(3, 0); lcd.print("MaximumBattery");
      lcd.setCursor(4, 1); lcd.print("Pls Unplug");      
      digitalWrite(mainRelay,HIGH);
      delay(2000);
      digitalWrite(mainRelay,LOW);
        }


}

else{
     lcd.clear();                       
     lcd.backlight();
     lcd.setCursor(3, 0); lcd.print("Press the Button ");
     lcd.setCursor(10, 1); lcd.print("&");
     lcd.setCursor(6, 2); lcd.print("Pluging");
  
  }
}
     

void display(){
// Serial.flush();
        float voltage = pzem.voltage();
    if( !isnan(voltage) ){
        Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
    } else {
        Serial.println("Error reading voltage");
    }

    float current = pzem.current();
    if( !isnan(current) ){
        Serial.print("Current: "); Serial.print(current); Serial.println("A");
    } else {
        Serial.println("Error reading current");
    }

    float power = pzem.power();
    if( !isnan(power) ){
        Serial.print("Power: "); Serial.print(power); Serial.println("W");
    } else {
        Serial.println("Error reading power");
    }
        float pf = pzem.pf();
    if( !isnan(pf) ){
        Serial.print("PF: "); Serial.println(pf);
    } else {
        Serial.println("Error reading power factor");
    }

    Serial.println();
    delay(1000);
 
     int v = analogRead(A1);  // อ่านอินพุต
     volt = map(v,0,1023, 0, 3000) + offset ;// map 0-1023 to 0-2500 และเช็คความถูกต้องด้วยoffset
     volt /=100;// หารโดย 100 เพื่อเป็นเลข 2 หลัก
     volt -=0.6;


//       lcd.clear(); 
//       lcd.setCursor(0, 0);lcd.print("Vbat.:");lcd.print(volt,1); lcd.print("V");
       lcd.setCursor(0, 1);lcd.print("Pac:");lcd.print(power); lcd.print("W");                       
       lcd.setCursor(0, 2);lcd.print("Vac:");lcd.print(voltage); lcd.print("V");
       lcd.setCursor(0, 3);lcd.print("Iac:");lcd.print(current-0.06);lcd.print("A");
       delay(1000);
       //       if( state1 == LOW && volt< 14 && volt >3){
//  lcd.setCursor(0, 1);lcd.print("Idc:");lcd.print(robojax.getCurrent() / 7.5); lcd.print("A");
//  
//  } 
//       if( state2 == LOW && volt< 28 && volt > 18 ){
//    lcd.setCursor(0, 1);lcd.print("Idc:");lcd.print(robojax.getCurrent() / 50); lcd.print("A");
//    
//        
//        }     
  
  }
