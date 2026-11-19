#include "global.h"
#include "overlay_74_wonder_cards_app_data.h"
#include "constants/heap.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sprite_resources_header.h"
#include "sys_task_api.h"

void sub_02014A38(BufferManager *bufferManager);
void sub_02014AA0(void);
void *sub_02014A4C(const BufferManager *bufferManager);
void sub_02014A8C(BufferManager *bufferManager);
void ov74_0222DEF0(WonderCardsAppData *appData);
BOOL ov74_0222DF2C(WonderCardsAppData *appData);
void ov74_0222DEF0(WonderCardsAppData *appData) {
    WonderCardFlipAnimManager *animMan = &appData->flipAnimMan;

    if (animMan->bufferManager) {
        sub_02014A38(animMan->bufferManager);
    }

    if (animMan->sysTask) {
        SysTask_Destroy(animMan->sysTask);
    }

    animMan->bufferManager = 0L;
    animMan->sysTask = 0L;

    sub_02014AA0();
}
BOOL ov74_0222DF2C(WonderCardsAppData *appData) {
    int scanline, offset;

    WonderCardFlipAnimManager *flipAnimMan = &appData->flipAnimMan;
    fx32 offsetFx32 = (168 / 2) * ((fx32)0x0000000000001000L);

    if (flipAnimMan->running == 0) {
        return 1;
    }

    if (flipAnimMan->animStage == WC_FLIP_STAGE_SHRINKING) {
        flipAnimMan->offsetStep += flipAnimMan->deltaOffsetStep;
        flipAnimMan->deltaOffsetStep += flipAnimMan->deltaOffsetStep;

        if (flipAnimMan->offsetStep / ((fx32)0x0000000000001000L) > 1000) {
            flipAnimMan->running = 0;
            offsetFx32 = 168;
            flipAnimMan->offsetStep = 0;
            flipAnimMan->deltaOffsetStep = 0;
        }
    } else {
        flipAnimMan->offsetStep -= flipAnimMan->deltaOffsetStep;
        flipAnimMan->deltaOffsetStep /= 2;

        if (flipAnimMan->deltaOffsetStep < ((fx32)0x0000000000001000L) / 4) {
            flipAnimMan->running = 0;
            flipAnimMan->offsetStep = 1 * ((fx32)0x0000000000001000L);
            flipAnimMan->deltaOffsetStep = 0;
        }
    }

    u16 *buffer = sub_02014A4C(flipAnimMan->bufferManager);

    for (scanline = 168 / 2; scanline < 168; scanline++) {
        offset = offsetFx32 / ((fx32)0x0000000000001000L);

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

    return 0;
}
