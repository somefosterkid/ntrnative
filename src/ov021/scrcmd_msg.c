// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov036/scrcmd_msg.c, ported by tools/port_file.py.
// The script commands of the message windows: the system, money, info, sign and checker windows, the balloons over
// actors and Trainers, the numbered windows, and the loading and formatting of the script's messages. The ROM has no
// name for the file; scrcmd_msg.c is descriptive. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)
#include "types.h"
#include "constants/arc.h"
#include "constants/script_text_banks.h"
#include "constants/version.h"
#include "battle/trainer_data.h"
#include "field/festival.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_camera.h"
#include "field/field_script.h"
#include "field/player_state.h"
#include "field/scrcmd_msg.h"
#include "gfl/g3d.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/mi.h"
#include "nnsys/g3d.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/gf_font.h"
#include "system/time_icon.h"
#include "system/vm.h"
#include "system/wordset.h"
#include "text/script/global_10520.h"

// The message file of the running script, for setupFormattedTextBuffer and LoadFieldScriptMessage
#define SCRIPT_MSG_FILE 0x400

StrBuf *func_ov021_021b162c(FieldScriptEnv *env, ScriptWork *work, u32 msgId);
void func_0202a310(u16 x, u16 y, StrBuf *strbuf, HeapID heapId);
u32 func_ov021_0219270c(FieldMsgBGWin *param_1, StrBuf *param_2, s32 param_3);
u32 func_ov021_0219272c(FieldMsgBG *param_1, StrBuf *param_2, s32 param_3);
u32 *func_ov021_02192570(FieldMsgBG *param_1, u32 param_2, s16 param_3, u8 param_4, u16 param_5, u16 param_6);
void func_ov021_02191830(FieldMsgBG *param_1, StrBuf *param_2, u8 *param_3, u8 *param_4);
void func_ov021_021918dc(HeapID *param_1, StrBuf *param_2, u32 param_3, u8 param_4, u8 param_5, u8 param_6, u8 param_7);
static void SystemMsgWin_Open(FieldScriptEnv *env, u8 type);
void func_ov021_0219269c(u32 *param_1, u16 param_2, u16 param_3, StrBuf *param_4);
BOOL func_ov021_021b1008(VM *vm, FieldScriptEnv *env, u32 isScream);
BOOL SetupActorMessageWindow(FieldScriptEnv *env, u16 actorId, u16 fileNo, u16 msgId, u16 pos, u32 a6);
void func_ov021_02191348(void *window, StrBuf *strbuf);
void func_ov021_021b09ec(u32 winPos, u32 *x, u32 *y);
void *func_ov021_02191a04(void *msgBGSys, u16 type);
MsgBGScrollWin *func_ov021_02191a10(FieldMsgBG *param_1, u32 param_2, Font *param_3);

// The up axis the balloon's offset is computed against
static const VecFx32 ACTOR_MSGWIN_CALC_AXIS_Y = {0, FX32_ONE, 0};

// The offset of a balloon from its actor, by the half of the screen the actor is in and the column of the screen
static const VecFx32 ACTOR_MSGWIN_ORIGIN_OFFSET_TABLE[2][4] = {
    {
        {0, FX32_CONST(16), FX32_CONST(-8)},
        {FX32_CONST(-8), FX32_CONST(12), FX32_CONST(-8)},
        {FX32_CONST(8), FX32_CONST(12), FX32_CONST(-8)},
        {0, FX32_CONST(16), FX32_CONST(-8)},
    },
    {
        {0, FX32_CONST(12), FX32_CONST(-8)},
        {FX32_CONST(-8), FX32_CONST(12), FX32_CONST(-8)},
        {FX32_CONST(8), FX32_CONST(12), FX32_CONST(-8)},
        {0, FX32_CONST(12), FX32_CONST(8)},
    },
};

// Opens the system message window at the top (1) or the bottom (2) of the screen, or for 0 away from the balloon
// over an actor
static void SystemMsgWin_Open(FieldScriptEnv *env, u8 pos) {
    ScriptFieldWork *fieldWork;
    MsgData *msgData;
    void *window;
    u32 row = 1;

    if (FieldScriptSubEvent_IsRegistered(1)) {
        func_ov021_02191164(getMapDisplayInfoPtr(env));
        return;
    }
    fieldWork = ScriptWork_GetFieldWork(FieldScriptEnv_GetScriptWork(env));
    msgData = GetFieldScriptMsgData(env);
    if (pos == 1) {
        // The top, as row says
    } else if (pos == 2) {
        row = 0x13;
    } else if (pos == 0) {
        switch (ActorMsgWin_GetPosActual(env)) {
        case 1:
        case 3:
        case 5:
            pos = 1;
            break;
        case 2:
        case 4:
        case 6:
            row = 0x13;
            pos = 2;
            break;
        case 7:
            row = 0x13;
            pos = 2;
            break;
        default:
            row = 0x13;
            pos = 2;
            break;
        }
    } else {
        row = 0x13;
        pos = 2;
    }
    window = func_ov021_02190fa0(fieldWork->msgBGSys, msgData, row);
    ActorMsgWin_SetPosActual(env, pos);
    setMapDisplayInfoPtr(env, window);
    FieldScriptSubEvent_Register(1);
}

