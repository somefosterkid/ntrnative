#include "global.h"

void ov08_0222458C(u16 *buttonData, u16 *screenData, u8 xOffset, u8 yOffset, u8 width, u8 height);

void ov08_0222458C(u16 *buttonData, u16 *screenData, u8 xOffset, u8 yOffset, u8 width, u8 height) {
    for (u16 i = 0; i < height; i++) {
        for (u16 l = 0; l < width; l++) {
            buttonData[i * width + l] = screenData[(yOffset + i) * (8 * (8 / 2)) + xOffset + l];
        }
    }
}
