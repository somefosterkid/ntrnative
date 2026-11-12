// The field's touch screen: the table of its screens (the C-Gear, the field menu, the report, ...), changing between
// them behind a wipe, and the glue that loads each screen's overlay and calls it. The name is the ROM's own, from
// GFL_HeapAllocate's file argument. pokebw2's src/ov036/field_subscreen.c (GPL-3.0,
// https://github.com/fuddlesworth/pokebw2) is Black 2's twin, with more screens; this is Black's code only, under
// Black's names

#include "types.h"
#include "field/beacon_view.h"
#include "field/c_gear.h"
#include "field/c_gear_power_onoff.h"
#include "field/c_gear_sw.h"
#include "field/dowsing.h"
#include "field/field.h"
#include "field/field_disp_control.h"
#include "field/field_menu.h"
#include "field/field_msgbg.h"
#include "field/no_gear.h"
#include "field/ov018_0217da54.h"
#include "field/player_state.h"
#include "field/report.h"
#include "field/subscreen.h"
#include "field/union_main.h"
#include "field/union_subdisp.h"
#include "field/zone.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/heap.h"
#include "save/c_gear_savedata.h"
#include "save/save_control.h"
#include "struct_decls.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/infowin.h"
#include "system/wipe.h"

// The field menu's BG (func_ov021_021a11a8): the first of the file's .rodata, in front of the screens' table
const BGSetup data_ov021_021d8708 = {
    .screenSize = 0x800,
    .resolution = 1,
    .screenBase = 0x0b,
    .charSize = 0x5800,
};

void func_ov021_021a0fe8(FieldSubscreen *subscreen, u32 previousScreenId);
void func_ov021_021a1050(FieldSubscreen *subscreen, u32 previousScreenId);
void func_ov021_021a10c0(FieldSubscreen *subscreen);
void func_ov021_021a10d4(FieldSubscreen *subscreen);
void func_ov021_021a10e0(FieldSubscreen *subscreen, BOOL active);
GameEvent *func_ov021_021a1138(FieldSubscreen *subscreen, BOOL enabled);
void func_ov021_021a1144(FieldSubscreen *subscreen, u32 previousScreenId);
void func_ov021_021a117c(FieldSubscreen *subscreen);
void func_ov021_021a1190(FieldSubscreen *subscreen);
void func_ov021_021a119c(FieldSubscreen *subscreen, BOOL active);
void func_ov021_021a11a8(FieldSubscreen *subscreen, u32 previousScreenId);
void func_ov021_021a1244(FieldSubscreen *subscreen);
void func_ov021_021a1288(FieldSubscreen *subscreen, BOOL active);
void func_ov021_021a1298(FieldSubscreen *subscreen, BOOL active);
void func_ov021_021a136c(FieldSubscreen *subscreen, u32 previousScreenId);
void func_ov021_021a1388(FieldSubscreen *subscreen);
void func_ov021_021a139c(FieldSubscreen *subscreen, BOOL active);
void func_ov021_021a13c0(FieldSubscreen *subscreen, BOOL active);
void func_ov021_021a13e4(FieldSubscreen *subscreen, u32 previousScreenId);
void func_ov021_021a13f8(FieldSubscreen *subscreen);
void func_ov021_021a1404(FieldSubscreen *subscreen, BOOL active);
void func_ov021_021a1410(FieldSubscreen *subscreen, BOOL active);
GameEvent *func_ov021_021a141c(FieldSubscreen *subscreen, BOOL enabled);
void func_ov021_021a1428(FieldSubscreen *subscreen, u32 previousScreenId);
void func_ov021_021a1448(FieldSubscreen *subscreen);
void func_ov021_021a145c(FieldSubscreen *subscreen, BOOL active);
void func_ov021_021a1468(FieldSubscreen *subscreen, BOOL active);
GameEvent *func_ov021_021a1474(FieldSubscreen *subscreen, BOOL enabled);
void func_ov021_021a1480(FieldSubscreen *subscreen, u32 previousScreenId);
void func_ov021_021a14a0(FieldSubscreen *subscreen);
void func_ov021_021a14b4(FieldSubscreen *subscreen, BOOL active);
void func_ov021_021a14c0(FieldSubscreen *subscreen, BOOL active);
void func_ov021_021a14cc(FieldSubscreen *subscreen, u32 previousScreenId);
void func_ov021_021a14ec(FieldSubscreen *subscreen);
void func_ov021_021a1500(FieldSubscreen *subscreen, BOOL active);
void func_ov021_021a150c(FieldSubscreen *subscreen, BOOL active);
void func_ov021_021a1560(FieldSubscreen *subscreen, u32 previousScreenId);
void func_ov021_021a1584(FieldSubscreen *subscreen, BOOL active);
void func_ov021_021a1590(FieldSubscreen *subscreen);

