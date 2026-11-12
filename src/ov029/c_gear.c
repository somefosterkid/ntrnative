// Black's code at ov029 0x021f1f80-0x021f31a8. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/c_gear.h"
#include "field/c_gear_sw.h"
#include "field/field_sound.h"
#include "field/intrude_work.h"
#include "gfl/arc.h"
#include "gfl/button_man.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/palanm.h"
#include "gfl/random.h"
#include "gfl/bg_sys.h"
#include "save/c_gear_savedata.h"
#include "system/wipe.h"
#include "gfl/sound.h"
#include "save/cgear_picture_save.h"
#include "save/save_control.h"
#include "field/event_irc.h"
#include "field/event_gsync.h"
#include "field/field_menu.h"
#include "field/event_research_radar.h"
#include "field/event_cgear_shutdown.h"
#include "field/ov021_021942c8.h"
#include "app/cg_help.h"
#include "gfl/overlay.h"
#include "nitro/rtc.h"
#include "gfl/rtc_cache.h"


void func_ov029_021f2d50(CGearSw *work);
void func_ov029_021f2dc8(CGearSw *work);
void func_ov029_021f1f80(CGearSw *work);
void func_ov029_021f2e5c(CGearSwFade *fade);
void func_ov029_021f251c(CGearSw *work, CGearSettings *settings, FieldSubscreen *param_3, GameSystem *gsys, s32 param_5);

// The table func_ov029_021f2e8c reads (0x18 bytes in the ROM)
extern const u32 data_ov029_021f3224[];


u32 func_ov029_021f30ec(CGearSw *work);

void func_ov029_021f21f8(CGearSw *work);
void func_ov029_021f3190(void *param_1, ArcTool *param_2, s32 param_3, u32 param_4);
void func_ov029_021f20ac(CGearSw *work);
void func_ov029_021f2e68(CGearSwFade *fade, u32 mask);
BOOL func_ov029_021f2e8c(CGearSwFade *fade);


GameEvent *func_ov029_021f2eec(CGearSw *work, s32 param_1);
void func_ov029_021f3144(CGearSw *work, u32 param_2);
GameEvent *func_ov010_0215fbf4(GameSystem *, Field *, void *, s32);
u32 func_ov029_021f2bf0(CGearSw *work);
void func_ov029_021f2e10(CGearSw *work, CGearSwFade *fade);
void func_ov029_021f2d84(CGearSw *param_1);


void func_ov029_021f1f80(CGearSw *work) {
    RTCTime time;
    ClActor *actor;
    u32 value;

    RTC_GetCachedTime(&time);
    func_0204b3dc(work->actors[0], FALSE);
    actor = work->actors[1];
    value = time.hour / 10;
    if (func_0204b918(actor) != value) {
        func_0204b908(actor, value);
    }
    if (value == 0) {
        func_0204b3dc(actor, FALSE);
    } else {
        func_0204b3dc(actor, TRUE);
    }
    actor = work->actors[2];
    value = time.hour % 10;
    if (func_0204b918(actor) != value) {
        func_0204b908(actor, value);
    }
    actor = work->actors[3];
    value = time.second & 1;
    if (func_0204b918(actor) != value) {
        func_0204b908(actor, value);
    }
    actor = work->actors[4];
    value = time.minute / 10;
    if (func_0204b918(actor) != value) {
        func_0204b908(actor, value);
    }
    actor = work->actors[5];
    value = time.minute % 10;
    if (func_0204b918(actor) != value) {
        func_0204b908(actor, value);
    }
    actor = work->actors[6];
    if (OS_IsRunOnTwl()) {
        value = func_ov029_021f1f70();
        if (func_0204b918(actor) != value) {
            func_0204b908(actor, value);
        }
        return;
    }
    if (func_0203568c() == 5) {
        value = 1;
    } else {
        value = 0;
    }
    if (func_0204b918(actor) != value) {
        func_0204b908(actor, value);
    }
}

