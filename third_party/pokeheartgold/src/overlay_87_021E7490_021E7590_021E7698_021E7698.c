#include "global.h"
#include "overlay_87_scratch_off_card_app.h"

void ov87_021E7F28(ScratchOffCardsAppSpriteManager *spriteMan, int index);

void ov87_021E7FD4(ScratchOffCardsAppSprite *appSprite, BOOL draw);

void ov87_021E7FEC(ScratchOffCardsAppSprite *appSprite, int x, int y);

void ov87_021E8078(ScratchOffCardsAppSprite *appSprite, BOOL draw);

extern const CoordinatesS16 ov87_021E82E4[9];

void ov87_021E7698(ScratchOffCardApp *app);

void ov87_021E7698(ScratchOffCardApp *app) {
    int i;

    app->highlightPairCounter++;
    if (app->highlightPairCounter >= 1) {
        app->highlightPairCounter = 0;
        app->highlightPairPalette++;
        if (app->highlightPairPalette >= 8) {
            app->highlightPairPalette = 0;
        }
        if (app->numRevealedCells == 2) {
            for (i = 0; i < 2; i++) {
                ov87_021E8078(app->matchingCellBoxSprites[i], 3);
                ov87_021E7FD4(app->matchingCellBoxSprites[i], 1);
                ov87_021E7FEC(app->matchingCellBoxSprites[i], ov87_021E82E4[app->scratchedOffCellIdxs[i]].x - 0x24, ov87_021E82E4[app->scratchedOffCellIdxs[i]].y - 0xA);
            }
        }
        ov87_021E7F28(&app->spriteMan, app->highlightPairPalette);
    }
}
