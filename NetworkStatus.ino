#include <ArduinoJson.h>
#include <TimeLib.h>

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

  unsigned long dnsQueriesToday = ((unsigned long)piHole1Data["dns_queries_today"]) + ((unsigned long)piHole2Data["dns_queries_today"]);
  unsigned long adsBlockedToday = ((unsigned long)piHole1Data["ads_blocked_today"]) + ((unsigned long)piHole2Data["ads_blocked_today"]);
  unsigned long dnsQueriesCachedToday = ((unsigned long)piHole1Data["queries_cached"]) + ((unsigned long)piHole2Data["queries_cached"]);
  
  // Blocked
  Serial.println(adsBlockedToday/dnsQueriesToday);
  byte percentageBlocked = ((float)adsBlockedToday / dnsQueriesToday) * 100;
  String row1Message = "Blocked " + String(percentageBlocked) + "%";
  String row2Message = String(adsBlockedToday) + " of " + String(dnsQueriesToday);
  sendToDisplay(row1Message, row2Message);
  delay(5000);

  // Cached
  byte percentageCached = ((float)dnsQueriesCachedToday / dnsQueriesToday) * 100;
  row1Message = "Cached " + String(percentageCached) + "%";
  row2Message = String(dnsQueriesCachedToday) + " of " + String(dnsQueriesToday);
  sendToDisplay(row1Message, row2Message);
  delay(5000);

  // Black List Update
  time_t piHole1Updated = piHole1Data["absolute"];
  time_t piHole2Updated = piHole2Data["absolute"];
  row1Message = "PiHole1 BlkList";
  row2Message = "Updated:" + String(month(piHole1Updated)) + "/" + String(day(piHole1Updated)) + "/" + String(year(piHole1Updated));
  sendToDisplay(row1Message, row2Message);
  delay(3000);
  row1Message = "PiHole2 BlkList";
  row2Message = "Updated:" + String(month(piHole2Updated)) + "/" + String(day(piHole2Updated)) + "/" + String(year(piHole2Updated));
  sendToDisplay(row1Message, row2Message);
  delay(3000);
}

void setupSerialCommunications()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  //while (!Serial) {
  // ; // wait for serial port to connect. Needed for native USB port only
  //}
}
