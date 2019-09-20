#include <ArduinoJson.h>
#include "PiHoleService.h"

const char _pihole1ServerAddress[] = "192.168.1.3";
const char _pihole2ServerAddress[] = "192.168.1.4";
const size_t _piHoleSummaryCapacity = 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(17) + 380;
const char _piHoleUrl[] = "/admin/api.php";

struct PiHoleSummary getPiHoleSummary()
{
  DynamicJsonDocument piHole1Data(_piHoleSummaryCapacity);
  DynamicJsonDocument piHole2Data(_piHoleSummaryCapacity);
  deserializeJson(piHole1Data, getHttpResponse(_pihole1ServerAddress, _piHoleUrl));
  deserializeJson(piHole2Data, getHttpResponse(_pihole2ServerAddress, _piHoleUrl));

  struct PiHoleSummary piHoleSummary;
  piHoleSummary.dnsQueriesToday = ((unsigned long)piHole1Data["dns_queries_today"]) + ((unsigned long)piHole2Data["dns_queries_today"]);
  piHoleSummary.adsBlockedToday = ((unsigned long)piHole1Data["ads_blocked_today"]) + ((unsigned long)piHole2Data["ads_blocked_today"]);
  piHoleSummary.dnsQueriesCachedToday = ((unsigned long)piHole1Data["queries_cached"]) + ((unsigned long)piHole2Data["queries_cached"]);
  piHoleSummary.piHole1Updated = ((JsonObject)piHole1Data["gravity_last_updated"])["absolute"];
  piHoleSummary.piHole2Updated = ((JsonObject)piHole2Data["gravity_last_updated"])["absolute"];
  
  return piHoleSummary;
}
