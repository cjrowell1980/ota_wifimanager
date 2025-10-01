// web_manager.c
// Simple web server for OTA WiFi Manager (ESP32)
#include "web_manager.h"
#include <WiFi.h>
#include <WebServer.h>

static WebServer server(80);


// Forward declarations for helpers
static String html_header(const char* title);
static String html_footer();

// Root page: navigation
static void handle_root() {
    String html = html_header("OTA WiFi Manager");
    html += "<h1>OTA WiFi Manager</h1>";
    html += "<ul>"
            "<li><a href='/status'>Status</a></li>"
            "<li><a href='/settings'>Settings</a></li>"
            "<li><a href='/wifi'>WiFi</a></li>"
            "<li><a href='/sd'>SD Card</a></li>"
            "<li><a href='/ota'>OTA Update</a></li>"
            "<li><a href='/menu'>Menu</a></li>"
            "</ul>";
    html += html_footer();
    server.send(200, "text/html", html);
}

// Status page
static void handle_status() {
    String html = html_header("Status");
    html += "<h2>Status</h2>";
    html += "<p>WiFi: " + WiFi.SSID() + " (" + WiFi.localIP().toString() + ")</p>";
    html += "<p>SD: " + String(SD.begin() ? "Ready" : "Not Ready") + "</p>";
    html += html_footer();
    server.send(200, "text/html", html);
}

#include "config.h"
#include "wifi_manager.h"

// Settings page (GET/POST)
static void handle_settings() {
    if (server.method() == HTTP_POST) {
        String ssid = server.arg("ssid");
        String password = server.arg("password");
        if (ssid.length() > 0 && password.length() > 0) {
            strncpy(wifi_ssid, ssid.c_str(), sizeof(wifi_ssid)-1);
            wifi_ssid[sizeof(wifi_ssid)-1] = '\0';
            strncpy(wifi_password, password.c_str(), sizeof(wifi_password)-1);
            wifi_password[sizeof(wifi_password)-1] = '\0';
            config_mark_changed();
            config_update();
            wifi_manager_disconnect();
            wifi_manager_init();
            server.send(200, "text/html", html_header("Settings") + "<p>Settings updated. SSID: " + ssid + "</p>" + html_footer());
        } else {
            server.send(400, "text/html", html_header("Settings") + "<p>SSID and password required.</p>" + html_footer());
        }
        return;
    }
    String html = html_header("Settings");
    html += "<h2>Settings</h2>";
    html += "<form method='POST' action='/settings'>";
    html += "WiFi SSID: <input name='ssid' value='" + String(wifi_ssid) + "'><br>";
    html += "WiFi Password: <input name='password' type='password' value='" + String(wifi_password) + "'><br>";
    html += "<input type='submit' value='Save'>";
    html += "</form>";
    html += html_footer();
    server.send(200, "text/html", html);
}

// WiFi page
static void handle_wifi() {
    String html = html_header("WiFi");
    html += "<h2>WiFi</h2>";
    html += "<p>SSID: " + WiFi.SSID() + "</p>";
    html += "<p>IP: " + WiFi.localIP().toString() + "</p>";
    html += html_footer();
    server.send(200, "text/html", html);
}

// SD Card page
static void handle_sd() {
    String html = html_header("SD Card");
    html += "<h2>SD Card Files</h2><ul>";
    File root = SD.open("/");
    if (root) {
        File file = root.openNextFile();
        while (file) {
            html += "<li>" + String(file.name()) + "</li>";
            file = root.openNextFile();
        }
    } else {
        html += "<li>SD not ready</li>";
    }
    html += "</ul>";
    html += html_footer();
    server.send(200, "text/html", html);
}

// OTA page
static void handle_ota() {
    String html = html_header("OTA Update");
    html += "<h2>OTA Update</h2><p>Use ArduinoOTA or PlatformIO for firmware updates.</p>";
    html += html_footer();
    server.send(200, "text/html", html);
}

// Menu navigation page
static void handle_menu() {
    String html = html_header("Menu");
    html += "<h2>Menu Navigation</h2>";
    html += "<ul>"
            "<li><a href='/status'>Status</a></li>"
            "<li><a href='/settings'>Settings</a></li>"
            "<li><a href='/wifi'>WiFi</a></li>"
            "<li><a href='/sd'>SD Card</a></li>"
            "<li><a href='/ota'>OTA Update</a></li>"
            "</ul>";
    html += html_footer();
    server.send(200, "text/html", html);
}

// HTML helpers
static String html_header(const char* title) {
    String html = "<!DOCTYPE html><html><head><meta charset='utf-8'><title>";
    html += title;
    html += "</title></head><body>";
    html += "<nav><a href='/'>Home</a></nav><hr>";
    return html;
}

static String html_footer() {
    return "<hr><footer><small>OTA WiFi Manager</small></footer></body></html>";
}


void web_manager_init(void) {
    server.on("/", handle_root);
    server.on("/status", handle_status);
    server.on("/settings", handle_settings);
    server.on("/wifi", handle_wifi);
    server.on("/sd", handle_sd);
    server.on("/ota", handle_ota);
    server.on("/menu", handle_menu);
    server.begin();
}

void web_manager_handle(void) {
    server.handleClient();
}
