#ifndef NetworkService_h
#define NetworkService_h

#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <string>
#include "arduino_secrets.h"

class NetworkService
{
    private:
        int status = WL_IDLE_STATUS;     // the Wifi radio's status
        WiFiClient wifiClient;
        
        void printWifiData() 
        {
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

        void printCurrentNet()
        {
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

        void printMacAddress(byte mac[])
        {
            for (int i = 5; i >= 0; i--) 
            {
                if (mac[i] < 16) 
                {
                    Serial.print("0");
                }
                Serial.print(mac[i], HEX);
                
                if (i > 0) 
                {
                    Serial.print(":");
                }
            }
            Serial.println();
        }

    public:
        void connect()
        {
            // check for the WiFi module:
            if (WiFi.status() == WL_NO_MODULE) 
            {
                Serial.println("No WiFi module found. Unable to connect.");
                // don't continue
                while (true);
            }

            Serial.print("Current WiFi Firmware Version is ");
            Serial.println(WiFi.firmwareVersion());

            // attempt to connect to Wifi network:
            while (status != WL_CONNECTED) 
            {
                Serial.print("Attempting to connect to open SSID: ");
                Serial.println(arduinoSecrets.SHED_SSID);
                status = WiFi.begin(arduinoSecrets.SHED_SSID, arduinoSecrets.SHED_PASSWORD);

                if(status != WL_CONNECTED)
                {
                    Serial.print("Attempting to connect to open SSID: ");
                    Serial.println(arduinoSecrets.HOUSE_SSID);
                    status = WiFi.begin(arduinoSecrets.HOUSE_SSID, arduinoSecrets.HOUSE_PASSWORD); 
                }

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

        String getHttpResponse(const char* serverAddress, String url)
        {
            HttpClient httpClient = HttpClient(wifiClient, serverAddress, 80);
            httpClient.get(url);

            int statusCode = httpClient.responseStatusCode();
            String response = httpClient.responseBody();
            
            Serial.print("Response from ");
            Serial.print(serverAddress);
            Serial.print(" StatusCode: ");
            Serial.println(statusCode);
            Serial.println(response);

            if(!wifiClient.connected())
            {
                wifiClient.stop();
            }

            return response;
        }
};

NetworkService networkService;

#endif