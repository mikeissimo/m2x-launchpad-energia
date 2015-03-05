#include <aJSON.h>
#include "SPI.h"
#include "WiFi.h"

#include "M2XStreamClient.h"

char ssid[] = "<ssid>"; //  your network SSID (name)
char pass[] = "<WPA password>";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

char deviceId[] = "<device id>"; // Device you want to read
char m2xKey[] = "<M2X access key>"; // Your M2X access key

WiFiClient client;
M2XStreamClient m2xClient(&client, m2xKey);

void setup() {
  Serial.begin(115200);

  // Setup pins of CC3000 BoosterPack
  WiFi.setCSpin(18);  // 18: P2_2 @ F5529, PE_0 @ LM4F/TM4C
  WiFi.setENpin(2);   //  2: P6_5 @ F5529, PB_5 @ LM4F/TM4C
  WiFi.setIRQpin(19); // 19: P2_0 @ F5529, PB_2 @ LM4F/TM4C

  delay(10);
  // Connect to an AP with WPA/WPA2 security
  Serial.println("Connecting to WiFi....");
  WiFi.begin(ssid, pass); // Use this if your wifi network requires a password
  // WiFi.begin(ssid);    // Use this if your wifi network is unprotected.

  Serial.println("Connect success!");
  Serial.println("Waiting for DHCP address");
  // Wait for DHCP address
  delay(5000);
  // Print WiFi status
  printWifiStatus();
}

void loop() {
  aJsonObject *object = NULL;
  int response = m2xClient.readLocation(deviceId, &object);
  Serial.print("M2x client response code: ");
  Serial.println(response);

  if (response == -1) while(1) ;

  if (object) {
    aJsonObject *waypoints = aJson.getObjectItem(object, "waypoints");
    for (unsigned char i = 0; i < aJson.getArraySize(waypoints); i++) {
      aJsonObject *item = aJson.getArrayItem(waypoints, i);
      aJsonObject *name = aJson.getObjectItem(item, "name");
      aJsonObject *timestamp = aJson.getObjectItem(item, "timestamp");
      aJsonObject *lat = aJson.getObjectItem(item, "latitude");
      aJsonObject *lon = aJson.getObjectItem(item, "longitude");
      aJsonObject *elevation = aJson.getObjectItem(item, "elevation");

      Serial.print("Found a location, index:");
      Serial.println(i);
      Serial.print("Name: ");
      Serial.println(name->valuestring);
      Serial.print("Latitude: ");
      Serial.println(lat->valuestring);
      Serial.print("Longitude: ");
      Serial.println(lon->valuestring);
      Serial.print("Elevation: ");
      Serial.println(elevation->valuestring);
      Serial.print("Timestamp: ");
      Serial.println(timestamp->valuestring);
    }
    aJson.deleteItem(object);
  }

  delay(5000);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
