#include <ArduinoJson.h>

const char _pihole1ServerAddress[] = "192.168.1.3";
const char _pihole2ServerAddress[] = "192.168.1.4";
const size_t _piHoleSummaryCapacity = 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(17) + 380;
const char _piHoleUrl[] = "/admin/api.php";

void setup() 
{
  setupSerialCommunications();
  configureDisplay();
  connectToWiFi();
}

void loop() 
{
  displayPiHoleSummary();
}

void displayPiHoleSummary()
{
  DynamicJsonDocument piHole1Data(_piHoleSummaryCapacity);
  DynamicJsonDocument piHole2Data(_piHoleSummaryCapacity);
  deserializeJson(piHole1Data, getHttpResponse(_pihole1ServerAddress, _piHoleUrl));
  deserializeJson(piHole2Data, getHttpResponse(_pihole2ServerAddress, _piHoleUrl));

  unsigned long dnsQueriesToday = ((long)piHole1Data["dns_queries_today"]) + ((long)piHole2Data["dns_queries_today"]);
  unsigned long adsBlockedToday = ((long)piHole1Data["ads_blocked_today"]) + ((long)piHole2Data["ads_blocked_today"]);
  unsigned long dnsQueriesCachedToday = ((long)piHole1Data["queries_cached"]) + ((long)piHole2Data["queries_cached"]);
  byte percentageBlocked = (adsBlockedToday / dnsQueriesToday) * 100;
  byte percentageCached = (dnsQueriesToday / dnsQueriesCachedToday) * 100;

  sendToDisplay("Blocked " + String(percentageBlocked) + "%", 0);
  sendToDisplay(String(adsBlockedToday) + "/" + String(dnsQueriesToday), 1);

  delay(5000);

  sendToDisplay("Cached " + String(percentageCached) + "%", 0);
  sendToDisplay(String(dnsQueriesCachedToday) + "/" + String(dnsQueriesToday), 1);

  delay(5000);
}

void setupSerialCommunications()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}
