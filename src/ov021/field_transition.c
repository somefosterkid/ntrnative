// Black's code at ov021 0x021be89c-0x021bf58c: pokebw2's field_transition.c (its functions, in its order)

#include "types.h"
#include "field/event_mapchange.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/field_task.h"
#include "field/zone.h"
#include "gfl/gx_layers.h"
#include "struct_decls.h"
#include "system/game_event.h"
#include "field/field_camera.h"
#include "field/event_season_banner.h"
#include "gfl/fade.h"
#include "gfl/graphics.h"
#include "nitro/os.h"
#include "field/building_enter_effect.h"
#include "system/wipe.h"
#include "gfl/sound.h"
#include "field/field_actor.h"
#include "field/field_effect.h"
#include "field/field_g3d_mapper.h"
#include "field/field_prop.h"
#include "gfl/key.h"

// The work of every transition event (pokebw2's TransitionEvent)
typedef struct {
    GameSystem *gsys;
    Field *field;
    u32 type;
    u32 fadeMode;
    u32 fadeSlowness;
    u32 skipWait;
    u32 keepLCD;
    u32 alpha;
    u8 startSeason;
    u8 endSeason;
    EventSeasonBannerCallback callback;
    void *callbackArg;
} TransitionEvent;

u32 func_ov021_021bea9c(s32 param_1, s32 param_2);
u32 func_ov021_021beacc(s32 param_1, s32 param_2);
u32 func_ov021_021bea6c(u32 param_1);
GameEventReturnCode func_ov021_021bee74(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021beeb4(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bef08(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bef8c(GameEvent *event, u32 *state, void *data);
GameEventReturnCode EventGameTransition_SeasonBanner_Callback(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bf0e4(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bf130(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bf19c(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bf208(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bf274(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bf2e0(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bf348(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bf3bc(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bf430(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bf4a4(GameEvent *event, u32 *state, void *data);
GameEventReturnCode func_ov021_021bf518(GameEvent *event, u32 *state, void *data);
void func_ov021_021bf564(Field *field);
void func_ov021_021bf584(Field *field);
void func_ov021_02189a58(Field *field, BOOL flag);
void func_ov021_02188df0(Field *field);
u32 func_ov021_021903cc(void *);
void func_ov021_021902ac(FieldMsgBG *param_1, s32 param_2);
GameEvent *func_ov021_021beafc(GameSystem *gsys, Field *field, u32 type, u32 skipWait);
GameEvent *func_ov021_021beb38(GameSystem *gsys, Field *field, u32 type, u32 skipWait, u32 keepLCD);
GameEvent *EventGameTransition_SeasonBanner_Create(GameSystem *gsys, Field *field, u8 startSeason, u8 endSeason);
GameEvent *func_ov021_021beb78(GameSystem *gsys, Field *field);
GameEvent *func_ov021_021beba8(GameSystem *gsys, Field *field);
GameEvent *func_ov021_021bec24(GameSystem *gsys, Field *field);
GameEvent *func_ov021_021bec54(GameSystem *gsys, Field *field);
GameEvent *func_ov021_021bec84(GameSystem *gsys, Field *field);
GameEvent *func_ov021_021becb4(GameSystem *gsys, Field *field);
GameEvent *func_ov021_021bece4(GameSystem *gsys, Field *field);
GameEvent *func_ov021_021bed14(GameSystem *gsys, Field *field);
GameEvent *func_ov021_021bed44(GameSystem *gsys, Field *field);
GameEvent *func_ov021_021bed74(GameSystem *gsys, Field *field);
GameEvent *func_ov021_021beda4(GameSystem *gsys, Field *field);
GameEvent *func_ov021_021bedd4(GameSystem *gsys, Field *field);
GameEvent *func_ov021_021bee24(GameSystem *gsys, Field *field);

u32 GetOutTransitionTypeBetweenZones(u16 fromZone, u16 toZone) {
    u32 result;

    result = func_02013fd8(fromZone);
    result = func_ov021_021bea9c(result, func_02013fd8(toZone));
    return result;
}

u32 func_ov021_021be8b8(u16 param_1, u16 param_2) {
    return func_ov021_021beacc(func_02013fd8(param_1), func_02013fd8(param_2));
}

GameEvent *CallFieldMapEntranceOutTransitionDefault(GameSystem *gsys, Field *field, u32 type, u32 a3) {
    switch (type) {
    case 2:
    default:
        return func_ov021_021beb78(gsys, field);
    case 0:
    case 1:
    case 3:
        return func_ov021_021beafc(gsys, field, type, a3);
    case 4:
        return func_ov021_021bec24(gsys, field);
    case 5:
        return func_ov021_021bec54(gsys, field);
    case 6:
        return func_ov021_021becb4(gsys, field);
    case 7:
        return func_ov021_021bed14(gsys, field);
    case 8:
        return func_ov021_021bed74(gsys, field);
    case 9:
        return func_ov021_021bedd4(gsys, field);
    }
}

GameEvent *CallFieldMapEntranceInTransition(GameSystem *gsys, Field *field, u32 type, u32 a3, u32 a4, u8 startSeason,
                                            u8 endSeason) {
    switch (type) {
    case 2:
    case 4:
    default:
        return func_ov021_021beba8(gsys, field);
    case 0:
    case 1:
        return func_ov021_021beb38(gsys, field, type, a3, a4);
    case 3:
        return EventGameTransition_SeasonBanner_Create(gsys, field, startSeason, endSeason);
    case 5:
        return func_ov021_021bec84(gsys, field);
    case 6:
        return func_ov021_021bece4(gsys, field);
    case 7:
        return func_ov021_021bed44(gsys, field);
    case 8:
        return func_ov021_021beda4(gsys, field);
    case 9:
        return func_ov021_021bee24(gsys, field);
    }
}

GameEvent *func_ov021_021be9a0(GameSystem *param_1, Field *param_2, u8 param_3, u8 param_4,
                               EventSeasonBannerCallback param_5, void *param_6) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021bf0e4, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = 3;
    data->startSeason = param_3;
    data->endSeason = param_4;
    data->fadeMode = func_ov021_021bea6c(data->type);
    data->callback = param_5;
    data->callbackArg = param_6;
    return event;
}

GameEvent *func_ov021_021be9f0(GameSystem *param_1, Field *param_2, u32 param_3, u32 param_4, u32 param_5) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021bf518, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = param_3;
    data->skipWait = param_4;
    data->fadeMode = func_ov021_021bea6c(param_3);
    data->fadeSlowness = param_5;
    return event;
}

GameEvent *func_ov021_021bea30(GameSystem *gsys, Field *field, u32 a2, u32 a3) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(gsys, NULL, func_ov021_021bee74, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = gsys;
    data->field = field;
    data->type = a2;
    data->skipWait = a3;
    data->fadeMode = func_ov021_021bea6c(a2);
    return event;
}

u32 func_ov021_021bea6c(u32 param_1) {
    switch(param_1) {
    default:
        return 3;
    case 1:
        return 0xc;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        return 2;
    }
}

u32 func_ov021_021bea9c(s32 param_1, s32 param_2) {
    u32 table[6][6] = {
#include "asset/field_transition_table.inc"
};

    return table[param_1][param_2];
}

u32 func_ov021_021beacc(s32 param_1, s32 param_2) {
    u32 table[6][6] = {
#include "asset/field_transition_table_2.inc"
};

    return table[param_1][param_2];
}

GameEvent *func_ov021_021beafc(GameSystem *param_1, Field *param_2, u32 param_3, u32 param_4) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021bee74, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = param_3;
    data->skipWait = param_4;
    data->fadeMode = func_ov021_021bea6c(param_3);
    return event;
}

GameEvent *func_ov021_021beb38(GameSystem *param_1, Field *param_2, u32 param_3, u32 param_4, u32 param_5) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021beeb4, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = param_3;
    data->skipWait = param_4;
    data->fadeMode = func_ov021_021bea6c(param_3);
    data->keepLCD = param_5;
    return event;
}

GameEvent *func_ov021_021beb78(GameSystem *param_1, Field *param_2) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021bef08, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = 2;
    data->fadeMode = func_ov021_021bea6c(2);
    return event;
}

GameEvent *func_ov021_021beba8(GameSystem *param_1, Field *param_2) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021bef8c, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = 2;
    data->fadeMode = func_ov021_021bea6c(2);
    return event;
}

GameEvent *EventGameTransition_SeasonBanner_Create(GameSystem *param_1, Field *param_2, u8 param_3, u8 param_4) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, EventGameTransition_SeasonBanner_Callback, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = 3;
    data->startSeason = param_3;
    data->endSeason = param_4;
    data->fadeMode = func_ov021_021bea6c(data->type);
    data->callback = NULL;
    data->callbackArg = NULL;
    return event;
}

GameEvent *func_ov021_021bec24(GameSystem *param_1, Field *param_2) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021bf130, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = 4;
    data->fadeMode = func_ov021_021bea6c(4);
    return event;
}

GameEvent *func_ov021_021bec54(GameSystem *param_1, Field *param_2) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021bf19c, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = 5;
    data->fadeMode = func_ov021_021bea6c(5);
    return event;
}

GameEvent *func_ov021_021bec84(GameSystem *param_1, Field *param_2) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021bf348, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = 5;
    data->fadeMode = func_ov021_021bea6c(5);
    return event;
}

GameEvent *func_ov021_021becb4(GameSystem *param_1, Field *param_2) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021bf208, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = 6;
    data->fadeMode = func_ov021_021bea6c(6);
    return event;
}

GameEvent *func_ov021_021bece4(GameSystem *param_1, Field *param_2) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021bf3bc, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = 6;
    data->fadeMode = func_ov021_021bea6c(6);
    return event;
}

GameEvent *func_ov021_021bed14(GameSystem *param_1, Field *param_2) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021bf274, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = 7;
    data->fadeMode = func_ov021_021bea6c(7);
    return event;
}

