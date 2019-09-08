#include <SPI.h>
#include <WiFiNINA.h>
#include "secrets.h"

char ssid[] = SECRET_SSID;        // your network SSID (name)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

WiFiClient httpClient;;

void connectToWiFi()
{
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to open SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  Serial.println();
  printCurrentNet();
  Serial.println();
  printWifiData();
  Serial.println();
}

String getHttpResponse(IPAddress server, String url)
{
  if (httpClient.connect(server, 80)) {
    Serial.print("connected to server ");
    Serial.println(server);
    // Make a HTTP request:
    httpClient.println("GET " + url + " HTTP/1.1");
    httpClient.println("Host: 192");
    httpClient.println("Connection: close");
    httpClient.println();
  }

  String response = "";
  bool waitingForResponse = true;
  while(waitingForResponse)
  {
    while (httpClient.available()) {
      response.concat(httpClient.readStringUntil('\r'));
    }
  
    // if the server's disconnected, stop the client:
    if (!httpClient.connected()) {
      Serial.println();
      Serial.println("disconnecting from server.");
      httpClient.stop();
      waitingForResponse = false;
    }
  }
  
  Serial.println(response);

  return response;
}

void printWifiData() {
  Serial.println("**************************");
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);

  // print your subnet mask:
  IPAddress subnet = WiFi.subnetMask();
  Serial.print("NetMask: ");
  Serial.println(subnet);

  // print your gateway address:
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("Gateway: ");
  Serial.println(gateway);
  Serial.println("**************************");
}

void printCurrentNet() {
  Serial.println("**************************");
  
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);

  Serial.println("**************************");
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
