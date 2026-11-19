#include "global.h"

#include "overlay_74_wonder_cards_app2_data.h"

void *sub_02014A4C(const BufferManager *bufferManager);
void sub_02014A8C(BufferManager *bufferManager);

BOOL ov74_0222F6C4(WonderCardsApp2Data *appData);

BOOL ov74_0222F6C4(WonderCardsApp2Data *appData) {
    int scanline, offset;
    WonderCardFlipAnimManager *flipAnimMan = &appData->flipAnimMan;
    fx32 offsetFx32 = (168 / 2) * FX32_ONE;
    u16 *buffer;

    if (flipAnimMan->running == FALSE) {
        return TRUE;
    }

    if (flipAnimMan->animStage == 1) {
        flipAnimMan->offsetStep += flipAnimMan->deltaOffsetStep;
        flipAnimMan->deltaOffsetStep += flipAnimMan->deltaOffsetStep;
        if (flipAnimMan->offsetStep / FX32_ONE > 1000) {
            flipAnimMan->running = FALSE;
            offsetFx32 = 168;
            flipAnimMan->offsetStep = 0;
            flipAnimMan->deltaOffsetStep = 0;
        }
    } else {
        flipAnimMan->offsetStep -= flipAnimMan->deltaOffsetStep;
        flipAnimMan->deltaOffsetStep /= 2;
        if (flipAnimMan->deltaOffsetStep < FX32_ONE / 4) {
            flipAnimMan->running = FALSE;
            flipAnimMan->offsetStep = 1 * FX32_ONE;
            flipAnimMan->deltaOffsetStep = 0;
        }
    }

    buffer = sub_02014A4C(flipAnimMan->bufferManager);
    for (scanline = 168 / 2; scanline < 168; scanline++) {
        offset = offsetFx32 / FX32_ONE;
        if (offset < 0) {
            offset = 0;
        }
        if (offset > 168) {
            offset = 168;
        }
        buffer[scanline * 4 + 1] = buffer[scanline * 4 + 3] = (0 - scanline + offset);
        buffer[(168 - scanline) * 4 + 1] = buffer[(168 - scanline) * 4 + 3] = (0 - (168 - scanline) + (168 - offset));
        offsetFx32 += flipAnimMan->offsetStep;
    }

    DC_FlushRange(buffer, sizeof(flipAnimMan->scrollsBuffers[0]));
    sub_02014A8C(flipAnimMan->bufferManager);
    return FALSE;
}
