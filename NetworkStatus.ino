#include "arduino_secrets.h"
#include <TimeLib.h>
#include "PiHoleService.h"

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
  struct PiHoleSummary piHoleSummary = getPiHoleSummary();
    
  // Blocked
  Serial.println(piHoleSummary.adsBlockedToday/piHoleSummary.dnsQueriesToday);
  byte percentageBlocked = ((float)piHoleSummary.adsBlockedToday / piHoleSummary.dnsQueriesToday) * 100;
  String row1Message = "Blocked " + String(percentageBlocked) + "%";
  String row2Message = String(piHoleSummary.adsBlockedToday) + " of " + String(piHoleSummary.dnsQueriesToday);
  sendToDisplay(row1Message, row2Message);
  delay(5000);

  // Recently Blocked Domain
  if(piHoleSummary.piHole1RecentBlockedDomain == piHoleSummary.piHole2RecentBlockedDomain)
  {
    String row1Message = "Blocked";
    String row2Message = piHoleSummary.piHole1RecentBlockedDomain;
    sendToDisplay(row1Message, row2Message);
  }
  else
  {
    String row1Message = "Blocked PiHole1";
    String row2Message = piHoleSummary.piHole1RecentBlockedDomain;
    sendToDisplay(row1Message, row2Message);
    delay(5000);

    row1Message = "Blocked PiHole2";
    row2Message = piHoleSummary.piHole2RecentBlockedDomain;
    sendToDisplay(row1Message, row2Message);
  }
  delay(5000);
  
  // Cached
  byte percentageCached = ((float)piHoleSummary.dnsQueriesCachedToday / piHoleSummary.dnsQueriesToday) * 100;
  row1Message = "Cached " + String(percentageCached) + "%";
  row2Message = String(piHoleSummary.dnsQueriesCachedToday) + " of " + String(piHoleSummary.dnsQueriesToday);
  sendToDisplay(row1Message, row2Message);
  delay(5000);

  // Black List Update
  row1Message = "PiHole1 BlkList";
  row2Message = "Updated:" + String(month(piHoleSummary.piHole1Updated)) + "/" + String(day(piHoleSummary.piHole1Updated)) + "/" + String(year(piHoleSummary.piHole1Updated)).substring(2);
  sendToDisplay(row1Message, row2Message);
  delay(3000);
  row1Message = "PiHole2 BlkList";
  row2Message = "Updated:" + String(month(piHoleSummary.piHole2Updated)) + "/" + String(day(piHoleSummary.piHole2Updated)) + "/" + String(year(piHoleSummary.piHole2Updated)).substring(2);
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