void func_ov029_021f20ac(CGearSw *work) {
    int i;
    int col;
    int row;
    ClActorPos pos;

    for (i = 0; i < LAMP_COUNT; i++) {
        if (func_ov029_021f133c(work, i + 1, &col, &row)) {
            col *= 8;
            row *= 8;
            pos.x = col + 0x10;
            pos.y = row + 0xf;
            func_0204b404(work->lampActors[i], &pos, 0);
            col += 0x20;
            func_0204b800(work->lampActors[i], col / 8);
        } else {
            pos.x = -0x40;
            pos.y = 0;
            func_0204b404(work->lampActors[i], &pos, 0);
        }
    }
}

void func_ov029_021f2130(CGearSw *param_1) {
    func_02051c88(param_1->buttonMan);
    func_ov029_021f2d50(param_1);
    func_ov029_021f2dc8(param_1);
    func_ov029_021f1f80(param_1);
    if (func_ov029_021f2da8(param_1) == 0) {
        if (param_1->beacons != 0) {
            func_ov029_021f05d0(param_1);
            func_ov029_021f0728(param_1);
            func_ov029_021f1cd8(param_1);
            return;
        }
    }
    else {
        param_1->touchAction = 0;
    }
}

void func_ov029_021f2180(CGearSw *param_1) {
    BOOL BVar1;

    if (param_1->callback != NULL) {
        param_1->callback(param_1->callbackWork);
    }
    param_1->callback = NULL;
    param_1->callbackWork = NULL;
    BVar1 = func_ov029_021f0d48(param_1);
    if (BVar1 != 0) {
        func_ov029_021f2130(param_1);
    }
}

void func_ov029_021f21b4(CGearSw *param_1) {
    BOOL BVar1;

    BVar1 = func_ov029_021f1060(param_1);
    if (BVar1 == 0) {
        if (param_1->callback != NULL) {
            param_1->callback(param_1->callbackWork);
        }
        param_1->callback = NULL;
        param_1->callbackWork = NULL;
        return;
    }
    func_ov029_021f0300(param_1);
    func_ov029_021efef8(param_1, func_ov029_021f2180, 0xf92);
}

void func_ov029_021f21f8(CGearSw *work) {
    switch (work->seq) {
    case 0:
        func_ov029_021f1010(work);
        func_ov029_021f0370(work);
        func_ov029_021f1a64(work, FALSE);
        work->seq++;
        return;
    case 1:
        if (func_ov029_021f0d48(work)) {
            func_ov029_021f1b50(work);
            func_ov029_021f1b30(work, TRUE);
            work->seq++;
            return;
        }
        break;
    case 2:
        if (func_ov029_021f1be4(work)) {
            func_ov029_021f0f70(work);
            work->counter = 0;
            work->seq++;
            return;
        }
        break;
    case 3:
        if (func_ov029_021f2bf0(work)) {
            func_ov029_021f0ff0(work);
            work->seq = 7;
            return;
        }
        if (work->counter < 8) {
            work->counter++;
            return;
        }
        func_ov029_021f1b30(work, FALSE);
        func_ov029_021f1bbc(work);
        func_ov029_021f0e70(work);
        work->seq++;
        work->counter = 0;
        return;
    case 4:
        if (func_ov029_021f2bf0(work)) {
            work->seq = 7;
            return;
        }
        if (func_ov029_021f11b4(work) == 1) {
            work->counter = 0;
            work->seq++;
            return;
        }
        break;
    case 5:
        if (func_ov029_021f2bf0(work)) {
            work->seq = 7;
            return;
        }
        work->counter++;
        if (work->counter >= 8) {
            func_ov029_021f122c(work);
            work->counter = 0;
            work->seq++;
            return;
        }
        break;
    case 6:
        work->counter++;
        if (work->counter >= 8) {
            func_ov029_021f1a64(work, TRUE);
            func_ov029_021f1a44(work, FALSE);
            func_ov029_021efef8(work, func_ov029_021f21b4, 0x100b);
            return;
        }
        break;
    case 7:
        func_ov029_021f1bbc(work);
        func_ov029_021f0e70(work);
        func_ov029_021f0aa0(work);
        func_ov029_021f122c(work);
        work->counter = 8;
        work->seq = 6;
        break;
    }
}


