#include <SPI.h>
#include <LoRa.h>
//https://github.com/sandeepmistry/arduino-LoRa

/*
ESP 8266                          SX1278 Pins
GND                                       GND
3.3V                                      VCC
D8                                        NSS
D7                                        MOSI
D6                                        MISO
D5                                        SCK
D0                                        RST
D2                                        DIO0
DIO0 pin is optional, it is only needed for receive callback mode. If DIO0 pin is used, it must be interrupt capable via attachInterupt()
*/
#define ss 15
#define rst 16
#define dio0 2

int counter = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");
LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("ID ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(1000);
}
