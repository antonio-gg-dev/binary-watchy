#include "Watchy_Binary.h"

namespace {
const int16_t TOP_DAY_X = 0;
const int16_t TOP_BATTERY_X = 164;
const int16_t BOTTOM_WEEKDAY_X = 0;
const int16_t BOTTOM_MONTH_X = 124;
const int16_t TOP_Y = 0;
const int16_t BOTTOM_Y = 184;
const int16_t HOUR_X = 22;
const int16_t MINUTE_X = 6;
const int16_t HOUR_Y = 70;
const int16_t MINUTE_Y = 102;

const int16_t SMALL_BIT_STRIDE = 16 + 4;
const int16_t DAY_VALUE_X = TOP_DAY_X + 5 * SMALL_BIT_STRIDE;
const int16_t BATTERY_VALUE_RIGHT_X = TOP_BATTERY_X - 4;
const int16_t WEEKDAY_VALUE_X = BOTTOM_WEEKDAY_X + 3 * SMALL_BIT_STRIDE;
const int16_t MONTH_VALUE_RIGHT_X = BOTTOM_MONTH_X - 4;
const int16_t HOUR_LABEL_Y = 66;
const int16_t MINUTE_LABEL_Y = 145;
const int16_t TOP_VALUE_Y = 13;
const int16_t BOTTOM_VALUE_Y = 197;

const char *const WEEKDAY_LABELS[] = {"DO", "LU", "MA", "MI",
                                      "JU", "VI", "SA"};
const char *const MONTH_LABELS[] = {"EN", "FE", "MR", "AB", "MY", "JN",
                                    "JL", "AG", "SE", "OC", "NO", "DI"};
}

void WatchyBinary::drawWatchFace() {
    // Desactiva el acelerómetro BMA423 porque esta esfera no utiliza pasos, movimiento ni inclinación.
    // see: https://github.com/sqfmi/Watchy/blob/master/src/bma.cpp#L164-L172
    sensor.disableAccel();

    // Desactiva Wi-Fi durante el funcionamiento normal de la esfera.
    // see: https://github.com/sqfmi/Watchy/blob/master/src/Watchy.cpp#L736-L739
    WiFi.mode(WIFI_OFF);

    // Desactiva Bluetooth durante el funcionamiento normal de la esfera.
    // see: https://github.com/sqfmi/Watchy/blob/master/src/Watchy.cpp#L736-L739
    btStop();

    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);

    drawBits(currentTime.Day, 5, SMALL_BIT_SIZE, TOP_DAY_X, TOP_Y);
    drawBits(batteryLevel(), 2, SMALL_BIT_SIZE, TOP_BATTERY_X, TOP_Y);
    
    drawBits(currentTime.Wday == 1 ? 7 : currentTime.Wday - 1, 3, SMALL_BIT_SIZE, BOTTOM_WEEKDAY_X, BOTTOM_Y);
    drawBits(currentTime.Month, 4, SMALL_BIT_SIZE, BOTTOM_MONTH_X, BOTTOM_Y);

    drawBits(currentTime.Hour, 5, LARGE_BIT_SIZE, HOUR_X, HOUR_Y);
    drawBits(currentTime.Minute, 6, LARGE_BIT_SIZE, MINUTE_X, MINUTE_Y);

    if (showHelp_) {
        drawHelp();
        showHelp_ = false;
    }
}

void WatchyBinary::handleButtonPress() {
    const uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();

    if ((wakeupBit & BACK_BTN_MASK) && guiState == WATCHFACE_STATE) {
        RTC.read(currentTime);
        showHelp_ = true;
        showWatchFace(false);
        return;
    }

    Watchy::handleButtonPress();
}

void WatchyBinary::drawBit(int16_t x, int16_t y, uint16_t size, bool on) {
    const int16_t outerRadius2 = static_cast<int16_t>(size);
    const int16_t innerRadius2 = static_cast<int16_t>(size - 2 * BIT_BORDER);

    for (int16_t py = 0; py < static_cast<int16_t>(size); ++py) {
        for (int16_t px = 0; px < static_cast<int16_t>(size); ++px) {
            const int16_t dx = 2 * px + 1 - static_cast<int16_t>(size);
            const int16_t dy = 2 * py + 1 - static_cast<int16_t>(size);
            const int32_t distance2 = dx * dx + dy * dy;

            if (distance2 <= outerRadius2 * outerRadius2 &&
                (on || distance2 > innerRadius2 * innerRadius2)) {
                display.drawPixel(x + px, y + py, GxEPD_BLACK);
            } else if (!on && distance2 <= innerRadius2 * innerRadius2) {
                display.drawPixel(x + px, y + py, GxEPD_WHITE);
            }
        }
    }
}

void WatchyBinary::drawBits(uint16_t value, uint8_t bitCount,
                            uint16_t size, int16_t x, int16_t y) {
    const int16_t stride = static_cast<int16_t>(size + BIT_SPACING);

    for (uint8_t index = 0; index < bitCount; ++index) {
        const uint8_t shift = bitCount - 1 - index;
        drawBit(x + index * stride, y, size, (value & (1u << shift)) != 0);
    }
}

