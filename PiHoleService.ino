#include <SPI.h>
#include <WiFiNINA.h>

String getSummaryStats(IPAddress server)
{
  return getHttpResponse(server, "/admin/api.php");
}
