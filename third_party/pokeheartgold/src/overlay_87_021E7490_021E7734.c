#include "global.h"
#include "overlay_87_scratch_off_card_app.h"

s32 ov87_021E7734(ScratchOffCardApp *app);

s32 ov87_021E7734(ScratchOffCardApp *app) {
    s32 v5;
    s32 v6;
    u8 v0;
    u8 v1;
    u8 v2;
    u8 v3;
    u8 v4;

    v5 = 0;
    if (app->numRevealedCells <= 1U) {
        return 0;
    }
    v0 = app->scratchCellValues[app->scratchedOffCellIdxs[1]];
    v3 = app->scratchCellValues[app->scratchedOffCellIdxs[0]];
    if (v3 == v0) {
        app->pairFound = 1;
    } else if (v3 == 4) {
        app->pairFound = 1;
    } else if (v0 == 4) {
        app->pairFound = 1;
    }
    if (app->numRevealedCells <= 2U) {
        return 0;
    }
    v4 = app->scratchCellValues[app->scratchedOffCellIdxs[1]];
    v1 = app->scratchCellValues[app->scratchedOffCellIdxs[0]];
    if ((v1 == v4) && (v1 == app->scratchCellValues[app->scratchedOffCellIdxs[2]])) {
        v5 = 1;
    }
    if ((v1 == 4) && (v4 == 4)) {
        v5 = 1;
    }
    if ((v1 == 4) && (app->scratchCellValues[app->scratchedOffCellIdxs[2]] == 4)) {
        v5 = 1;
    }
    if ((v4 == 4) && (app->scratchCellValues[app->scratchedOffCellIdxs[2]] == 4)) {
        v5 = 1;
    }
    if ((v1 == 4) && (v4 == app->scratchCellValues[app->scratchedOffCellIdxs[2]])) {
        v5 = 1;
    }
    if ((v4 == 4) && (v1 == app->scratchCellValues[app->scratchedOffCellIdxs[2]])) {
        v5 = 1;
    }
    if ((app->scratchCellValues[app->scratchedOffCellIdxs[2]] == 4) && (v1 == v4)) {
        v5 = 1;
    }
    if (v5 == 1) {
        v6 = 0;
loop_33:
        v2 = app->scratchCellValues[app->scratchedOffCellIdxs[v6]];
        if (v2 != 4) {
            app->winningCellValue = v2;
        } else {
            v6 += 1;
            if (v6 < 3) {
                goto loop_33;
            }
        }
        return 1;
    }
    return 0;
}

void ov87_021E80A8(ScratchOffCardsAppSprite *appSprite, BOOL draw);

s32 ov87_021E788C(ScratchOffCardApp *app);

s32 ov87_021E788C(ScratchOffCardApp *app) {
    s32 v1;
    s32 i;
    u8 v0;

    v1 = 0;
    i = 0;
    do {
        if (app->scratchCellValues[app->scratchedOffCellIdxs[i]] == 4) {
            ov87_021E80A8(app->winMonSprites[i], 1);
            v1 = 1;
        }
        i += 1;
    } while (i < 3);
    app->mosaicSize = 0U;
    v0 = app->mosaicSize;
    *(s8 *)(REG_MOSAIC_ADDR + 1) = v0 | (v0 * 0x10);
    return v1;
}
