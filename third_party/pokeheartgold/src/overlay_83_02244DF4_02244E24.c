#include "global.h"

#include "frontier/overlay_80_022340E8.h"

#include "party.h"
#include "pokemon_types_def.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "battle_castle_app_sprite.h"

void ov83_02246114(BattleCastleOpponentApp *arg0, s32 arg1);

struct BattleCastleAppSprite *ov83_02247454(BattleCastleAppSpriteManager *spriteMan, u32 param_2, u32 param_3, u32 param_4, u32 param_5, s16 param_6, s16 param_7, u32 param_8, const u8 *unused);

void *ov83_0224753C(struct BattleCastleAppSprite *sprite);

BOOL ov83_02247624(struct BattleCastleAppSprite *sprite);

u8 ov83_02247768(u8, u32, u32);

u32 ov83_02244E24(BattleCastleOpponentApp *param_1, u32 param_2, u32 param_3);

u32 ov83_02244E24(BattleCastleOpponentApp *param_1, u32 param_2, u32 param_3) {
    s32 iVar1;
    s32 iVar2;
    u32 uVar3;

    iVar1 = ov83_02247768(param_1->numSlots, param_2, param_3);
    Party_GetMonByIndex(param_1->opponentsParty, iVar1);
    switch (param_3) {
    case 0:
        param_1->arrowSpriteVisible = FALSE;
        return 1;
    case 1:
        if (param_1->arrowSpriteVisible == FALSE) {
            param_1->arrowSpriteVisible = TRUE;
            if (param_1->selectedLevelAdjustment == 1) {
                uVar3 = 9;
            } else {
                uVar3 = 10;
            }
            iVar2 = ov80_02237D8C(param_1->challengeType);
            if (iVar2 == 0) {
                iVar2 = 0x50;
            } else {
                iVar2 = 0x30;
            }
            param_1->arrowSprite = ov83_02247454(&param_1->spriteMan, 0, 0, 0, uVar3, iVar2 + iVar1 * 0x40, 0x32, 0, NULL);
        }
        iVar1 = ov83_02247624(param_1->arrowSprite);
        if (iVar1 == 0) {
            ov83_0224753C(param_1->arrowSprite);
            param_1->arrowSprite = NULL;
            param_1->arrowSpriteVisible = FALSE;
            return 1;
        }
        break;
    case 3:
        iVar1 = ov80_02237D8C(param_1->challengeType);
        if (iVar1 == 0) {
            ov83_02246114(param_1, 0);
            param_1->arrowSpriteVisible = FALSE;
            return 1;
        }
        param_1->arrowSpriteVisible = FALSE;
        return 1;
    case 4:
        iVar1 = ov80_02237D8C(param_1->challengeType);
        if (iVar1 == 0) {
            ov83_02246114(param_1, 0);
            param_1->arrowSpriteVisible = FALSE;
            return 1;
        }
        param_1->arrowSpriteVisible = FALSE;
        return 1;
    case 5:
        param_1->arrowSpriteVisible = FALSE;
        return 1;
    }
    return 0;
}