void func_ov029_021f2370(CGearSw *work) {
    if (work->counter == 0) {
        func_ov029_021f0e70(work);
        func_ov029_021f1a64(work, 1);
        func_ov029_021f1a44(work, 0);
        func_ov029_021f1bbc(work);
        func_ov029_021f1010(work);
        func_ov029_021f0370(work);
    }
    work->counter++;
    if (work->counter >= 0x10) {
        work->counter = 0;
        func_ov029_021efef8(work, func_ov029_021f21b4, 0x1041);
    }
}

u32 func_ov029_021f23cc(CGearSw *work, GameSystem *gsys) {
    s32 i;
    u32 charBlock;
    u32 result = 0;
    u16 *scr;
    CGearPictureSave *picture;
    SaveControl *save;

    picture = GFL_HeapAllocate(0x70, 0x3000, FALSE, "c_gear.c", 0x104e);
    save = GameData_GetSaveControl(GSYS_GetGameData(gsys));
    if (func_020072cc(save, 4, 0x70, picture, 0x3000) == 1) {
        u16 crc = getCRC16(picture, 0x2600);
        u16 savedCrc = func_0200910c(func_02009210(save));

        charBlock = (u16)work->charBlock;
        if (crc == savedCrc && func_0200c6f0(picture)) {
            result = 1;
            for (i = 0; i < 0xf; i++) {
                GFL_BGSysLoadChar(6, picture->chars + i * 0x220, 0x220, charBlock + (i + 5) * 0x20);
            }
            GFL_BGSysUploadStdPalette(6, picture->chars + 0x1fe0, 0x20, 0x140);
            GFL_BGSysBufferScrDefault(5, picture->unk2000, 0x600);
            scr = GFL_BGSysIsScrHeapExists(5);
            for (i = 0; i < 0x300; i++) {
                scr[i] += (u16)charBlock;
            }
            GFL_BGSysLoadScr(5);
        }
    }
    freeIntermediateSaveExtraBlksAfterLoad2(save, 4);
    GFL_HeapFree(picture);
    return result;
}



void func_ov029_021f24c0(CGearSw *param_1) {
    func_ov029_021f0ff0(param_1);
    if (param_1->tcbMgr != NULL) {
        GFL_TCBMgrUpdate(param_1->tcbMgr);
    }
    func_ov029_021f0e70(param_1);
    func_ov029_021f1010(param_1);
    if (param_1->tcbMgr != NULL) {
        GFL_TCBMgrUpdate(param_1->tcbMgr);
    }
    param_1->unk_451 = 1;
}

void func_ov029_021f24fc(CGearSw *work) {
    work->unk_451 = 2;
}

void func_ov029_021f2508(TCB *tcb, CGearSw *work) {
    if (work->fade != NULL) {
        func_02021d50(work->fade);
    }
}

void func_ov029_021f251c(CGearSw *work, CGearSettings *settings, FieldSubscreen *param_3, GameSystem *gsys, s32 param_5) {
    TCB *tcb;
    GameCommSys *commSys;
    void *buffer;
    u32 unk = 0;

    work->heapId = 0x70;
    work->settings = settings;
    work->unk_18 = param_3;
    work->gsys = gsys;
    work->unk_454 = 1;
    work->beacons = func_0201214c(gsys);
    work->unk_45B = 1;
    work->touchAction = 0;
    work->unk_218 = 0;
    work->arc = GFL_ArcSysCreateFileHandle(0x1e, 0x70);
    buffer = GFL_HeapAllocate(0x70, GFL_TCBMgrCalcAllocSize(4), TRUE, "c_gear.c", 0x10b8);
    work->unk_20c = buffer;
    work->tcbMgr = GFL_TCBMgrCreate(4, buffer);
    if (param_5 != 0) {
        func_ov029_021efef8(work, func_ov029_021f21f8, 0x10bc);
    } else {
        func_ov029_021f0298(work);
        func_ov029_021efef8(work, func_ov029_021f2180, 0x10bf);
    }
    func_ov029_021f1624(work);
    func_ov029_021f1de8(work, param_5);
    if (func_02009100(work->settings)) {
        unk = func_ov029_021f23cc(work, gsys);
    }
    func_ov029_021f15fc(work, unk);
    func_ov029_021f0a0c(work);
    work->fade = func_02021518(0x70);
    func_02021ee4(work->fade, 1);
    func_0202155c(work->fade, 3, 0x1c0, 0x70);
    func_0202155c(work->fade, 1, 0x1c0, 0x70);
    func_020216d4(work->fade, 3, 0, 0x1c0);
    func_020216d4(work->fade, 1, 0, 0x1c0);
    func_ov029_021f0db4(work, work->beacons);
    func_02021d50(work->fade);
    tcb = GFL_VBlankTCBAdd((TCBFunc)func_ov029_021f2508, work, 1);
    work->unk_208 = tcb;
    func_02035010((UICallback)func_ov029_021f24c0, work);
    func_02035040((UICallback)func_ov029_021f24fc, work);
    func_ov029_021f2e10(work, &work->unk_21C);
    func_ov029_021f1cd8(work);
    func_ov029_021f1f80(work);
    if (param_5 == 0) {
        u32 flag = 1;
        if (GSYS_GetEventRunningFlag(gsys)) {
            flag = 0;
        }
        func_ov029_021f3144(work, flag);
        func_ov029_021f2d84(work);
    } else {
        func_ov029_021f21f8(work);
    }
    if (work->tcbMgr != NULL) {
        GFL_TCBMgrUpdate(work->tcbMgr);
    }
    commSys = GSYS_GetGameCommSystem(gsys);
    if (commSys != NULL && func_ov010_021585e0(commSys) == 2) {
        func_ov010_02158614(commSys);
    }
}


