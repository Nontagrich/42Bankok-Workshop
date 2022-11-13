#include <SimpleDHT.h>          //โหลดไลบารี่ DHT11

int DHTSENSOR = 15;             //GPIO15 (ADC2_CH3)
byte temperature = 0;           //กำหนดตัวแปรเก็บค่าอุณหภูมิ
byte humidity = 0;              //กำหนดตัวแปรเก็บค่าความชื้นสัมสัทธ์

SimpleDHT11 dht11;              //ระบุรุ่นเซ็นเซอร์รุ่น DHT11

void setup() {
  Serial.begin(115200);
  pinMode(15, INPUT); 
}

void loop() {
  dht11.read(DHTSENSOR, &temperature, &humidity, NULL);  //อ่านค่าจากเซ็นเซอร์
  while (temperature == 0) {
  dht11.read(DHTSENSOR, &temperature, &humidity, NULL);  //อ่านค่าจากเซ็นเซอร์
  Serial.println("DHT11 get failed");
  }
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("% | ");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("C");
  
  //#################################################
  delay(2000);
}
