#include <Preferences.h>
#include "net.h"
#include "tftUtil.h"

Preferences prefs;

void getWiFiCity() {
  prefs.begin("clock");
  ssid = prefs.getString("ssid", "");
  pass = prefs.getString("pass", "");
  city = prefs.getString("city", "");
  adm = prefs.getString("adm", "");
  location = prefs.getString("location", "");
  locationLat = prefs.getString("locationLat", "");
  locationLon = prefs.getString("locationLon", "");
  prefs.end();
}

void setWiFiCity() {
  prefs.begin("clock");
  prefs.putString("ssid", ssid);
  prefs.putString("pass", pass);
  prefs.putString("city", city);
  prefs.putString("adm", adm);
  prefs.putString("location", location);
  prefs.putString("locationLat", locationLat);
  prefs.putString("locationLon", locationLon);
  prefs.end();
}

void clearWiFiCity() {
  prefs.begin("clock");
  prefs.remove("ssid");
  prefs.remove("pass");
  prefs.remove("city");
  prefs.remove("adm");
  prefs.remove("location");
  prefs.remove("locationLat");
  prefs.remove("locationLon");
  prefs.remove("backColor");
  prefs.end();
}

void getBackColor() {
  prefs.begin("clock");
  backColor = prefs.getInt("backColor", BACK_BLACK);
  prefs.end();
}

void setBackColor(int backColor) {
  prefs.begin("clock");
  prefs.putInt("backColor", backColor);
  prefs.end();
}

void setInfo4Test() {
  prefs.begin("clock");
  prefs.putString("ssid", PRESET_WIFI_SSID);
  prefs.putString("pass", PRESET_WIFI_PASS);
  prefs.putString("city", PRESET_CITY);
  prefs.putString("adm", PRESET_ADM);
  prefs.putString("location", "");
  prefs.putString("locationLat", "");
  prefs.putString("locationLon", "");
  prefs.end();
}