void func_ov029_021f26e0(CGearSw *work) {
    GameCommSys *commSys;
    u32 result;
    GameData *gameData;
    FieldSound *fieldSound;

    commSys = GSYS_GetGameCommSystem(work->gsys);
    if (commSys != NULL && (result = func_ov010_021585e0(commSys), result == 2)) {
        func_ov010_02158614(commSys);
    }
    func_ov029_021f2e5c(&work->unk_21C);
    func_02035010(NULL, NULL);
    func_02035040(NULL, NULL);
    func_0203d8ac();
    func_0203d34c(0, work->heapId);
    func_020215a0(work->fade, 3);
    func_020215a0(work->fade, 1);
    func_02021540(work->fade);
    func_02030da4(work->tcbMgr);
    GFL_HeapFree(work->unk_20c);
    GFL_TCBRemove(work->unk_208);
    if (work->unk_218 != 5) {
        gameData = GSYS_GetGameData(work->gsys);
        fieldSound = GameData_GetFieldSoundSystem(gameData);
        func_02028a70(fieldSound);
    }
    GFL_ArcToolFree(work->arc);
    func_ov029_021f1f24(work);
    reg_G2S_DB_BLDCNT = 0;
    MI_CpuFill8(work, 0, sizeof(CGearSw));
}

CGearSw *func_ov029_021f279c(CGearSettings *param_1, FieldSubscreen *param_2, GameSystem *param_3, s32 param_4) {
    CGearSw *pCVar1;

    pCVar1 = GFL_HeapAllocate(0x70, sizeof(CGearSw), 1, "c_gear.c", 0x1147);
    func_ov029_021f251c(pCVar1, param_1, param_2, param_3, param_4);
    return pCVar1;
}

CGearSw *func_ov029_021f27d8(CGearSettings *param_1, FieldSubscreen *param_2, GameSystem *param_3, CGearSwCallback callback, void *callbackWork, s32 param_6) {
    CGearSw *pCVar1;
    pCVar1 = func_ov029_021f279c(param_1, param_2, param_3, param_6);
    pCVar1->callback = callback;
    pCVar1->callbackWork = callbackWork;
    if (param_6 != 0) {
        func_ov029_021efef8(pCVar1, func_ov029_021f21f8, 0x1160);
        pCVar1->unk_45B = 0;
    } else {
        if (pCVar1->callback != NULL) {
            pCVar1->callback(pCVar1->callbackWork);
        }
        pCVar1->callback = NULL;
        pCVar1->callbackWork = NULL;
    }
    func_ov029_021f3190(param_1, pCVar1->arc, GFL_RandomLC(10), 0x70);
    func_ov029_021f20ac(pCVar1);
    return pCVar1;
}

