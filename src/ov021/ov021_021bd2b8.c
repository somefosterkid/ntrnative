// Black's code at ov021 0x021bd2b8-0x021bdc94. No name is known: the module and address.

#include "types.h"
#include "field/ov021_021bd068.h"
#include "field/ov021_021bd2b8.h"
#include "field/event_entrance_effect.h"
#include "field/event_mapchange.h"
#include "field/field.h"
#include "field/event_action_call.h"
#include "field/field_camera.h"
#include "field/field_event.h"
#include "field/field_player.h"
#include "field/field_sound.h"
#include "gfl/bmp.h"
#include "gfl/heap.h"
#include "gfl/sound.h"
#include "gfl/str.h"
#include "struct_decls.h"
#include "system/bmp_oam.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/iss_dungeon_sys.h"
#include "system/iss_sys.h"
#include "system/printsys.h"

void func_ov021_021bd604(Font *param_1, u16 param_2, HeapID param_3, u8 *param_4, u8 *param_5);
u32 func_ov021_021bd36c(Ov021Work *work);
u32 func_ov021_021bd394(Ov021Work *work);
void func_ov021_021bd3ec(Ov021Work *work);
void func_ov021_021bd430(Ov021Work *work);
void func_ov021_021bd5d4(Ov021Work *work);
void func_ov021_021bd47c(Ov021Work *work);
void func_ov021_021bd484(Ov021Work *work);
// The data of the events that play the entrance when the player goes out of a zone and into one (pokebw2's WarpOutData
// and WarpInData, entrance_camera.c). Black keeps the player's walk out of the door in unk4, which Black 2 leaves 0.
typedef struct {
    u32 unk0;
    FieldAcmdTCB *walkTask;
    WarpSequence *warp;
} WarpOutData;

typedef struct {
    WarpSequence *warp;
} WarpInData;

void func_ov021_021bdc80(WarpInData *in);
void func_ov021_021bd988(WarpOutData *out, GameEvent *event);
u32 EventWarpSequenceOut_CheckISSPersist(WarpOutData *out);
GameEventReturnCode func_ov021_021bd874(GameEvent *event, u32 *state, void *work);
GameEventReturnCode func_ov021_021bda70(GameEvent *event, u32 *state, void *work);
GameEventReturnCode func_ov021_021bdb1c(GameEvent *event, u32 *state, void *work);
GameEventReturnCode func_ov021_021bdb9c(GameEvent *event, u32 *state, void *work);
u32 func_ov021_021bd8c0(WarpOutData *out);
void func_ov021_021bd96c(WarpOutData *out, GameEvent *event);
u32 EventWarpSequenceOut_GetPlayerExState(WarpOutData *out);
void func_ov021_021bdc2c(WarpInData *in, GameEvent *event);
void func_ov021_021bdc0c(WarpInData *in, GameEvent *event);
void func_ov021_021bdbf8(WarpInData *in);
GameEvent *EventBGMFadeOut_Create(GameSystem *param_1, u16 param_2);
GameEvent *EventBGMFadeIn_Create(GameSystem *param_1, u16 param_2);
void EventWarpSequenceOut_ChangeToZoneBGM(WarpOutData *out);
GameEventReturnCode func_ov021_021bd6c0(GameEvent *event, u32 *state, void *work);
GameEventReturnCode func_ov021_021bd7a8(GameEvent *event, u32 *state, void *work);
GameEventReturnCode func_ov021_021bd704(GameEvent *event, u32 *state, void *work);
GameEventReturnCode func_ov021_021bd734(GameEvent *event, u32 *state, void *work);
GameEventReturnCode func_ov021_021bd7e0(GameEvent *event, u32 *state, void *work);
GameEventReturnCode func_ov021_021bda30(GameEvent *event, u32 *state, void *work);
GameEventReturnCode func_ov021_021bdaa8(GameEvent *event, u32 *state, void *work);
GameEventReturnCode func_ov021_021bdb5c(GameEvent *event, u32 *state, void *work);

