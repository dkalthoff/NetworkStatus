#include <SPI.h>
#include <ArduinoJson.h>

const size_t capacity = 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(17) + 380;
DynamicJsonDocument doc(capacity);

String getSummaryStats(char serverAddress[])
{
  deserializeJson(doc, getHttpResponse(serverAddress, "/admin/api.php"));

  int dns_queries_today = doc["dns_queries_today"]; // 2161
  int ads_blocked_today = doc["ads_blocked_today"]; // 10

  Serial.print("DNS Queries Today: ");
  Serial.println(dns_queries_today);
  Serial.print("Blocked: ");
  Serial.println(ads_blocked_today);
}
