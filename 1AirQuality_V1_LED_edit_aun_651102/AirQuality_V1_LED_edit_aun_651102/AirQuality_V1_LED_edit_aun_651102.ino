int LED1 = 14;
int LED2 = 26;
int LED3 = 33;
void setup() {
  Serial.begin(115200); 
  pinMode(LED1, OUTPUT); 
  pinMode(LED2, OUTPUT); 
  pinMode(LED3, OUTPUT); 
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  digitalWrite(LED3,HIGH);
  delay(1000);
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
}

void loop() {
  digitalWrite(LED1,HIGH);
  delay(500);
  digitalWrite(LED1,LOW);
  
  delay(500);
  
  digitalWrite(LED2,HIGH);
  delay(500);
  digitalWrite(LED2,LOW);
  
  delay(500);
  
  digitalWrite(LED3,HIGH);
  delay(500);
  digitalWrite(LED3,LOW);
  
  delay(2000);
}