u16 func_ov021_021bd2b8(Ov021Work *work) {
    return work->heapId;
}

void func_ov021_021bd2bc(Ov021Work *work, u16 heapId) {
    work->heapId = heapId;
}

Font *func_ov021_021bd2c0(Ov021Work *work) {
    return work->font;
}

void func_ov021_021bd2c4(Ov021Work *work, Font *font) {
    work->font = font;
}

u16 func_ov021_021bd2c8(Ov021Work *work) {
    return work->letter;
}

void func_ov021_021bd2d0(Ov021Work *work, u16 letter) {
    work->letter = letter;
}

u32 func_ov021_021bd2d8(Ov021Work *work) {
    return work->active;
}

u32 func_ov021_021bd2dc(Ov021Work *work) {
    return (s32)work->motion.pos[0];
}

u32 func_ov021_021bd2e8(Ov021Work *work) {
    return (s32)work->motion.pos[1];
}

u8 func_ov021_021bd2f4(Ov021Work *work) {
    return work->width;
}

u8 func_ov021_021bd2fc(Ov021Work *work) {
    return work->height;
}

Ov021Motion *func_ov021_021bd304(Ov021Work *work) {
    return &work->motion;
}

GFLBitmap *func_ov021_021bd308(Ov021Work *work) {
    return work->bitmap;
}

void func_ov021_021bd30c(Ov021Work *work) {
    Font *font;
    u16 letter;
    HeapID heapId;

    font = func_ov021_021bd2c0(work);
    letter = func_ov021_021bd2c8(work);
    heapId = func_ov021_021bd2b8(work);
    func_ov021_021bd604(font, letter, heapId, &work->width, &work->height);
}

u32 func_ov021_021bd340(Ov021Work *work) {
    s32 result;

    result = func_ov021_021bd2d8(work);
    if (result == 0) {
        return 1;
    }
    result = func_ov021_021bd36c(work);
    if ((result != 0) && (result = func_ov021_021bd394(work), result != 0)) {
        return 1;
    }
    return 0;
}

u32 func_ov021_021bd36c(Ov021Work *work) {
    Ov021Motion *motion;

    motion = func_ov021_021bd304(work);
    if (func_ov021_021bd2d8(work) == 0) {
        return 1;
    }
    if (motion->frames[0] > 0) {
        return 0;
    }
    return 1;
}

u32 func_ov021_021bd394(Ov021Work *work) {
    Ov021Motion *motion;

    motion = func_ov021_021bd304(work);
    if (func_ov021_021bd2d8(work) == 0) {
        return 1;
    }
    if (motion->frames[1] > 0) {
        return 0;
    }
    return 1;
}



void func_ov021_021bd3bc(Ov021Work *work) {
    s32 result;

    result = func_ov021_021bd2d8(work);
    if (result != 0) {
        func_ov021_021bd3ec(work);
        func_ov021_021bd430(work);
        func_ov021_021bd5d4(work);
        result = func_ov021_021bd340(work);
        if (result == 1) {
            func_ov021_021bd47c(work);
        }
    }
}

void func_ov021_021bd3ec(Ov021Work *work) {
    Ov021Motion *motion;
    s32 frames;

    frames = func_ov021_021bd36c(work);
    if (frames != 1) {
        motion = &work->motion;
        motion->pos[0] = motion->pos[0] + (motion->vel[0] + motion->acc[0] / 2.0f);
        motion->vel[0] = motion->vel[0] + motion->acc[0];
        frames = motion->frames[0] - 1;
        motion->frames[0] = frames;
        if (frames == 0) {
            motion->pos[0] = motion->target[0];
        }
    }
}

void func_ov021_021bd430(Ov021Work *work) {
    Ov021Motion *motion;
    s32 frames;

    frames = func_ov021_021bd394(work);
    if (frames != 1) {
        motion = &work->motion;
        motion->pos[1] = motion->pos[1] + (motion->vel[1] + motion->acc[1] / 2.0f);
        motion->vel[1] = motion->vel[1] + motion->acc[1];
        frames = motion->frames[1] - 1;
        motion->frames[1] = frames;
        if (frames == 0) {
            motion->pos[1] = motion->target[1];
        }
    }
}