const FieldSubscreenState data_ov021_021d8728[10] = {
    { 0, func_ov021_021a0fe8, func_ov021_021a10e0, NULL, func_ov021_021a1138, func_ov021_021a10c0, func_ov021_021a10d4 },
    { 1, func_ov021_021a11a8, func_ov021_021a1288, func_ov021_021a1298, NULL, func_ov021_021a1244, NULL },
    { 2, func_ov021_021a136c, func_ov021_021a139c, func_ov021_021a13c0, NULL, func_ov021_021a1388, NULL },
    { 3, func_ov021_021a13e4, func_ov021_021a1404, func_ov021_021a1410, func_ov021_021a141c, func_ov021_021a13f8, NULL },
    { 4, func_ov021_021a1428, func_ov021_021a145c, func_ov021_021a1468, func_ov021_021a1474, func_ov021_021a1448, NULL },
    { 5, func_ov021_021a1144, func_ov021_021a119c, NULL, NULL, func_ov021_021a117c, func_ov021_021a1190 },
    { 6, func_ov021_021a1480, func_ov021_021a14b4, func_ov021_021a14c0, NULL, func_ov021_021a14a0, NULL },
    { 7, func_ov021_021a14cc, func_ov021_021a1500, func_ov021_021a150c, NULL, func_ov021_021a14ec, NULL },
    { 8, func_ov021_021a1050, func_ov021_021a10e0, NULL, func_ov021_021a1138, func_ov021_021a10c0, func_ov021_021a10d4 },
    { 9, func_ov021_021a1560, func_ov021_021a1584, NULL, NULL, func_ov021_021a1590, NULL },
};

u32 Field_IsEventRunning(Field *field);

FieldSubscreen *func_ov021_021a0cf4(u32 param_1, Field *param_2, u32 param_3) {
    FieldSubscreen *sub;
    u32 id;

    sub = GFL_HeapAllocate((u16) param_1, sizeof(FieldSubscreen), 1, "field_subscreen.c", 0x13CU);
    sub->unk_8 = 0;
    sub->unk_c = (u16) param_1;
    sub->unk_20 = NULL;
    sub->field = param_2;
    id = FieldSubscreen_GetIDForChange(sub, param_3);
    sub->unk_0 = id;
    sub->unk_4 = id;
    data_ov021_021d8728[id].init(sub, 0xB);
    return sub;
}

u8 func_ov021_021a0d44(FieldSubscreen *param_1) {
    u32 id;

    id = param_1->unk_0;
    data_ov021_021d8728[id].terminate(param_1);
    GFL_HeapFree(param_1);
    return id;
}

void func_ov021_021a0d68(FieldSubscreen *param_1) {
    BOOL BVar1;
    u32 result;

    switch (param_1->unk_8) {
    case 0:
        result = Field_IsEventRunning(param_1->field);
        data_ov021_021d8728[param_1->unk_0].update(param_1, result == 0);
        return;
    case 1:
        GFL_WipeSet(4, 0, 0, 0, 1, 1, param_1->unk_c);
        param_1->unk_8 = 2;
        return;
    case 2:
        BVar1 = GFL_WipeIsFinished();
        if (BVar1 == 1) {
            data_ov021_021d8728[param_1->unk_0].terminate(param_1);
            data_ov021_021d8728[param_1->unk_4].init(param_1, param_1->unk_0);
            param_1->unk_0 = param_1->unk_4;
            param_1->unk_8 = 4;
            return;
        }
        break;
    case 4:
        GFL_WipeSet(4, 1, 1, 0, 1, 1, param_1->unk_c);
        param_1->unk_8 = 5;
        return;
    case 5:
        BVar1 = GFL_WipeIsFinished();
        if (BVar1 == 1) {
            param_1->unk_8 = 0;
        }
        result = Field_IsEventRunning(param_1->field);
        data_ov021_021d8728[param_1->unk_0].update(param_1, result == 0);
    }
}