static void func_ov021_021b0480(FieldScriptEnv *env) {
    WaitIcon *icon;

    if (FieldScriptSubEvent_IsRegistered(1)) {
        func_ov021_02191008(getMapDisplayInfoPtr(env));
        icon = func_ov010_0215a630(env);
        if (icon != NULL) {
            func_0202f038(icon);
            SetSpecialMessageIconPtr(env, NULL);
        }
        setMapDisplayInfoPtr(env, NULL);
        FieldScriptSubEvent_Unregister(1);
    }
}

static BOOL ScriptNative_SystemMsgWinWait(VM *vm, void *env) {
    if (func_ov021_021910c0(getMapDisplayInfoPtr(env)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL s0034_SystemMsg(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    u16 msgId = ScriptReadAny(vm, env);
    StrBuf *strbuf;

    SystemMsgWin_Open(env, VM_Read16(vm));
    strbuf = func_ov021_021b162c(env, work, msgId);
    func_ov021_02191084(getMapDisplayInfoPtr(env), 0, 0, strbuf);
    VM_SetNativeCallback(vm, ScriptNative_SystemMsgWinWait);
    return TRUE;
}

BOOL s0035_SystemMsgAsync(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    StrBuf *strbuf = ScriptWork_GetMainStrBuf(work);
    u16 msgId = ScriptReadAny(vm, env);
    MsgData *msgData;
    WordSet *wordSet;
    StrBuf *altStrBuf;
    void *window;

    SystemMsgWin_Open(env, VM_Read16(vm));
    msgData = GetFieldScriptMsgData(env);
    wordSet = ScriptWork_GetWordSet(work);
    altStrBuf = ScriptWork_GetAltStrBuf(work);
    GFL_MsgDataLoadStrbuf(msgData, msgId, altStrBuf);
    GFL_WordSetFormatStrbuf(wordSet, strbuf, altStrBuf);
    window = getMapDisplayInfoPtr(env);
    func_ov021_02191164(window);
    func_ov021_02191184(window, 0, 0, strbuf);
    return FALSE;
}


BOOL s0037_MsgSetLoadingSpinner(VM *vm, FieldScriptEnv *env) {
    HeapID heapId = Field_GetHeapID(GSYS_GetField(FieldScriptEnv_GetGameSystem(env)));
    u16 hide = ScriptReadAny(vm, env);
    WaitIcon *icon;
    BmpWin *window;

    if (!FieldScriptSubEvent_IsRegistered(1)) {
        return FALSE;
    }
    icon = func_ov010_0215a630(env);
    if (icon != NULL) {
        func_0202f038(icon);
        SetSpecialMessageIconPtr(env, NULL);
    }
    if (hide == 0) {
        window = func_ov021_021911b4(getMapDisplayInfoPtr(env));
        SetSpecialMessageIconPtr(env, func_0202ee30(GFL_VBlankGetTCBMgr(), window, 0xf, 0x10, heapId));
    }
    return FALSE;
}

BOOL s0036_InfoMsgClose(VM *vm, FieldScriptEnv *env) {
    func_ov021_021b0480(env);
    return FALSE;
}

BOOL func_ov021_021b0630(FinishScriptSubEventsWork *work, u32 *state) {
    func_ov021_021b0480(work->env);
    return TRUE;
}

BOOL s0040_MoneyWinDisp(VM *vm, FieldScriptEnv *env) {
    u16 x = ScriptReadAny(vm, env);
    u16 y = ScriptReadAny(vm, env);
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = GSYS_GetGameData(gsys);
    GameData_GetPlayerState(gameData);
    Field *field = GSYS_GetField(gsys);
    void *msgBGSys = Field_GetMsgBGSys(field);
    void *moneyWin = Field_GetMoneyWin(field);
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    WordSet *wordSet = ScriptWork_GetWordSet(work);
    StrBuf *text = ScriptWork_GetAltStrBuf(work);
    HeapID heapId = Field_GetHeapID(field);
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SCRIPT_MESSAGE, SCRIPT_TEXT_GLOBAL_10520, heapId);
    StrBuf *strbuf = GFL_StrBufCreate(0x80, heapId);
    u32 cash = getCash(getTrainerCardDataBlkAddress(gameData));

    if (moneyWin != NULL) {
        func_ov021_02190728(moneyWin);
    }
    moneyWin = FieldMsgBG_CreateMoneyWin(msgBGSys, msgData, x - 9, y, 9, 2);
    Field_SetMoneyWin(field, moneyWin);
    WordSetNumber(wordSet, 2, cash, 7, 1, TRUE);
    GFL_MsgDataLoadStrbuf(msgData, Global10520_Text_001, text);
    GFL_WordSetFormatStrbuf(wordSet, strbuf, text);
    func_ov021_02190758(moneyWin, 0, 0, strbuf);
    GFL_StrBufFree(strbuf);
    GFL_MsgDataFree(msgData);
    return FALSE;
}

BOOL s0042_MoneyWinUpdate(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys = FieldScriptEnv_GetGameSystem(env);
    GameData *gameData = GSYS_GetGameData(gsys);
    GameData_GetPlayerState(gameData);
    Field *field = GSYS_GetField(gsys);
    void *moneyWin = Field_GetMoneyWin(field);
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    WordSet *wordSet = ScriptWork_GetWordSet(work);
    StrBuf *text = ScriptWork_GetAltStrBuf(work);
    HeapID heapId = Field_GetHeapID(field);
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SCRIPT_MESSAGE, SCRIPT_TEXT_GLOBAL_10520, heapId);
    StrBuf *strbuf = GFL_StrBufCreate(0x80, heapId);

    WordSetNumber(wordSet, 2, getCash(getTrainerCardDataBlkAddress(gameData)), 7, 1, TRUE);
    GFL_MsgDataLoadStrbuf(msgData, Global10520_Text_001, text);
    GFL_WordSetFormatStrbuf(wordSet, strbuf, text);
    func_ov021_02190788(moneyWin);
    func_ov021_02190758(moneyWin, 0, 0, strbuf);
    GFL_StrBufFree(strbuf);
    GFL_MsgDataFree(msgData);
    return FALSE;
}

BOOL s0041_MoneyWinClose(VM *vm, FieldScriptEnv *env) {
    Field *field = GSYS_GetField(FieldScriptEnv_GetGameSystem(env));
    void *moneyWin = Field_GetMoneyWin(field);

    if (moneyWin != NULL) {
        func_ov021_02190728(moneyWin);
        Field_SetMoneyWin(field, NULL);
    }
    return FALSE;
}

// The offset of a balloon over an actor at pos, by where on the screen the actor is
static void ActorMsgWin_CalcAttachmentOffsetDynamic(const VecFx32 *pos, VecFx32 *offset, G3DCamera *g3dCamera,
                                                    FieldCamera *camera) {
    VecFx32 world = *pos;
    VecFx32 dir;
    VecFx32 flatDir;
    VecFx32 side;
    VecFx32 up;
    VecFx32 camPos;
    VecFx32 target;
    int x;
    int y;
    u32 tileX;
    u32 column;

    GFL_G3DCameraFlush(g3dCamera);
    GFL_G3DCameraGetLookatTarget(g3dCamera, &target);
    GFL_G3DCameraGetLookatPos(g3dCamera, &camPos);
    VEC_Subtract(&target, &camPos, &dir);
    flatDir = dir;
    flatDir.y = 0;
    VEC_Normalize(&flatDir, &flatDir);
    VEC_CrossProduct(&flatDir, &ACTOR_MSGWIN_CALC_AXIS_Y, &side);
    VEC_CrossProduct(&dir, &side, &up);
    VEC_Normalize(&up, &up);
    world.x += up.x;
    world.y += up.y;
    world.z += up.z;
    NNS_G3dWorldPosToScrPos(&world, &x, &y);
    tileX = (u32)x / 8;
    if (tileX < 10) {
        column = 0;
    } else if (tileX >= 10 && tileX <= 15) {
        column = 1;
    } else if (tileX >= 16 && tileX < 22) {
        column = 2;
    } else {
        column = 3;
    }
    // y becomes the half of the screen, in rows of 12 tiles
    y = (u32)y / 8 / 12;
    if (column < 4 && (u32)y < 2) {
        *offset = ACTOR_MSGWIN_ORIGIN_OFFSET_TABLE[y][column];
    } else {
        VecFx32 zero = { 0, 0, 0 };

        *offset = zero;
    }
}

// Turns vec from the camera's space to the world's
static void ActorMsgWin_MulVecInvCamRot(VecFx32 *vec, G3DCamera *camera) {
    VecFx32 camPos;
    VecFx32 camUp;
    VecFx32 target;
    MtxFx43 lookAt;
    MtxFx33 rot;

    GFL_G3DCameraGetLookatPos(camera, &camPos);
    GFL_G3DCameraGetLookatUpVector(camera, &camUp);
    GFL_G3DCameraGetLookatTarget(camera, &target);
    MTX_LookAt(&camPos, &camUp, &target, &lookAt);
    MI_Copy36B(&lookAt, &rot);
    MTX_Inverse33(&rot, &rot);
    MTX_MultVec33(vec, &rot, vec);
}

static inline void ActorMsgWin_GetFixedOffset(VecFx32 *offset, u32 half, u32 column) {
    *offset = ACTOR_MSGWIN_ORIGIN_OFFSET_TABLE[half][column];
}

static void ActorMsgWin_GetAttachmentOffset(const VecFx32 *pos, VecFx32 *offset, G3DCamera *g3dCamera,
                                            FieldCamera *camera, u8 winPos) {
    u32 column = 0;
    u32 half = 0;

    switch (winPos) {
    case 3:
        column = 1;
        break;
    case 4:
        column = 1;
        half = 1;
        break;
    case 5:
        column = 2;
        break;
    case 6:
        column = 2;
        half = 1;
        break;
    case 0:
    case 1:
    case 2:
        ActorMsgWin_CalcAttachmentOffsetDynamic(pos, offset, g3dCamera, camera);
        ActorMsgWin_MulVecInvCamRot(offset, g3dCamera);
        return;
    }
    *offset = ACTOR_MSGWIN_ORIGIN_OFFSET_TABLE[half][column];
    ActorMsgWin_MulVecInvCamRot(offset, g3dCamera);
}

void func_ov021_021b09d8(const VecFx32 *pos, VecFx32 *offset, G3DCamera *g3dCamera, FieldCamera *camera, u8 winPos) {
    ActorMsgWin_GetAttachmentOffset(pos, offset, g3dCamera, camera, winPos);
}

void func_ov021_021b09ec(u32 winPos, u32 *a1, u32 *a2) {
    *a1 = 0;
    *a2 = 0;
    switch (winPos) {
    case 3:
        *a2 = 7;
        break;
    case 4:
        *a1 = 1;
        *a2 = 5;
        break;
    case 5:
        *a2 = 8;
        break;
    case 6:
        *a1 = 1;
        *a2 = 6;
        break;
    case 2:
        *a1 = 1;
        break;
    case 0:
        *a1 = 1;
        *a2 = 2;
        break;
    }
}

u8 ActorMsgWin_CalcWinPosAuto(FieldActor *player, const VecFx32 *pos) {
    VecFx32 playerPos;
    u8 winPos = 2;

    CopyActorWPos(player, &playerPos);
    if (pos->z < playerPos.z) {
        winPos = 1;
    }
    return winPos;
}

static BOOL func_ov021_021b0a60(VM *vm, void *env) {
    if (func_ov021_021912f8(getMapDisplayInfoPtr(env)) == TRUE) {
        setMapDisplayInfoPtr(env, NULL);
        FieldScriptSubEvent_Unregister(2);
        return TRUE;
    }
    return FALSE;
}

static void func_ov021_021b0a88(VM *vm, FieldScriptEnv *env) {
    if (FieldScriptSubEvent_IsRegistered(2)) {
        func_ov021_021912d8(getMapDisplayInfoPtr(env));
        VM_SetNativeCallback(vm, func_ov021_021b0a60);
    }
}

static void func_ov021_021b0ab0(FieldScriptEnv *env) {
    if (FieldScriptSubEvent_IsRegistered(2)) {
        func_ov021_0219131c(getMapDisplayInfoPtr(env));
        setMapDisplayInfoPtr(env, NULL);
        FieldScriptSubEvent_Unregister(2);
    }
}

void ActorMsgWin_CreateWindow(FieldScriptEnv *env, void *msgBGSys, u32 a2, const VecFx32 *pos, StrBuf *strbuf,
                              u32 a5, u32 a6) {
    setMapDisplayInfoPtr(env, ActorMsgWin_CheckAndCreate(msgBGSys, a2, pos, strbuf, a5, a6));
    FieldScriptSubEvent_Register(2);
}

// Moves the balloon with its actor. force recomputes the offset even when the actor hasn't moved
static void ActorMsgWin_CalcAttachmentPos(FieldScriptEnv *env, BOOL force) {
    MMSys *mmSys = GetScrEnvMMdlSys(env);
    ScriptActorWork *actorWork = GetFieldScriptActorWk(env);
    FieldActor *actor = FindFieldActor(mmSys, actorWork->actorId);
    VecFx32 pos;
    FieldCamera *camera;
    G3DCamera *g3dCamera;

    if (actor != NULL) {
        GetActorFaceDir(actor);
        CopyActorWPos(actor, &pos);
        if (force == TRUE || pos.x != actorWork->actorPos.x || pos.y != actorWork->actorPos.y
            || pos.z != actorWork->actorPos.z) {
            actorWork->actorPos = pos;
            camera = Field_GetCameraSystem(GSYS_GetField(FieldScriptEnv_GetGameSystem(env)));
            g3dCamera = FieldCamera_GetG3DCamera(camera);
            ActorMsgWin_GetAttachmentOffset(&actorWork->actorPos, &actorWork->offset, g3dCamera, camera,
                                            ActorMsgWin_GetPos(env));
        }
        actorWork->pos.x = actorWork->actorPos.x + actorWork->offset.x;
        actorWork->pos.y = actorWork->actorPos.y + actorWork->offset.y;
        actorWork->pos.z = actorWork->actorPos.z + actorWork->offset.z;
    }
}

BOOL SetupActorMessageWindow(FieldScriptEnv *env, u16 actorId, u16 fileNo, u16 msgId, u16 pos, u32 a6) {
    StrBuf *strbuf;
    u32 winPos = pos;
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    MsgData *msgData = GetFieldScriptMsgData(env);
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);
    MMSys *mmSys = GetScrEnvMMdlSys(env);
    FieldActor *player = FindFieldActor(mmSys, 0xff);
    FieldActor *actor = FindFieldActor(mmSys, actorId);
    WordSet *wordSet;
    StrBuf *altStrBuf;
    ScriptActorWork *actorWork;
    VecFx32 actorPos;
    u32 winX;
    u32 winY;

    if (actor == NULL) {
        return FALSE;
    }
    strbuf = ScriptWork_GetMainStrBuf(work);
    wordSet = ScriptWork_GetWordSet(work);
    altStrBuf = ScriptWork_GetAltStrBuf(work);
    if (fileNo == SCRIPT_MSG_FILE) {
        GFL_MsgDataLoadStrbuf(msgData, msgId, altStrBuf);
    } else {
        HeapID heapId = FieldScriptEnv_GetHeapID(env);
        MsgData *fileData = GFL_MsgSysLoadData(FALSE, 3, fileNo, heapId);

        GFL_MsgDataLoadStrbuf(fileData, msgId, altStrBuf);
        GFL_MsgDataFree(fileData);
    }
    GFL_WordSetFormatStrbuf(wordSet, strbuf, altStrBuf);
    actorWork = GetFieldScriptActorWk(env);
    if (FieldScriptSubEvent_IsRegistered(2)) {
        func_ov021_02191348(getMapDisplayInfoPtr(env), strbuf);
    } else {
        MI_CpuFill8(actorWork, 0, sizeof(ScriptActorWork));
        actorWork->actorId = actorId;
        if (pos == 0) {
            ActorMsgWin_GetPosActual(env);
            CopyActorWPos(actor, &actorPos);
            winPos = ActorMsgWin_CalcWinPosAuto(player, &actorPos);
        }
        func_ov021_021b09ec(winPos, &winX, &winY);
        actorWork->winX = winX;
        ActorMsgWin_SetPosActual(env, winPos);
        ActorMsgWin_SetPos(env, pos);
        ActorMsgWin_CalcAttachmentPos(env, TRUE);
        ActorMsgWin_CreateWindow(env, fieldWork->msgBGSys, winX, &actorWork->pos, strbuf, a6, winY);
    }
    return TRUE;
}


