#include <ArduinoJson.h>
#include "PiHoleService.h"

const String _pihole1ServerAddress = "192.168.1.3";
const String _pihole2ServerAddress = "192.168.1.4";

const size_t _piHoleSummaryCapacity = 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(17) + 380;
const String _piHoleSummaryUrl = "/admin/api.php";
DynamicJsonDocument _piHole1Data(_piHoleSummaryCapacity);
DynamicJsonDocument _piHole2Data(_piHoleSummaryCapacity);

String _piHoleRecentBlockedUrl = "/admin/api.php?recentBlocked&auth=" + String(SECRET_API_TOKEN);
  
struct PiHoleSummary getPiHoleSummary()
{
  // Get Summary
  deserializeJson(_piHole1Data, getHttpResponse(_pihole1ServerAddress, _piHoleSummaryUrl));
  deserializeJson(_piHole2Data, getHttpResponse(_pihole2ServerAddress, _piHoleSummaryUrl));

  // Get Recently Blocked
  String piHole1RecentBlockedDomain = getHttpResponse(_pihole1ServerAddress, _piHoleRecentBlockedUrl);
  String piHole2RecentBlockedDomain = getHttpResponse(_pihole2ServerAddress, _piHoleRecentBlockedUrl);

  struct PiHoleSummary piHoleSummary;
  piHoleSummary.dnsQueriesToday = ((unsigned long)_piHole1Data["dns_queries_today"]) + ((unsigned long)_piHole2Data["dns_queries_today"]);
  piHoleSummary.adsBlockedToday = ((unsigned long)_piHole1Data["ads_blocked_today"]) + ((unsigned long)_piHole2Data["ads_blocked_today"]);
  piHoleSummary.dnsQueriesCachedToday = ((unsigned long)_piHole1Data["queries_cached"]) + ((unsigned long)_piHole2Data["queries_cached"]);
  piHoleSummary.piHole1Updated = _piHole1Data["gravity_last_updated"]["absolute"];
  piHoleSummary.piHole2Updated = _piHole2Data["gravity_last_updated"]["absolute"];
  piHoleSummary.piHole1RecentBlockedDomain = piHole1RecentBlockedDomain;
  piHoleSummary.piHole2RecentBlockedDomain = piHole2RecentBlockedDomain;
  
  return piHoleSummary;
}
