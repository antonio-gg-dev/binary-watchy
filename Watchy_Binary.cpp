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
}

void WatchyBinary::drawWatchFace() {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);

    drawBits(currentTime.Day, 5, SMALL_BIT_SIZE, TOP_DAY_X, TOP_Y);
    drawBits(batteryLevel(), 2, SMALL_BIT_SIZE, TOP_BATTERY_X, TOP_Y);
    drawBits(currentTime.Wday == 1 ? 7 : currentTime.Wday - 1, 3,
             SMALL_BIT_SIZE, BOTTOM_WEEKDAY_X, BOTTOM_Y);
    drawBits(currentTime.Month, 4, SMALL_BIT_SIZE, BOTTOM_MONTH_X, BOTTOM_Y);

    drawBits(currentTime.Hour, 5, LARGE_BIT_SIZE, HOUR_X, HOUR_Y);
    drawBits(currentTime.Minute, 6, LARGE_BIT_SIZE, MINUTE_X, MINUTE_Y);
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
