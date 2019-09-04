#include <SPI.h>
#include <WiFiNINA.h>

IPAddress pihole1(192,168,1,3);
IPAddress pihole2(192,168,1,4);

void setup() 
{
  initSerialCommunications();
  initDisplay();
  connectToWiFi();
  sendToDisplay("Connected to WiFi", 1);
}

void loop() {
  Serial.println("PiHole 1:");
  Serial.println(getSummaryStats(pihole1));
  Serial.println();
  Serial.println("PiHole 2:");
  Serial.println(getSummaryStats(pihole2));
  Serial.println();
  delay(1000); 
}

void initSerialCommunications()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}