void func_ov029_021f2848(CGearSw *work, u32 param_2) {
    u32 mode;
    GameCommSys *commSys;
    GameData *gameData;
    WifiList *wifiList;
    FieldSound *fieldSound;
    u32 flags;
    u32 bit;

    if (work->unk_451 == 1) {
        return;
    }
    func_ov029_021f3144(work, param_2);
    if (func_ov029_021f2da8(work) == 0) {
        commSys = GSYS_GetGameCommSystem(work->gsys);
        mode = func_ov010_02158630(commSys);
        if (mode == 3) {
            if (work->unk_463 == 0) {
                func_ov029_021f06b8(work, TRUE, FALSE);
                work->unk_463 = 1;
            }
        } else if (work->unk_463 == 1) {
            func_ov029_021f06b8(work, FALSE, FALSE);
            work->unk_463 = 0;
        }
        if (mode == 0) {
            gameData = GSYS_GetGameData(work->gsys);
            wifiList = func_0201283c(gameData);
            flags = func_0201058c(wifiList);
            gameData = GSYS_GetGameData(work->gsys);
            fieldSound = GameData_GetFieldSoundSystem(gameData);
            bit = flags & 8;
            if (bit) {
                if (work->unk_462 == 0) {
                    work->unk_462 = 1;
                    func_ov029_021f06b8(work, TRUE, TRUE);
                }
                func_02028a64(fieldSound);
            } else if (bit == 0) {
                if (work->unk_462 != 0) {
                    func_02028a70(fieldSound);
                    work->unk_462 = 0;
                    func_ov029_021f06b8(work, FALSE, FALSE);
                }
            }
        }
    }
    if (work->unk_451 == 2) {
        work->unk_451 = 0;
        if (func_ov029_021f2da8(work) == 0) {
            func_ov029_021efef8(work, func_ov029_021f2370, 0x11c5);
        } else {
            func_ov029_021f1bbc(work);
            func_ov029_021f1a64(work, TRUE);
            func_ov029_021f13fc(work);
            func_ov029_021f0eb0(work, func_ov029_021f2da8(work));
            func_ov029_021f0298(work);
            func_ov029_021efef8(work, func_ov029_021f2180, 0x11d4);
        }
    }
    if (work->state != NULL) {
        work->state(work);
    }
    if (work->tcbMgr != NULL) {
        GFL_TCBMgrUpdate(work->tcbMgr);
    }
}


void func_ov029_021f2980(void *param_1) {
}

void func_ov029_021f2984(void *param_1) {
    func_ov029_021f26e0(param_1);
    GFL_HeapFree(param_1);
}

BOOL func_ov029_021f2994(CGearSw *work) {
    CGearSettings *settings = work->settings;
    FieldSubscreen *subscreen = work->unk_18;
    GameSystem *gsys = work->gsys;

    if (work->state == func_ov029_021f2180) {
        func_0202223c(1, 0);
        func_ov029_021f26e0(work);
        func_ov029_021f251c(work, settings, subscreen, gsys, 0);
        killBrightnessEitherEngine(1);
        return TRUE;
    }
    return FALSE;
}

void func_ov029_021f3098(CGearSw *work);
void func_ov029_021f3118(CGearSw *param_1);
void func_ov029_021f2cd4(CGearSw *work);

GameEvent *func_ov029_021f29d8(CGearSw *work, BOOL enabled, FieldSubscreen *subscreen) {
    GameEvent *event = NULL;
    Field *field;

    GSYS_GetGameCommSystem(work->gsys);
    field = GSYS_GetField(work->gsys);
    if (!enabled || field == NULL) {
        return NULL;
    }
    switch (work->touchAction) {
    case 1:
    case 6:
        if (!func_ov029_021f05b4(work)) {
            GFL_SndSEPlay(0x54e);
            event = func_ov029_021f2eec(work, work->touchAction);
        } else {
            GFL_SndSEPlay(0x7e3);
            func_ov029_021f3098(work);
            func_ov029_021efef8(work, func_ov029_021f3118, 0x1238);
        }
        break;
    case 5:
        GFL_SndSEPlay(0x54e);
        event = func_ov029_021f2eec(work, work->touchAction);
        break;
    case 12:
    case 13:
        GFL_SndSEPlay(0x628);
        event = func_ov029_021f2eec(work, work->touchAction);
        break;
    case 7:
    case 11:
        if (!func_ov029_021f05b4(work)) {
            GFL_SndSEPlay(0x628);
            event = func_ov029_021f2eec(work, work->touchAction);
        } else {
            GFL_SndSEPlay(0x7e3);
        }
        break;
    }
    if (event != NULL) {
        work->unk_218 = work->touchAction;
        func_ov029_021efef8(work, func_ov029_021f2cd4, 0x1256);
    }
    work->touchAction = 0;
    return event;
}