void func_ov021_021a0e58(FieldSubscreen *param_1) {
    u32 id;

    switch (param_1->unk_8) {
    case 0:
        if (data_ov021_021d8728[param_1->unk_0].updateAltFrame != NULL) {
            id = Field_IsEventRunning(param_1->field);
            data_ov021_021d8728[param_1->unk_0].updateAltFrame(param_1, id == 0);
        }
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        break;
    }
}

GameEvent *func_ov021_021a0eb0(FieldSubscreen *param_1, u32 enabled) {
    if (param_1->unk_8 == 0 && data_ov021_021d8728[param_1->unk_0].checkEvent != NULL) {
        return data_ov021_021d8728[param_1->unk_0].checkEvent(param_1, enabled);
    }
    return NULL;
}

BOOL FieldSubscreen_IsReady(FieldSubscreen *subscreen) {
    if (!subscreen->unk_8) {
        return TRUE;
    }
    return FALSE;
}

u32 FieldSubscreen_GetIDForChange(FieldSubscreen *subscreen, u32 heapId) {
    u16 zoneId;
    GameSystem *gsys;
    GameData *gameData;
    BOOL BVar1;
    SaveControl *save;
    void *block;

    if (heapId == 0) {
        gsys = Field_GetGameSystem(subscreen->field);
        gameData = GSYS_GetGameData(gsys);
        zoneId = Field_GetPlayerStateZoneID(subscreen->field);
        BVar1 = IsZoneRoyalUnova(zoneId);
        if ((BVar1 != 0) || (BVar1 = IsZoneLEQ76(zoneId), BVar1 != 0)) {
            return 5;
        }
        save = GameData_GetSaveControl(gameData);
        block = func_02009210(save);
        BVar1 = func_020090e0(block);
        if (BVar1 == 0) {
            return 5;
        }
    }
    return heapId;
}

void FieldSubscreen_ReqChange(FieldSubscreen *subscreen, u32 mode) {
    subscreen->unk_4 = FieldSubscreen_GetIDForChange(subscreen, mode);
    subscreen->unk_8 = 1;
}

void func_ov021_021a0f40(FieldSubscreen *param_1, u32 param_2) {
    u32 id;

    id = FieldSubscreen_GetIDForChange(param_1, param_2);
    data_ov021_021d8728[param_1->unk_0].terminate(param_1);
    param_1->unk_4 = id;
    data_ov021_021d8728[id].init(param_1, param_1->unk_0);
    param_1->unk_0 = param_1->unk_4;
    param_1->unk_8 = 0;
}

void func_ov021_021a0f7c(FieldSubscreen *param_1, u32 param_2, void (*param_3)(void *work), void *param_4) {
    param_1->unk_18 = param_3;
    param_1->unk_1c = param_4;
    param_1->unk_4 = FieldSubscreen_GetIDForChange(param_1, param_2);
    param_1->unk_8 = 1;
}

u32 FieldSubscreen_GetScreenID(FieldSubscreen *subscreen) {
    return subscreen->unk_0;
}

u32 FieldSubscreen_GetReturnSubscreen(FieldSubscreen *subscreen) {
    if (subscreen != NULL) {
        return subscreen->unk_10;
    }
    return 0;
}

void FieldSubscreen_SaveReturnSubscreen(FieldSubscreen *subscreen, u32 screenId) {
    if (subscreen != NULL) {
        subscreen->unk_10 = screenId;
    }
}

void func_ov021_021a0fa8(FieldSubscreen *subscreen) {
    FieldSubscreen_SaveReturnSubscreen(subscreen, 0);
}

void func_ov021_021a0fb4(FieldSubscreen *subscreen, HeapID heapId) {
    FieldDispControl *control;

    control = FieldG2D_GetDispControl(subscreen->field);
    FieldDispControl_ReqSetAllA(control, 1, 0, 0, 0, 0);
    setAlphaBlend_wrapper(0);
    GFL_BGSysClearScr(1);
    func_ov021_021902a4(1, heapId);
}

void func_ov021_021a0fe8(FieldSubscreen *param_1, u32 previousScreenId) {
    GameSystem *gsys;
    void *saveControl;
    u32 result;
    u32 r7;

    gsys = Field_GetGameSystem(param_1->field);
    saveControl = func_02009210(GameData_GetSaveControl(GSYS_GetGameData(gsys)));
    GFL_OvlLoad(OVERLAY_ID(29));
    r7 = func_02009208(saveControl);
    result = func_0201214c(gsys);
    param_1->unk_20 = func_ov029_021f279c(saveControl, param_1, gsys, result == 1 && r7 == 0);
    func_02009200(saveControl, result);
    if (func_02012204(gsys) != 0) {
        func_02012210(gsys);
    }
}

