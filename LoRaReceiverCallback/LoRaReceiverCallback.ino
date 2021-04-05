#include <SPI.h>
#include <LoRa.h>

#ifdef ARDUINO_SAMD_MKRWAN1300
#error "This example is not compatible with the Arduino MKR WAN 1300 board!"
#endif
byte counter = 0;
int total = 0;
void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver Callback");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // Uncomment the next line to disable the default AGC and set LNA gain, values between 1 - 6 are supported
  // LoRa.setGain(6);

  // register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();
}

void loop() {
  // do nothing
}

void onReceive(int packetSize) {
  // received a packet
  counter++;
  if (counter == 100) {
    Serial.print(counter);
    Serial.println("/100");
    counter++;
  }
  if(counter>100) return;
  Serial.print("Received ");
  Serial.print(counter);
  Serial.print(" packet '");

  String temp;
  // read packet
  for (int i = 0; i < packetSize; i++) {
    temp += (char)LoRa.read();
  }
  Serial.print(temp);
  temp = temp.substring(3, temp.length());
  total = temp.toInt();

  if (total <= 1) {
    counter = 1;
  }

  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
}