u32 func_ov029_021f2ab8(CGearSw *work) {
    if (work == NULL) {
        return 0;
    }
    if (work->unk_218 == 5) {
        return 1;
    }
    return 0;
}

void func_ov029_021f2ad4(CGearSw *param_1) {
    if (param_1 != (CGearSw *)0x0) {
        param_1->unk_462 = 0;
        func_ov029_021f06b8(param_1, 0, 0);
    }
}

void func_ov029_021f2aec(CGearSw *param_1, CGearSettings *param_2, s32 param_3, s32 param_4, u32 param_5) {
    u32 result;
    s32 row;
    s32 col;
    row = 0;
    do {
        col = 0;
        do {
            result = func_ov029_021f2b9c(param_1, col, row);
            if (param_5 == result) {
                result = func_ov029_021f2bb8(param_1, col, row);
                if (result != 0) {
                    func_020091ac(param_2->unk_00, col, row, param_5, 0, 1);
                    func_ov029_021f1290(param_1, col, row, param_5);
                } else {
                    result = func_ov029_021f2bd4(param_1, col, row, param_5);
                    if (result != 0) {
                        func_020091ac(param_2->unk_00, col, row, param_5, 0, 0);
                    }
                }
            }
            col = col + 1;
        } while (col < 9);
        row = row + 1;
    } while (row < 4);
    func_020091ac(param_2->unk_00, param_3, param_4, param_5, 1, 0);
    func_ov029_021f1290(param_1, param_3, param_4, param_5);
    GFL_BGSysLoadScr(4);
    func_ov029_021f20ac(param_1);
}

int func_ov029_021f2b9c(CGearSw *work, int param_2, int param_3) {
    if (work->beacons != 0) {
        return func_02009110(work->settings->unk_00, param_2, param_3);
    }
    return 0;
}

BOOL func_ov029_021f2bb8(CGearSw *work, int param_2, int param_3) {
    if (work->beacons != 0) {
        return func_02009134(work->settings->unk_00, param_2, param_3);
    }
    return 0;
}

BOOL func_ov029_021f2bd4(CGearSw *work, int param_2, int param_3, int param_4) {
    if (work->beacons != 0) {
        return func_02009160(work->settings->unk_00, param_2, param_3, param_4);
    }
    return 0;
}

u32 func_ov029_021f2bf0(CGearSw *work) {
    BOOL BVar1;

    if ((work->unk_45B != 0) && (BVar1 = func_02035c60(), BVar1 != 0)) {
        return 1;
    }
    return 0;
}

