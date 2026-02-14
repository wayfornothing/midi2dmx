#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

#include "config.h"

// ============ CONFIGURATION WIFI ============
void wifi_init() {
    Serial.println("Démarrage WiFi Access Point...");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);

    delay(100);

    IPAddress IP = WiFi.softAPIP();
    Serial.printf("AP IP: %s\n", IP.toString().c_str());
    Serial.printf("SSID: %s\n", AP_SSID);
    Serial.printf("Password: %s\n", AP_PASS);
}