void func_ov021_021bd474(Ov021Work *work) {
    work->active = 1;
}

void func_ov021_021bd47c(Ov021Work *work) {
    work->active = 0;
}

void func_ov021_021bd484(Ov021Work *work) {
    f32 posItem;
    f32 posItem2;
    Ov021Motion *motion;

    motion = &work->motion;
    posItem = motion->target[0] - motion->pos[0];
    posItem2 = motion->target[1] - motion->pos[1];
    if (posItem != 0.0f) {
        motion->acc[0] = (motion->endVel[0] * motion->endVel[0] - motion->vel[0] * motion->vel[0]) / (2.0f * posItem);
    }
    if (posItem2 != 0.0f) {
        motion->acc[1] = (motion->endVel[1] * motion->endVel[1] - motion->vel[1] * motion->vel[1]) / (2.0f * posItem2);
    }
    if (motion->acc[0] != 0.0f) {
        motion->frames[0] = (s32)((motion->endVel[0] - motion->vel[0]) / motion->acc[0]);
    }
    if (motion->acc[1] != 0.0f) {
        motion->frames[1] = (s32)((motion->endVel[1] - motion->vel[1]) / motion->acc[1]);
    }
}


void func_ov021_021bd554(Ov021Work *work, const PlaceNameWidths *args) {
    work->motion.pos[0] = args->unk_0[0];
    work->motion.pos[1] = args->unk_0[1];
    work->motion.vel[0] = args->unk_0[2];
    work->motion.vel[1] = args->unk_0[3];
    work->motion.target[0] = args->unk_0[4];
    work->motion.target[1] = args->unk_0[5];
    work->motion.endVel[0] = args->unk_0[6];
    work->motion.endVel[1] = args->unk_0[7];
    func_ov021_021bd484(work);
}


void func_ov021_021bd57c(Ov021Work *work) {
    GFL_BitmapFill(work->bitmap, 0);
}

void func_ov021_021bd588(Ov021Work *work) {
    u16 str[2];
    StrBuf *strbuf;

    str[0] = func_ov021_021bd2c8(work);
    str[1] = GFL_StrBufGetTerminator();
    strbuf = GFL_StrBufCreate(2, work->heapId);
    GFL_StrBufLoadFixedString(strbuf, str, 2);
    GFL_TextRendererDrawToBitmapEx(work->bitmap, 0, 0, strbuf, work->font, 0x440);
    func_020257d0(work->actor);
    GFL_StrBufFree(strbuf);
}


void func_ov021_021bd5d4(Ov021Work *work) {
    Ov021Motion *motion;

    motion = func_ov021_021bd304(work);
    func_0202590c(work->actor, motion->pos[0], motion->pos[1]);
}


void func_ov021_021bd5f8(Ov021Work *work, BOOL param_2) {
    func_02025774(work->actor, param_2);
}

void func_ov021_021bd604(Font *param_1, u16 param_2, HeapID param_3, u8 *param_4, u8 *param_5) {
    u16 str[2];
    StrBuf *strbuf;

    str[0] = param_2;
    str[1] = GFL_StrBufGetTerminator();
    strbuf = GFL_StrBufCreate(2, param_3);
    GFL_StrBufLoadFixedString(strbuf, str, 2);
    *param_4 = GFL_FontGetBlockWidth(strbuf, param_1, 0);
    *param_5 = GFL_FontGetBlockHeight(strbuf, param_1);
    GFL_StrBufFree(strbuf);
}

