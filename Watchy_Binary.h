#ifndef WATCHY_BINARY_H
#define WATCHY_BINARY_H

#include <Watchy.h>

class WatchyBinary : public Watchy {
public:
    using Watchy::Watchy;

    void drawWatchFace() override;

private:
    static const uint16_t SMALL_BIT_SIZE = 16;
    static const uint16_t LARGE_BIT_SIZE = 28;
    static const uint16_t BIT_BORDER = 2;
    static const uint16_t BIT_SPACING = 4;

    void drawBit(int16_t x, int16_t y, uint16_t size, bool on);
    void drawBits(uint16_t value, uint8_t bitCount, uint16_t size,
                  int16_t x, int16_t y);
    uint8_t batteryLevel();
};

#endif
