#include "global.h"

#include "bg_window.h"
#include "trainer_card_main.h"
#include "gf_gfx_planes.h"

void ov51_021E6D44(BgConfig *bgConfig, enum GFBgLayer bgLayer, u8 *signature);

s32 ov51_021E757C(void *, void *, void *);
void ov51_021E7664(Window *window, BOOL showColon, String *string);

void ov51_021E6D44(BgConfig *bgConfig, enum GFBgLayer bgLayer, u8 *signature) {
    BG_LoadCharTilesData(bgConfig, bgLayer, signature, 24 * 8 * (8 * 8), 1);

    u16 *tilemapBuffer = (u16 *)GetBgTilemapBuffer(bgConfig, bgLayer);
    u8 x, y;
    u16 tileIndex = 0;

    for (y = 0; y < 8; y++) {
        for (x = 0; x < 24; x++) {
            tilemapBuffer[(12 + y) * 32 + x + 4] = 1 + tileIndex;
            tileIndex++;
        }
    }

    ScheduleBgTilemapBufferTransfer(bgConfig, bgLayer);
}

void ov51_021E6DA8(TrainerCardMain *cardMain, BOOL liveIgt) {
    u8 v0;

    if (liveIgt != 0) {
        if (cardMain->showingBack == 0) {
            v0 = cardMain->colonBlinkTimer;
            if (v0 == 0) {
                ov51_021E757C(&cardMain->windows[0], cardMain->card, cardMain->unk_33C8);
                ov51_021E7664(&cardMain->windows[5], 1, cardMain->colonString);
            } else if (v0 == 0xF) {
                ov51_021E7664(&cardMain->windows[5], 0, cardMain->colonString);
            }
        }
        cardMain->colonBlinkTimer = (u8)((s32)(cardMain->colonBlinkTimer + 1) % 30);
    }
}

void ov51_021E6E10(TrainerCardMain *cardMain, int showButtons) {
    if (showButtons == 1) {
        ov51_021E7D44(&cardMain->sprites, 1, 2, 0);
        ov51_021E7D44(&cardMain->sprites, 0, 0, 0);
    } else {
        ov51_021E7D44(&cardMain->sprites, 1, 2, 1);
        ov51_021E7D44(&cardMain->sprites, 0, 0, 1);
    }
    ov51_021E78D0(cardMain, showButtons);
}