static BOOL ScriptNative_ActorMsgWinWait(VM *vm, void *env) {
    void *msgWin = getMapDisplayInfoPtr(env);

    ActorMsgWin_CalcAttachmentPos(env, FALSE);
    BOOL done = FALSE;
    if (func_ov021_02191388(msgWin) == TRUE) {
        done = TRUE;
    }
    return done;
}

BOOL func_ov021_021b0d14(VM *vm, FieldScriptEnv *env) {
    u16 a = ScriptReadAny(vm, env);
    u16 b = ScriptReadAny(vm, env);
    u16 c = ScriptReadAny(vm, env);
    u16 d = ScriptReadAny(vm, env);
    u16 e = ScriptReadAny(vm, env);

    if (SetupActorMessageWindow(env, c, a, b, d, e) == TRUE) {
        VM_SetNativeCallback(vm, ScriptNative_ActorMsgWinWait);
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_021b0d74(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    FieldActor *actor = ScriptWork_GetParentActor(work);
    u16 a = ScriptReadAny(vm, env);
    u16 b = ScriptReadAny(vm, env);
    u16 c = ScriptReadAny(vm, env);
    u16 d = ScriptReadAny(vm, env);
    u8 uid;

    if (actor == NULL) {
        return TRUE;
    }
    uid = GetActorUID(actor);
    if (SetupActorMessageWindow(env, uid, a, b, c, d) == TRUE) {
        VM_SetNativeCallback(vm, ScriptNative_ActorMsgWinWait);
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_021b0df0(VM *vm, FieldScriptEnv *env) {
    u16 a = ScriptReadAny(vm, env);
    u16 b = ScriptReadAny(vm, env);
    u16 c = ScriptReadAny(vm, env);
    u16 d = ScriptReadAny(vm, env);
    u16 e = ScriptReadAny(vm, env);
    u16 f = ScriptReadAny(vm, env);
    GameData *gameData = FieldScriptEnv_GetGameData(env);
    PlayerState *playerState = GameData_GetPlayerState(gameData);

    if (getTrainerGender(&playerState->playerInfo) != 0) {
        b = c;
    }
    if (SetupActorMessageWindow(env, d, a, b, e, f) == TRUE) {
        VM_SetNativeCallback(vm, ScriptNative_ActorMsgWinWait);
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov021_021b0e74(VM *vm, FieldScriptEnv *env) {
    u16 a = ScriptReadAny(vm, env);
    u16 b;
    u16 c;
    u16 d;
    u16 e;

#if GAME_VERSION == VERSION_WHITE
    b = ScriptReadAny(vm, env);
    ScriptReadAny(vm, env);
#else
    ScriptReadAny(vm, env);
    b = ScriptReadAny(vm, env);
#endif
    c = ScriptReadAny(vm, env);
    d = ScriptReadAny(vm, env);
    e = ScriptReadAny(vm, env);

    if (SetupActorMessageWindow(env, c, a, b, d, e) == TRUE) {
        VM_SetNativeCallback(vm, ScriptNative_ActorMsgWinWait);
        return TRUE;
    }
    return FALSE;
}


BOOL func_ov021_021b0edc(VM *vm, FieldScriptEnv *env) {
    func_ov021_021b0a88(vm, env);
    return TRUE;
}

BOOL func_ov021_021b0ee8(FinishScriptSubEventsWork *work, u32 *state) {
    func_ov021_021b0ab0(work->env);
    return TRUE;
}

BOOL func_ov021_021b0ef4(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);
    u16 x = ScriptReadAny(vm, env);
    u16 y = ScriptReadAny(vm, env);
    u16 id = ScriptReadAny(vm, env);
    StrBuf *strbuf = ScriptWork_GetMainStrBuf(work);
    ScriptActorWork *actorWork;
    HeapID heapId;
    MMSys *mmSys;
    FieldActor *actor;
    FieldPlayer *player;
    FieldActor *playerActor;
    VecFx32 pos;
    u8 result;
    BOOL attach;

    if (FieldScriptSubEvent_IsRegistered(2)) {
        return TRUE;
    }
    heapId = FieldScriptEnv_GetHeapID(env);
    func_0202a310(x, y, strbuf, heapId);
    mmSys = GetScrEnvMMdlSys(env);
    actor = FindFieldActor(mmSys, id);
    if (actor == NULL) {
        id = 0xff;
    }
    actorWork = GetFieldScriptActorWk(env);
    MI_CpuFill8(actorWork, 0, sizeof(ScriptActorWork));
    actorWork->actorId = id;
    player = Field_GetPlayer(fieldWork->field);
    mmSys = GetScrEnvMMdlSys(env);
    playerActor = FieldPlayer_GetActor(player);
    actor = FindFieldActor(mmSys, actorWork->actorId);
    CopyActorWPos(actor, &pos);
    result = ActorMsgWin_CalcWinPosAuto(playerActor, &pos);
    attach = (result != 1);
    actorWork->winX = attach;
    ActorMsgWin_CalcAttachmentPos(env, TRUE);
    ActorMsgWin_CreateWindow(env, fieldWork->msgBGSys, attach, &actorWork->pos, strbuf, 0, 0);
    ActorMsgWin_SetPosActual(env, result);
    VM_SetNativeCallback(vm, ScriptNative_ActorMsgWinWait);
    return TRUE;
}


BOOL func_ov021_021b0ff0(VM *vm, void *env) {
    if (func_ov021_021916e0(getMapDisplayInfoPtr(env)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov021_021b1008(VM *vm, FieldScriptEnv *env, u32 isScream) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    StrBuf *strbuf;
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);
    u16 msgId = ScriptReadAny(vm, env);
    u8 flag = *vm->pc;
    u8 type = 1;
    void *window;

    vm->pc++;
    strbuf = func_ov021_021b162c(env, work, msgId);
    if (flag != 1) {
        type = 0x13;
    }
    if (FieldScriptSubEvent_IsRegistered(3)) {
        window = getMapDisplayInfoPtr(env);
        func_ov021_021915ec(window);
    } else {
        window = func_ov021_02191558(fieldWork->msgBGSys, 0, isScream, 1, type, 0x1e, 4);
        setMapDisplayInfoPtr(env, window);
    }
    func_ov021_021916a8(window, 8, 0, strbuf);
    FieldScriptSubEvent_Register(3);
    VM_SetNativeCallback(vm, func_ov021_021b0ff0);
    return TRUE;
}


static void func_ov021_021b10a4(FieldScriptEnv *env) {
    if (FieldScriptSubEvent_IsRegistered(3)) {
        func_ov021_021915b4(getMapDisplayInfoPtr(env));
        setMapDisplayInfoPtr(env, NULL);
        FieldScriptSubEvent_Unregister(3);
    }
}

BOOL s0038_InfoMsg(VM *vm, FieldScriptEnv *env) {
    return func_ov021_021b1008(vm, env, 0);
}

BOOL s004A_ScreamMsg(VM *vm, FieldScriptEnv *env) {
    return func_ov021_021b1008(vm, env, 1);
}


BOOL s0039_InfoMsgClose(VM *vm, FieldScriptEnv *env) {
    func_ov021_021b10a4(env);
    return FALSE;
}

BOOL func_ov021_021b10f4(FinishScriptSubEventsWork *work, u32 *state) {
    func_ov021_021b10a4(work->env);
    return TRUE;
}

BOOL s003A_MultiMsg(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);
    u16 msgId = VM_Read16(vm);
    u16 x = ScriptReadAny(vm, env);
    u16 y = ScriptReadAny(vm, env);
    u16 index = VM_Read16(vm);
    u8 width;
    u8 height = 2;
    StrBuf *strbuf = GFL_StrBufCreate(0x40, FieldScriptEnv_GetHeapID(env));
    MsgData *msgData = GetFieldScriptMsgData(env);
    WordSet *wordSet = ScriptWork_GetWordSet(work);
    StrBuf *altStrBuf = ScriptWork_GetAltStrBuf(work);

    GFL_MsgDataLoadStrbuf(msgData, msgId, altStrBuf);
    GFL_WordSetFormatStrbuf(wordSet, strbuf, altStrBuf);
    func_ov021_02191830(fieldWork->msgBGSys, strbuf, &width, &height);
    if (x + width > 31) {
        x = 31 - width;
    }
    if (y + height > 24) {
        y = height = 4;
    }
    func_ov021_021918dc(fieldWork->msgBGSys, strbuf, index, x, y, width, height);
    GFL_StrBufFree(strbuf);
    FieldScriptSubEvent_Register(5);
    return FALSE;
}


BOOL s003B_MsgWinCloseNo(VM *vm, FieldScriptEnv *env) {
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(FieldScriptEnv_GetScriptWork(env));

    func_ov021_0219196c(fieldWork->msgBGSys, VM_Read16(vm));
    if (!func_ov021_021919ac(fieldWork->msgBGSys)) {
        FieldScriptSubEvent_Unregister(5);
    }
    return FALSE;
}

static void func_ov021_021b120c(FieldScriptEnv *env) {
    if (FieldScriptSubEvent_IsRegistered(5)) {
        ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(FieldScriptEnv_GetScriptWork(env));

        func_ov021_02191978(fieldWork->msgBGSys);
        FieldScriptSubEvent_Unregister(5);
    }
}

BOOL func_ov021_021b1234(FinishScriptSubEventsWork *work, u32 *state) {
    func_ov021_021b120c(work->env);
    return TRUE;
}

BOOL func_ov021_021b1240(VM *vm, void *env) {
    StrBuf *strbuf = ScriptWork_GetMainStrBuf(FieldScriptEnv_GetScriptWork(env));

    if (func_ov021_02191be8(getMapDisplayInfoPtr(env), strbuf) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

static void func_ov021_021b1268(FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    Font *font;

    if (FieldScriptSubEvent_IsRegistered(4)) {
        func_ov021_02191b14(getMapDisplayInfoPtr(env));
        font = func_ov010_02158f84(work);
        func_ov010_02158f80(work, NULL);
        if (font != NULL) {
            GFL_FontFree(font);
        }
        setMapDisplayInfoPtr(env, NULL);
        FieldScriptSubEvent_Unregister(4);
    }
}

BOOL s0043_MsgPlaceSign(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    u16 msgId = VM_Read16(vm);
    u16 type = VM_Read16(vm);
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);
    MsgData *msgData = GetFieldScriptMsgData(env);
    StrBuf *mainStrBuf;
    WordSet *wordSet;
    StrBuf *altStrBuf;

    setMapDisplayInfoPtr(env, func_ov021_02191a04(fieldWork->msgBGSys, type));
    mainStrBuf = ScriptWork_GetMainStrBuf(work);
    wordSet = ScriptWork_GetWordSet(work);
    altStrBuf = ScriptWork_GetAltStrBuf(work);
    GFL_MsgDataLoadStrbuf(msgData, msgId, altStrBuf);
    GFL_WordSetFormatStrbuf(wordSet, mainStrBuf, altStrBuf);
    VM_SetNativeCallback(vm, func_ov021_021b1240);
    FieldScriptSubEvent_Register(4);
    return TRUE;
}

BOOL func_ov021_021b1334(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);
    u16 msgId = VM_Read16(vm);
    u16 type = VM_Read16(vm);
    HeapID heapId = Field_GetHeapID(fieldWork->field);
    ScriptFieldWork *msgFieldWork = ScriptWork_GetFieldWork(work);
    MsgData *msgData = GetFieldScriptMsgData(env);
    Font *font = GFL_FontCreate(0x17, 4, 0, FALSE, heapId);
    StrBuf *mainStrBuf;
    WordSet *wordSet;
    StrBuf *altStrBuf;

    func_ov010_02158f80(work, font);
    setMapDisplayInfoPtr(env, func_ov021_02191a10(msgFieldWork->msgBGSys, type, font));
    mainStrBuf = ScriptWork_GetMainStrBuf(work);
    wordSet = ScriptWork_GetWordSet(work);
    altStrBuf = ScriptWork_GetAltStrBuf(work);
    GFL_MsgDataLoadStrbuf(msgData, msgId, altStrBuf);
    GFL_WordSetFormatStrbuf(wordSet, mainStrBuf, altStrBuf);
    VM_SetNativeCallback(vm, func_ov021_021b1240);
    FieldScriptSubEvent_Register(4);
    return TRUE;
}


BOOL s0044_MsgPlaceSignClose(VM *vm, FieldScriptEnv *env) {
    func_ov021_021b1268(env);
    return FALSE;
}

BOOL func_ov021_021b13ec(FinishScriptSubEventsWork *work, u32 *state) {
    func_ov021_021b1268(work->env);
    return TRUE;
}

static void func_ov021_021b13f8(FieldScriptEnv *env) {
    if (FieldScriptSubEvent_IsRegistered(6)) {
        func_ov021_02192628(getMapDisplayInfoPtr(env));
        setMapDisplayInfoPtr(env, NULL);
        FieldScriptSubEvent_Unregister(6);
    }
}

BOOL func_ov021_021b1424(VM *vm, void *env) {
    if (func_ov021_021926d8(getMapDisplayInfoPtr(env)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL s0045_CheckerMsg(VM *vm, FieldScriptEnv *env) {
    ScriptWork *work = FieldScriptEnv_GetScriptWork(env);
    u16 msgId = VM_Read16(vm);
    u8 x = VM_Read8(vm);
    u8 y = VM_Read8(vm);
    u16 type = VM_Read16(vm);
    StrBuf *strbuf = func_ov021_021b162c(env, work, msgId);
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work);
    u16 width = func_ov021_0219270c(fieldWork->msgBGSys, strbuf, 4);
    u16 height = func_ov021_0219272c(fieldWork->msgBGSys, strbuf, 4);
    void *window = func_ov021_02192570(fieldWork->msgBGSys, type, x, y, width, height);

    setMapDisplayInfoPtr(env, window);
    func_ov021_0219269c(window, 4, 4, strbuf);
    VM_SetNativeCallback(vm, func_ov021_021b1424);
    FieldScriptSubEvent_Register(6);
    return TRUE;
}


BOOL s0046_CheckerMsgClose(VM *vm, FieldScriptEnv *env) {
    func_ov021_021b13f8(env);
    return FALSE;
}

BOOL func_ov021_021b14e8(FinishScriptSubEventsWork *work, u32 *state) {
    func_ov021_021b13f8(work->env);
    return TRUE;
}

BOOL s003F_MsgWinCloseAll(VM *vm, FieldScriptEnv *env) {
    if (FieldScriptSubEvent_IsRegistered(1)) {
        func_ov021_021b0480(env);
    }
    if (FieldScriptSubEvent_IsRegistered(3)) {
        func_ov021_021b10a4(env);
    }
    if (FieldScriptSubEvent_IsRegistered(4)) {
        func_ov021_021b1268(env);
    }
    if (FieldScriptSubEvent_IsRegistered(6)) {
        func_ov021_021b13f8(env);
    }
    if (FieldScriptSubEvent_IsRegistered(2)) {
        func_ov021_021b0a88(vm, env);
    }
    return TRUE;
}

// Waits for A or B, and skips the open window's printing to the end
static BOOL ScriptNative_AdvanceMessages(VM *vm, void *env) {
    u32 keys = GCTX_HIDGetPressedKeys();
    void *window = getMapDisplayInfoPtr(env);

    if (keys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        GFL_SndSEPlay(0x547);
        return TRUE;
    }
    if (FieldScriptSubEvent_IsRegistered(1)) {
        func_ov021_02191134(window);
    } else if (FieldScriptSubEvent_IsRegistered(3)) {
        func_ov021_02191794(window);
    } else if (FieldScriptSubEvent_IsRegistered(2)) {
        func_ov021_021914cc(window);
    } else {
        return TRUE;
    }
    return FALSE;
}

BOOL s004B_MsgWaitAdvance(VM *vm, FieldScriptEnv *env) {
    VM_SetNativeCallback(vm, ScriptNative_AdvanceMessages);
    return TRUE;
}

BOOL s0033_MsgSetAutoscrolls(VM *vm, FieldScriptEnv *env) {
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(FieldScriptEnv_GetScriptWork(env));

    if (VM_Read16(vm)) {
        func_ov021_021904dc(fieldWork->msgBGSys, TRUE);
        FieldScriptSubEvent_Register(9);
    } else {
        func_ov021_021904dc(fieldWork->msgBGSys, FALSE);
        FieldScriptSubEvent_Unregister(9);
    }
    return FALSE;
}

BOOL func_ov021_021b1604(FinishScriptSubEventsWork *work, u32 *state) {
    ScriptFieldWork *fieldWork = ScriptWork_GetFieldWork(work->scriptWork);

    if (FieldScriptSubEvent_IsRegistered(9)) {
        func_ov021_021904dc(fieldWork->msgBGSys, FALSE);
        FieldScriptSubEvent_Unregister(9);
    }
    return TRUE;
}

StrBuf *func_ov021_021b162c(FieldScriptEnv *env, ScriptWork *work, u32 msgId) {
    MsgData *msgData = GetFieldScriptMsgData(env);
    StrBuf *dest = ScriptWork_GetMainStrBuf(work);
    WordSet *wordSet = ScriptWork_GetWordSet(work);
    StrBuf *strbuf = ScriptWork_GetAltStrBuf(work);

    GFL_MsgDataLoadStrbuf(msgData, msgId, strbuf);
    GFL_WordSetFormatStrbuf(wordSet, dest, strbuf);
    return dest;
}

