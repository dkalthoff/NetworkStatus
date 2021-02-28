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
    String piHole1TopQuery;
    unsigned long piHole1TopQueryCount;
    String piHole1TopBlocked;
    unsigned long piHole1TopBlockedCount;
};

class PiHoleService
{
    private:
        const char* _pihole1ServerAddress = "192.168.1.3";
        const char* _pihole2ServerAddress = "192.168.1.4";
        const size_t _piHoleSummaryCapacity = 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(17) + 380;
        const char* _piHoleSummaryUrl = "/admin/api.php";
        String _piHoleRecentBlockedUrl = "/admin/api.php?recentBlocked&auth=";
        String _piHoleTopItemsUrl = "/admin/api.php?topItems=1&auth=";

    public:
        void init()
        {
            _piHoleRecentBlockedUrl.concat(arduinoSecrets.PI_HOLE_API_TOKEN);
            _piHoleTopItemsUrl.concat(arduinoSecrets.PI_HOLE_API_TOKEN);
        }

        struct PiHoleSummary getPiHoleSummary()
        {
            DynamicJsonDocument _piHole1Data(_piHoleSummaryCapacity);
            DynamicJsonDocument _piHole2Data(_piHoleSummaryCapacity);

            // Get Summary
            deserializeJson(_piHole1Data, networkService.getHttpResponse(_pihole1ServerAddress, _piHoleSummaryUrl));
            deserializeJson(_piHole2Data, networkService.getHttpResponse(_pihole2ServerAddress, _piHoleSummaryUrl));

            struct PiHoleSummary piHoleSummary;
            piHoleSummary.dnsQueriesToday = ((unsigned long)_piHole1Data["dns_queries_today"]) + ((unsigned long)_piHole2Data["dns_queries_today"]);
            piHoleSummary.adsBlockedToday = ((unsigned long)_piHole1Data["ads_blocked_today"]) + ((unsigned long)_piHole2Data["ads_blocked_today"]);
            piHoleSummary.dnsQueriesCachedToday = ((unsigned long)_piHole1Data["queries_cached"]) + ((unsigned long)_piHole2Data["queries_cached"]);
            piHoleSummary.piHole1Updated = _piHole1Data["gravity_last_updated"]["absolute"];
            piHoleSummary.piHole2Updated = _piHole2Data["gravity_last_updated"]["absolute"];

            // Recently Blocked
            piHoleSummary.piHole1RecentBlockedDomain = networkService.getHttpResponse(_pihole1ServerAddress, _piHoleRecentBlockedUrl);
            piHoleSummary.piHole2RecentBlockedDomain = networkService.getHttpResponse(_pihole2ServerAddress, _piHoleRecentBlockedUrl);
            
             // Top Query and Blocked Ads
            DynamicJsonDocument piHoleTopItemsData(1024);
            deserializeJson(piHoleTopItemsData, networkService.getHttpResponse(_pihole1ServerAddress, _piHoleTopItemsUrl));
            JsonObject root = piHoleTopItemsData.as<JsonObject>();

            const char* piHole1TopQuery;

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
            return piHoleSummary;
        }
};

PiHoleService piHoleService;

#endif