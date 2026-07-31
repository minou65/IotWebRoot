#include <Arduino.h>
#include <WebServer.h>
#include <IotWebRoot.h>

WebServer server(80);
HtmlRootFormatProvider HTML;

// Beispiel-Daten
float temperature = 23.5;
int humidity = 65;
String deviceStatus = "Online";

void handleRoot() {
    String html = "";

    // HTML Head
    html += HTML.getHtmlHead("Simple Example");
    html += HTML.getHtmlHeadEnd();
    html += HTML.getBodyStart();

    // Hauptüberschrift
    html += "<h2>Sensor Dashboard</h2>";
    html += HTML.addNewLine();

    // Fieldset für Sensordaten
    html += HTML.getHtmlFieldset("Sensor Werte");
    html += HTML.getHtmlTable();
    html += HTML.getHtmlTableRowText("Temperatur:", String(temperature) + " °C");
    html += HTML.getHtmlTableRowText("Luftfeuchtigkeit:", String(humidity) + " %");
    html += HTML.getHtmlTableRowText("Status:", deviceStatus);
    html += HTML.getHtmlTableEnd();
    html += HTML.getHtmlFieldsetEnd();

    html += HTML.addNewLine(2);

    // Fieldset für Geräteinformationen
    html += HTML.getHtmlFieldset("Gerät Information");
    html += HTML.getHtmlTable();
    html += HTML.getHtmlTableRowText("Chip ID:", String(ESP.getChipId(), HEX));
    html += HTML.getHtmlTableRowText("Free Heap:", String(ESP.getFreeHeap()) + " Bytes");
    html += HTML.getHtmlTableRowText("Uptime:", String(millis() / 1000) + " Sekunden");
    html += HTML.getHtmlTableEnd();
    html += HTML.getHtmlFieldsetEnd();

    // Version
    html += HTML.addNewLine();
    html += HTML.getHtmlVersion("1.0.0");

    // HTML End
    html += HTML.getBodyEnd();
    html += HTML.getHtmlEnd();

    server.send(200, "text/html", html);
}

void setup() {
    Serial.begin(115200);

    // WiFi Setup (hier vereinfacht)
    // WiFi.begin("SSID", "PASSWORD");

    // Webserver Routen
    server.on("/", handleRoot);

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();

    // Simuliere Sensorwerte
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 5000) {
        temperature = 20.0 + random(0, 100) / 10.0;
        humidity = 50 + random(0, 40);
        lastUpdate = millis();
    }
}