GameEvent *EventWarpSequence_CreateOut(WarpSequence *warp) {
    GameEvent *event;
    const GameEventCallback callbacks[6] = {
        func_ov021_021bd6c0, func_ov021_021bd704, func_ov021_021bd734,
        func_ov021_021bd7a8, func_ov021_021bd7e0, func_ov021_021bd874,
    };
    s32 idx;
    WarpOutData *data;

    switch (warp->unk10) {
    case 0:
        idx = 0;
        break;
    case 1:
        idx = 0;
        break;
    case 2:
        idx = 2;
        break;
    case 3:
        idx = 1;
        break;
    case 4:
        idx = 2;
        break;
    case 5:
        idx = 3;
        break;
    case 6:
        idx = 5;
        break;
    default:
        idx = 4;
        break;
    }
    event = GameEvent_Create(warp->gsys, warp->parent, callbacks[idx], sizeof(WarpOutData));
    data = GameEvent_GetData(event);
    data->walkTask = NULL;
    data->warp = warp;
    return event;
}


GameEventReturnCode func_ov021_021bd6c0(GameEvent *event, u32 *state, void *work) {
    WarpOutData *out = work;
    WarpSequence *warp = out->warp;

    switch (*state) {
    case 0:
        EventWarpSequenceOut_ChangeToZoneBGM(out);
        if (warp->outTransition != 2) {
            GFL_SndSEPlay(0x559);
        }
        func_ov021_021bd988(out, event);
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}



GameEventReturnCode func_ov021_021bd704(GameEvent *event, u32 *state, void *work) {
    WarpOutData *out = work;
    WarpSequence *warp = out->warp;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, func_ov021_021a7e3c(warp));
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bd734(GameEvent *event, u32 *state, void *work) {
    WarpOutData *out = work;
    WarpSequence *warp = out->warp;
    Field *field = warp->field;

    switch (*state) {
    case 0:
        if (func_ov021_021bd8c0(out) != 0) {
            func_ov021_021bd96c(out, event);
        } else {
            EventWarpSequenceOut_ChangeToZoneBGM(out);
        }
        out->walkTask = func_ov010_02161b1c(field);
        (*state)++;
        break;
    case 1:
        if (func_ov010_02161b74(out->walkTask) == 0) {
            break;
        }
        if (warp->outTransition != 2) {
            GFL_SndSEPlay(0x559);
        }
        func_ov021_021bd988(out, event);
        (*state)++;
        break;
    case 2:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode func_ov021_021bd7a8(GameEvent *event, u32 *state, void *work) {
    WarpOutData *out = work;
    GameSystem *gsys = out->warp->gsys;
    Field *field = out->warp->field;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, func_ov021_021bfe00(NULL, gsys, field));
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bd7e0(GameEvent *event, u32 *state, void *work) {
    WarpOutData *out = work;
    WarpSequence *warp = out->warp;
    FieldCamera *camera = Field_GetCameraSystem(warp->field);

    switch (*state) {
    case 0:
        if (FieldCamera_SupportsDelay(camera) && FieldCamera_IsDelayActive(camera)) {
            FieldCamera_FinishDelay(camera);
            *state = 1;
        } else {
            *state = 2;
        }
        return GAMEEVENT_CONTINUE_DIRECT;
    case 1:
        if (!FieldCamera_SupportsDelay(camera)) {
            *state = 2;
            return GAMEEVENT_CONTINUE_DIRECT;
        }
        if (!FieldCamera_IsDelayActive(camera)) {
            FieldCamera_ClearBind(camera);
            *state = 2;
            return GAMEEVENT_CONTINUE_DIRECT;
        }
        break;
    case 2:
        GameEvent_ChainNext(event, func_ov021_021a7e3c(warp));
        *state = 3;
        return GAMEEVENT_CONTINUE_DIRECT;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode func_ov021_021bd874(GameEvent *event, u32 *state, void *work) {
    WarpOutData *out = work;
    GameSystem *gsys = out->warp->gsys;
    Field *field = out->warp->field;

    switch (*state) {
    case 0:
        EventWarpSequenceOut_ChangeToZoneBGM(out);
        GFL_SndSEPlay(0x737);
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, 2, 0));
        (*state)++;
        break;
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


// Whether the BGM keeps playing into the next zone (pokebw2's EventWarpSequenceOut_ShouldKeepBGM)
u32 func_ov021_021bd8c0(WarpOutData *out) {
    WarpSequence *warp = out->warp;
    GameData *gameData = warp->gameData;

    if (EventWarpSequenceOut_CheckISSPersist(out) == 0) {
        return 0;
    }
    if (GameData_IsZoneBGMOverriden(gameData, (u16)warp->spawn.zoneId) != 0) {
        return 0;
    }
    if (EventWarpSequenceOut_GetPlayerExState(out) == 1 && GameData_GetNowBGM(gameData) == 0x3f4) {
        return 0;
    }
    return 1;
}




u32 EventWarpSequenceOut_CheckISSPersist(WarpOutData *out) {
    WarpSequence *warp = out->warp;
    ISSDungeonSys *sys = ISS_GetDungeonSys(GameSystem_GetISS(warp->gsys));

    if (ISSDungeonSys_IsEnabled(sys) && ISSDungeonSys_IsZoneRegist(sys, warp->zoneId) &&
        ISSDungeonSys_IsZoneRegist(sys, warp->spawn.zoneId)) {
        return 1;
    }
    return 0;
}

u32 EventWarpSequenceOut_GetPlayerExState(WarpOutData *out) {
    return FieldPlayer_GetExState(Field_GetPlayer(out->warp->field));
}

void EventWarpSequenceOut_ChangeToZoneBGM(WarpOutData *out) {
    WarpSequence *warp = out->warp;
    GameData *gameData = warp->gameData;

    FieldSnd_SetZoneBGM(GameData_GetFieldSoundSystem(gameData), gameData, warp->spawn.zoneId, warp->endSeason);
    warp->unk48 = 1;
}

// pokebw2's EventWarpSequenceOut_FadeOutBGM
void func_ov021_021bd96c(WarpOutData *out, GameEvent *event) {
    WarpSequence *warp = out->warp;

    GameEvent_ChainNext(event, EventBGMFadeOut_Create(warp->gsys, 0x1e));
    warp->unk4C = 1;
}

// pokebw2's EventWarpSequenceOut_PlayTransition
void func_ov021_021bd988(WarpOutData *out, GameEvent *event) {
    WarpSequence *warp = out->warp;
    GameSystem *gsys = warp->gsys;
    Field *field = warp->field;

    if (warp->seasonChanged) {
        GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, 0, 0));
        return;
    }
    GameEvent_ChainNext(event, CallFieldMapEntranceOutTransitionDefault(gsys, field, warp->outTransition, 0));
}

GameEvent *EventWarpSequence_CreateIn(WarpSequence *warp) {
    GameEvent *event;
    const GameEventCallback callbacks[6] = {
        func_ov021_021bda30, func_ov021_021bda70, func_ov021_021bdaa8,
        func_ov021_021bdb1c, func_ov021_021bdb5c, func_ov021_021bdb9c,
    };
    s32 idx;
    WarpInData *data;

    switch (warp->transitionType) {
    case 0:
        idx = 0;
        break;
    case 1:
        idx = 0;
        break;
    case 2:
        idx = 2;
        break;
    case 3:
        idx = 1;
        break;
    case 4:
        idx = 2;
        break;
    case 5:
        idx = 3;
        break;
    case 6:
        idx = 5;
        break;
    default:
        idx = 4;
        break;
    }
    event = GameEvent_Create(warp->gsys, warp->parent, callbacks[idx], sizeof(WarpInData));
    data = GameEvent_GetData(event);
    data->warp = warp;
    return event;
}


GameEventReturnCode func_ov021_021bda30(GameEvent *event, u32 *state, void *work) {
    WarpInData *in = work;
    WarpSequence *warp = in->warp;

    switch (*state) {
    case 0:
        if (warp->unk48 != 0) {
            func_ov021_021bdbf8(in);
        }
        func_ov021_021bdc2c(in, event);
        (*state)++;
        break;
    case 1:
        func_ov021_021bdc80(in);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode func_ov021_021bda70(GameEvent *event, u32 *state, void *work) {
    WarpInData *in = work;
    WarpSequence *warp = in->warp;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, func_ov021_021a7b20(warp));
        (*state)++;
        break;
    case 1:
        func_ov021_021bdc80(in);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bdaa8(GameEvent *event, u32 *state, void *work) {
    WarpInData *in = work;
    WarpSequence *warp = in->warp;
    GameSystem *gsys = warp->gsys;
    Field *field = warp->field;

    switch (*state) {
    case 0:
        func_ov021_021bdc2c(in, event);
        (*state)++;
        break;
    case 1:
        if (warp->unk48 != 0) {
            func_ov021_021bdbf8(in);
        } else if (warp->unk4C != 0) {
            func_ov021_021bdc0c(in, event);
        }
        (*state)++;
        break;
    case 2:
        GameEvent_ChainNext(event, CallMoveOneTileFrontEvent(gsys, field));
        (*state)++;
        break;
    case 3:
        func_ov021_021bdc80(in);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}


GameEventReturnCode func_ov021_021bdb1c(GameEvent *event, u32 *state, void *work) {
    WarpInData *in = work;
    GameSystem *gsys = in->warp->gsys;
    Field *field = in->warp->field;

    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, EventPlayerSpinDown_Create(NULL, gsys, field));
        (*state)++;
        break;
    case 1:
        func_ov021_021bdc80(in);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bdb5c(GameEvent *event, u32 *state, void *work) {
    WarpInData *in = work;
    WarpSequence *warp = in->warp;

    Field_GetCameraSystem(warp->field);
    switch (*state) {
    case 0:
        GameEvent_ChainNext(event, func_ov021_021a7b20(warp));
        (*state)++;
        break;
    case 1:
        func_ov021_021bdc80(in);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bdb9c(GameEvent *event, u32 *state, void *work) {
    WarpInData *in = work;
    WarpSequence *warp = in->warp;
    GameSystem *gsys = warp->gsys;
    Field *field = warp->field;

    switch (*state) {
    case 0:
        if (warp->unk48 != 0) {
            func_ov021_021bdbf8(in);
        }
        (*state)++;
        break;
    case 1:
        GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, 2, 0, 0, 0, 0));
        (*state)++;
        break;
    case 2:
        func_ov021_021bdc80(in);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}






// pokebw2's EventWarpSequenceIn_FadeInBGM
void func_ov021_021bdbf8(WarpInData *in) {
    GameData *gameData = in->warp->gameData;

    FieldSnd_FadeInImmediate(GameData_GetFieldSoundSystem(gameData), gameData);
}

// pokebw2's EventWarpSequenceIn_ChainBGMFadeIn
void func_ov021_021bdc0c(WarpInData *in, GameEvent *event) {
    WarpSequence *warp = in->warp;
    GameSystem *gsys = warp->gsys;

    GameData_GetFieldSoundSystem(warp->gameData);
    GameEvent_ChainNext(event, EventBGMFadeIn_Create(gsys, 6));
}

// pokebw2's EventWarpSequenceIn_PlayTransition
void func_ov021_021bdc2c(WarpInData *in, GameEvent *event) {
    WarpSequence *warp = in->warp;
    GameSystem *gsys = warp->gsys;
    Field *field = warp->field;

    if (warp->seasonChanged) {
        GameEvent_ChainNext(event,
                            CallFieldMapEntranceInTransition(gsys, field, 3, 0, 0, warp->startSeason, warp->endSeason));
        return;
    }
    GameEvent_ChainNext(event, CallFieldMapEntranceInTransition(gsys, field, warp->inTransition, 0, 1, 0, 0));
}


// pokebw2's EventWarpSequenceIn_ShowPlaceName
void func_ov021_021bdc80(WarpInData *in) {
    WarpSequence *warp = in->warp;

    func_ov021_021bc550(Field_GetPlaceName(warp->field), warp->spawn.zoneId);
}