GameEvent *func_ov021_021bed44(GameSystem *param_1, Field *param_2) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021bf430, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = 7;
    data->fadeMode = func_ov021_021bea6c(7);
    return event;
}

GameEvent *func_ov021_021bed74(GameSystem *param_1, Field *param_2) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021bf2e0, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = 8;
    data->fadeMode = func_ov021_021bea6c(8);
    return event;
}

GameEvent *func_ov021_021beda4(GameSystem *param_1, Field *param_2) {
    GameEvent *event;
    TransitionEvent *data;

    event = GameEvent_Create(param_1, NULL, func_ov021_021bf4a4, sizeof(TransitionEvent));
    data = GameEvent_GetData(event);
    data->gsys = param_1;
    data->field = param_2;
    data->type = 8;
    data->fadeMode = func_ov021_021bea6c(8);
    return event;
}

GameEvent *func_ov021_021bedd4(GameSystem *gsys, Field *field) {
    switch (FieldPlayer_GetFaceDir(Field_GetPlayer(field))) {
    case 2:
    default:
        return func_ov021_021bed14(gsys, field);
    case 3:
        return func_ov021_021bed74(gsys, field);
    case 0:
        return func_ov021_021bec54(gsys, field);
    case 1:
        return func_ov021_021becb4(gsys, field);
    }
}

