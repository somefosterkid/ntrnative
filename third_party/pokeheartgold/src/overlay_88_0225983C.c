#include "global.h"
#include "palette.h"
#include "unk_02005D10.h"
#include "overlay_88_wifi_plaza_news.h"

void ov88_0225983C(WiFiPlazaNewsFlash *newsFlash, u16 plttIndex, u32 seIndex);

void ov88_0225985C(WiFiPlazaNewsFlash *newsFlash);
void ov88_02259878(WiFiPlazaNewsFlash *newsFlash);
void ov88_02259884(WiFiPlazaNewsFlash *newsFlash);

void ov88_0225983C(WiFiPlazaNewsFlash *newsFlash, u16 plttIndex, u32 seIndex) {
    memset(newsFlash, 0, sizeof(WiFiPlazaNewsFlash));
    newsFlash->paletteIndex = plttIndex;
    newsFlash->soundEffect = seIndex;
}

void ov88_0225985C(WiFiPlazaNewsFlash *newsFlash) {
    u8 *bytes = (u8 *)newsFlash;

    bytes[0] = 0;
    bytes[1] = 0;
    bytes[2] = 0;
    bytes[3] = 0;
    bytes[4] = 0;
    bytes[5] = 0;
    bytes[6] = 0;
    bytes[7] = 0;
    bytes[8] = 0;
    bytes[9] = 0;
    bytes[10] = 0;
    bytes[11] = 0;
}

void ov88_02259878(WiFiPlazaNewsFlash *newsFlash) {
    newsFlash->active = 1;
    newsFlash->frameCounter = 0;
}

void ov88_02259884(WiFiPlazaNewsFlash *newsFlash) {
    u16 src;
    s32 cur;
    u16 v0;
    u32 v1;

    if ((u32)newsFlash->frameCounter > 0x1CU) {
        newsFlash->active = 0U;
    }
    if (newsFlash->active != 0) {
        if (newsFlash->frameCounter == 0) {
            PlaySE((u16)newsFlash->soundEffect);
        }
        v0 = newsFlash->frameCounter;
        if ((u32)v0 < 2U) {
            v1 = v0 * 0x10;
            cur = (s32)(v1 + (v1 >> 0x1F)) >> 1;
        } else if ((u32)v0 < 0xEU) {
            cur = 0x10;
        } else {
            cur = 0x10 - ((s32)((v0 - 0xE) * 0x10) / 14);
        }
        src = 0xE;
        BlendPalette(&src, &newsFlash->blendColor, 1U, (u8)cur, 0x19U);
        DC_FlushRange(&newsFlash->blendColor, 2U);
        GX_LoadBGPltt(&newsFlash->blendColor, (u32)newsFlash->paletteIndex, 2U);
        newsFlash->frameCounter = (u16)(newsFlash->frameCounter + 1);
    }
}
