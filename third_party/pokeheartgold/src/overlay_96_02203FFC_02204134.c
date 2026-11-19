#include "global.h"

void ov96_02204134(float x, float y, int *out3, int *out4);

void ov96_02204134(float x, float y, int *out3, int *out4) {
    float a;
    float b;

    a = 4.0 / (1.0 + (y - 57.0) * (y - 57.0)) + (11.0 + y * (y * y) / 7600.0 - (24.0 - y) * ((24.0 - y) * (24.0 - y)) / 4400.0 - 1.0);
    a *= 8.0;
    *out4 = a;
    b = 10.0 + (8.0 / 23.0) * x - (20.0 / 31.0) * (a / 8.0 - 10.0) * (64.0 - x) / 64.0;
    b *= 8.0;
    *out3 = b;
}
