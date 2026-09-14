#ifndef SETTINGS_H
#define SETTINGS_H

#define NTP_SERVER "es.pool.ntp.org"
#define GMT_OFFSET_SEC 3600 * 2 // Mainland Spain uses UTC+2 during CEST and UTC+1 during CET

watchySettings settings{
    // No solicita datos meteorológicos: esta esfera no invoca getWeatherData().
    // see: https://github.com/sqfmi/Watchy/blob/master/src/Watchy.cpp#L657-L721
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
    // Desactiva la vibración automática al inicio de cada hora.
    // see: https://github.com/sqfmi/Watchy/blob/master/src/Watchy.cpp#L43-L53
    .vibrateOClock = false,
};

#endif
