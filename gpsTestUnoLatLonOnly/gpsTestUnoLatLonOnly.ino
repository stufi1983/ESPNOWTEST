#include <SoftwareSerial.h>

#include <TinyGPS.h>

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

TinyGPS gps;
SoftwareSerial ss(4, 3);

void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;


  while (ss.available() > 0)
  {
    char c = ss.read();
    // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
    if (gps.encode(c)) // Did a new valid sentence come in?
      newData = true;
  }


  if (newData)
  {
    float flat = 0, flon = 0;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    if (flat != 0 && flon != 0) {
      Serial.println(flat);
      Serial.println(flon);
    }
  }
}