GameEvent *func_ov021_021bee24(GameSystem *gsys, Field *field) {
    switch (FieldPlayer_GetFaceDir(Field_GetPlayer(field))) {
    case 2:
    default:
        return func_ov021_021bed44(gsys, field);
    case 3:
        return func_ov021_021beda4(gsys, field);
    case 0:
        return func_ov021_021bec84(gsys, field);
    case 1:
        return func_ov021_021bece4(gsys, field);
    }
}

GameEventReturnCode func_ov021_021bee74(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    switch (*state) {
    case 0:
        GFL_FadeSet(work->fadeMode, 0, 0x10, -1);
        (*state)++;
        break;
    case 1:
        if (work->skipWait == 1) {
            return GAMEEVENT_DONE;
        }
        if (GFL_FadeIsRunning() == 0) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021beeb4(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    switch (*state) {
    case 0:
        if (work->keepLCD != 0) {
            func_ov021_021bf564(work->field);
            func_ov021_021bf584(work->field);
        }
        GFL_FadeSet(work->fadeMode, 0x10, 0, -1);
        (*state)++;
        break;
    case 1:
        if (work->skipWait == 1) {
            return GAMEEVENT_DONE;
        }
        if (GFL_FadeIsRunning() == 0) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bef08(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    if (*state == 0) {
        GFL_FadeSet(work->fadeMode, 0, 0x10, -1);
        GX_SetBankForLCDC(8);
        reg_GX_DISPCAPCNT = 0x80330010;
        OS_WaitVBlankIntr();
        OS_WaitVBlankIntr();
        {
            BGSysLCDConfig config = { 1, 5, 0, 1 };

            GFL_BGSysSetLCDConfig(&config);
        }
        reg_G2_BG2CNT = (reg_G2_BG2CNT & 0x43) | 0x4084;
        GX_SetBankForBG(8);
        GFL_BGSysSetBGPriority(2, 0);
        GFL_BGSysSetBGEnabled(2, 1);
        setBuildingEnterVBlankCallback();
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bef8c(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    FieldMsgBG *msgBGSys;

    switch (*state) {
    case 0:
        func_ov021_02189a58(work->field, 1);
        work->alpha = 0x10;
        GFL_BGSysSetBGPriority(2, 0);
        GFL_BGSysSetBGPriority(0, 1);
        GFL_BGSysSetBGEnabled(0, 1);
        G2_SetBlendAlpha(4, 1, work->alpha, 0);
        GFL_FadeSet(work->fadeMode, 0x10, 0, -1);
        (*state)++;
        break;
    case 1:
        if (work->alpha != 0) {
            u32 alpha = work->alpha - 1;
            work->alpha = alpha;
            reg_G2_BLDALPHA = alpha | ((0x10 - alpha) << 8);
            break;
        }
        GFL_BGSysSetBGEnabled(2, 0);
        GX_SetBankForLCDC(8);
        MIi_CpuClearFast(0, (void *)HW_LCDC_VRAM, 0x20000);
        GX_DisableBankForLCDC();
        GX_SetBankForBG(8);
        func_ov010_02166b2c();
        (*state)++;
        break;
    case 2:
        func_ov021_021bf564(work->field);
        (*state)++;
        break;
    case 3:
        func_ov021_02188df0(work->field);
        (*state)++;
        break;
    case 4:
        msgBGSys = Field_GetMsgBGSys(work->field);
        if (func_ov021_021903cc(msgBGSys) != 0) {
            func_ov021_021902ac(msgBGSys, 0);
            (*state)++;
        }
        break;
    case 5:
        msgBGSys = Field_GetMsgBGSys(work->field);
        func_ov021_021902ac(msgBGSys, 1);
        (*state)++;
        break;
    case 6:
        msgBGSys = Field_GetMsgBGSys(work->field);
        func_ov021_021902ac(msgBGSys, 2);
        (*state)++;
        break;
    case 7:
        (*state)++;
        /* fall through */
    case 8:
        func_ov021_02189a58(work->field, 0);
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode EventGameTransition_SeasonBanner_Callback(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    GameSystem *gsys = work->gsys;
    Field *field = work->field;

    switch (*state) {
    case 0: {
        GameEvent *next = EventSeasonBanner_CreateFieldTransition(gsys, field, work->startSeason, work->endSeason);
        GameEvent_ChainNext(event, next);
        (*state)++;
        break;
    }
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bf0e4(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    GameSystem *gsys = work->gsys;
    Field *field = work->field;

    switch (*state) {
    case 0: {
        GameEvent *next = EventSeasonBanner_CreateFieldTransitionEx(gsys, field, work->startSeason, work->endSeason,
                                                                    work->callback, work->callbackArg);
        GameEvent_ChainNext(event, next);
        (*state)++;
        break;
    }
    case 1:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bf130(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    Field *field = work->field;

    switch (*state) {
    case 0:
        GFL_FadeSet(work->fadeMode, 0, 0x10, -1);
        (*state)++;
        break;
    case 1:
        GFL_WipeSet(3, 8, 8, 0, 6, 1, Field_GetHeapID(field));
        (*state)++;
        break;
    case 2:
        if (GFL_WipeIsFinished() != 0) {
            (*state)++;
        }
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bf19c(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    Field *field = work->field;

    switch (*state) {
    case 0:
        GFL_FadeSet(work->fadeMode, 0, 0x10, -1);
        (*state)++;
        break;
    case 1:
        GFL_WipeSet(3, 2, 2, 0, 6, 1, Field_GetHeapID(field));
        (*state)++;
        break;
    case 2:
        if (GFL_WipeIsFinished() != 0) {
            (*state)++;
        }
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bf208(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    Field *field = work->field;

    switch (*state) {
    case 0:
        GFL_FadeSet(work->fadeMode, 0, 0x10, -1);
        (*state)++;
        break;
    case 1:
        GFL_WipeSet(3, 4, 4, 0, 6, 1, Field_GetHeapID(field));
        (*state)++;
        break;
    case 2:
        if (GFL_WipeIsFinished() != 0) {
            (*state)++;
        }
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bf274(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    Field *field = work->field;

    switch (*state) {
    case 0:
        GFL_FadeSet(work->fadeMode, 0, 0x10, -1);
        (*state)++;
        break;
    case 1:
        GFL_WipeSet(3, 10, 10, 0, 6, 1, Field_GetHeapID(field));
        (*state)++;
        break;
    case 2:
        if (GFL_WipeIsFinished() != 0) {
            (*state)++;
        }
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bf2e0(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    Field *field = work->field;

    switch (*state) {
    case 0:
        GFL_FadeSet(work->fadeMode, 0, 0x10, -1);
        (*state)++;
        break;
    case 1:
        GFL_WipeSet(3, 6, 6, 0, 6, 1, Field_GetHeapID(field));
        (*state)++;
        break;
    case 2:
        if (GFL_WipeIsFinished() != 0) {
            (*state)++;
        }
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bf348(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    Field *field = work->field;

    switch (*state) {
    case 0:
        func_ov021_021bf564(field);
        func_ov021_021bf584(work->field);
        GFL_FadeSet(work->fadeMode, 0x10, 0, -1);
        (*state)++;
        break;
    case 1:
        GFL_WipeSet(3, 3, 3, 0, 6, 1, Field_GetHeapID(field));
        (*state)++;
        break;
    case 2:
        if (GFL_WipeIsFinished() != 0) {
            (*state)++;
        }
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bf3bc(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    Field *field = work->field;

    switch (*state) {
    case 0:
        func_ov021_021bf564(field);
        func_ov021_021bf584(work->field);
        GFL_FadeSet(work->fadeMode, 0x10, 0, -1);
        (*state)++;
        break;
    case 1:
        GFL_WipeSet(3, 5, 5, 0, 6, 1, Field_GetHeapID(field));
        (*state)++;
        break;
    case 2:
        if (GFL_WipeIsFinished() != 0) {
            (*state)++;
        }
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bf430(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    Field *field = work->field;

    switch (*state) {
    case 0:
        func_ov021_021bf564(field);
        func_ov021_021bf584(work->field);
        GFL_FadeSet(work->fadeMode, 0x10, 0, -1);
        (*state)++;
        break;
    case 1:
        GFL_WipeSet(3, 7, 7, 0, 6, 1, Field_GetHeapID(field));
        (*state)++;
        break;
    case 2:
        if (GFL_WipeIsFinished() != 0) {
            (*state)++;
        }
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bf4a4(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    Field *field = work->field;

    switch (*state) {
    case 0:
        func_ov021_021bf564(field);
        func_ov021_021bf584(work->field);
        GFL_FadeSet(work->fadeMode, 0x10, 0, -1);
        (*state)++;
        break;
    case 1:
        GFL_WipeSet(3, 11, 11, 0, 6, 1, Field_GetHeapID(field));
        (*state)++;
        break;
    case 2:
        if (GFL_WipeIsFinished() != 0) {
            (*state)++;
        }
        break;
    case 3:
        return GAMEEVENT_DONE;
    }
    return GAMEEVENT_CONTINUE;
}

GameEventReturnCode func_ov021_021bf518(GameEvent *event, u32 *state, void *data) {
    TransitionEvent *work = data;
    switch (*state) {
    case 0:
        GFL_FadeSet(work->fadeMode, 0x10, 0, work->fadeSlowness);
        func_ov021_021bf564(work->field);
        func_ov021_021bf584(work->field);
        (*state)++;
        break;
    case 1:
        if (work->skipWait == 1) {
            return GAMEEVENT_DONE;
        }
        if (GFL_FadeIsRunning() == 0) {
            return GAMEEVENT_DONE;
        }
        break;
    }
    return GAMEEVENT_CONTINUE;
}

void func_ov021_021bf564(Field *field) {
    u32 enabled;
    u32 enabled_00;

    enabled = GFL_BGSysGetEnabledBGsA();
    enabled_00 = GFL_BGSysGetEnabledBGsB();
    FieldG2D_SetLCDConfig();
    GFL_BGSysSetEnabledBGsA(enabled);
    GFL_BGSysSetEnabledBGsB(enabled_00);
}

void func_ov021_021bf584(Field *field) {
    FieldG2D_Prepare3DSurface(field);
}
