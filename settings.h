#ifndef SETTINGS_H
#define SETTINGS_H

#define NTP_SERVER "es.pool.ntp.org"
#define GMT_OFFSET_SEC 3600 * 2 // Mainland Spain uses UTC+2 during CEST and UTC+1 during CET

watchySettings settings{
    .cityID = "",
    .lat = "",
    .lon = "",
    .weatherAPIKey = "",
    .weatherURL = "",
    .weatherUnit = "metric",
    .weatherLang = "es",
    .weatherUpdateInterval = 30,
    .ntpServer = NTP_SERVER,
    .gmtOffset = GMT_OFFSET_SEC,
    .vibrateOClock = true,
};

#endif