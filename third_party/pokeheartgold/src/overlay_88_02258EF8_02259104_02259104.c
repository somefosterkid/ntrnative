#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "vram_transfer_manager.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_88_wifi_plaza_news.h"
#include "gf_gfx_planes.h"

struct UnkStruct_ov66_0223177C_t;

enum {
    WIFI_PLAZA_NEWS_ANIM_PHASE_IDLE = 0,
    WIFI_PLAZA_NEWS_ANIM_PHASE_JOIN_START,
    WIFI_PLAZA_NEWS_ANIM_PHASE_JOIN_PLAYING,
    WIFI_PLAZA_NEWS_ANIM_PHASE_LEAVE_START,
    WIFI_PLAZA_NEWS_ANIM_PHASE_LEAVE_PLAYING,
};

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

static void ov88_02259244(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, const UnkStruct_ov66_0223177C *playerList, const WiFiPlazaSystem *plazaSys, u32 animFrame);

static void ov88_02259264(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, const UnkStruct_ov66_0223177C *playerList, const WiFiPlazaSystem *plazaSys, u32 animFrame);

void ov88_02259104(WiFiPlazaNewsAnimState *animState, WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, const UnkStruct_ov66_0223177C *playerList, const WiFiPlazaSystem *plazaSys);

void ov88_02259104(WiFiPlazaNewsAnimState *animState, WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, const UnkStruct_ov66_0223177C *playerList, const WiFiPlazaSystem *plazaSys) {
    switch (animState->animationState) {
    case WIFI_PLAZA_NEWS_ANIM_PHASE_IDLE:
        break;
    case WIFI_PLAZA_NEWS_ANIM_PHASE_JOIN_START:
        animState->animationState = WIFI_PLAZA_NEWS_ANIM_PHASE_JOIN_PLAYING;
        animState->animationFrame = 0;
        break;
    case WIFI_PLAZA_NEWS_ANIM_PHASE_JOIN_PLAYING:
        ov88_02259244(digits, graphics, playerList, plazaSys, animState->animationFrame);
        animState->animationFrame++;

        if (animState->animationFrame >= 32) {
            animState->animationState = WIFI_PLAZA_NEWS_ANIM_PHASE_IDLE;
        }
        break;
    case WIFI_PLAZA_NEWS_ANIM_PHASE_LEAVE_START:
        animState->animationState = WIFI_PLAZA_NEWS_ANIM_PHASE_LEAVE_PLAYING;
        animState->animationFrame = 0;
        break;
    case WIFI_PLAZA_NEWS_ANIM_PHASE_LEAVE_PLAYING:
        ov88_02259264(digits, graphics, playerList, plazaSys, animState->animationFrame);
        animState->animationFrame++;

        if (animState->animationFrame >= 32) {
            animState->animationState = WIFI_PLAZA_NEWS_ANIM_PHASE_IDLE;
        }

        break;
    }
}

BOOL ov45_0222AB28(const WiFiPlazaSystem *param0, u32 param1);

BOOL ov45_0222DCFC(const UnkStruct_ov66_0223177C *param0, u32 param1);

u32 ov45_0222DD08(const UnkStruct_ov66_0223177C *param0, u32 param1);

BOOL ov45_0222DD14(const UnkStruct_ov66_0223177C *param0, u32 param1);

BOOL ov45_0222DD20(const UnkStruct_ov66_0223177C *param0, u32 param1);

BOOL ov45_0222DD2C(const UnkStruct_ov66_0223177C *param0, u32 param1);

void ov88_02258DE8(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, u8 column, u8 row, u32 param4, u32 param5, BOOL param6, BOOL param7, BOOL param8);

void ov88_0225918C(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, const UnkStruct_ov66_0223177C *playerList, const WiFiPlazaSystem *plazaSys, u32 iconFrame);

