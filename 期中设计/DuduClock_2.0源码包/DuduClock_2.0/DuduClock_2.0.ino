#include <OneButton.h>
#include "net.h"
#include "common.h"
#include "PreferencesUtil.h"
#include "tftUtil.h"
#include "task.h"

unsigned int prevDisplay = 0;
unsigned int preTimerDisplay = 0;
unsigned long startMillis = 0;
int synDataRestartTime = 60;
bool isCouting = false;
OneButton myButton(BUTTON, true);

void initDatas();
void click();
void doubleclick();
void longclick();

bool hasValidWeatherKey() {
  return String(KEY) != "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
}

bool hasPresetWiFiConfig() {
  return String(PRESET_WIFI_SSID).length() > 0 &&
         String(PRESET_WIFI_PASS).length() > 0 &&
         String(PRESET_CITY).length() > 0;
}

void setup() {
  Serial.begin(115200);
  tftInit();
  drawText("系统启动中...");

  getWiFiCity();
  if ((ssid.length() == 0 || pass.length() == 0 || city.length() == 0) && hasPresetWiFiConfig()) {
    setInfo4Test();
    getWiFiCity();
  }

  if (ssid.length() == 0 || pass.length() == 0 || city.length() == 0) {
    currentPage = SETTING;
    wifiConfigBySoftAP();
    return;
  }

  currentPage = WEATHER;
  if (!connectWiFi(30)) {
    currentPage = SETTING;
    wifiConfigBySoftAP();
    return;
  }

  if (!hasValidWeatherKey()) {
    Serial.println("和风天气 KEY 未配置，请先修改 common.h");
    draw2LineText("请先填写和风Key", "common.h");
    while (true) {
      delay(1000);
    }
  }

  if (location.equals("") || locationLat.equals("") || locationLon.equals("")) {
    getCityID();
  }

  initDatas();
  drawWeatherPage();
  startRunner();
  startTimerQueryWeather();

  myButton.attachClick(click);
  myButton.attachDoubleClick(doubleclick);
  myButton.attachLongPressStart(longclick);
  myButton.setPressMs(2000);
  myButton.setDebounceMs(10);
}

void loop() {
  myButton.tick();

  switch (currentPage) {
    case SETTING:
      doClient();
      break;
    case WEATHER:
      executeRunner();
      time_t now;
      time(&now);
      if (now != prevDisplay) {
        prevDisplay = now;
        drawDateWeek();
      }
      break;
    case AIR:
    case FUTUREWEATHER:
    case THEME:
    case RESET:
      executeRunner();
      break;
    case TIMER:
      if (isCouting && (millis() / 10) != preTimerDisplay) {
        preTimerDisplay = millis() / 10;
        drawNumsByCount(timerCount + millis() - startMillis);
      }
      break;
    default:
      break;
  }
}

void click() {
  if (currentPage == TIMER) {
    if (!isCouting) {
      startMillis = millis();
    } else {
      timerCount += millis() - startMillis;
      drawNumsByCount(timerCount);
    }
    isCouting = !isCouting;
  }
}

void doubleclick() {
  switch (currentPage) {
    case WEATHER:
      disableAnimScrollText();
      drawAirPage();
      currentPage = AIR;
      break;
    case AIR:
      drawFutureWeatherPage();
      currentPage = FUTUREWEATHER;
      break;
    case FUTUREWEATHER:
      drawThemePage();
      currentPage = THEME;
      break;
    case THEME:
      drawTimerPage();
      currentPage = TIMER;
      break;
    case TIMER:
      drawResetPage();
      currentPage = RESET;
      break;
    case RESET:
      drawWeatherPage();
      enableAnimScrollText();
      currentPage = WEATHER;
      break;
    default:
      break;
  }
}

void longclick() {
  if (currentPage == RESET) {
    Serial.println("恢复出厂设置");
    clearWiFiCity();
    restartSystem("已恢复出厂设置", false);
  } else if (currentPage == THEME) {
    Serial.println("更改主题");
    if (backColor == BACK_BLACK) {
      backColor = BACK_WHITE;
      backFillColor = 0xFFFF;
      penColor = 0x0000;
    } else {
      backColor = BACK_BLACK;
      backFillColor = 0x0000;
      penColor = 0xFFFF;
    }
    setBackColor(backColor);
    drawWeatherPage();
    enableAnimScrollText();
    currentPage = WEATHER;
  } else if (currentPage == TIMER) {
    Serial.println("计时器归零");
    timerCount = 0;
    isCouting = false;
    drawNumsByCount(timerCount);
  }
}

void initDatas() {
  startTimerShowTips();

  time_t start;
  time(&start);

  getNTPTime();
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    time_t end;
    time(&end);
    if ((end - start) > synDataRestartTime) {
      restartSystem("同步数据失败", true);
    }
    Serial.println("时钟对时失败...");
    getNTPTime();
  }

  getNowWeather();
  while (!queryNowWeatherSuccess) {
    time_t end;
    time(&end);
    if ((end - start) > synDataRestartTime) {
      restartSystem("同步数据失败", true);
    }
    getNowWeather();
  }

  // Air quality is optional at startup.
  getAir();

  getFutureWeather();
  while (!queryFutureWeatherSuccess) {
    time_t end;
    time(&end);
    if ((end - start) > synDataRestartTime) {
      restartSystem("同步数据失败", true);
    }
    getFutureWeather();
  }

  timerEnd(timerShowTips);
  isStartQuery = false;
}
