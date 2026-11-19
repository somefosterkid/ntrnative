#include "global.h"

#include "overlay_manager.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

s32 ov80_02237254(u8);

u32 ov81_0223E318(void *param_1);

s32 ov81_0223EC88(void *);

s32 ov81_0223ECE4(void *);

s32 ov81_0223F1A4(BattleFactoryApp *);

s32 ov81_0223F38C(void *);

s32 ov81_0223F6A8(void *);

s32 ov81_0223F770(void *);

s32 ov81_0223FBAC(void *);

s32 ov81_0223FC74(void *);

s32 ov81_02240008(void *);

s32 ov81_02240048(void *);

s32 ov81_02240088(void *);

s32 ov81_022400D0(void *);

s32 ov81_02240F08(void *, u32);

u8 ov81_02240F18(u8);

s32 ov81_02240F28(void *);

void ov81_02242C48(void *);

u32 ov81_0223DEA8(OverlayManager *param_1, u32 *param_2);

u32 ov81_0223DEA8(OverlayManager *param_1, u32 *param_2) {
    BattleFactoryApp *app;
    u32 uVar2;
    s32 iVar3;

    app = OverlayManager_GetData(param_1);
    if (app->partnerTradedMon == 1) {
        switch (*param_2) {
        case 6:
        case 7:
        case 8:
        case 9:
            if (!app->summaryScreenOpen) {
                ov81_022404AC(app, param_2, 0xb);
            }
        }
    }
    ov81_02242514(app);
    switch (*param_2) {
    case 0:
        iVar3 = ov81_0223E318(app);
        if (iVar3 == 1) {
            ov81_022404AC(app, param_2, 1);
        }
        break;
    case 2:
        iVar3 = ov81_0223EC88(app);
        if (iVar3 == 1) {
            ov81_022404AC(app, param_2, 1);
        } else {
            return 0;
        }
    case 1:
        iVar3 = ov81_0223E520(app);
        if (iVar3 == 1) {
            iVar3 = ov81_02240F08(app, 0);
            if (iVar3 == 1) {
                ov81_022404AC(app, param_2, 3);
            } else {
                ov81_022404AC(app, param_2, 6);
            }
        }
        break;
    case 3:
        ov81_02241144(app);
        iVar3 = ov81_0223ECE4(app);
        if (iVar3 == 1) {
            if (app->summaryScreenOpened == 1) {
                ov81_022404AC(app, param_2, 2);
            } else {
                uVar2 = ov81_02240F18(app->challengeType);
                if (app->numMonsSelected == uVar2) {
                    ov81_022404AC(app, param_2, 4);
                } else {
                    iVar3 = ov81_02240F28(app);
                    if (iVar3 == 1) {
                        ov81_022404AC(app, param_2, 0xd);
                    } else {
                        ov81_022404AC(app, param_2, 3);
                    }
                }
            }
        }
        break;
    case 4:
        ov81_02241144(app);
        iVar3 = ov81_0223F1A4(app);
        if (iVar3 == 1) {
            uVar2 = ov81_02240F18(app->challengeType);
            if (app->numMonsSelected == uVar2) {
                ov81_022404AC(app, param_2, 0xd);
            } else {
                ov81_022404AC(app, param_2, 5);
            }
        }
        break;
    case 5:
        iVar3 = ov81_0223F314(app);
        if (iVar3 == 1) {
            ov81_022404AC(app, param_2, 3);
        }
        break;
    case 6:
        iVar3 = ov81_0223F38C(app);
        if (iVar3 == 1) {
            if (app->summaryScreenOpened == 1) {
                ov81_022404AC(app, param_2, 2);
            } else {
                iVar3 = ov81_02240F28(app);
                if (iVar3 == 1) {
                    ov81_02240F38(app, 0);
                    ov81_022404AC(app, param_2, 7);
                } else {
                    ov81_022404AC(app, param_2, 10);
                }
            }
        }
        break;
    case 7:
        iVar3 = ov81_0223F6A8(app);
        if (iVar3 == 1) {
            iVar3 = ov81_02240F28(app);
            if (iVar3 == 1) {
                ov81_02240F38(app, 0);
                ov81_022404AC(app, param_2, 6);
            } else {
                iVar3 = ov80_02237254(app->challengeType);
                if (iVar3 == 1) {
                    app->tradedMon = 0;
                    ov81_022404AC(app, param_2, 0xb);
                } else {
                    ov81_022404AC(app, param_2, 0xd);
                }
            }
        }
        break;
    case 8:
        iVar3 = ov81_0223F770(app);
        if (iVar3 == 1) {
            iVar3 = ov81_02240F28(app);
            if (iVar3 == 1) {
                ov81_02240F38(app, 0);
                ov81_022404AC(app, param_2, 9);
            } else if (app->numMonsSelected == 0) {
                ov81_022404AC(app, param_2, 10);
            } else {
                iVar3 = ov80_02237254(app->challengeType);
                if (iVar3 == 1) {
                    ov81_022404AC(app, param_2, 0xb);
                } else {
                    ov81_022404AC(app, param_2, 0xd);
                }
            }
        }
        break;
    case 9:
        iVar3 = ov81_0223FBAC(app);
        if (iVar3 == 1) {
            iVar3 = ov81_02240F28(app);
            if (iVar3 == 1) {
                ov81_02240F38(app, 0);
                ov81_022404AC(app, param_2, 8);
            } else {
                iVar3 = ov80_02237254(app->challengeType);
                if (iVar3 == 1) {
                    app->tradedMon = 0;
                    ov81_022404AC(app, param_2, 0xb);
                } else {
                    ov81_022404AC(app, param_2, 0xd);
                }
            }
        }
        break;
    case 10:
        iVar3 = ov81_0223FC74(app);
        if (iVar3 == 1) {
            if (app->numMonsSelected == 0) {
                ov81_0223E8BC(app);
                ov81_022404AC(app, param_2, 6);
            } else {
                ov81_0223EA98(app);
                ov81_022404AC(app, param_2, 8);
            }
        }
        break;
    case 0xb:
        iVar3 = ov81_02240008(app);
        if (iVar3 == 1) {
            if (app->partnerTradedMon == 1) {
                ov81_022404AC(app, param_2, 0xe);
            } else {
                ov81_022404AC(app, param_2, 0xc);
            }
        }
        break;
    case 0xc:
        iVar3 = ov81_02240048(app);
        if (iVar3 == 1) {
            ov81_022404AC(app, param_2, 0xd);
        }
        break;
    case 0xd:
        iVar3 = ov81_02240088(app);
        if (iVar3 == 1) {
            return 1;
        }
        break;
    case 0xe:
        iVar3 = ov81_022400D0(app);
        if (iVar3 == 1) {
            ov81_022404AC(app, param_2, 0xc);
        }
    }
    SpriteList_RenderAndAnimateSprites(app->spriteMan.spriteList);
    ov81_02242C48(app->monSpriteMan);
    return 0;
}
