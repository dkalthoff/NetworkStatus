#include <SPI.h>
#include <WiFiNINA.h>

char pihole1ServerAddress[] = "192.168.1.3";
char pihole2ServerAddress[] = "192.168.1.4";

void setup() 
{
  initSerialCommunications();
  initDisplay();
  connectToWiFi();
  sendToDisplay("Connected to WiFi", 0);
  sendToDisplay("test", 1);
}

void loop() {
  Serial.println("PiHole 1:");
  getSummaryStats(pihole1ServerAddress);
  
  Serial.println("PiHole 2:");
  getSummaryStats(pihole2ServerAddress);

  delay(60000); 
}

void initSerialCommunications()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}
