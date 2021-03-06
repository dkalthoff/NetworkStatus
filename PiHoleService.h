#ifndef PiHoleService_h
#define PiHoleService_h

#include <ArduinoJson.h>
#include <string>
#include "arduino_secrets.h"
#include "NetworkService.h"

struct PiHoleSummary
{
    unsigned long dnsQueriesToday;
    unsigned long adsBlockedToday;
    unsigned long dnsQueriesCachedToday;
    unsigned long piHole1Updated;
    unsigned long piHole2Updated;
    String piHole1RecentBlockedDomain;
    String piHole2RecentBlockedDomain;
    const char* piHole1TopQuery;
    unsigned long piHole1TopQueryCount;
    const char* piHole1TopBlocked;
    unsigned long piHole1TopBlockedCount;
};

class PiHoleService
{
    private:
        const char* _pihole1ServerAddress = "192.168.1.3";
        const char* _pihole2ServerAddress = "192.168.1.4";
        const char* _piHoleSummaryUrl = "/admin/api.php";
        String _piHoleRecentBlockedUrl = "/admin/api.php?recentBlocked&auth=";
        String _piHoleTopItemsUrl = "/admin/api.php?topItems=1&auth=";
        StaticJsonDocument<768> piHole1SummaryData;
        StaticJsonDocument<768> piHole2SummaryData;
        StaticJsonDocument<192> piHoleTopItemsData;

    public:
        void init()
        {
            _piHoleRecentBlockedUrl.concat(arduinoSecrets.PI_HOLE_API_TOKEN);
            _piHoleTopItemsUrl.concat(arduinoSecrets.PI_HOLE_API_TOKEN);
        }

        struct PiHoleSummary getPiHoleSummary()
        {
            Serial.println("--- Start fetching Pi-hole data ---");
            
            // Get Summary
            deserializeJson(piHole1SummaryData, networkService.getHttpResponse(_pihole1ServerAddress, _piHoleSummaryUrl));
            deserializeJson(piHole2SummaryData, networkService.getHttpResponse(_pihole2ServerAddress, _piHoleSummaryUrl));

            struct PiHoleSummary piHoleSummary;
            piHoleSummary.dnsQueriesToday = ((unsigned long)piHole1SummaryData["dns_queries_today"]) + ((unsigned long)piHole2SummaryData["dns_queries_today"]);
            piHoleSummary.adsBlockedToday = ((unsigned long)piHole1SummaryData["ads_blocked_today"]) + ((unsigned long)piHole2SummaryData["ads_blocked_today"]);
            piHoleSummary.dnsQueriesCachedToday = ((unsigned long)piHole1SummaryData["queries_cached"]) + ((unsigned long)piHole2SummaryData["queries_cached"]);
            piHoleSummary.piHole1Updated = piHole1SummaryData["gravity_last_updated"]["absolute"];
            piHoleSummary.piHole2Updated = piHole2SummaryData["gravity_last_updated"]["absolute"];
            Serial.println("Summary Data Complete");

            // Recent Block
            piHoleSummary.piHole1RecentBlockedDomain = networkService.getHttpResponse(_pihole1ServerAddress, _piHoleRecentBlockedUrl);
            piHoleSummary.piHole2RecentBlockedDomain = networkService.getHttpResponse(_pihole2ServerAddress, _piHoleRecentBlockedUrl);
            Serial.println("Recent Block Data Complete");

             // Top Query and Blocked Ad
            deserializeJson(piHoleTopItemsData, networkService.getHttpResponse(_pihole1ServerAddress, _piHoleTopItemsUrl));

            JsonObject root = piHoleTopItemsData.as<JsonObject>();

            for (JsonPair kv : root["top_queries"].as<JsonObject>()) 
            {
                piHoleSummary.piHole1TopQuery = kv.key().c_str();
                piHoleSummary.piHole1TopQueryCount = kv.value().as<u_long>();
            }

            for (JsonPair kv : root["top_ads"].as<JsonObject>()) 
            {
                piHoleSummary.piHole1TopBlocked = kv.key().c_str();
                piHoleSummary.piHole1TopBlockedCount = kv.value().as<u_long>();
            }
            Serial.println("Top Query and Blocked Ad Complete");

            Serial.println("--- Finished fetching Pi-hole data ---");

            return piHoleSummary;
        }
};

PiHoleService piHoleService;

#endif