void func_ov021_021a1050(FieldSubscreen *param_1, u32 previousScreenId) {
    GameSystem *gsys;
    void *saveControl;
    int result;

    gsys = Field_GetGameSystem(param_1->field);
    saveControl = GameData_GetSaveControl(GSYS_GetGameData(gsys));
    GFL_OvlLoad(OVERLAY_ID(29));
    result = func_0201214c(gsys);
    saveControl = func_02009210(saveControl);
    param_1->unk_20 = func_ov029_021f27d8(saveControl, param_1, Field_GetGameSystem(param_1->field), param_1->unk_18, param_1->unk_1c, result);
    func_02009200(saveControl, result);
    if (func_02012204(gsys) != 0) {
        func_02012210(gsys);
    }
    param_1->unk_0 = 0;
    param_1->unk_4 = 0;
}

void func_ov021_021a10c0(FieldSubscreen *param_1) {
    func_ov029_021f2984(param_1->unk_20);
    GFL_OvlUnload(OVERLAY_ID(29));
}

void func_ov021_021a10d4(FieldSubscreen *param_1) {
    func_ov029_021f2980(param_1->unk_20);
}

void func_ov021_021a10e0(FieldSubscreen *param_1, BOOL param_2) {
    GameSystem *gsys;
    void *saveControl;

    if (param_2 != 0) {
        gsys = Field_GetGameSystem(param_1->field);
        saveControl = func_02009210(GameData_GetSaveControl(GSYS_GetGameData(gsys)));
        if (func_02012204(gsys) != 0) {
            if (func_0201214c(gsys) != 0) {
                func_02012210(gsys);
            } else if (func_ov029_021f2994(param_1->unk_20) != 0) {
                func_02009200(saveControl, 0);
                func_02012210(gsys);
            }
        }
    }
    func_ov029_021f2848(param_1->unk_20, param_2);
}

GameEvent *func_ov021_021a1138(FieldSubscreen *param_1, BOOL param_2) {
    return func_ov029_021f29d8(param_1->unk_20, param_2, param_1);
}

void func_ov021_021a1144(FieldSubscreen *param_1, u32 previousScreenId) {
    void *saveControl;

    GFL_OvlLoad(OVERLAY_ID(30));
    saveControl = func_02009210(GameData_GetSaveControl(GSYS_GetGameData(Field_GetGameSystem(param_1->field))));
    param_1->unk_20 = func_ov030_021f00c8(saveControl, param_1, Field_GetGameSystem(param_1->field));
}

void func_ov021_021a117c(FieldSubscreen *param_1) {
    func_ov030_021f0118(param_1->unk_20);
    GFL_OvlUnload(OVERLAY_ID(30));
}

void func_ov021_021a1190(FieldSubscreen *param_1) {
    func_ov030_021f0114(param_1->unk_20);
}

void func_ov021_021a119c(FieldSubscreen *param_1, BOOL active) {
    func_ov030_021f00f8(param_1->unk_20);
}

void func_ov021_021a11a8(FieldSubscreen *param_1, u32 param_2) {
    GameSystem *gsys;
    u16 zoneId;
    EventWork *eventWork;
    GameData *gameData;
    BOOL flag;

    gsys = Field_GetGameSystem(param_1->field);
    zoneId = PlayerState_GetZoneID(GSYS_GetPlayerState(gsys));
    gameData = GSYS_GetGameData(gsys);
    eventWork = GameData_GetEventWork(gameData);
    flag = FALSE;
    if (param_2 == 0 || param_2 - 2 <= 4) {
        flag = TRUE;
    }
    GFL_BGSysCreateBG(4, &data_ov021_021d8708, 0);
    GFL_BGSysSetBGEnabled(4, TRUE);
    GFL_BGSysClearBG(4);
    GFL_BGSysFillScr(4, 0);
    GFL_BGSysSetBGEnabledB(0x10, TRUE);
    param_1->unk_20 = func_ov021_021a836c(0x70, param_1->unk_c, param_1, param_1->field, flag);
    if (func_ov021_021a8b18(gameData, eventWork, zoneId) != 1) {
        func_02024a20(4, 0xe, gameData, param_1->unk_c);
    }
    func_ov021_021a8afc(param_1->unk_c, flag);
}

