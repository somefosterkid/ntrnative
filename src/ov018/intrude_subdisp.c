// Black's code at ov018 0x0217ce08-0x0217fb5c. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_script.h"
#include "field/intrude_field.h"
#include "field/fld_vreq.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcbl.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "struct_decls.h"
#include "system/bmp_oam.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "system/game_comm.h"
#include "field/intrude_comm.h"
#include "system/game_data.h"
#include "field/event_mapchange.h"
#include "field/ov018_0217da54.h"
#include "gfl/tcb.h"
#include "gfl/wm_icon.h"
#include "field/player_state.h"
#include "field/zone.h"
#include "gfl/sound.h"
#include "field/intrude_work.h"
#include "gfl/net.h"
#include "field/beacon_view.h"
#include "field/field.h"
#include "gfl/bmp.h"
#include "gfl/key.h"
#include "save/save_control.h"
#include "nitro/math.h"
#include "field/ov018_0217c458.h"
#include "gfl/touchpanel.h"
#include "field/subscreen.h"
#include "system/palanm.h"
#include "field/field_palace.h"
#include "field/field_msgbg.h"

typedef struct {
    Field *field;
    FieldSubscreen *subscreen;
    s32 frame;
} Ov018FadeWork;

GameEventReturnCode func_ov018_0217cf0c(GameEvent *event, u32 *state, void *data);

void func_ov018_0217daa8(Ov018Work *work);
void func_ov018_0217db4c(Ov018Work *work, ArcTool *arc);
void func_ov018_0217dbac(Ov018Work *work, ArcTool *arc);
void func_ov018_0217dc14(Ov018Work *work, ArcTool *arc);
void func_ov018_0217dc9c(Ov018Work *work, ArcTool *arc);
void func_ov018_0217dcfc(Ov018Work *work, ArcTool *arc);
void func_ov018_0217dd48(Ov018Work *work, ArcTool *arc);
u32 func_ov018_0217f5a0(IntrudeMissionWork *work);
u32 func_ov018_0217f284(Ov018Work *work);

void func_ov018_0217f118(struct Ov018Work *param_1, s32 param_2);
void func_ov018_0217e2e0(Ov018Work *work);
void func_ov018_0217f214(void);
BOOL func_ov018_0217ec08(u16 param_1, u16 param_2, u16 param_3);

// Where each player's mark sits against its zone's: x and y
static const s8 data_ov018_021836a4[3][2] = { { 0, -4 }, { -4, -4 }, { 4, -4 } };

GameEvent *func_ov018_0217ce08(GameSystem *gsys, Field *field, u16 zoneId, u32 param) {
    VecFx32 pos;
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    GameData *gameData = GSYS_GetGameData(gsys);
    func_ov010_02158344(commSys);
    if (func_ov018_0217bed4(zoneId, &pos) == 0) {
        return NULL;
    }
    if (GameData_IsForceSeasonSync(gameData) == 0 && zoneId == 0x117) {
        return EventEntralinkWarpIn_Create(gsys, zoneId, &pos, param);
    }
    return EventMapChangeEnding_Create(gsys, zoneId, &pos);
}

GameEvent *func_ov018_0217ce6c(GameSystem *gsys, Field *field, u32 idx) {
    VecFx32 pos;
    IntrudeComm *comm = func_ov010_02158344(GSYS_GetGameCommSystem(gsys));
    if (comm == NULL || (comm->unk_8FB & (1 << idx)) == 0) {
        return NULL;
    }
    IntrudeFieldUnkEntry *entry = func_ov018_0217bea8(comm->unk_08C[idx].zoneId);
    if (entry == NULL) {
        return NULL;
    }
    u16 dir = entry->unk_00_20;
    pos = comm->unk_08C[idx].status.pos;
    pos.z += 0x20000;
    return EventMapChangeWarp_CreateGrid(gsys, field, dir, &pos, 0, FALSE);
}


GameEvent *func_ov018_0217cee8(GameSystem *param_1, Field *field, FieldSubscreen *subscreen) {
    GameEvent *event;
    Ov018FadeWork *work;

    event = GameEvent_Create(param_1, NULL, func_ov018_0217cf0c, sizeof(Ov018FadeWork));
    work = GameEvent_GetData(event);
    work->field = field;
    work->subscreen = subscreen;
    return event;
}