void ov88_0225918C(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, const UnkStruct_ov66_0223177C *playerList, const WiFiPlazaSystem *plazaSys, u32 iconFrame) {
    int row, col;
    u32 v2;
    BOOL v3;
    BOOL v4;
    BOOL v5;
    BOOL occupied;
    BOOL v7;
    u32 slotIndex;
    u32 drawFrame;

    FillBgTilemapRect(graphics->bgConfig, GF_BG_LYR_MAIN_3, 0, 21, 4, 5 * 2, 4 * 2, 0);

    for (row = 0; row < 4; row++) {
        for (col = 0; col < 5; col++) {
            slotIndex = (row * 5) + col;
            occupied = ov45_0222DCFC(playerList, slotIndex);

            if (occupied) {
                v2 = ov45_0222DD08(playerList, slotIndex);
                v3 = ov45_0222DD14(playerList, slotIndex);
                v4 = ov45_0222DD20(playerList, slotIndex);
                v5 = ov45_0222DD2C(playerList, slotIndex);
                v7 = ov45_0222AB28(plazaSys, slotIndex);

                if ((iconFrame == 2) && (v5 == 1)) {
                    drawFrame = 0;
                } else {
                    drawFrame = iconFrame;
                }

                ov88_02258DE8(digits, graphics, col, row, drawFrame, v2, v3, v4, v7);
            }
        }
    }
}

static void ov88_02259244(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, const UnkStruct_ov66_0223177C *playerList, const WiFiPlazaSystem *plazaSys, u32 animFrame) {
    u32 blinkFrame;

    if ((animFrame % (32 / 4)) == 0) {
        blinkFrame = animFrame / (32 / 4);

        if ((blinkFrame % 2) == 0) {
            blinkFrame = 2;
        } else {
            blinkFrame = 0;
        }

        ov88_0225918C(digits, graphics, playerList, plazaSys, blinkFrame);
    }
}

static void ov88_02259264(WiFiPlazaNewsDigits *digits, WiFiPlazaNewsGraphics *graphics, const UnkStruct_ov66_0223177C *playerList, const WiFiPlazaSystem *plazaSys, u32 animFrame) {
    u32 blinkFrame;

    if ((animFrame % (32 / 4)) == 0) {
        blinkFrame = animFrame / (32 / 4);

        if ((blinkFrame % 2) == 0) {
            blinkFrame = 1;
        } else {
            blinkFrame = 0;
        }

        ov88_0225918C(digits, graphics, playerList, plazaSys, blinkFrame);
    }
}

u32 ov45_0222DDE4(const UnkStruct_ov66_0223177C *param0, int param1, u32 param2);

void ov88_02259280(WiFiPlazaNewsNamePalette *namePalette, const UnkStruct_ov66_0223177C *playerList, int page, const WiFiPlazaSystem *plazaSys);

void ov88_02259280(WiFiPlazaNewsNamePalette *namePalette, const UnkStruct_ov66_0223177C *playerList, int page, const WiFiPlazaSystem *plazaSys) {
    u32 slotIndex;
    BOOL isHighlighted;

    for (int i = 0; i < 4; i++) {
        slotIndex = ov45_0222DDE4(playerList, page, i);

        if (slotIndex != 20) {
            isHighlighted = ov45_0222AB28(plazaSys, slotIndex);

            if (isHighlighted == 1) {
                namePalette->paletteIndices[i] = 9;
            } else {
                namePalette->paletteIndices[i] = 5;
            }
        }
    }
}

void ov88_022592B8(const WiFiPlazaNewsNamePalette *namePalette, const NNSG2dPaletteData *plttData, u32 paletteSlot);

void ov88_022592B8(const WiFiPlazaNewsNamePalette *namePalette, const NNSG2dPaletteData *plttData, u32 paletteSlot) {
    BOOL success;
    const u8 *data = plttData->pRawData;

    for (int i = 0; i < 4; i++) {
        success = GF_CreateNewVramTransferTask(NNS_GFD_DST_2D_BG_PLTT_MAIN, (paletteSlot * 0x20) + (5 * 2) + (i * 0x4), (void *)(&data[(7 * 0x20) + (namePalette->paletteIndices[i] * 2)]), 0x4);
        ((success) ? (void)0 : GF_AssertFail());
    }
}