void func_ov021_021a1244(FieldSubscreen *param_1) {
    u16 zoneId;
    GameSystem *gsys;
    PlayerState *playerState;
    GameData *gameData;
    EventWork *eventWork;

    gsys = Field_GetGameSystem(param_1->field);
    playerState = GSYS_GetPlayerState(gsys);
    zoneId = PlayerState_GetZoneID(playerState);
    gameData = GSYS_GetGameData(gsys);
    eventWork = GameData_GetEventWork(gameData);
    if (func_ov021_021a8b18(gameData, eventWork, (u32)zoneId) != 1) {
        func_02024dcc();
    }
    func_ov021_021a84bc(param_1->unk_20);
    GFL_BGSysReleaseBG(4);
}

void func_ov021_021a1288(FieldSubscreen *param_1, BOOL active) {
    func_ov021_021a85b4(param_1->unk_20);
    func_02024abc();
}

void func_ov021_021a1298(FieldSubscreen *param_1, BOOL active) {
    func_ov021_021a8840(param_1->unk_20);
}

u32 func_ov021_021a12a4(FieldSubscreen *subscreen) {
    u32 screenID;

    screenID = FieldSubscreen_GetScreenID(subscreen);
    if (screenID != 1) {
        return 0;
    }
    screenID = func_ov021_021a8e10(subscreen->unk_20);
    return screenID;
}

void func_ov021_021a12bc(FieldSubscreen *subscreen, u32 heapId) {
    if (FieldSubscreen_GetScreenID(subscreen) == 1) {
        func_ov021_021a8e20(subscreen->unk_20, heapId);
    }
}

void func_ov021_021a12d4(FieldSubscreen *subscreen, HeapID heapId) {
    ArcTool *arc;
    FieldDispControl *pFVar1;

    arc = GFL_ArcSysCreateFileHandle(0x47, heapId);
    GFL_G2DIOLoadArcNCLR(arc, 0, 0, 0x140, 0x140, 0x20, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 3, 1, 100, 0, 0, heapId);
    GFL_G2DIOLoadNSCRSync(arc, 0xf, 1, 0, 100, 0, 0, heapId);
    GFL_ArcToolFree(arc);
    pFVar1 = FieldG2D_GetDispControl(subscreen->field);
    FieldDispControl_ReqSetAllA(pFVar1, 1, 0, 0, 0, 0xfffffffa);
    pFVar1 = FieldG2D_GetDispControl(subscreen->field);
    FieldDispControl_ReqSetBGEnabled(pFVar1, 1, 1);
}

void *func_ov021_021a1358(FieldSubscreen *param_1) {
    if (param_1->unk_0 == 0 || param_1->unk_0 == 8) {
        return param_1->unk_20;
    }
    return NULL;
}

void func_ov021_021a136c(FieldSubscreen *param_1, u32 previousScreenId) {
    GFL_OvlLoad(OVERLAY_ID(31));
    param_1->unk_20 = func_ov031_021efec0(Field_GetGameSystem(param_1->field));
}

void func_ov021_021a1388(FieldSubscreen *param_1) {
    func_ov031_021effe8(param_1->unk_20);
    GFL_OvlUnload(OVERLAY_ID(31));
}

void func_ov021_021a139c(FieldSubscreen *param_1, BOOL param_2) {
    GameSystem *gsys = Field_GetGameSystem(param_1->field);

    if (param_2 == TRUE && !func_ov012_0217394c(gsys)) {
        param_2 = 0;
    }
    func_ov031_021f0054(param_1->unk_20, param_2);
}

void func_ov021_021a13c0(FieldSubscreen *param_1, BOOL param_2) {
    GameSystem *gsys = Field_GetGameSystem(param_1->field);

    if (param_2 == TRUE && !func_ov012_0217394c(gsys)) {
        param_2 = 0;
    }
    func_ov031_021f00d4(param_1->unk_20, param_2);
}

void func_ov021_021a13e4(FieldSubscreen *param_1, u32 previousScreenId) {
    GameSystem *pGVar1;

    pGVar1 = Field_GetGameSystem(param_1->field);
    param_1->unk_20 = func_ov018_0217cfe4(pGVar1);
}

void func_ov021_021a13f8(FieldSubscreen *param_1) {
    func_ov018_0217d160(param_1->unk_20);
}