BOOL func_ov029_021f2c10(CGearSw *work) {
    switch (work->seq) {
    case 0:
        func_0204b92c(work->actors[work->unk_416], 1);
        func_0204b954(work->actors[work->unk_416], 0x2000);
        work->seq++;
        break;
    case 1:
        if (!func_0204b98c(work->actors[work->unk_416])) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

u32 func_ov029_021f2c78(CGearSw *work) {
    switch (work->seq) {
    case 0:
        switch (work->unk_416) {
        case 3:
            func_ov029_021f2e68(&work->unk_21C, 0x1c0);
            break;
        case 4:
            func_ov029_021f2e68(&work->unk_21C, 0x200);
            break;
        }
        work->seq++;
        break;
    case 1:
        if (func_ov029_021f2e8c(&work->unk_21C)) {
            return 1;
        }
        break;
    }
    return 0;
}



void func_ov029_021f2cd4(CGearSw *work) {
}

void func_ov029_021f2cd8(CGearSw *work) {
    if (work->unk_45C == 0) {
        work->unk_45C = 1;
        work->unk_45E = 1;
        work->unk_45F = 0;
    }
}

void func_ov029_021f2cf4(CGearSw *work) {
    if (work->unk_45D == 0) {
        work->unk_45D = 1;
        work->unk_45E = 1;
        work->unk_45F = 0;
    }
}

void func_ov029_021f2d10(CGearSw *work) {
    if (work->unk_45D != 0) {
        work->unk_45D = 0;
        work->unk_45E = 1;
        work->unk_45F = 0;
    }
}

void func_ov029_021f2d30(CGearSw *work) {
    if (work->unk_45C == 1) {
        work->unk_45C = 0;
        work->unk_45E = 1;
        work->unk_45F = 0;
    }
}

void func_ov029_021f2d50(CGearSw *work) {
    if (work->unk_45E != 0) {
        work->unk_45F++;
        if (work->unk_45F >= 8) {
            func_ov029_021f0eb0(work, func_ov029_021f2da8(work));
            work->unk_45E = 0;
        }
    }
}


void func_ov029_021f2d84(CGearSw *param_1) {
    BOOL on;

    if (param_1->unk_45E != 0) {
        on = func_ov029_021f2da8(param_1);
        func_ov029_021f0eb0(param_1, on);
        param_1->unk_45E = 0;
    }
}

BOOL func_ov029_021f2da8(CGearSw *work) {
    if (work->unk_45C != 0) {
        return 1;
    }
    if (work->unk_45D != 0) {
        return 1;
    }
    return 0;
}

void func_ov029_021f2dc8(CGearSw *work) {
    if (func_ov029_021f2da8(work) == 0) {
        if (work->alertChanged != 0) {
            work->alertWait++;
            if (work->alertWait >= 8) {
                if (func_ov029_021f05b4(work)) {
                    func_ov029_021f0f28(work, TRUE);
                } else {
                    func_ov029_021f0f28(work, FALSE);
                }
                work->alertChanged = 0;
            }
        }
    }
}


void func_ov029_021f2e10(CGearSw *work, CGearSwFade *fade) {
    ClActor *actor;
    ClActorSetup setup;

    setup.x = 0;
    setup.y = 0;
    setup.sequence = 0x25;
    setup.priority = 0;
    setup.bgPriority = 2;
    actor = func_0204b294(work->unit, work->chars, work->palette, work->cellAnims, &setup, 0, work->heapId);
    fade->actor = actor;
    func_0204b3dc(actor, 0);
    fade->fade = work->fade;
    fade->tcbMgr = work->tcbMgr;
}

void func_ov029_021f2e5c(CGearSwFade *fade) {
    func_0204b3b4(fade->actor);
}

void func_ov029_021f2e68(CGearSwFade *fade, u32 mask) {
    func_0204b9a0(fade->actor);
    func_0204b92c(fade->actor, 1);
    func_0204b954(fade->actor, 0x2000);
    fade->active = 1;
    fade->paletteMask = mask;
}

BOOL func_ov029_021f2e8c(CGearSwFade *fade) {
    if (fade->active == 0) {
        return TRUE;
    }
    if (!func_0204b98c(fade->actor)) {
        fade->active = 0;
        return TRUE;
    }
    func_0202173c(fade->fade, 8, fade->paletteMask, -0x78, 0, data_ov029_021f3224[func_0204b918(fade->actor)], 0, fade->tcbMgr);
    return FALSE;
}

// The data of the event a touched button starts (pokebw2's CGearSwEvent)
typedef struct {
    CGearSw *work;
    u32 action;
} CGearSwEvent;

GameEventReturnCode func_ov029_021f2f60(GameEvent *event, u32 *state, void *data);


GameEvent *func_ov029_021f2eec(CGearSw *work, s32 param_1) {
    GameEvent *event;
    CGearSwEvent *data;

    event = GameEvent_Create(work->gsys, NULL, func_ov029_021f2f60, sizeof(CGearSwEvent));
    data = GameEvent_GetData(event);
    data->work = work;
    data->action = param_1;
    switch (param_1) {
    case 1:
    case 5:
    case 6:
        func_ov029_021f0194(data->work);
        break;
    case 11:
        work->unk_416 = 4;
        break;
    case 7:
        work->unk_416 = 3;
        break;
    case 12:
        work->unk_416 = 8;
        break;
    case 13:
        work->unk_416 = 10;
        break;
    }
    return event;
}

extern u32 OVERLAY_0_ID[];

extern u32 OVERLAY_0_ID[];
extern u32 OVERLAY_124_ID[];

GameEventReturnCode func_ov029_021f2f60(GameEvent *event, u32 *state, void *data) {
    CGearSwEvent *args = data;
    CGearSw *work = args->work;
    Field *field;
    GameEvent *next;
    u32 result = 0;
    CGHelpParam *param;

    field = GSYS_GetField(work->gsys);
    switch (args->action) {
    case 1:
    case 5:
    case 6:
        result = func_ov029_021f009c(args->work);
        break;
    case 7:
    case 11:
        result = func_ov029_021f2c78(args->work);
        break;
    case 12:
    case 13:
        result = func_ov029_021f2c10(args->work);
        break;
    }
    if (result == 0) {
        return 0;
    }
    switch (args->action) {
    case 6:
        next = GameEvent_CreateOverlayDelegate(work->gsys, OVERLAY_ID(0), func_ov000_02154668, field);
        break;
    case 1:
        next = CallIRC(work->gsys, field, NULL, TRUE);
        break;
    case 5:
        next = func_ov010_0215fbf4(work->gsys, field, NULL, TRUE);
        break;
    case 11:
        next = func_ov010_02167044(work->gsys, field);
        break;
    case 7:
        next = EventFieldMenuReturn_Create(work->gsys, field, 4);
        break;
    case 12:
        param = GFL_HeapAllocate(4, sizeof(CGHelpParam), TRUE, "c_gear.c", 0x14b7);
        param->playerInfo = GetGameDataPlayerInfo(GSYS_GetGameData(work->gsys));
        param->gsys = work->gsys;
        next = func_02014808(work->gsys, field, OVERLAY_ID(124), &data_ov124_021d4d40, param, NULL, param);
        break;
    case 13:
        next = EventCGearShutdown_Create(work->gsys);
        break;
    }
    GameEvent_Replace(event, next);
    return 0;
}




void func_ov029_021f3098(CGearSw *work) {
    work->touchType = func_ov029_021f2b9c(work, work->touchCol, work->touchRow);
    work->touchTimer = 0;
    func_ov029_021eff58(work, work->touchCol, work->touchRow);
}


void func_ov029_021f30c8(CGearSw *work) {
    ClActor *actor;
    s32 i;

    for (i = 0; i < TOUCH_EFFECT_COUNT; i++) {
        actor = work->touchEffects[i].actor;
        if (actor != NULL) {
            func_0204b3b4(actor);
            work->touchEffects[i].actor = NULL;
        }
    }
}

u32 func_ov029_021f30ec(CGearSw *work) {
    u32 result = 1;
    s32 i;
    for (i = 0; i < 20; i++) {
        ClActor *actor = work->touchEffects[i].actor;
        if (actor != NULL && func_0204b98c(actor)) {
            result = 0;
            break;
        }
    }
    return result;
}


void func_ov029_021f3118(CGearSw *param_1) {
    func_ov029_021f2130(param_1);
    if (func_ov029_021f30ec(param_1) != 0) {
        func_ov029_021f30c8(param_1);
        func_ov029_021efef8(param_1, func_ov029_021f2180, 0x1514);
    }
}

void func_ov029_021f3144(CGearSw *work, u32 param_2) {
    BOOL BVar1;

    BVar1 = func_0203cd98();
    if ((BVar1 == 0) && (work->beacons != 0)) {
        func_ov029_021f2cf4(work);
    }
    else {
        func_ov029_021f2d10(work);
    }
    if (work->unk_454 != param_2) {
        if (param_2 == 0) {
            func_ov029_021f2cd8(work);
        }
        else {
            func_ov029_021f2d30(work);
        }
        work->unk_454 = (u8)param_2;
    }
}

void func_ov029_021f3190(void *param_1, ArcTool *param_2, s32 param_3, u32 param_4) {
    GFL_ArcToolRead(param_2, param_3 + 0x19, func_020091fc(param_1));
}

