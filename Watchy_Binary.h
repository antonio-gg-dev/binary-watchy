#ifndef WATCHY_BINARY_H
#define WATCHY_BINARY_H

#include <Watchy.h>

class WatchyBinary : public Watchy {
public:
    using Watchy::Watchy;

    void drawWatchFace() override;
    void handleButtonPress() override;

private:
    static const uint16_t SMALL_BIT_SIZE = 16;
    static const uint16_t LARGE_BIT_SIZE = 28;
    static const uint16_t BIT_BORDER = 2;
    static const uint16_t BIT_SPACING = 4;

    void drawBit(int16_t x, int16_t y, uint16_t size, bool on);
    void drawBits(uint16_t value, uint8_t bitCount, uint16_t size,
                  int16_t x, int16_t y);
    void drawHelp();
    void drawCenteredText(const char *text, int16_t centerX, int16_t baseline);
    void drawTextAt(const char *text, int16_t x, int16_t baseline);
    void drawRightAlignedText(const char *text, int16_t rightX,
                              int16_t baseline);
    void drawTwoDigitValue(uint8_t value, int16_t x, int16_t baseline);
    void drawBatteryValue(int16_t rightX, int16_t baseline);
    uint8_t batteryLevel();
    uint8_t batteryPercentage();

    bool showHelp_ = false;
};

#endif