GameEventReturnCode func_ov018_0217cf0c(GameEvent *event, u32 *state, void *data) {
    Ov018FadeWork *work = data;
    FieldDispControl *control;
    switch (*state) {
    case 0:
        if (++work->frame > 0xf) {
            control = FieldG2D_GetDispControl(work->field);
            func_ov021_021d324c(control, -16);
            (*state)++;
        }
        break;
    case 1:
        func_ov021_021a0f40(work->subscreen, 0);
        (*state)++;
        break;
    case 2:
        control = FieldG2D_GetDispControl(work->field);
        func_ov021_021d324c(control, 0);
        (*state)++;
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


BOOL func_ov018_0217cf68(VM *vm, FieldScriptEnv *param_2) {
    GameSystem *gsys;
    Field *pFVar1;

    gsys = FieldScriptEnv_GetGameSystem(param_2);
    pFVar1 = GSYS_GetField(gsys);
    GSYS_GetGameCommSystem(gsys);
    func_ov018_0217bf84(pFVar1);
    return 0;
}

BOOL func_ov018_0217cf88(VM *vm, FieldScriptEnv *param_2) {
    GameSystem *gsys;
    Field *pFVar2;
    GameCommSys *comm;
    IntrudeComm *comm2;
    gsys = FieldScriptEnv_GetGameSystem(param_2);
    GSYS_GetGameData(gsys);
    pFVar2 = GSYS_GetField(gsys);
    comm = GSYS_GetGameCommSystem(gsys);
    comm2 = NULL;
    if (GameCommSys_BootCheck(comm) == 2) {
        comm2 = func_02026474(comm);
    }
    if (comm2 != NULL) {
        func_ov018_0217bbcc(pFVar2, gsys, comm2);
        func_ov018_0217bbd8(pFVar2, gsys, comm2);
    } else {
        func_ov018_0217bc94(pFVar2, 1);
    }
    func_ov018_0217c194(pFVar2);
    return 0;
}

Ov018Work *func_ov018_0217cfe4(GameSystem *gsys) {
    GameCommSys *commSys = GSYS_GetGameCommSystem(gsys);
    Ov018Work *work;
    IntrudeComm *comm;
    ArcTool *arc;
    PassPowerLevel *level;
    u16 zoneId;
    s32 i;

    work = GFL_HeapAllocate(0x70, sizeof(Ov018Work), 1, "intrude_subdisp.c", 0x240);
    work->gsys = gsys;
    work->unk_105 = 0xff;
    work->unk_11b = 0xff;
    work->unk_169 = 4;
    work->unk_16a = 4;
    work->unk_10a = -1;
    for (i = 0; i < 3; i++) {
        work->slots[i].zone = 0x1ab;
    }
    comm = NULL;
    if (GameCommSys_BootCheck(commSys) == 2) {
        comm = func_02026474(commSys);
    }
    if (comm != NULL && comm->unk_8F8_7 == 1) {
        comm = NULL;
    }
    func_ov018_0217efcc(work, comm);
    work->unk_11e = work->sub.unk_02;
    arc = GFL_ArcSysCreateFileHandle(0x24, 0x15);
    func_ov018_0217d624(work);
    func_ov018_0217d720(work, arc);
    func_ov018_0217d890(work);
    func_ov018_0217d918(work, arc);
    func_ov018_0217da54(work, arc);
    GFL_ArcToolFree(arc);
    func_0203d34c(0, 0x15);
    func_02037174();
    G2S_SetWndOBJInsidePlane(0x1f, FALSE);
    G2S_SetWndOutsidePlane(0x1f, TRUE);
    GXS_SetVisibleWnd(GX_WNDMASK_OW);
    work->unk_34 = GFL_VBlankTCBAdd(func_ov018_0217d5ac, work, 10);
    zoneId = PlayerState_GetZoneID(GSYS_GetPlayerState(work->gsys));
    level = func_ov018_0217ebd8(work);
    func_ov018_0217e638(work, zoneId);
    func_ov018_0217de68(work, level);
    func_ov018_0217e014(work, level);
    func_ov018_0217e0f4(work, func_ov010_02158344(commSys), level);
    func_ov018_0217e3d0(work, level, zoneId);
    func_ov018_0217e4d0(work, level);
    func_ov018_0217e524(work, level);
    return work;
}


void func_ov018_0217d160(Ov018Work *work) {
    GFL_TCBRemove(work->unk_34);
    MIi_CpuClear16(0, (void *)HW_DB_BG_PLTT, 2);
    func_ov018_0217de44(work);
    func_ov018_0217da90(work);
    func_ov018_0217da18(work);
    func_ov018_0217d904(work);
    func_ov018_0217d870(work);
    func_ov018_0217d6c8(work);
    GFL_HeapFree(work);
    func_02037190();
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    reg_G2S_DB_BLDCNT = 0;
}

void func_ov018_0217d1c0(Ov018Work *work, s32 param_2) {
    GameCommSys *commSys = GSYS_GetGameCommSystem(work->gsys);
    GameData *gameData = GSYS_GetGameData(work->gsys);
    IntrudeComm *comm = func_ov010_02158344(commSys);
    PlayerState *playerState = GSYS_GetPlayerState(work->gsys);
    u16 zoneId = PlayerState_GetZoneID(playerState);
    PrintQueue *queue;
    u32 netId;
    s32 i;

    func_0201058c(func_0201283c(gameData));
    func_02037174();
    if (comm == NULL || (comm != NULL && comm->unk_8F8_7 == 0)) {
        func_ov018_0217f008(work, comm);
    }
    if (work->unk_11e < work->sub.unk_02) {
        if (GameData_IsForceSeasonSync(gameData) == TRUE) {
            GFL_SndSEPlay(0x699);
        }
        work->unk_11e = work->sub.unk_02;
    }
    GFL_TCBExMgrUpdate(work->unk_20);
    func_0201c4e4(work->unk_28);
    queue = work->unk_28;
    if (work->win44.flushPending != 0) {
        if (func_0201c6c4(queue, BmpWin_GetBitmap(work->win44.window)) == FALSE) {
            BmpWin_FlushChar(work->win44.window);
            work->win44.flushPending = 0;
        }
    }
    queue = work->unk_28;
    if (work->win4c.flushPending != 0) {
        if (func_0201c6c4(queue, BmpWin_GetBitmap(work->win4c.window)) == FALSE) {
            BmpWin_FlushChar(work->win4c.window);
            work->win4c.flushPending = 0;
        }
    }
    for (i = 0; i < 2; i++) {
        if (func_0201c6c4(work->unk_28, work->unk_b4[i]) == FALSE) {
            func_020257d0(work->unk_c0[i]);
        }
    }
    if (param_2 == 0) {
        if (func_ov018_0217f0fc(work) == 1 || work->unk_116 == 1) {
            G2S_BlendNone();
        } else {
            G2S_SetBlendBrightness(0x3f, -8);
            return;
        }
    } else {
        G2S_BlendNone();
    }
    if (param_2 == 1) {
        func_ov018_0217ec7c(comm, work, zoneId);
    }
    if (comm != NULL && work->unk_106 == 1) {
        netId = func_ov010_0215841c(gameData);
        if (func_0203d1c0(netId) == FALSE) {
            work->unk_106 = 0;
            return;
        }
        switch (work->unk_107) {
        case 0:
            if (func_ov011_021746ac(comm, netId) == TRUE) {
                work->unk_107++;
            }
            break;
        case 1:
            if (func_ov010_02158428(comm) == 1) {
                Field_GetSubscreen(GSYS_GetField(work->gsys));
                work->unk_110 = 0x126;
                work->unk_112 = 1;
                work->unk_106 = 0;
            }
            break;
        default:
            GFL_ASSERT(FALSE);
            break;
        }
    }
}


void func_ov018_0217d3ac(Ov018Work *work) {
    GameCommSys *commSys = GSYS_GetGameCommSystem(work->gsys);
    GameData *gameData = GSYS_GetGameData(work->gsys);
    IntrudeComm *comm = func_ov010_02158344(commSys);
    PlayerState *playerState = GSYS_GetPlayerState(work->gsys);
    u16 zoneId = PlayerState_GetZoneID(playerState);
    BOOL update = FALSE;
    PassPowerLevel *level = func_ov018_0217ebd8(work);

    if (comm != NULL && func_ov011_02175da8(&comm->unk_1AC) == FALSE) {
        update = TRUE;
    } else if (comm == NULL && GameData_IsForceSeasonSync(gameData) == TRUE && GameCommSys_BootCheck(commSys) != 2
               && func_0202648c(commSys) == 0 && IsZoneEntralinkHub(zoneId) == TRUE) {
        update = TRUE;
    }
    if (comm != NULL && comm->unk_8F8_7 == TRUE) {
        update = FALSE;
    }
    if (update == TRUE) {
        func_ov018_0217eeb4(work);
        func_ov018_0217ef44(work, comm);
        func_ov018_0217e638(work, zoneId);
        func_ov018_0217e714(work, comm, zoneId);
        func_ov018_0217de68(work, level);
        func_ov018_0217e014(work, level);
        func_ov018_0217e0f4(work, comm, level);
        func_ov018_0217e3d0(work, level, zoneId);
        func_ov018_0217e4d0(work, level);
        func_ov018_0217e524(work, level);
        func_ov018_0217f194(work);
    } else if (comm != NULL && func_ov011_02175da8(&comm->unk_1AC) == TRUE
               && func_ov011_02175b8c(&comm->unk_1AC) == TRUE && work->unk_16c == 0 && work->sub.unk_08 == 0) {
        func_ov018_0217e524(work, level);
        work->unk_16c = 1;
    }
}

GameEvent *func_ov018_0217d4dc(Ov018Work *work, s32 param_2, FieldSubscreen *subscreen) {
    GameCommSys *commSys = GSYS_GetGameCommSystem(work->gsys);
    GameData *gameData = GSYS_GetGameData(work->gsys);
    Field *field = GSYS_GetField(work->gsys);
    GameEvent *event = NULL;

    if (param_2 == 0 || field == NULL) {
        return NULL;
    }
    if (work->unk_168 == 1 || (GameData_IsForceSeasonSync(gameData) == 0 && func_ov010_02158344(commSys) == NULL)) {
        GFL_SndSEPlay(0x551);
        func_ov018_0217f174(work);
        return func_ov018_0217cee8(work->gsys, field, subscreen);
    }
    switch (work->unk_112) {
    case 1:
        GFL_SndSEPlay(0x737);
        func_ov018_0217f0c4(work);
        event = func_ov018_0217ce08(work->gsys, field, work->unk_110, 0);
        break;
    case 2:
        GFL_SndSEPlay(0x737);
        event = func_ov018_0217ce6c(work->gsys, field, func_ov010_02158378(commSys));
        break;
    }
    if (event != NULL) {
        work->unk_112 = 0;
        return event;
    }
    return NULL;
}




void func_ov018_0217d5ac(TCB *tcb, void *data) {
    Ov018Work *work = data;

    work->unk_11d = work->unk_11d ^ 1;
    if (work->unk_11d != 0) {
        if (work->unk_11c != 0) {
            DC_FlushRange(work->unk_130, 8);
            GXS_LoadOBJPltt(work->unk_130, 0x102, 8);
            work->unk_11c = 0;
        }
        if (work->unk_10f != 0) {
            DC_FlushRange(work->unk_d0, 0x200);
            GXS_LoadOBJ(work->unk_d0, work->unk_d4, 0x200);
            work->unk_10f = 0;
        }
    }
}

static const ClActSurfaceSetup data_ov018_021836b4 = { 0, 0, 0x100, 0xc0, 1, 2 };
void func_ov018_0217d624(Ov018Work *work) {
    work->unk_54 = func_0204b100(0x29, 5, 0x70);
    work->unk_58 = func_0204b034(&data_ov018_021836b4, 1, 0x70);
    func_0204b258(work->unk_54, work->unk_58);
    work->unk_b0 = func_02025538(0x70, work->unk_54);
    work->unk_20 = GFL_TCBExMgrCreate(0x70, 0x70, 4, 0x20);
    work->unk_1c = GFL_FontCreate(0x17, 0, 0, 0, 0x70);
    work->unk_24 = GFL_WordSetSystemCreateDefault(0x70);
    work->unk_28 = func_0201c440(0x70);
    work->unk_2c = GFL_MsgSysLoadData(0, 2, 0x2d, 0x70);
    work->unk_30 = GFL_MsgSysLoadData(0, 2, 0x42, 0x70);
    work->unk_38 = GFL_StrBufCreate(0x80, 0x70);
    work->unk_3c = GFL_StrBufCreate(0x80, 0x70);
    work->unk_40 = GFL_StrBufCreate(0x80, 0x70);
}


void func_ov018_0217d6c8(Ov018Work *work) {
    GFL_StrBufFree(work->unk_38);
    GFL_StrBufFree(work->unk_3c);
    GFL_StrBufFree(work->unk_40);
    GFL_MsgDataFree(work->unk_2c);
    GFL_MsgDataFree(work->unk_30);
    func_0201c6ec(work->unk_28);
    func_0201c4c0(work->unk_28);
    GFL_WordSetSystemFree(work->unk_24);
    GFL_FontFree(work->unk_1c);
    GFL_TCBExMgrFree(work->unk_20);
    func_02025588(work->unk_b0);
    func_0204b084(work->unk_58);
    func_0204b1cc(work->unk_54);
}

void func_ov018_0217d720(Ov018Work *work, ArcTool *arc) {
    NNSG2dScreenData *screen;
    void *ptr;
    u32 file;
    static const BGSetup data_ov018_0218370c[2] = {
        { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x00000, 0x7000,
          GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
        { 0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, 0x7000,
          GX_BG_EXTPLTT_01, 3, GX_BG_AREAOVER_XLU, FALSE },
    };

    file = 6;
    GFL_BGSysCreateBG(6, &data_ov018_0218370c[0], 0);
    GFL_BGSysCreateBG(7, &data_ov018_0218370c[1], 0);
    MIi_CpuClear32(0, (void *)HW_DB_BG_VRAM, 0x20000);
    GXS_SetVisibleWnd(GX_WNDMASK_NONE);
    reg_G2S_DB_BLDCNT = 0;
    G2S_SetBlendBrightness(0x3f, -8);
    GFL_BGSysLoadArcNCGRStatic(arc, 4, 7, 0, 0, TRUE, 0x15);
    loadBGScrToVramByFileNoReserveNegAlign(arc, 5, 7, 0, 0x800, TRUE, 0x15);
    if (work->sub.unk_02 <= 1) {
        work->unk_11f = 0;
    } else {
        file = 7;
        work->unk_11f = 1;
    }
    loadBGScrToVramByFileNoReserveNegAlign(arc, file, 6, 0, 0x800, TRUE, 0x15);
    func_ov018_0217ef70(work, work->sub.netId);
    ptr = GFL_G2DIOReadNSCRArc(arc, 8, TRUE, &screen, 0x15);
    MIi_CpuCopy16(screen->rawData, work->unk_138, 0x30);
    GFL_HeapFree(ptr);
    if (func_ov018_0217f1e8(work) == 1) {
        func_ov018_0217f240(work, 0);
    }
    GFL_G2DIOLoadArcNCLRDefault(arc, 9, 4, 0, 0x140, 0x15);
    GFL_BGSysLoadNCLRDefault(0x17, 5, 4, 0x1c0, 0x20, 0x15);
    GFL_BGSysSetBGEnabled(4, 0);
    GFL_BGSysSetBGEnabled(5, 0);
    GFL_BGSysSetBGEnabled(6, 1);
    GFL_BGSysSetBGEnabled(7, 1);
}


void func_ov018_0217d870(Ov018Work *work) {
    GFL_BGSysSetBGEnabled(6, 0);
    GFL_BGSysSetBGEnabled(7, 0);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysReleaseBG(7);
}

void func_ov018_0217d890(Ov018Work *work) {
    BmpWin *win;

    win = BmpWin_CreateDynamic(6, 0, 0, 0x20, 2, 0xe, 1);
    work->win44.window = win;
    work->win44.flushPending = 0;
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
    win = BmpWin_CreateDynamic(6, 1, 0x12, 0x1e, 4, 0xe, 1);
    work->win4c.window = win;
    work->win4c.flushPending = 0;
    BmpWin_FlushChar(win);
    BmpWin_FlushMap(win);
    GFL_BGSysLoadScr(BmpWin_GetBGIndex(win));
}


void func_ov018_0217d904(Ov018Work *work) {
    BmpWin_Free(work->win44.window);
    BmpWin_Free(work->win4c.window);
}

void func_ov018_0217d918(Ov018Work *work, ArcTool *arc) {
    ArcTool *handle;

    work->unk_08 = func_0204ac18(arc, 3, 1, 0, 0, 9, 0x15);
    work->unk_04 = func_0204a6c8(arc, 0, 1, 1, 0x15);
    work->unk_10 = func_0204af28(arc, 1, 2, 0x70);
    handle = GFL_ArcSysCreateFileHandle(0x17, 0x15);
    work->unk_0c = func_0204acec(handle, 5, 1, 0x1a0, 0x15);
    GFL_ArcToolFree(handle);
    work->unk_14 = func_0204a6c8(arc, 10, 1, 1, 0x15);
    work->unk_18 = func_0204af28(arc, 0xc, 0xd, 0x70);
    work->unk_c8 = GFL_G2DIOReadBGNCGRArc(arc, 10, 1, &work->unk_cc, 0x70);
    work->unk_d4 = func_0204abbc(work->unk_14, 2);
    work->unk_d0 = GFL_HeapAllocate(0x70, 0x200, 1, "intrude_subdisp.c", 0x495);
    MIi_CpuCopy16((u16 *)HW_DB_OBJ_PLTT + 7 * 16 + 1, work->unk_120, 8);
    MIi_CpuCopy16(work->unk_120, work->unk_130, 8);
    MIi_CpuCopy16((u16 *)HW_DB_OBJ_PLTT + 8 * 16 + 1, work->unk_128, 8);
}

void func_ov018_0217da18(Ov018Work *work) {
    GFL_HeapFree(work->unk_d0);
    GFL_HeapFree(work->unk_c8);
    func_0204ada4(work->unk_0c);
    func_0204ada4(work->unk_08);
    func_0204a8d4(work->unk_04);
    func_0204a8d4(work->unk_14);
    func_0204afd8(work->unk_10);
    func_0204afd8(work->unk_18);
}

void func_ov018_0217da54(Ov018Work *work, ArcTool *arc) {
    func_ov018_0217daa8(work);
    func_ov018_0217db4c(work, arc);
    func_ov018_0217dbac(work, arc);
    func_ov018_0217dc14(work, arc);
    func_ov018_0217dc9c(work, arc);
    func_ov018_0217dcfc(work, arc);
    func_ov018_0217dd48(work, arc);
}

void func_ov018_0217da90(Ov018Work *work) {
    s32 i;

    i = 0;
    do {
        func_0204b3b4(work->actors[i]);
        i = i + 1;
    } while (i < 21);
}



void func_ov018_0217daa8(Ov018Work *work) {
    ClActorSetup setup = { 0, 0, 7, 0x5a, 0x02 };
    s32 i;

    for (i = 0; i <= 7; i++) {
        setup.x = data_ov011_02176aa0[i].unk_04;
        setup.y = data_ov011_02176aa0[i].unk_05;
        work->actors[i] = func_0204b294(work->unk_54, work->unk_04, work->unk_08, work->unk_10, &setup, 0, 0x70);
        func_0204b3dc(work->actors[i], FALSE);
        func_0204b92c(work->actors[i], TRUE);
    }
    setup.sequence = 8;
    setup.x = 0x80;
    setup.y = 0x5c;
    work->actors[8] = func_0204b294(work->unk_54, work->unk_04, work->unk_08, work->unk_10, &setup, 0, 0x70);
    func_0204b3dc(work->actors[8], FALSE);
    func_0204b92c(work->actors[8], TRUE);
}



void func_ov018_0217db4c(Ov018Work *work, ArcTool *arc) {
    ClActorSetup setup = { 0, 0, 0, 0x32, 0x02 };
    s32 i;

    for (i = 9; i <= 11; i++) {
        work->actors[i] = func_0204b294(work->unk_54, work->unk_04, work->unk_08, work->unk_10, &setup, 0, 0x70);
        func_0204b6f4(work->actors[i], (u8)(i - 8), 1);
        func_0204b3dc(work->actors[i], FALSE);
    }
}

void func_ov018_0217dbac(Ov018Work *work, ArcTool *arc) {
    ClActorSetup setup = { 0, 0, 1, 0x14, 0x02 };
    s32 i;

    for (i = 12; i <= 14; i++) {
        work->actors[i] = func_0204b294(work->unk_54, work->unk_04, work->unk_08, work->unk_10, &setup, 0, 0x70);
        func_0204b6f4(work->actors[i], (u8)(i - 11), 1);
        func_0204b3dc(work->actors[i], FALSE);
        func_0204b92c(work->actors[i], TRUE);
    }
}



void func_ov018_0217dc14(Ov018Work *work, ArcTool *arc) {
    ClActorSetup setup = { 0, 0, 2, 0x13, 0x02 };
    GameData *gameData;
    PlayerInfo *info;

    gameData = GSYS_GetGameData(work->gsys);
    info = GetGameDataPlayerInfo(gameData);
    if (getTrainerGender(info) == 1) {
        setup.sequence = 3;
    }
    work->actors[15] = func_0204b294(work->unk_54, work->unk_04, work->unk_08, work->unk_10, &setup, 0, 0x70);
    func_0204b6f4(work->actors[15], (u8)(func_02012b58(gameData) + 1), 1);
    func_0204b3dc(work->actors[15], FALSE);
    func_0204b92c(work->actors[15], TRUE);
}


void func_ov018_0217dc9c(Ov018Work *work, ArcTool *arc) {
    ClActorSetup setup = { 0, 0, 0x0b, 0x1e, 0x02 };
    s32 i;

    for (i = 17; i <= 19; i++) {
        work->actors[i] = func_0204b294(work->unk_54, work->unk_04, work->unk_08, work->unk_10, &setup, 0, 0x70);
        func_0204b6f4(work->actors[i], (u8)(i - 16), 1);
        func_0204b3dc(work->actors[i], FALSE);
    }
}





void func_ov018_0217dcfc(Ov018Work *work, ArcTool *arc) {
    ClActorSetup setup = { 0xd0, 0xb0, 0, 0x0a, 0x02 };
    ClActor *actor;
    BOOL visible;

    visible = func_ov018_0217f1e8(work);
    actor = func_0204b294(work->unk_54, work->unk_14, work->unk_08, work->unk_18, &setup, 0, 0x70);
    work->actors[20] = actor;
    func_0204b3dc(actor, visible);
}

const ClActorSetup data_ov018_021836aa = { 0xe0, 0x80, 9, 5, 2 };

void func_ov018_0217dd48(Ov018Work *work, ArcTool *arc) {
    GameData *gameData;
    ClActor *actor;
    s32 i;

    gameData = GSYS_GetGameData(work->gsys);
    actor = func_0204b294(work->unk_54, work->unk_04, work->unk_08, work->unk_10, &data_ov018_021836aa, 0, 0x70);
    work->actors[16] = actor;
    func_0204b3dc(actor, 0);
    for (i = 0; i < 2; i++) {
        BmpOamActorSetup setup = {NULL, 0xd0, 0x78, 0, 0, 4, 2, 0, 1};
        StrBuf *strbuf;

        work->unk_b4[i] = GFL_BitmapCreate(6, 2, 0x20, 0x70);
        setup.bitmap = work->unk_b4[i];
        setup.palette = work->unk_0c;
        work->unk_c0[i] = func_020255a0(work->unk_b0, &setup);
        func_02025774(work->unk_c0[i], 0);
        strbuf = GFL_MsgDataLoadStrbufNew(work->unk_2c, i + 0x45);
        func_0201c724(work->unk_28, work->unk_b4[i], 0, 0, strbuf, work->unk_1c, 0x3c40);
        GFL_StrBufFree(strbuf);
    }
    if (work->unk_168 == 0 && GameData_IsForceSeasonSync(gameData) == FALSE) {
        func_0204b3dc(work->actors[16], 1);
        func_02025774(work->unk_c0[1], 1);
    }
}



void func_ov018_0217de44(Ov018Work *work) {
    s32 i;

    i = 0;
    do {
        func_0202570c(work->unk_c0[i]);
        GFL_BitmapFree(work->unk_b4[i]);
        i = i + 1;
    } while (i < 2);
}

void func_ov018_0217de68(Ov018Work *work, PassPowerLevel *level) {
    GameData *gameData = GSYS_GetGameData(work->gsys);
    s32 i;
    ClActor *actor;
    u32 palette;

    if (func_ov018_0217f284(work) == 1) {
        for (i = 0; i <= 7; i++) {
            func_0204b3dc(work->actors[i], FALSE);
        }
        func_0204b3dc(work->actors[8], FALSE);
    } else if (work->sub.netId == func_02012b58(gameData) || GameData_IsForceSeasonSync(gameData) == FALSE) {
        for (i = 0; i <= 7; i++) {
            func_0204b3dc(work->actors[i], FALSE);
        }
        func_0204b3dc(work->actors[8], FALSE);
    } else {
        for (i = 0; i <= 7; i++) {
            func_0204b3dc(work->actors[i], TRUE);
        }
        func_0204b3dc(work->actors[8], TRUE);
    }
    for (i = 6; i <= 8; i++) {
        MIi_CpuCopy16((u16 *)HW_DB_OBJ_PLTT + 1 + (work->sub.netId + 1) * 16, (u16 *)HW_DB_OBJ_PLTT + 6 + i * 16, 2);
    }
    if (work->unk_114 == 1) {
        if (work->unk_115 % 2 == 0) {
            palette = (((u32)work->unk_115 >> 1) & 1) ? 6 : 7;
            if (work->unk_113 != 8) {
                actor = work->actors[work->unk_113];
            } else {
                actor = work->actors[8];
            }
            func_0204b6f4(actor, (u8)palette, 1);
        }
        if (work->unk_115 < 10) {
            work->unk_115++;
        }
        return;
    }
    if (work->unk_11b == 0xff) {
        return;
    }
    if (work->unk_11a == 0) {
        work->unk_118 = work->unk_118 + 0x100;
        if (work->unk_118 >= 0x1100) {
            work->unk_118 = 0x2000 - work->unk_118;
            work->unk_11a = 1;
        }
    } else {
        work->unk_118 = work->unk_118 - 0x100;
        if (work->unk_118 < 0) {
            work->unk_118 = 0x100 - work->unk_118;
            work->unk_11a = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        func_02021f64(&work->unk_120[i], &work->unk_130[i], 1, (u8)(work->unk_118 >> 8), work->unk_128[i]);
    }
    work->unk_11c = 1;
}


void func_ov018_0217e014(Ov018Work *work, PassPowerLevel *level) {
    ClActor *actor;
    s32 count;
    s32 j;
    s32 k;
    s32 i;
    s32 netId;
    s32 next;
    ClActorPos pos;
    s32 self;
    GSYS_GetGameData(work->gsys);
    {
        self = 0;
        u8 list[3] = {0, 0, 0};
        next = work->sub.unk_02;
        netId = work->sub.netId;
        count = 0;

        pos.y = 0x1c;
        for (i = 0; i < 3; i++) {
            if (work->sub.unk_01 & (1 << i)) {
                list[count] = i;
                if (i == netId) {
                    self = count;
                }
                count++;
            }
        }
        j = 0;
        for (k = 0; k < count; k++) {
            if (self != k) {
                if (list[k] == netId + 1 || (list[k] == 0 && netId == next - 1)) {
                    pos.x = 0xa0;
                } else {
                    pos.x = 0x60;
                }
                actor = work->actors[j + 9];
                func_0204b404(actor, &pos, 0);
                func_0204b6f4(actor, (u8)(list[k] + 1), 1);
                func_0204b3dc(actor, 1);
                j++;
            }
        }
        for (; k < 3; k++) {
            func_0204b3dc(work->actors[k + 9], 0);
        }
    }
}

void func_ov018_0217e0f4(Ov018Work *work, IntrudeComm *comm, PassPowerLevel *level) {
    GameData *gameData = GSYS_GetGameData(work->gsys);
    u32 netId = work->sub.netId;
    s32 j;
    ClActor *actor;
    IntrudeCommUnk8C *status;
    s32 i;
    IntrudeFieldUnkEntry *oldEntry;
    u16 oldZone;
    ClActor *icon;
    IntrudeFieldUnkEntry *entry;
    ClActorPos poses[2];

    func_ov018_0217e2e0(work);
    for (i = 0; i < 3; i++) {
        oldEntry = work->slots[i].entry;
        entry = NULL;
        oldZone = work->slots[i].zone;
        if (i != func_02012b58(gameData)) {
            work->slots[i].entry = NULL;
            work->slots[i].zone = 0x1ab;
        }
        actor = work->actors[i + 12];
        icon = work->actors[i + 17];
        if (i == func_02012b58(gameData)) {
            func_0204b3dc(actor, FALSE);
            func_0204b3dc(icon, FALSE);
        } else if (comm == NULL) {
            func_0204b3dc(actor, FALSE);
            func_0204b3dc(icon, FALSE);
        } else {
            if (work->sub.unk_01 & (1 << i)) {
                status = &comm->unk_08C[i];
            } else {
                func_0204b3dc(actor, FALSE);
                func_0204b3dc(icon, FALSE);
                continue;
            }
            if (status->unk_16_9 == 2) {
                func_0204b954(actor, 0x2000);
            } else {
                func_0204b954(actor, 0x1000);
            }
            if (status->unk_16_0 == netId) {
                if (IsZoneEntralinkHub(status->zoneId) == TRUE) {
                    poses[1].x = data_ov018_021836a4[i][0] + 0x80;
                    poses[1].y = data_ov018_021836a4[i][1] + 0x4c;
                    func_0204b3dc(icon, FALSE);
                } else {
                    if (status->zoneId != oldZone) {
                        entry = func_ov018_0217bea8(status->zoneId);
                    } else {
                        entry = oldEntry;
                    }
                    if (entry != NULL) {
                        poses[1].x = ((u8 *)&entry->unk_0a)[0] + data_ov018_021836a4[i][0];
                        poses[1].y = ((u8 *)&entry->unk_0a)[1] + data_ov018_021836a4[i][1];
                        func_0204b3dc(icon, FALSE);
                    } else {
                        poses[1].x = 0xe8;
                        poses[1].y = i * 0x18 + 0x50;
                        poses[0] = poses[1];
                        poses[0].x = 0xf0;
                        func_0204b404(icon, &poses[0], 0);
                        func_0204b3dc(icon, TRUE);
                    }
                }
                func_0204b404(actor, &poses[1], 0);
                work->slots[i].entry = entry;
                work->slots[i].zone = status->zoneId;
            } else {
                func_0204b3dc(icon, FALSE);
                for (j = 0; j < 3; j++) {
                    if (i + 1 == func_0204b714(work->actors[j + 9])) {
                        func_0204b454(work->actors[j + 9], &poses[1], 0);
                        break;
                    }
                }
                if (j == 3) {
                    func_0204b3dc(actor, FALSE);
                    continue;
                }
                poses[1].x += data_ov018_021836a4[i][0];
                poses[1].y += data_ov018_021836a4[i][1];
                func_0204b404(actor, &poses[1], 0);
            }
            func_0204b3dc(actor, TRUE);
        }
    }
}


void func_ov018_0217e2e0(Ov018Work *work) {
    IntrudeMissionEntry *entry;
    s32 next;
    u16 buf[16];
    u8 level;

    if (work->sub.unk_04 == 3 && work->sub.entry != NULL) {
        entry = work->sub.entry;
        level = entry->player.unk_10;
        next = -1;
        if (work->unk_16a != 4 && work->unk_16a != level) {
            func_0204b6f4(work->actors[work->unk_16a + 0xc], (u8)(work->unk_16a + 1), 1);
        }
        if (work->unk_16a != level) {
            MIi_CpuCopy16((u16 *)HW_DB_OBJ_PLTT + (level + 1) * 16, buf, 0x20);

            func_02021f64(buf, buf, 0x10, 8, 0x7fff);
            MIi_CpuCopy16(buf, (u16 *)HW_DB_OBJ_PLTT + 9 * 16, 0x20);
            work->unk_16a = level;
        }
        work->unk_16b++;
        if (work->unk_16b == 0xf) {
            next = 9;
        } else if (work->unk_16b >= 0x1e) {
            next = level + 1;
            work->unk_16b = 0;
        }
        if (next != -1) {
            func_0204b6f4(work->actors[level + 0xc], (u8)next, 1);
        }
    } else {
        if (work->unk_16a != 4) {
            func_0204b6f4(work->actors[work->unk_16a + 0xc], (u8)(work->unk_16a + 1), 1);
            work->unk_16a = 4;
            work->unk_16b = 0;
        }
    }
}

void func_ov018_0217e3d0(Ov018Work *work, PassPowerLevel *level, u16 zoneId) {
    GameData *gameData = GSYS_GetGameData(work->gsys);
    u32 myId = func_02012b58(gameData);
    s32 select = 0xff;
    ClActorPos pos;
    IntrudeFieldUnkEntry *entry;
    ClActor *actor;
    s32 i;

    if (IsZoneEntralinkHub(zoneId) == TRUE) {
        pos.x = data_ov018_021836a4[myId][0] + 0x80;
        pos.y = data_ov018_021836a4[myId][1] + 0x4c;
        select = 8;
        work->slots[myId].entry = NULL;
    } else {
        if (zoneId != work->slots[myId].zone) {
            entry = func_ov018_0217bea8(zoneId);
        } else {
            entry = work->slots[myId].entry;
        }
        work->slots[myId].entry = entry;
        work->slots[myId].zone = zoneId;
        if (entry != NULL) {
            pos.x = ((u8 *)&entry->unk_0a)[0] + data_ov018_021836a4[myId][0];
            pos.y = ((u8 *)&entry->unk_0a)[1] + data_ov018_021836a4[myId][1];
            for (i = 0; i < 8; i++) {
                if (entry->unk_00_0 == data_ov011_02176aa0[i].zoneA || entry->unk_00_0 == data_ov011_02176aa0[i].zoneB) {
                    select = i;
                    break;
                }
            }
        } else {
            pos.x = 0xe8;
            pos.y = myId * 0x18 + 0x50;
            select = 0xff;
            work->slots[myId].entry = NULL;
        }
    }
    actor = work->actors[15];
    func_0204b404(actor, &pos, 0);
    func_0204b3dc(actor, 1);
    func_ov018_0217f118(work, select);
    work->slots[myId].zone = zoneId;
}




void func_ov018_0217e4d0(Ov018Work *work, PassPowerLevel *level) {
    GameData *gameData;
    BOOL BVar1;

    gameData = GSYS_GetGameData(work->gsys);
    if ((work->sub.unk_11 == 0) &&
          (BVar1 = GameData_IsForceSeasonSync(gameData), BVar1 == 1)) {
        switch(work->sub.unk_04) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            func_0204b3dc(work->actors[16], 0);
            func_02025774(work->unk_c0[0], 0);
        }
    }
}

void func_ov018_0217e524(Ov018Work *work, PassPowerLevel *level) {
    GameData *gameData = GSYS_GetGameData(work->gsys);
    s32 num;
    s32 i;
    s32 digit;
    s32 dst;
    u8 *chars;
    s32 src;

    if (func_ov018_0217f1e8(work) == 0) {
        if (func_0204b3f4(work->actors[20]) == 1) {
            func_0204b3dc(work->actors[20], 0);
            func_ov018_0217f214();
            return;
        }
        return;
    }
    if (func_0204b3f4(work->actors[20]) == 0) {
        func_0204b3dc(work->actors[20], 1);
        func_ov018_0217f240(work, 1);
    }
    func_020127b8(gameData);
    num = work->sub.unk_08;
    if (work->unk_10a != -1 && work->unk_10a != num && num <= 5 && work->sub.unk_04 == 3) {
        GFL_SndSEPlay(0x550);
    }
    if (work->unk_10a != num) {
        chars = work->unk_cc->pRawData;
        work->unk_10a = num;
        for (i = 0; i < 3; i++) {
            digit = num % 10;
            num = num / 10;
            dst = (2 - i) * 0x40;
            src = digit * 0x40;
            MIi_CpuCopy32(chars + src, (u8 *)work->unk_d0 + dst, 0x40);
            MIi_CpuCopy32(chars + (src + 0x280), (u8 *)work->unk_d0 + (dst + 0x100), 0x40);
        }
        work->unk_10f = 1;
    }
}


void func_ov018_0217e638(Ov018Work *work, u32 zoneId) {
    GameData *gameData;
    GFLBitmap *bitmap;
    u32 msgId;
    s32 state;

    gameData = GSYS_GetGameData(work->gsys);
    bitmap = BmpWin_GetBitmap(work->win44.window);
    if (func_0201c6c4(work->unk_28, bitmap) == 1) {
        return;
    }
    if (GameData_IsForceSeasonSync(gameData) == 1) {
        if (work->sub.netId == func_02012b58(gameData)) {
            state = 2;
            msgId = 0x43;
        } else {
            state = work->sub.netId + 3;
            msgId = 0x44;
        }
    } else {
        state = 7;
    }
    if (work->unk_10c[0] == state || state == 0) {
        return;
    }
    work->unk_10c[0] = state;
    if (state == 7) {
        GFL_BitmapFill(BmpWin_GetBitmap(work->win44.window), 0);
        BmpWin_FlushChar(work->win44.window);
        return;
    }
    if (state >= 3 && state <= 6) {
        copyVarForText(work->unk_24, 0, func_02012950(gameData, work->sub.netId));
    }
    GFL_MsgDataLoadStrbuf(work->unk_2c, msgId, work->unk_38);
    GFL_WordSetFormatStrbuf(work->unk_24, work->unk_40, work->unk_38);
    GFL_BitmapFill(BmpWin_GetBitmap(work->win44.window), 0);
    {
        PrintQueue *queue = work->unk_28;
        func_0201c724(queue, BmpWin_GetBitmap(work->win44.window), 0, 0, work->unk_40, work->unk_1c, 0x3c40);
    }
    work->win44.flushPending = 1;
}

void func_ov018_0217e714(Ov018Work *work, IntrudeComm *comm, u16 zoneId) {
    GameCommSys *commSys = GSYS_GetGameCommSystem(work->gsys);
    GameData *gameData = GSYS_GetGameData(work->gsys);
    BOOL found = FALSE;
    u32 myId;
    u8 prev;
    s32 i;
    u32 msgId;
    u32 sub4;
    IntrudeFieldUnkEntry *entry;
    IntrudeFieldUnkEntry *other;
    PlayerInfo *info;
    IntrudeCommUnk8C *status;
    Field *field;
    s32 dx;
    s32 dz;
    s32 adz;
    s32 adx;
    GameCommLogOut log;
    VecFx32 playerPos;

    if (work->unk_108 > 0) {
        work->unk_108 = work->unk_108 - 1;
        if (work->unk_108 != 0) {
            return;
        }
        if (comm != NULL) {
            return;
        }
        GFL_BitmapFill(BmpWin_GetBitmap(work->win4c.window), 0);
        BmpWin_FlushChar(work->win4c.window);
        return;
    }
    prev = work->unk_10c[1];
    work->unk_10c[1] = 0;
    myId = func_02012b58(gameData);
    if (comm != NULL && func_0203d1b4() <= 1) {
        if (comm->unk_925 != 0) {
            msgId = comm->unk_925 == 1 ? 0x29 : 0x2a;
            for (i = 0; i < comm->unk_925; i++) {
                func_0201ee0c(work->unk_24, i, comm->unk_928[i], comm->unk_938[i], 1, comm->unk_93A[i]);
            }
            GFL_MsgDataLoadStrbuf(work->unk_2c, msgId, work->unk_38);
            GFL_WordSetFormatStrbuf(work->unk_24, work->unk_3c, work->unk_38);
        } else {
            GFL_MsgDataLoadStrbuf(work->unk_2c, 0x33, work->unk_3c);
        }
        found = TRUE;
    }
    if (!found && comm != NULL && func_0203d1b4() > 1 && func_ov010_021585bc(comm)) {
        GFL_MsgDataLoadStrbuf(work->unk_2c, 0x35, work->unk_3c);
        found = TRUE;
    }
    if (!found && GameData_IsForceSeasonSync(gameData) == FALSE) {
        entry = work->slots[myId].entry;
        if (entry != NULL) {
            for (i = 0; i < 3; i++) {
                if (myId != i && (other = work->slots[i].entry) != NULL && ((u8 *)&entry->unk_0a)[0] == ((u8 *)&other->unk_0a)[0]
                    && ((u8 *)&entry->unk_0a)[1] == ((u8 *)&other->unk_0a)[1]) {
                    GFL_MsgDataLoadStrbuf(work->unk_2c, 0x31, work->unk_3c);
                    found = TRUE;
                    break;
                }
            }
        }
        if (!found && work->sub.unk_11 == 1) {
            GFL_MsgDataLoadStrbuf(work->unk_2c, 0x34, work->unk_3c);
            found = TRUE;
        }
        if (!found && work->sub.unk_12 == 1) {
            GFL_MsgDataLoadStrbuf(work->unk_2c, 0x32, work->unk_3c);
            found = TRUE;
        }
    }
    if (!found && comm != NULL && work->unk_169 != 4 && func_ov010_021583ec(gameData, work->unk_169) != NULL) {
        info = func_ov010_021583ec(gameData, work->unk_169);
        if (comm->unk_08C[work->unk_169].unk_16_9 == 2) {
            msgId = 0x36;
        } else {
            msgId = 0x28;
        }
        GFL_MsgDataLoadStrbuf(work->unk_2c, msgId, work->unk_38);
        copyVarForText(work->unk_24, 0, info);
        GFL_WordSetFormatStrbuf(work->unk_24, work->unk_3c, work->unk_38);
        work->unk_169 = 4;
        found = TRUE;
    } else {
        sub4 = work->sub.unk_04;
        if (sub4 != 0 && work->sub.entry != NULL && (sub4 == 3 || prev != sub4)) {
            if (work->sub.unk_04 == 3 && comm != NULL && work->sub.entry->mission.kind != 4) {
                field = GSYS_GetField(work->gsys);
                status = &comm->unk_08C[work->sub.entry->player.unk_10];
                FieldPlayer_GetWPos(Field_GetPlayer(field), &playerPos);
                func_ov011_021762c0(work->unk_24, 0, &work->sub.entry->player, 0x70);
                entry = func_ov018_0217bea8(status->zoneId);
                if (work->sub.netId == work->sub.unk_10 && entry != NULL
                    && func_ov018_0217ec08(zoneId, entry->unk_00_0, entry->unk_00_10) == TRUE) {
                    if (status->status.unk0 == 1) {
                        GFL_MsgDataLoadStrbuf(work->unk_2c, 0x2f, work->unk_38);
                        GFL_WordSetFormatStrbuf(work->unk_24, work->unk_3c, work->unk_38);
                        found = TRUE;
                    } else {
                        dx = status->status.pos.x - playerPos.x;
                        dz = status->status.pos.z - playerPos.z;
                        if (dz < 0) {
                            adz = -dz;
                        } else {
                            adz = dz;
                        }
                        adx = dx < 0 ? -dx : dx;
                        if (adx > adz) {
                            msgId = 0x2d;
                            if (dx <= 0) {
                                msgId = 0x2e;
                            }
                        } else {
                            msgId = 0x2c;
                            if (dz <= 0) {
                                msgId = 0x2b;
                            }
                        }
                        GFL_MsgDataLoadStrbuf(work->unk_2c, msgId, work->unk_38);
                        GFL_WordSetFormatStrbuf(work->unk_24, work->unk_3c, work->unk_38);
                        found = TRUE;
                    }
                } else {
                    entry = func_ov018_0217bea8(status->zoneId);
                    other = func_ov018_0217bea8(zoneId);
                    if (entry != NULL && other != NULL && ((u8 *)&entry->unk_0a)[0] == ((u8 *)&other->unk_0a)[0]
                        && ((u8 *)&entry->unk_0a)[1] == ((u8 *)&other->unk_0a)[1]) {
                        GFL_MsgDataLoadStrbuf(work->unk_2c, 0x2f, work->unk_38);
                        GFL_WordSetFormatStrbuf(work->unk_24, work->unk_3c, work->unk_38);
                        found = TRUE;
                    }
                }
            }
            if (!found && GameData_IsForceSeasonSync(gameData) == TRUE) {
                work->unk_10c[1] = 0;
                switch (work->sub.unk_04) {
                case 1:
                    GFL_MsgDataLoadStrbuf(work->unk_2c, 0x21, work->unk_3c);
                    break;
                case 2:
                    GFL_MsgDataLoadStrbuf(work->unk_2c, 0x22, work->unk_3c);
                    break;
                case 3:
                    if (work->unk_10c[2] == 0) {
                        GFL_MsgDataLoadStrbuf(work->unk_2c, work->sub.entry->mission.kind + 0x37, work->unk_3c);
                    } else {
                        GFL_MsgDataLoadStrbuf(work->unk_30, work->sub.entry->mission.unk_04_7, work->unk_38);
                        func_ov011_021762d8(&work->sub.entry->mission, &work->sub.entry->player, work->unk_24, 0x70);
                        GFL_WordSetFormatStrbuf(work->unk_24, work->unk_3c, work->unk_38);
                    }
                    work->unk_10c[2] = work->unk_10c[2] ^ 1;
                    break;
                case 0:
                case 4:
                default:
                    return;
                }
                if (work->sub.unk_04 != 3) {
                    work->unk_10c[2] = 0;
                }
                work->unk_10c[1] = work->sub.unk_04;
                found = TRUE;
            }
        } else if (func_02026608(commSys, &log) == 1) {
            work->unk_10c[1] = 0;
            GFL_MsgDataLoadStrbuf(work->unk_2c, log.beaconType, work->unk_38);
            for (i = 0; i < 2; i++) {
                if (log.name[i] != NULL) {
                    func_0201ee0c(work->unk_24, log.unk8[i], log.name[i], log.gender[i], 1, 2);
                }
            }
            GFL_WordSetFormatStrbuf(work->unk_24, work->unk_3c, work->unk_38);
            found = TRUE;
        }
    }
    if (!found && GameData_IsForceSeasonSync(gameData) == TRUE && !IsZoneEntralinkHub(zoneId)
        && !GetZoneIsEntreeForest(zoneId) && !FieldPalaceSys_CheckEventFlag(gameData, zoneId)) {
        GFL_MsgDataLoadStrbuf(work->unk_2c, 0x30, work->unk_3c);
        found = TRUE;
    }
    work->unk_169 = 4;
    if (found == TRUE) {
        GFL_BitmapFill(BmpWin_GetBitmap(work->win4c.window), 0);
        PrintWindow_PrintColor(&work->win4c, work->unk_28, 0, 0, work->unk_3c, work->unk_1c, 0x3c40);
        work->unk_108 = 0x5a;
    }
}



PassPowerLevel *func_ov018_0217ebd8(Ov018Work *param_1) {
    GameData *gameData = GSYS_GetGameData(param_1->gsys);
    u8 netId = param_1->sub.netId;
    if (netId == func_02012b58(gameData) || netId == 0x80) {
        return func_020127b8(gameData);
    } else {
        return func_020127ac(gameData, netId);
    }
}

BOOL func_ov018_0217ec08(u16 param_1, u16 param_2, u16 param_3) {
    s32 id;

    if (param_1 == param_2 || param_1 == param_3) {
        return TRUE;
    }
    if ((u16)(param_1 + 0xfeda) <= 1) {
        id = 0x126;
        if (param_1 == 0x126) {
            id = 0x127;
        }
        if ((u16)id == param_2 || (u16)id == param_3) {
            return TRUE;
        }
    }
    return FALSE;
}



void func_ov018_0217ec44(s32 param_1, s32 param_2, s32 param_3, s32 param_4, s32 *param_5) {
    param_5[1] = param_2 - param_4;
    param_5[3] = param_2 + param_4;
    *param_5 = param_1 - param_3;
    param_5[2] = param_1 + param_3;
}

BOOL func_ov018_0217ec5c(s32 param_1, s32 param_2, s32 *param_3) {
    if (param_3[0] < param_1 && param_3[2] > param_1 && param_3[1] < param_2 && param_3[3] > param_2) {
        return TRUE;
    }
    return FALSE;
}

void func_ov018_0217ec7c(IntrudeComm *comm, struct Ov018Work *param_2, u16 zoneId) {
    Field *field = GSYS_GetField(param_2->gsys);
    FieldSubscreen *subscreen = Field_GetSubscreen(field);
    u32 x;
    u32 y;
    ClActorPos pos;
    s32 rect[4];
    u32 myId;
    GameData *gameData;
    s32 i;
    ClActor *actor;
    const IntrudeZonePair *pair;
    u16 zoneA;
    IntrudeFieldUnkEntry *zoneInfo;

    GSYS_GetGameCommSystem(param_2->gsys);
    gameData = GSYS_GetGameData(param_2->gsys);
    if (param_2->unk_112 == 0 && param_2->unk_106 != 1 && func_02035d20(&x, &y) != 0
        && FieldSubscreen_GetReturnSubscreen(subscreen) == 0) {
        myId = func_02012b58(gameData);
        for (i = 0; i < 3; i++) {
            if (i != myId && (param_2->sub.unk_01 & (1 << i)) != 0) {
                actor = param_2->actors[12 + i];
                if (func_0204b3f4(actor) == 1) {
                    func_0204b454(actor, &pos, 0);
                    if (x - 12 <= pos.x && x + 12 >= pos.x && y - 12 <= pos.y
                        && y + 12 >= pos.y && comm != NULL
                        && func_ov018_0217bea8(comm->unk_08C[i].zoneId) != NULL) {
                        param_2->unk_169 = i;
                        return;
                    }
                }
            }
        }
        if (func_ov018_0217f284(param_2) != 1) {
            myId = func_02012b58(gameData);
            if (param_2->sub.netId != myId) {
                for (i = 0; i < 8; i++) {
                    pair = &data_ov011_02176aa0[i];
                    func_ov018_0217ec44(pair->unk_04, pair->unk_05, 0x10, 0x10, rect);
                    if (func_ov018_0217ec5c(x, y, rect) == 1) {
                        zoneA = pair->zoneA;
                        if (zoneId == zoneA || zoneId == pair->zoneB
                            || (func_02013dd0(zoneId) == 1 && func_02013dd0(zoneA) == 1)) {
                            GFL_SndSEPlay(0x557);
                            return;
                        }
                        if (i == 7) {
                            param_2->unk_106 = 1;
                            param_2->unk_107 = 0;
                        } else {
                            zoneInfo = func_ov018_0217bea8(zoneA);
                            if (zoneInfo == NULL) {
                                continue;
                            }
                            param_2->unk_110 = zoneInfo->unk_00_20;
                            param_2->unk_112 = 1;
                        }
                        param_2->unk_113 = i;
                        return;
                    }
                }
            }
            myId = func_02012b58(gameData);
            if (param_2->sub.netId != myId) {
                func_ov018_0217ec44(0x80, 0x5c, 0x10, 0x10, rect);
                if (func_ov018_0217ec5c(x, y, rect) == 1) {
                    if (IsZoneEntralinkHub(zoneId) == TRUE) {
                        GFL_SndSEPlay(0x557);
                        return;
                    }
                    param_2->unk_110 = 0x117;
                    param_2->unk_112 = 1;
                    param_2->unk_113 = 8;
                    return;
                }
            }
            if (param_2->unk_168 == 0 && func_020257c4(param_2->unk_c0[1]) == 1) {
                func_ov018_0217ec44(0xe0, 0x80, 0x1e, 0xc, rect);
                if (func_ov018_0217ec5c(x, y, rect) == 1) {
                    param_2->unk_168 = 1;
                }
            }
        }
    }
}


void func_ov018_0217eeb4(struct Ov018Work *param_1) {
    int changed = 0;

    if (param_1->unk_11f == 0) {
        if (param_1->sub.unk_02 > 1) {
            loadBGScrToVramByNarcNoReserveNegAlign(0x24, 7, 6, 0, 0x800, TRUE, 0x15);
            param_1->unk_11f = 1;
            changed++;
        }
    } else {
        if (param_1->sub.unk_02 <= 1) {
            loadBGScrToVramByNarcNoReserveNegAlign(0x24, 6, 6, 0, 0x800, TRUE, 0x15);
            param_1->unk_11f = 0;
            changed++;
        }
    }
    if (changed != 0) {
        BmpWin_FlushMap(param_1->win44.window);
        BmpWin_FlushMap(param_1->win4c.window);
        GFL_BGSysQueueScrLoad(6);
        if (func_ov018_0217f1e8(param_1) == 1) {
            func_ov018_0217f240(param_1, 1);
        }
    }
}


void func_ov018_0217ef44(Ov018Work *work, IntrudeComm *comm) {
    GSYS_GetGameData(work->gsys);
    GSYS_GetGameCommSystem(work->gsys);
    if (work->sub.netId != work->unk_105) {
        func_ov018_0217ef70(work, work->sub.netId);
    }
}



void func_ov018_0217ef70(Ov018Work *work, u8 param_2) {
    GFL_BGSysSetScrPaletteNo(7, 0, 2, 0x20, 0x10, param_2 + 5);
    GFL_BGSysSetScrPaletteNo(6, 0, 2, 0x20, 0x10, param_2 + 1);
    if (work->win44.window != NULL) {
        BmpWin_FlushMap(work->win44.window);
        BmpWin_FlushMap(work->win4c.window);
    }
    GFL_BGSysQueueScrLoad(7);
    GFL_BGSysQueueScrLoad(6);
    work->unk_105 = param_2;
}


void func_ov018_0217efcc(struct Ov018Work *param_1, IntrudeComm *param_2) {
    GameData *gameData;
    struct Ov018Sub *sub = &param_1->sub;

    gameData = GSYS_GetGameData(param_1->gsys);
    if (param_2 == NULL) {
        sub->netId = func_02012b58(gameData);
        sub->unk_01 = 1 << sub->netId;
        sub->unk_02 = 1;
        sub->unk_03 = 0;
        sub->unk_04 = 0;
        sub->unk_08 = 0;
        return;
    }
    func_ov018_0217f008(param_1, param_2);
}

void func_ov018_0217f008(struct Ov018Work *param_1, IntrudeComm *param_2) {
    struct Ov018Sub *sub = &param_1->sub;
    GameData *gameData = GSYS_GetGameData(param_1->gsys);
    u32 myId;
    int i;
    u8 bits;

    if (param_2 == NULL) {
        MI_CpuFill8(&param_1->sub, 0, sizeof(struct Ov018Sub));
        param_1->unk_11e = 1;
        return;
    }
    myId = func_02012b58(gameData);
    sub->netId = param_2->unk_070.unk_16_0;
    bits = param_2->unk_8FB;
    sub->unk_01 = bits;
    sub->unk_02 = MATH_CountPopulation(bits);
    sub->unk_03 = param_2->unk_910;
    sub->unk_04 = func_ov018_0217f5a0(&param_2->unk_1AC);
    sub->unk_08 = func_ov011_021758c8(&param_2->unk_1AC);
    sub->entry = func_ov011_02175b34(&param_2->unk_1AC);
    if (sub->entry != NULL) {
        sub->unk_10 = param_2->unk_08C[sub->entry->player.unk_10].unk_16_0;
    }
    sub->unk_11 = func_ov018_0217c070(param_2);
    sub->unk_12 = 0;
    for (i = 0; i < 3; i++) {
        if (i != myId && sub->netId == param_2->unk_08C[i].unk_16_0) {
            sub->unk_12 = 1;
            return;
        }
    }
}



void func_ov018_0217f0c4(Ov018Work *work) {
    s32 i;

    i = 0;
    work->unk_115 = 0;
    work->unk_114 = 1;
    do {
        func_0204b6f4(work->actors[i], 6, 1);
        i = i + 1;
    } while (i < 7);
    func_0204b6f4(work->actors[8], 6, 1);
}

u32 func_ov018_0217f0fc(Ov018Work *work) {
    if ((work->unk_114 == 1) && (work->unk_115 < 10)) {
        return 1;
    }
    return 0;
}

void func_ov018_0217f118(struct Ov018Work *param_1, s32 param_2) {
    if (param_1->unk_11b == param_2) {
        return;
    }
    if (param_1->unk_114 == 1) {
        return;
    }
    if (param_1->unk_11b < 8) {
        func_0204b6f4(param_1->actors[param_1->unk_11b], 6, 1);
    } else if (param_1->unk_11b == 8) {
        func_0204b6f4(param_1->actors[8], 6, 1);
    }
    if (param_2 < 8) {
        func_0204b6f4(param_1->actors[param_2], 8, 1);
    } else if (param_2 == 8) {
        func_0204b6f4(param_1->actors[8], 8, 1);
    }
    param_1->unk_11b = param_2;
    param_1->unk_118 = 0;
    param_1->unk_11a = 0;
}


void func_ov018_0217f174(Ov018Work *work) {
    work->unk_117 = 0;
    work->unk_116 = 1;
    func_0204b6f4(work->actors[16], 6, 1);
}

void func_ov018_0217f194(Ov018Work *work) {
    if (work->unk_116 != 0) {
        if (work->unk_117 % 2 == 0) {
            func_0204b6f4(work->actors[16], (u8)((((u32)work->unk_117 >> 1) & 1) ? 6 : 7), 1);
        }
        if (work->unk_117 < 10) {
            work->unk_117++;
        }
    }
}


u32 func_ov018_0217f1e8(Ov018Work *work) {
    GameData *gameData;
    BOOL BVar1;
    u32 val;

    gameData = GSYS_GetGameData(work->gsys);
    BVar1 = GameData_IsForceSeasonSync(gameData);
    if ((BVar1 == 1) && (((val = work->sub.unk_04, val == 2 || (val == 3)) || (val == 4)))) {
        return 1;
    }
    return 0;
}

void func_ov018_0217f214(void) {
    GFL_BGSysFillScrArea(6, 0x3001, 0x14, 0x16, 0xc, 2, 0x11);
    GFL_BGSysQueueScrLoad(6);
}


void func_ov018_0217f240(Ov018Work *work, s32 param_2) {
    GFL_BGSysLoadScrArea
                        (6, 0x14, 0x16, 0xc, 2, work->unk_138, 0, 0, 0xc, 2);
    if (param_2 == 1) {
        GFL_BGSysQueueScrLoad(6);
        return;
    }
    GFL_BGSysLoadScr(6);
}

u32 func_ov018_0217f284(Ov018Work *work) {
    GameData *gameData;
    u32 result;
    BOOL BVar2;

    gameData = GSYS_GetGameData(work->gsys);
    if (((work->sub.entry == NULL) && (result = func_02012b58(gameData), result == 0)) &&

          (BVar2 = func_ov010_02158590(gameData), BVar2 == 0)) {
        return 1;
    }
    return 0;
}
