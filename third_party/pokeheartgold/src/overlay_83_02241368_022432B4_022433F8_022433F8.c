#include "global.h"

#include "options.h"
#include "overlay_80_02237A70.h"
#include "party.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "save_frontier.h"
#include "system.h"
#include "touchscreen_list_menu.h"
#include "unk_02005D10.h"
#include "unk_02030A98.h"
#include "unk_0205BFF0.h"
#include "yes_no_prompt.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov83_02247CF0(void);

void ov80_02237FA4(void *, u8, u32);

u8 ov83_022448AC(u8 *, u32, u8);

void ov83_022448E4(u8 *, u8 *);

void ov83_02244A74(u8 *);

void ov83_02244A88(u8 *);

void ov83_02244A90(u8 *);

void ov83_02244A98(u8 *, u32, u32, u32, u32);

void ov83_02244AB0(u8 *, u32, BoxPokemon *);

void ov83_02244BEC(u8 *);

void ov83_02244C4C(u8 *);

void ov83_02244C58(u8 *);

void ov83_02244C88(u8 *);

void ov83_02244C9C(u8 *);

void ov83_02244CCC(u8 *);

s32 ov83_02244E24(u8 *, u32, u32);

u16 ov83_02245068(u8);

void ov83_02245094(u8 *);

void ov83_022453C0(u8 *);

void ov83_022453DC(u8 *, u8 *);

void ov83_02245554(u8 *, s8);

void ov83_02245748(u8 *, u32);

void ov83_02245838(u8 *, u32, s32);

void ov83_022459A0(u8 *);

s32 ov83_02245A40(u8 *, u32, u32);

void ov83_02245ACC(u8 *, u8, u8);

void ov83_02246114(u8 *, u32);

void ov83_02246988(u8 *);

void ov83_022469D8(u8 *, u32, u8);

void ov83_02246CC0(u8 *);

void ov83_02246D40(u8 *);

void ov83_02247630(void *, u32, u32);

u8 ov83_02247768(u8, u8);

u8 ov83_0224777C(SaveData *, u8, u8);

void ov83_022477B0(u32, u16);

void ov83_022478B4(u8 *);

void ov83_02247944(u8 *, u32);

s32 ov83_02247AD4(u32);

void ov83_02247B04(u32);

u32 ov83_022433F8(u8 *param_1, u32 param_2, u32 param_3, u32 param_4);

