/*
  ESP32 SwanLink Tool (2 in 1)

  Switch ON  → D-Ark SwanLink Punching
  Switch OFF → D-3 SwanLink Jogress Evolution

  SwanLink Jogress Evolution for D-3(2000).
  Jogress with Wonderswan(Digimon Adventure 02: D1 Tamers).
  Fouce Adult Level or Perfect Level Jogress to Ultimate Level.

  A punching bag for D-Ark(2001).
  Versus Wonderswan Impmon from Digimon Tamers Battle Spirit.
  Not 100% winning!

  Author : joushiikuta
  Date   : 2025/05/16
  Rev    : 1.0.0
  link   : https://www.youtube.com/@joushiikuta

  ESP32 : GPIO_NUM_15 → SWITCH → GND
          GPIO_NUM_16 → HDMI PIN13
          GPIO_NUM_17 → HDMI PIN9
          GND         → HDMI PIN17
*/

#define SW_PIN GPIO_NUM_15
#define RX2_PIN GPIO_NUM_16
#define TX2_PIN GPIO_NUM_17
#define LED_PIN GPIO_NUM_2

unsigned long packet_recive[4];

unsigned long packet_send_jogress[2] = {
  0b00000010101000000000000111100001,
  0b00101101111000000000000111100001
};

unsigned long packet_send_battle[4] = {
  0b00001111001000000000000100000001,
  0b00111110101000000000000111100001,
  0b00111101101000000000000110100001,
  0b00010111111000000000000111100001
};

void SwanLinkSend(unsigned long packet) {
  for (int i = 0; i < 32; i++) {
    digitalWrite(TX2_PIN, (packet & (1 << i)) ? HIGH : LOW);
    delayMicroseconds(100);
  }
  digitalWrite(TX2_PIN, 0);
}

unsigned long SwanLinkRecive(unsigned char t) { // TODO
  delay(t);
  return 0;
}

bool IsJogress() {
  return digitalRead(SW_PIN) ? true : false;
}

void WaitingJogress() {
  while (digitalRead(RX2_PIN) == HIGH) {
    if (!IsJogress()) return ;
  }
  Serial.println("D-3 Jogress Mode.");
  while (digitalRead(RX2_PIN) == LOW) {
    if (!IsJogress()) return ;
  }
  digitalWrite(LED_PIN, 1);
  Serial.println("Try D-3 Jogress!");
  for (int i = 0; i < 2; i++) {
    packet_recive[i] = SwanLinkRecive(10);
    SwanLinkSend(packet_send_jogress[i]);
  }
  delay(1000);
  digitalWrite(LED_PIN, 0);
}

void WaitingBattle() {
  while (digitalRead(RX2_PIN) == HIGH) {
    if (IsJogress()) return ;
  }
  Serial.println("D-Ark Battle Mode.");
  while (digitalRead(RX2_PIN) == LOW) {
    if (IsJogress()) return ;
  }
  digitalWrite(LED_PIN, 1);
  Serial.println("Try D-Ark Battle!");
  for (int i = 0; i < 4; i++) {
    packet_recive[i] = SwanLinkRecive(25);
    SwanLinkSend(packet_send_battle[i]);
  }
  delay(1000);
  digitalWrite(LED_PIN, 0);
}
void setup() {
  Serial.begin(115200);
  pinMode(SW_PIN, INPUT_PULLUP);
  pinMode(RX2_PIN, INPUT_PULLDOWN);
  pinMode(TX2_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(TX2_PIN, 0);
  digitalWrite(LED_PIN, 0);
  Serial.println("ESP32 SwanLink Tool Start!");
}

void loop() {
  IsJogress() ? WaitingJogress() : WaitingBattle();
  delay(500);
}
