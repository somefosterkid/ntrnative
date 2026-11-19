#include "global.h"

#include "overlay_88_wifi_plaza_news.h"

void ov88_02258F74(WiFiPlazaNewsCountdown *countdown, WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics);

s32 ov88_02258F88();

void ov88_02258F60(WiFiPlazaNewsCountdown *countdown, WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics);

void ov88_02258F60(WiFiPlazaNewsCountdown *countdown, WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics) {
    if (countdown->waitingForTimeSync == 1) {
        ov88_02258F74(countdown, digits, graphics);
        return;
    }
    ov88_02258F88();
}