u32 ov83_022433F8(u8 *param_1, u32 param_2, u32 param_3, u32 param_4) {
    u32 bVar1;
    u8 uVar2;
    u16 uVar3;
    Pokemon *mon;
    BoxPokemon *pBVar4;
    u32 uVar5;
    u32 uVar6;
    YesNoResponse YVar7;
    u32 uVar8;
    u32 uVar9;
    BOOL BVar10;
    s32 iVar11;
    s32 sVar12;

    switch (((BattleCastleOpponentApp *)param_1)->subState) {
    case 0:
        ((BattleCastleOpponentApp *)param_1)->unused = 0;
        ((BattleCastleOpponentApp *)param_1)->subState = 1;
        bVar1 = (u32)(*((u8 *)((u8 *)&((BattleCastleOpponentApp *)param_1)->levelAdjustment + 0x1)) << 24) >> 27;
        if (bVar1 == 1) {
            ov83_02245094(param_1 + 0xc0);
            ov83_02244BEC(param_1);
        } else if (bVar1 == 2) {
            ov83_02245094(param_1 + 0xc0);
            ov83_02244C9C(param_1);
            ov83_02247630(*((void **)&((BattleCastleOpponentApp *)param_1)->sparklesSprite), 200, 0x69);
            ((BattleCastleOpponentApp *)param_1)->subState = 6;
        } else if (bVar1 == 3) {
            ((BattleCastleOpponentApp *)param_1)->subState = 0xe;
        }
        *((u8 *)((u8 *)&((BattleCastleOpponentApp *)param_1)->levelAdjustment + 0x1)) = *((u8 *)((u8 *)&((BattleCastleOpponentApp *)param_1)->levelAdjustment + 0x1)) & ~0xF8;
        break;
    case 1:
        uVar6 = ov83_02247AD4(((BattleCastleOpponentApp *)param_1)->unk_5F0);
        switch (uVar6) {
        case 0:
        case 1:
        case 2:
        case 3:
            ov83_02244C4C(param_1);
            ov83_02244C58(param_1);
            ((BattleCastleOpponentApp *)param_1)->subState = 2;
            break;
        case 4:
            PlaySE(0x5dc);
            return 1;
        case 0xfffffffe:
            if (((BattleCastleOpponentApp *)param_1)->selectedMonSlot != ((BattleCastleOpponentApp *)param_1)->exitSlot) {
                ov83_02247B04(((BattleCastleOpponentApp *)param_1)->unk_5F0);
                ov83_022469D8(param_1, 4, ((BattleCastleOpponentApp *)param_1)->selectedMonSlot);
            }
            break;
        }
        break;
    case 2:
        uVar6 = TouchscreenListMenu_HandleInput(*((TouchscreenListMenu **)&((BattleCastleOpponentApp *)param_1)->unk_5F8));
        ov83_022477B0(uVar6, 0x5dc);
        ov83_02246CC0(param_1);
        {
            switch (uVar6) {
            case 0:
                *((s8 *)&((BattleCastleOpponentApp *)param_1)->selectedMenuEntry) = (s8)uVar6;
                iVar11 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, ((BattleCastleOpponentApp *)param_1)->selectedMonSlot);
                if (*(u8 *)(((BattleCastleOpponentApp *)param_1)->identityUnlocked + iVar11) == 0) {
                    ov83_02244C88(param_1);
                    ov83_022453C0(param_1);
                    ov83_02244A98(param_1, 0, 1, 4, 0);
                    uVar2 = ov83_022448AC(param_1, 0x10, 1);
                    ((BattleCastleOpponentApp *)param_1)->printerID = uVar2;
                    ov83_02244A74(param_1);
                    ((BattleCastleOpponentApp *)param_1)->subState = 3;
                } else {
                    ov83_02244C88(param_1);
                    iVar11 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, ((BattleCastleOpponentApp *)param_1)->selectedMonSlot);
                    mon = Party_GetMonByIndex(((BattleCastleOpponentApp *)param_1)->opponentsParty, iVar11);
                    ov83_022453C0(param_1);
                    pBVar4 = Mon_GetBoxMon(mon);
                    ov83_02244AB0(param_1, 0, pBVar4);
                    uVar2 = ov83_022448AC(param_1, 0x14, 1);
                    ((BattleCastleOpponentApp *)param_1)->printerID = uVar2;
                    ((BattleCastleOpponentApp *)param_1)->subState = 0x10;
                }
                break;
            case 1:
                *((s8 *)&((BattleCastleOpponentApp *)param_1)->selectedMenuEntry) = (s8)uVar6;
                ov83_02244C88(param_1);
                ov83_022453C0(param_1);
                ov83_02244AB0(param_1, 0, (struct BoxPokemon *)(((BattleCastleOpponentApp *)param_1)->boxMon));
                uVar2 = ov83_022448AC(param_1, 0x15, 1);
                ((BattleCastleOpponentApp *)param_1)->printerID = uVar2;
                ov83_02244A88(param_1);
                ((BattleCastleOpponentApp *)param_1)->subState = 4;
                break;
            case 2:
                ov83_02244C88(param_1);
                ov83_02244C9C(param_1);
                ((BattleCastleOpponentApp *)param_1)->subState = 6;
                break;
            case 6:
            case 0xfffffffe:
                ov83_02244C88(param_1);
                ov83_02244BEC(param_1);
                ((BattleCastleOpponentApp *)param_1)->subState = 0;
                break;
            case 0xffffffff:
                break;
            }
        }
        break;
    case 3:
        YVar7 = YesNoPrompt_HandleInput(*((YesNoPrompt **)&((BattleCastleOpponentApp *)param_1)->unk_604));
        switch (YVar7) {
        case YESNORESPONSE_YES:
            ov83_022478B4(param_1 + 0x604);
            ov83_02245094(param_1 + 0xc0);
            uVar8 = sub_0205C1F0(((BattleCastleOpponentApp *)param_1)->challengeType);
            uVar9 = sub_0205C1F0(((BattleCastleOpponentApp *)param_1)->challengeType);
            uVar9 = sub_0205C268(uVar9);
            uVar3 = FrontierSave_GetStat(((FrontierSave *)((BattleCastleOpponentApp *)param_1)->frontier), uVar8, uVar9);
            if (uVar3 < 1) {
                ov83_022453C0(param_1);
                uVar2 = ov83_022448AC(param_1, 0x1c, 1);
                ((BattleCastleOpponentApp *)param_1)->printerID = uVar2;
                ((BattleCastleOpponentApp *)param_1)->subState = 0x10;
            } else {
                BVar10 = ov80_02237D8C(((BattleCastleOpponentApp *)param_1)->challengeType);
                if (BVar10 == 0) {
                    ov80_02237FA4(*((void **)&((BattleCastleOpponentApp *)param_1)->frontier), ((BattleCastleOpponentApp *)param_1)->challengeType, 1);
                    ov83_022453DC(param_1, param_1 + 0x50);
                    ov83_02245748(param_1, ((BattleCastleOpponentApp *)param_1)->selectedMonSlot);
                    ov83_02246988(param_1);
                    ov83_02246114(param_1, 0);
                    ((BattleCastleOpponentApp *)param_1)->subState = 0xb;
                } else {
                    ((BattleCastleOpponentApp *)param_1)->syncPurchaseAction = 1;
                    return 1;
                }
            }
            break;
        case YESNORESPONSE_NO:
            ov83_022478B4(param_1 + 0x604);
            ov83_02245094(param_1 + 0xc0);
            ov83_02244C58(param_1);
            ((BattleCastleOpponentApp *)param_1)->subState = 2;
        }
        break;
    case 4:
        uVar6 = TouchscreenListMenu_HandleInput(*((TouchscreenListMenu **)&((BattleCastleOpponentApp *)param_1)->unk_5F8));
        ov83_022477B0(uVar6, 0x5dc);
        switch (uVar6) {
        case 2:
        case 0xfffffffe:
            ov83_02245094(param_1 + 0xc0);
            ov83_02244A90(param_1);
            ov83_02244C58(param_1);
            ((BattleCastleOpponentApp *)param_1)->subState = 2;
            break;
        case 0: {
            uVar8 = sub_0205C1F0(((BattleCastleOpponentApp *)param_1)->challengeType);
            uVar9 = sub_0205C1F0(((BattleCastleOpponentApp *)param_1)->challengeType);
            uVar9 = sub_0205C268(uVar9);
            FrontierSave_GetStat(((FrontierSave *)((BattleCastleOpponentApp *)param_1)->frontier), uVar8, uVar9);
            ov83_02244A90(param_1);
            iVar11 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, ((BattleCastleOpponentApp *)param_1)->selectedMonSlot);
            if (*(u8 *)(((BattleCastleOpponentApp *)param_1)->levelAdjustments + iVar11) == 1) {
                ov83_022453C0(param_1);
                uVar2 = ov83_022448AC(param_1, 0x1d, 1);
                ((BattleCastleOpponentApp *)param_1)->printerID = uVar2;
                ((BattleCastleOpponentApp *)param_1)->subState = 0x10;
            } else {
                ov83_02245554(param_1, 1);
                ((BattleCastleOpponentApp *)param_1)->subState = 5;
            }
            break;
        }
        case 1: {
            uVar8 = sub_0205C1F0(((BattleCastleOpponentApp *)param_1)->challengeType);
            uVar9 = sub_0205C1F0(((BattleCastleOpponentApp *)param_1)->challengeType);
            uVar9 = sub_0205C268(uVar9);
            FrontierSave_GetStat(((FrontierSave *)((BattleCastleOpponentApp *)param_1)->frontier), uVar8, uVar9);
            ov83_02244A90(param_1);
            iVar11 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, ((BattleCastleOpponentApp *)param_1)->selectedMonSlot);
            if (*(u8 *)(((BattleCastleOpponentApp *)param_1)->levelAdjustments + iVar11) == 2) {
                ov83_022453C0(param_1);
                uVar2 = ov83_022448AC(param_1, 0x1e, 1);
                ((BattleCastleOpponentApp *)param_1)->printerID = uVar2;
                ((BattleCastleOpponentApp *)param_1)->subState = 0x10;
            } else {
                ov83_02245554(param_1, 2);
                ((BattleCastleOpponentApp *)param_1)->subState = 5;
            }
            break;
        }
        case 0xffffffff:
            break;
        }
        break;
    case 5:
        YVar7 = YesNoPrompt_HandleInput(*((YesNoPrompt **)&((BattleCastleOpponentApp *)param_1)->unk_604));
        switch (YVar7) {
        case YESNORESPONSE_YES:
            ov83_022478B4(param_1 + 0x604);
            uVar8 = sub_0205C1F0(((BattleCastleOpponentApp *)param_1)->challengeType);
            uVar9 = sub_0205C1F0(((BattleCastleOpponentApp *)param_1)->challengeType);
            uVar9 = sub_0205C268(uVar9);
            uVar3 = FrontierSave_GetStat(((FrontierSave *)((BattleCastleOpponentApp *)param_1)->frontier), uVar8, uVar9);
            uVar6 = ov83_02245068(((BattleCastleOpponentApp *)param_1)->levelAdjustment);
            if (uVar3 < uVar6) {
                ov83_022453C0(param_1);
                uVar2 = ov83_022448AC(param_1, 0x1c, 1);
                ((BattleCastleOpponentApp *)param_1)->printerID = uVar2;
                ((BattleCastleOpponentApp *)param_1)->subState = 0x10;
                return 0;
            } else {
                ((BattleCastleOpponentApp *)param_1)->selectedLevelAdjustment = ((BattleCastleOpponentApp *)param_1)->levelAdjustment;
                BVar10 = ov80_02237D8C(((BattleCastleOpponentApp *)param_1)->challengeType);
                if (BVar10 == 0) {
                    ov83_02245094(param_1 + 0xc0);
                    uVar5 = ov83_02245068(((BattleCastleOpponentApp *)param_1)->levelAdjustment);
                    ov80_02237FA4(*((void **)&((BattleCastleOpponentApp *)param_1)->frontier), ((BattleCastleOpponentApp *)param_1)->challengeType, uVar5);
                    ov83_022453DC(param_1, param_1 + 0x50);
                    ov83_02245838(param_1, ((BattleCastleOpponentApp *)param_1)->selectedMonSlot, ((BattleCastleOpponentApp *)param_1)->levelAdjustment);
                    ((BattleCastleOpponentApp *)param_1)->subState = 0xc;
                } else {
                    ((BattleCastleOpponentApp *)param_1)->syncPurchaseAction = 1;
                    return 1;
                }
            }
            break;
        case YESNORESPONSE_NO:
            ov83_022478B4(param_1 + 0x604);
            ov83_02245094(param_1 + 0xc0);
            ov83_02244C58(param_1);
            ((BattleCastleOpponentApp *)param_1)->subState = 2;
        }
        break;
    case 6:
        uVar6 = TouchscreenListMenu_HandleInput(*((TouchscreenListMenu **)&((BattleCastleOpponentApp *)param_1)->unk_5F8));
        ov83_022477B0(uVar6, 0x5dc);
        ov83_02246D40(param_1);
        {
            {
                {
                    switch (uVar6) {
                    case 0xfffffffe:
                        ov83_02245094(param_1 + 0xc0);
                        ov83_02244CCC(param_1);
                        ov83_02244C58(param_1);
                        ((BattleCastleOpponentApp *)param_1)->subState = 2;
                        break;
                    case 0xffffffff:
                        break;
                    case 3:
                        ((BattleCastleOpponentApp *)param_1)->selectedMenuEntry = uVar6;
                        ov83_02244CCC(param_1);
                        iVar11 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, ((BattleCastleOpponentApp *)param_1)->selectedMonSlot);
                        if (*(u8 *)(((BattleCastleOpponentApp *)param_1)->statsUnlocked + iVar11) == 0) {
                            ov83_022453C0(param_1);
                            ov83_02244A98(param_1, 0, 2, 4, 0);
                            uVar2 = ov83_022448AC(param_1, 0x2b, 1);
                            ((BattleCastleOpponentApp *)param_1)->printerID = uVar2;
                            ov83_02244A74(param_1);
                            ((BattleCastleOpponentApp *)param_1)->subState = 7;
                        } else {
                            ((BattleCastleOpponentApp *)param_1)->subState = 0x11;
                        }
                        break;
                    case 4:
                        ((BattleCastleOpponentApp *)param_1)->selectedMenuEntry = uVar6;
                        ov83_02244CCC(param_1);
                        iVar11 = ov83_0224777C(*((void **)&((BattleCastleOpponentApp *)param_1)->saveData), ((BattleCastleOpponentApp *)param_1)->challengeType, 2);
                        if (iVar11 == 1) {
                            uVar2 = ov83_022448AC(param_1, 0x2a, 1);
                            ((BattleCastleOpponentApp *)param_1)->printerID = uVar2;
                            ((BattleCastleOpponentApp *)param_1)->subState = 0xf;
                            return 0;
                        }
                        iVar11 = ov83_02247768(((BattleCastleOpponentApp *)param_1)->numSlots, ((BattleCastleOpponentApp *)param_1)->selectedMonSlot);
                        if (*(u8 *)(((BattleCastleOpponentApp *)param_1)->movesUnlocked + iVar11) == 0) {
                            ov83_022453C0(param_1);
                            ov83_02244A98(param_1, 0, 5, 4, 0);
                            uVar2 = ov83_022448AC(param_1, 0x4f, 1);
                            ((BattleCastleOpponentApp *)param_1)->printerID = uVar2;
                            ov83_02244A74(param_1);
                            ((BattleCastleOpponentApp *)param_1)->subState = 8;
                        } else {
                            ((BattleCastleOpponentApp *)param_1)->subState = 0x12;
                        }
                        break;
                    case 5:
                        iVar11 = ov83_0224777C(*((void **)&((BattleCastleOpponentApp *)param_1)->saveData), ((BattleCastleOpponentApp *)param_1)->challengeType, 2);
                        if (iVar11 == 2) {
                            ov83_02245094(param_1 + 0xc0);
                            ov83_02244CCC(param_1);
                            ov83_02244C58(param_1);
                            ((BattleCastleOpponentApp *)param_1)->subState = 2;
                        } else {
                            ((BattleCastleOpponentApp *)param_1)->selectedMenuEntry = uVar6;
                            ov83_02244CCC(param_1);
                            uVar8 = sub_0205C1F0(((BattleCastleOpponentApp *)param_1)->challengeType);
                            uVar9 = sub_0205C1F0(((BattleCastleOpponentApp *)param_1)->challengeType);
                            uVar9 = sub_0205C268(uVar9);
                            FrontierSave_GetStat(((FrontierSave *)((BattleCastleOpponentApp *)param_1)->frontier), uVar8, uVar9);
                            ov83_02244A98(param_1, 0, 0x32, 4, 0);
                            uVar2 = ov83_022448AC(param_1, 0x5b, 1);
                            ((BattleCastleOpponentApp *)param_1)->printerID = uVar2;
                            ov83_02244A74(param_1);
                            ((BattleCastleOpponentApp *)param_1)->subState = 9;
                        }
                        break;
                    }
                }
            }
        }
        break;
    case 7:
        YVar7 = YesNoPrompt_HandleInput(*((YesNoPrompt **)&((BattleCastleOpponentApp *)param_1)->unk_604));
        switch (YVar7) {
        case YESNORESPONSE_YES:
            ov83_022478B4(param_1 + 0x604);
            iVar11 = ov83_02245A40(param_1, 2, 0x2e);
            if (iVar11 == 1) {
                return 1;
            }
            break;
        case YESNORESPONSE_NO:
            ov83_022478B4(param_1 + 0x604);
            ov83_02244C9C(param_1);
            ((BattleCastleOpponentApp *)param_1)->subState = 6;
        }
        break;
    case 8:
        YVar7 = YesNoPrompt_HandleInput(*((YesNoPrompt **)&((BattleCastleOpponentApp *)param_1)->unk_604));
        switch (YVar7) {
        case YESNORESPONSE_YES:
            ov83_022478B4(param_1 + 0x604);
            iVar11 = ov83_02245A40(param_1, 5, 0x52);
            if (iVar11 == 1) {
                return 1;
            }
            break;
        case YESNORESPONSE_NO:
            ov83_022478B4(param_1 + 0x604);
            ov83_02244C9C(param_1);
            ((BattleCastleOpponentApp *)param_1)->subState = 6;
        }
        break;
    case 9:
        YVar7 = YesNoPrompt_HandleInput(*((YesNoPrompt **)&((BattleCastleOpponentApp *)param_1)->unk_604));
        switch (YVar7) {
        case YESNORESPONSE_YES:
            ov83_022478B4(param_1 + 0x604);
            uVar8 = sub_0205C1F0(((BattleCastleOpponentApp *)param_1)->challengeType);
            uVar9 = sub_0205C1F0(((BattleCastleOpponentApp *)param_1)->challengeType);
            uVar9 = sub_0205C268(uVar9);
            uVar3 = FrontierSave_GetStat(((FrontierSave *)((BattleCastleOpponentApp *)param_1)->frontier), uVar8, uVar9);
            ov83_0224777C(*((void **)&((BattleCastleOpponentApp *)param_1)->saveData), ((BattleCastleOpponentApp *)param_1)->challengeType, 2);
            if (uVar3 < 0x32) {
                uVar8 = Options_GetFrame(((BattleCastleOpponentApp *)param_1)->options);
                ov83_02247944(param_1 + 0xc0, uVar8);
                uVar2 = ov83_022448AC(param_1, 0x52, 1);
                ((BattleCastleOpponentApp *)param_1)->printerID = uVar2;
                ((BattleCastleOpponentApp *)param_1)->subState = 0xf;
            } else {
                BVar10 = ov80_02237D8C(((BattleCastleOpponentApp *)param_1)->challengeType);
                if (BVar10 == 0) {
                    ov83_02245ACC(param_1, ((BattleCastleOpponentApp *)param_1)->selectedMonSlot, 5);
                    ((BattleCastleOpponentApp *)param_1)->subState = 10;
                } else {
                    ((BattleCastleOpponentApp *)param_1)->syncPurchaseAction = 1;
                    return 1;
                }
            }
            break;
        case YESNORESPONSE_NO:
            ov83_022478B4(param_1 + 0x604);
            ov83_02244C9C(param_1);
            ((BattleCastleOpponentApp *)param_1)->subState = 6;
        }
        break;
    case 10:
        sVar12 = ov83_02247CF0();
        if (sVar12 == 1) {
            ov83_02244C9C(param_1);
            ov83_02246114(param_1, 0);
            ov83_02247630(*((void **)&((BattleCastleOpponentApp *)param_1)->sparklesSprite), 200, 0x69);
            ((BattleCastleOpponentApp *)param_1)->subState = 6;
        }
        break;
    case 0xb:
        ov83_022448E4(param_1, param_1 + 0x80);
        ((BattleCastleOpponentApp *)param_1)->subState = 0xc;
    case 0xc:
        iVar11 = ov83_02244E24(param_1, ((BattleCastleOpponentApp *)param_1)->selectedMonSlot, ((BattleCastleOpponentApp *)param_1)->selectedMenuEntry);
        if (iVar11 == 1) {
            ((BattleCastleOpponentApp *)param_1)->subState = 0x10;
        }
        break;
    case 0xd:
        iVar11 = ov83_02244E24(param_1, ((BattleCastleOpponentApp *)param_1)->selectedMonSlot, ((BattleCastleOpponentApp *)param_1)->selectedMenuEntry);
        if (iVar11 == 1) {
            ((BattleCastleOpponentApp *)param_1)->subState = 0xe;
        }
        break;
    case 0xe:
        if (((BattleCastleOpponentApp *)param_1)->selectedMenuEntry == 3) {
            ((BattleCastleOpponentApp *)param_1)->subState = 0x11;
        } else {
            ((BattleCastleOpponentApp *)param_1)->subState = 0x12;
        }
        break;
    case 0xf:
        sVar12 = ov83_02247CF0();
        if (sVar12 == 1) {
            PlaySE(0x5dc);
            ov83_02244C9C(param_1);
            ((BattleCastleOpponentApp *)param_1)->subState = 6;
        }
        break;
    case 0x10:
        sVar12 = ov83_02247CF0();
        if (sVar12 == 1) {
            PlaySE(0x5dc);
            ov83_02245094(param_1 + 0xc0);
            ov83_02244BEC(param_1);
            ((BattleCastleOpponentApp *)param_1)->subState = 0;
        }
        break;
    case 0x11:
        if ((((((u32 *)&gSystem)[0x12] & 0x20) == 0) && ((((u32 *)&gSystem)[0x12] & 0x10) == 0)) && (sVar12 = ov83_02247CF0(), sVar12 == 1)) {
            PlaySE(0x5dc);
            ov83_022459A0(param_1);
            ov83_02244C58(param_1);
            ((BattleCastleOpponentApp *)param_1)->subState = 2;
        }
        break;
    case 0x12:
        if ((((((u32 *)&gSystem)[0x12] & 0x20) == 0) && ((((u32 *)&gSystem)[0x12] & 0x10) == 0)) && (sVar12 = ov83_02247CF0(), sVar12 == 1)) {
            PlaySE(0x5dc);
            ov83_022459A0(param_1);
            ov83_02244C58(param_1);
            ((BattleCastleOpponentApp *)param_1)->subState = 2;
        }
    }
    return 0;
}
