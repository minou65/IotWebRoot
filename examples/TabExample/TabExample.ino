#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <IotWebRoot.h>
#include <IotWebRootTab.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WebServer server(80);
TabHtmlRootFormatProvider htmlFormat;

void handleRoot() {
    String html = "";
    
    // HTML Head
    html += htmlFormat.getHtmlHead("Tab Example");
    html += htmlFormat.getHtmlStyle();
    html += htmlFormat.getHtmlHeadEnd();
    
    // Create tabs
    htmlFormat.addTab("Info");
    htmlFormat.addTab("Data");
    htmlFormat.addTab("Settings");
    
    // Content for Tab "Info"
    htmlFormat.addString(htmlFormat.getHtmlFieldset("System Information"), "Info");
    htmlFormat.addString(htmlFormat.getHtmlTable(), "Info");
    htmlFormat.addString(htmlFormat.getHtmlTableRowText("Chip ID", String((uint32_t)ESP.getEfuseMac())), "Info");
    htmlFormat.addString(htmlFormat.getHtmlTableRowText("Flash Size", String(ESP.getFlashChipSize())), "Info");
    htmlFormat.addString(htmlFormat.getHtmlTableRowText("Free Heap", String(ESP.getFreeHeap())), "Info");
    htmlFormat.addString(htmlFormat.getHtmlTableEnd(), "Info");
    htmlFormat.addString(htmlFormat.getHtmlFieldsetEnd(), "Info");
    
    // Content for Tab "Data"
    htmlFormat.addString(htmlFormat.getHtmlFieldset("Sensor Data"), "Data");
    htmlFormat.addString(htmlFormat.getHtmlTable(), "Data");
    htmlFormat.addString(htmlFormat.getHtmlTableRowSpan("Temperature", "25.5 °C", "temp"), "Data");
    htmlFormat.addString(htmlFormat.getHtmlTableRowSpan("Humidity", "60 %", "humidity"), "Data");
    htmlFormat.addString(htmlFormat.getHtmlTableEnd(), "Data");
    htmlFormat.addString(htmlFormat.getHtmlFieldsetEnd(), "Data");
    
    // Content for Tab "Settings"
    htmlFormat.addString(htmlFormat.getHtmlFieldset("Configuration"), "Settings");
    htmlFormat.addString("<form method='POST' action='/save'>", "Settings");
    htmlFormat.addString("<label>Update Interval (ms):</label><br>", "Settings");
    htmlFormat.addString("<input type='number' name='interval' value='5000'><br>", "Settings");
    htmlFormat.addString(htmlFormat.addNewLine(), "Settings");
    htmlFormat.addString("<button type='submit'>Save</button>", "Settings");
    htmlFormat.addString("</form>", "Settings");
    htmlFormat.addString(htmlFormat.getHtmlFieldsetEnd(), "Settings");
    
    // Render tabs
    html += htmlFormat.getHtmlTabs();
    
    // Version and end (NO getHtmlScript() call anymore!)
    html += htmlFormat.getHtmlVersion("1.0.0");
    html += htmlFormat.getHtmlEnd();
    
    server.send(200, "text/html", html);
}

void handleData() {
    String json = "{";
    json += "\"temp\":\"" + String(random(20, 30)) + ".5 °C\",";
    json += "\"humidity\":\"" + String(random(40, 80)) + " %\"";
    json += "}";
    
    server.send(200, "application/json", json);
}

void handleSave() {
    if (server.hasArg("interval")) {
        String interval = server.arg("interval");
        Serial.println("New interval: " + interval);
    }
    server.sendHeader("Location", "/");
    server.send(303);
}

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());
    
    // Web server routes
    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.on("/save", HTTP_POST, handleSave);
    
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}