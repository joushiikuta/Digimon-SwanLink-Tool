/*
  SwanLink Jogress Evolution for D-3(2000).
  Jogress with Wonderswan(Digimon Adventure 02: D1 Tamers).
  Fouce Adult Level or Perfect Level Jogress to Ultimate Level.
  
  Author : joushiikuta
  Date   : 2025/03/21
  Rev    : 1.0.2
  link   : https://www.youtube.com/@joushiikuta

  ESP32 : GPIO_NUM_17 → HDMI PIN9
          GPIO_NUM_16 → HDMI PIN13
          GND         → HDMI PIN17
*/

#define RX2_Pin GPIO_NUM_16
#define TX2_Pin GPIO_NUM_17
#define LED_Pin GPIO_NUM_2

unsigned long Packet_Recive[2];
unsigned long Packet_Send[2] = {
  0b00000010101000000000000111100001, 
  0b00101101111000000000000111100001
};

void SwanLinkSend(unsigned long Packet) {
  for (int i = 0; i < 32; i++) {
    digitalWrite(TX2_Pin, (Packet & (1 << i)) ? HIGH : LOW);
    delayMicroseconds(100);
  }
  digitalWrite(TX2_Pin, 0);
}

unsigned long SwanLinkRecive() { // TODO
  delay(10);
  return 0;
}

void Waiting() {
  while (digitalRead(RX2_Pin) == HIGH) {};
  Serial.println("Waiting...");
  while (digitalRead(RX2_Pin) == LOW) {};
  digitalWrite(LED_Pin, 1);
  Serial.println("Try Jogress!");
  for (int i = 0; i < 2; i++) {
    Packet_Recive[i] = SwanLinkRecive();
    SwanLinkSend(Packet_Send[i]);
  }
  delay(1000);
  digitalWrite(LED_Pin, 0);
}

void setup() {
  Serial.begin(115200);
  pinMode(RX2_Pin, INPUT_PULLDOWN);
  pinMode(TX2_Pin, OUTPUT);
  pinMode(LED_Pin, OUTPUT);
  digitalWrite(TX2_Pin, 0);
  digitalWrite(LED_Pin, 0);
  Serial.println("D-3 SwanLink Start!");
}

void loop() {
  Waiting();
}