void WatchyBinary::drawHelp() {
    display.setFont(&FreeMonoBold9pt7b);

    const uint8_t hourWeights[] = {16, 8, 4, 2, 1};
    const uint8_t minuteWeights[] = {32, 16, 8, 4, 2, 1};

    for (uint8_t index = 0; index < 5; ++index) {
        char label[3] = {static_cast<char>('0' + hourWeights[index] / 10),
                         static_cast<char>('0' + hourWeights[index] % 10), '\0'};
        if (hourWeights[index] < 10) {
            label[0] = label[1];
            label[1] = '\0';
        }
        drawCenteredText(label,
                         HOUR_X + index * (LARGE_BIT_SIZE + BIT_SPACING) +
                             LARGE_BIT_SIZE / 2,
                         HOUR_LABEL_Y);
    }

    for (uint8_t index = 0; index < 6; ++index) {
        char label[3] = {static_cast<char>('0' + minuteWeights[index] / 10),
                         static_cast<char>('0' + minuteWeights[index] % 10), '\0'};
        if (minuteWeights[index] < 10) {
            label[0] = label[1];
            label[1] = '\0';
        }
        drawCenteredText(label,
                         MINUTE_X + index * (LARGE_BIT_SIZE + BIT_SPACING) +
                             LARGE_BIT_SIZE / 2,
                         MINUTE_LABEL_Y);
    }

    drawTwoDigitValue(currentTime.Day, DAY_VALUE_X, TOP_VALUE_Y);
    drawTextAt(currentTime.Wday >= 1 && currentTime.Wday <= 7
                   ? WEEKDAY_LABELS[currentTime.Wday - 1]
                   : "??",
               WEEKDAY_VALUE_X, BOTTOM_VALUE_Y);
    drawRightAlignedText(currentTime.Month >= 1 && currentTime.Month <= 12
                             ? MONTH_LABELS[currentTime.Month - 1]
                             : "??",
                         MONTH_VALUE_RIGHT_X, BOTTOM_VALUE_Y);
    drawBatteryValue(BATTERY_VALUE_RIGHT_X, TOP_VALUE_Y);
}

void WatchyBinary::drawCenteredText(const char *text, int16_t centerX,
                                    int16_t baseline) {
    int16_t x1;
    int16_t y1;
    uint16_t width;
    uint16_t height;
    display.getTextBounds(text, 0, baseline, &x1, &y1, &width, &height);
    display.setCursor(centerX - (x1 + static_cast<int16_t>(width) / 2),
                      baseline);
    display.print(text);
}

void WatchyBinary::drawRightAlignedText(const char *text, int16_t rightX,
                                        int16_t baseline) {
    int16_t x1;
    int16_t y1;
    uint16_t width;
    uint16_t height;
    display.getTextBounds(text, 0, baseline, &x1, &y1, &width, &height);
    display.setCursor(rightX - (x1 + static_cast<int16_t>(width)), baseline);
    display.print(text);
}

void WatchyBinary::drawTextAt(const char *text, int16_t x, int16_t baseline) {
    display.setCursor(x, baseline);
    display.print(text);
}

void WatchyBinary::drawTwoDigitValue(uint8_t value, int16_t x,
                                     int16_t baseline) {
    char text[3] = {static_cast<char>('0' + (value / 10) % 10),
                    static_cast<char>('0' + value % 10), '\0'};
    drawTextAt(text, x, baseline);
}

void WatchyBinary::drawBatteryValue(int16_t rightX, int16_t baseline) {
    const uint8_t measuredPercentage = batteryPercentage();
    const uint8_t percentage = measuredPercentage > 99 ? 99 : measuredPercentage;
    char text[3] = {static_cast<char>('0' + percentage / 10),
                    static_cast<char>('0' + percentage % 10), '\0'};
    drawRightAlignedText(text, rightX, baseline);
}

uint8_t WatchyBinary::batteryLevel() {
    const float voltage = getBatteryVoltage();

    if (voltage >= 4.0f) {
        return 3;
    }
    if (voltage >= 3.6f) {
        return 2;
    }
    if (voltage >= 3.2f) {
        return 1;
    }
    return 0;
}

uint8_t WatchyBinary::batteryPercentage() {
    const float voltage = getBatteryVoltage();

    // Approximation for a LiPo discharge curve; the voltage is not an exact
    // state-of-charge measurement because it varies with load and temperature.
    static const float voltages[] = {3.30f, 3.50f, 3.60f, 3.70f, 3.80f,
                                     3.90f, 4.00f, 4.10f, 4.20f};
    static const uint8_t percentages[] = {0, 5, 10, 20, 40,
                                          60, 80, 90, 100};

    if (voltage <= voltages[0]) {
        return percentages[0];
    }
    if (voltage >= voltages[8]) {
        return percentages[8];
    }

    for (uint8_t index = 1; index < 9; ++index) {
        if (voltage <= voltages[index]) {
            const float range = voltages[index] - voltages[index - 1];
            const float position = (voltage - voltages[index - 1]) / range;
            return static_cast<uint8_t>(
                percentages[index - 1] +
                position * (percentages[index] - percentages[index - 1]));
        }
    }

    return 100;
}
