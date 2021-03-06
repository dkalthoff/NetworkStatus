#include <string>
#include <TimeLib.h>
#include "arduino_secrets.h"
#include "DisplayService.h"
#include "NetworkService.h"
#include "PiHoleService.h"

void setup() 
{
  setupSerialCommunications();
  displayService.configureDisplay();
  displayService.sendToDisplay("Connecting to", "WiFi...");
  networkService.connect();
  piHoleService.init();
}

void loop() 
{
  displayPiHoleSummary();
}

void displayPiHoleSummary()
{
  displayService.sendToDisplay("Fetching Pi-hole", "Data...");
  struct PiHoleSummary piHoleSummary = piHoleService.getPiHoleSummary();
    
  // Blocked
  byte percentageBlocked = ((float)piHoleSummary.adsBlockedToday / piHoleSummary.dnsQueriesToday) * 100;
  String row1Message = "Blocked " + String(percentageBlocked) + "%";
  String row2Message = String(piHoleSummary.adsBlockedToday) + " of " + String(piHoleSummary.dnsQueriesToday);
  displayService.sendToDisplay(row1Message, row2Message);
  delay(5000);

  // Recently Blocked Domain
  if(piHoleSummary.piHole1RecentBlockedDomain == piHoleSummary.piHole2RecentBlockedDomain || piHoleSummary.piHole2RecentBlockedDomain == "")
  {
    displayService.sendToDisplay("Recent Block", piHoleSummary.piHole1RecentBlockedDomain);
  }
  else
  {
    displayService.sendToDisplay("Recent Block PH1", piHoleSummary.piHole1RecentBlockedDomain);
    delay(3000);

    displayService.sendToDisplay("Recent Block PH2", piHoleSummary.piHole2RecentBlockedDomain);
  }
  delay(5000);
  
  // Top Query
  displayService.sendToDisplay("Top Qry: " + String(piHoleSummary.piHole1TopQueryCount), piHoleSummary.piHole1TopQuery);
  delay(5000);

  // Top Blocked
  displayService.sendToDisplay("Top Block: " + String(piHoleSummary.piHole1TopBlockedCount), piHoleSummary.piHole1TopBlocked);
  delay(5000);

  // Cached
  byte percentageCached = ((float)piHoleSummary.dnsQueriesCachedToday / piHoleSummary.dnsQueriesToday) * 100;
  row1Message = "Cached " + String(percentageCached) + "%";
  row2Message = String(piHoleSummary.dnsQueriesCachedToday) + " of " + String(piHoleSummary.dnsQueriesToday);
  displayService.sendToDisplay(row1Message, row2Message);
  delay(5000);

  // Black List Update
  row1Message = "PiHole1 BlkList";
  row2Message = "Updated:" + String(month(piHoleSummary.piHole1Updated)) + "/" + String(day(piHoleSummary.piHole1Updated)) + "/" + String(year(piHoleSummary.piHole1Updated)).substring(2);
  displayService.sendToDisplay(row1Message, row2Message);
  delay(3000);
  row1Message = "PiHole2 BlkList";
  row2Message = "Updated:" + String(month(piHoleSummary.piHole2Updated)) + "/" + String(day(piHoleSummary.piHole2Updated)) + "/" + String(year(piHoleSummary.piHole2Updated)).substring(2);
  displayService.sendToDisplay(row1Message, row2Message);
}

void setupSerialCommunications()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }
}