void func_ov021_021a1404(FieldSubscreen *param_1, BOOL param_2) {
    func_ov018_0217d1c0(param_1->unk_20, param_2);
}

void func_ov021_021a1410(FieldSubscreen *param_1, BOOL active) {
    func_ov018_0217d3ac(param_1->unk_20);
}

GameEvent *func_ov021_021a141c(FieldSubscreen *param_1, BOOL param_2) {
    return func_ov018_0217d4dc(param_1->unk_20, param_2, param_1);
}

void func_ov021_021a1428(FieldSubscreen *param_1, u32 previousScreenId) {
    GFL_OvlLoad(OVERLAY_ID(32));
    param_1->unk_20 = func_ov032_021f2234(Field_GetGameSystem(param_1->field), param_1);
}

void func_ov021_021a1448(FieldSubscreen *param_1) {
    func_ov032_021f22fc(param_1->unk_20);
    GFL_OvlUnload(OVERLAY_ID(32));
}

void func_ov021_021a145c(FieldSubscreen *param_1, BOOL active) {
    func_ov032_021f2344(param_1->unk_20, active);
}

void func_ov021_021a1468(FieldSubscreen *param_1, BOOL active) {
    func_ov032_021f23f8(param_1->unk_20);
}

GameEvent *func_ov021_021a1474(FieldSubscreen *param_1, BOOL active) {
    return func_ov032_021f2404(param_1->unk_20, active);
}

void func_ov021_021a1480(FieldSubscreen *param_1, u32 previousScreenId) {
    GFL_OvlLoad(OVERLAY_ID(33));
    param_1->unk_20 = func_ov033_021f000c(0x70, param_1->unk_c, param_1, param_1->field);
}

void func_ov021_021a14a0(FieldSubscreen *param_1) {
    func_ov033_021f0124(param_1->unk_20);
    GFL_OvlUnload(OVERLAY_ID(33));
}

void func_ov021_021a14b4(FieldSubscreen *param_1, BOOL active) {
    func_ov033_021f0174(param_1->unk_20, active);
}

void func_ov021_021a14c0(FieldSubscreen *param_1, BOOL active) {
    func_ov033_021f05a4(param_1->unk_20, active);
}

void func_ov021_021a14cc(FieldSubscreen *param_1, u32 previousScreenId) {
    GFL_OvlLoad(OVERLAY_ID(34));
    param_1->unk_20 = func_ov034_021efec0(Field_GetGameSystem(param_1->field), param_1->unk_c);
}

void func_ov021_021a14ec(FieldSubscreen *param_1) {
    func_ov034_021eff34(param_1->unk_20);
    GFL_OvlUnload(OVERLAY_ID(34));
}

void func_ov021_021a1500(FieldSubscreen *param_1, BOOL active) {
    func_ov034_021eff5c(param_1->unk_20);
}

void func_ov021_021a150c(FieldSubscreen *param_1, BOOL active) {
    func_ov034_021effb4(param_1->unk_20);
}

BOOL func_ov021_021a1518(FieldSubscreen *subscreen) {
    return func_ov034_021f0044(subscreen->unk_20);
}

void func_ov021_021a1524(FieldSubscreen *subscreen) {
    func_ov034_021f0058(subscreen->unk_20);
}

void func_ov021_021a1530(FieldSubscreen *subscreen) {
    func_ov034_021f00bc(subscreen->unk_20);
}

BOOL func_ov021_021a153c(FieldSubscreen *subscreen) {
    return func_ov034_021f00f0(subscreen->unk_20);
}

void func_ov021_021a1548(FieldSubscreen *subscreen) {
    func_ov034_021f0108(subscreen->unk_20);
}

BOOL func_ov021_021a1554(FieldSubscreen *subscreen) {
    BOOL BVar1;

    BVar1 = func_ov034_021f009c(subscreen->unk_20);
    return BVar1;
}

void func_ov021_021a1560(FieldSubscreen *param_1, u32 previousScreenId) {
    GFL_OvlLoad(OVERLAY_ID(35));
    param_1->unk_20 = func_ov035_021efec0(param_1, Field_GetGameSystem(param_1->field), 0x70);
}

void func_ov021_021a1584(FieldSubscreen *param_1, BOOL active) {
    func_ov035_021eff3c(param_1->unk_20, active);
}

void func_ov021_021a1590(FieldSubscreen *param_1) {
    func_ov035_021eff2c(param_1->unk_20);
    GFL_OvlUnload(OVERLAY_ID(35));
}

