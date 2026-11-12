// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov034/union_eff.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The Union Room's effects on the field (the file's name is from its heap allocation): a focus on the player that the
// others look at, and the marks over the players who are in a trade or a battle, drawn as models of one archive. The
// work is allocated by func_ov019_0217f8c0 and kept by the field. Also the colosseum's checks of who is ready and the
// spots that the Union Room's players stand on
#include "field/union_eff.h"
#include "types.h"
#include "field/colosseum.h"
#include "field/comm_player.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/player_state.h"
#include "gfl/arc.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/std.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_mtx.h"
#include "save/player_info.h"
#include "save/wifi_list.h"
#include "system/game_system.h"
#include "constants/pokemon.h"
#include "field/union_comm.h"
#include "nitro/math.h"

// The archive of the effects' models and animations
#define UNION_EFF_ARC_ID 0x59
#define UNION_EFF_FOCUS_NSBMD 0x14
#define UNION_EFF_FOCUS_NSBCA 0x15
#define UNION_EFF_MARK_TOMO_NSBMD 0x16
#define UNION_EFF_MARK_ASONDA_NSBMD 0x17

#define FOCUS_SLOT_MAX 1
#define MARK_SLOT_MAX 10
// The frames that the focus is shown for
#define FOCUS_FRAMES 0x5A

// One effect on an actor
typedef struct {
    FieldActor *actor;
    G3DModel *model;
    void *anm;
    G3DActor *g3dActor;
    u16 serial; // the oldest is freed when there is no room
    u16 frame;
} UnionEffSlot;

typedef struct {
    UnionEffSlot slot[FOCUS_SLOT_MAX];
    void *res_nsbmd;
    void *res_nsbca;
    u16 serial;
} FocusMan;

typedef struct {
    UnionEffSlot slot[MARK_SLOT_MAX];
    void *res_nsbmd_tomo;
    void *res_nsbmd_asonda;
    u16 serial;
} MarkMan;

typedef struct {
    FocusMan focus;
    MarkMan mark;
} UnionEffect;

static void func_ov019_0217f97c(FocusMan *focus_man, HeapID heapId);
static void func_ov019_0217f9bc(FocusMan *focus_man);
static void func_ov019_0217f9dc(FocusMan *focus_man, FieldActor *actor);
static void func_ov019_0217fa10(FocusMan *focus_man, UnionEffSlot *slot, FieldActor *actor, u16 serial);
static void func_ov019_0217fa54(UnionEffSlot *slot);
static void func_ov019_0217fa78(UnionEffSlot *slot);
static void func_ov019_0217fa9c(UnionEffSlot *slot, SRTMatrix *mtx);
static void func_ov019_0217fabc(FocusMan *focus_man);
static void func_ov019_0217facc(FocusMan *focus_man);
static void func_ov019_0217faf0(FocusMan *focus_man);
static UnionEffSlot *func_ov019_0217fb30(FocusMan *focus_man);
static void func_ov019_0217fb70(MarkMan *mark_man, HeapID heapId);
static void func_ov019_0217fbd0(MarkMan *mark_man);
static void func_ov019_0217fc08(MarkMan *mark_man, FieldActor *actor, BOOL asonda);
static void func_ov019_0217fc64(MarkMan *mark_man, UnionEffSlot *slot, FieldActor *actor, u16 serial, BOOL asonda);
static UnionEffSlot *func_ov019_0217fd88(MarkMan *mark_man);
static void func_ov019_0217fca4(UnionEffSlot *slot);
static void func_ov019_0217fcc0(UnionEffSlot *slot);
static void func_ov019_0217fcc4(UnionEffSlot *slot, SRTMatrix *mtx);
static void func_ov019_0217fce4(MarkMan *mark_man);
static void func_ov019_0217fd04(MarkMan *mark_man);
static void func_ov019_0217fd38(MarkMan *mark_man);
static u16 UnionEff_GetMemberLook(PlayerInfo *info);

// pokebw2's UnionEff_IsPlayerOnSpot
BOOL func_ov019_0217f7e4(Field *field) {
    static const s16 spots[1][2] = { { 11, 15 } };
    FieldPlayer *player = Field_GetPlayer(field);
    PlayerState *state = GSYS_GetPlayerState(Field_GetGameSystem(field));
    VecFx32 pos;
    u32 i;

    FieldPlayer_GetWPos(player, &pos);
    pos.x = (pos.x >> 4) / FX32_ONE;
    pos.z = (pos.z >> 4) / FX32_ONE;
    func_02012ad0(state);
    for (i = 0; i < 1; i++) {
        if (pos.x == spots[i][0] && pos.z == spots[i][1]) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's UnionEff_GetKindStep
int func_ov019_0217f858(int kind) {
    if (kind >= 4 && kind <= 0x17) {
        return 1;
    }
    switch (kind) {
    case 3:
        return 4;
    case 0x18:
        return 2;
    case 0x19:
        return 5;
    case 0x1a:
        return 3;
    default:
        return 0;
    }
}

// pokebw2's UnionEff_IsFriendBeacon
BOOL func_ov019_0217f894(WifiList *wifiList, const UnionBeacon *beacon) {
    u32 friendIndex;

    if (func_02009d50(wifiList, beacon->name, beacon->trainerId, beacon->gender, &friendIndex) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionEff_Create
void *func_ov019_0217f8c0(HeapID heapId) {
    UnionEffect *eff = GFL_HeapAllocate(heapId, sizeof(UnionEffect), TRUE, "union_eff.c", 142);

    func_ov019_0217f97c(&eff->focus, heapId);
    func_ov019_0217fb70(&eff->mark, heapId);
    return eff;
}

// pokebw2's UnionEff_Free
void func_ov019_0217f8ec(void *work) {
    UnionEffect *eff = work;

    if (eff != NULL) {
        func_ov019_0217fce4(&eff->mark);
        func_ov019_0217fbd0(&eff->mark);
        func_ov019_0217fabc(&eff->focus);
        func_ov019_0217f9bc(&eff->focus);
        GFL_HeapFree(eff);
    }
}

// pokebw2's UnionEff_Update
void func_ov019_0217f914(void *work) {
    UnionEffect *eff = work;

    if (eff != NULL) {
        func_ov019_0217fd04(&eff->mark);
        func_ov019_0217facc(&eff->focus);
    }
}

// pokebw2's UnionEff_Draw
void func_ov019_0217f928(void *work) {
    UnionEffect *eff = work;

    if (eff != NULL) {
        func_ov019_0217fd38(&eff->mark);
        func_ov019_0217faf0(&eff->focus);
    }
}

// pokebw2's UnionEff_AddFocus
void func_ov019_0217f93c(void *work, FieldActor *actor) {
    UnionEffect *eff = work;

    if (eff != NULL) {
        func_ov019_0217f9dc(&eff->focus, actor);
    }
}

// pokebw2's UnionEff_AddMark
void func_ov019_0217f948(void *work, FieldActor *actor, BOOL asonda) {
    UnionEffect *eff = work;

    if (eff != NULL) {
        func_ov019_0217fc08(&eff->mark, actor, asonda);
    }
}

// pokebw2's UnionEff_RemoveMark
void func_ov019_0217f958(void *work, FieldActor *actor) {
    UnionEffect *eff = work;
    MarkMan *mark_man = &eff->mark;
    int i;

    for (i = 0; i < MARK_SLOT_MAX; i++) {
        if (mark_man->slot[i].actor == actor) {
            func_ov019_0217fca4(&mark_man->slot[i]);
            return;
        }
    }
}

// pokebw2's UnionEff_LoadFocus
static void func_ov019_0217f97c(FocusMan *focus_man, HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(UNION_EFF_ARC_ID, heapId);
    BOOL ret;

    focus_man->res_nsbmd = GFL_G3DSysReadArcToolResource(arc, UNION_EFF_FOCUS_NSBMD);
    focus_man->res_nsbca = GFL_G3DSysReadArcToolResource(arc, UNION_EFF_FOCUS_NSBCA);
    ret = GFL_G3DResUploadTexData(focus_man->res_nsbmd);
    GFL_ASSERT(ret);
    GFL_ArcToolFree(arc);
}

// pokebw2's UnionEff_UnloadFocus
static void func_ov019_0217f9bc(FocusMan *focus_man) {
    GFL_G3DResFreeTexData(focus_man->res_nsbmd);
    GFL_G3DResFree(focus_man->res_nsbca);
    focus_man->res_nsbca = NULL;
    GFL_G3DResFree(focus_man->res_nsbmd);
    focus_man->res_nsbmd = NULL;
}

// pokebw2's UnionEff_StartFocus
static void func_ov019_0217f9dc(FocusMan *focus_man, FieldActor *actor) {
    UnionEffSlot *slot;

    GFL_ASSERT(focus_man->res_nsbmd != NULL);
    slot = func_ov019_0217fb30(focus_man);
    func_ov019_0217fa10(focus_man, slot, actor, focus_man->serial);
    focus_man->serial++;
}

// pokebw2's UnionEff_InitFocusSlot
static void func_ov019_0217fa10(FocusMan *focus_man, UnionEffSlot *slot, FieldActor *actor, u16 serial) {
    MI_CpuFill8(slot, 0, sizeof(UnionEffSlot));
    slot->actor = actor;
    slot->serial = serial;
    slot->model = GFL_G3DMdlCreate(focus_man->res_nsbmd, 0, focus_man->res_nsbmd);
    slot->anm = GFL_G3DAnmCreate(slot->model, focus_man->res_nsbca, 0);
    slot->g3dActor = GFL_G3DActorCreate(slot->model, &slot->anm, 1);
    GFL_G3DActorBindAnm(slot->g3dActor, 0);
}

// pokebw2's UnionEff_FreeFocusSlot
static void func_ov019_0217fa54(UnionEffSlot *slot) {
    GFL_G3DAnmFree(slot->anm);
    GFL_G3DActorFree(slot->g3dActor);
    GFL_G3DMdlFree(slot->model);
    MI_CpuFill8(slot, 0, sizeof(UnionEffSlot));
}

// pokebw2's UnionEff_StepFocusSlot
static void func_ov019_0217fa78(UnionEffSlot *slot) {
    GFL_G3DActorStepAnmFrameLoop(slot->g3dActor, 0, FX32_ONE);
    slot->frame++;
    if (slot->frame > FOCUS_FRAMES) {
        func_ov019_0217fa54(slot);
    }
}

// pokebw2's UnionEff_DrawFocusSlot
static void func_ov019_0217fa9c(UnionEffSlot *slot, SRTMatrix *mtx) {
    CopyActorWPos(slot->actor, &mtx->translation);
    mtx->translation.y -= 6 * FX32_ONE;
    GFL_G3DSysDrawObjBBoxCull(slot->g3dActor, mtx);
}

// pokebw2's UnionEff_ExitFocus
static void func_ov019_0217fabc(FocusMan *focus_man) {
    if (focus_man->slot[0].g3dActor != NULL) {
        func_ov019_0217fa54(&focus_man->slot[0]);
    }
}

// pokebw2's UnionEff_UpdateFocus
static void func_ov019_0217facc(FocusMan *focus_man) {
    UnionEffSlot *slot = &focus_man->slot[0];

    if (slot->g3dActor != NULL) {
        if (func_ov010_0216da30(slot->actor) == TRUE) {
            func_ov019_0217fa78(slot);
        } else {
            func_ov019_0217fa54(slot);
        }
    }
}

// pokebw2's UnionEff_DrawFocus
static void func_ov019_0217faf0(FocusMan *focus_man) {
    SRTMatrix mtx = { { 0, 0, 0 }, { FX32_ONE, FX32_ONE, FX32_ONE }, { 0 } };

    MTX_Identity33_(&mtx.rotation);
    if (focus_man->slot[0].g3dActor != NULL && func_ov010_0216da30(focus_man->slot[0].actor) == TRUE) {
        func_ov019_0217fa9c(&focus_man->slot[0], &mtx);
    }
}

// pokebw2's UnionEff_GetFreeFocusSlot
static UnionEffSlot *func_ov019_0217fb30(FocusMan *focus_man) {
    int i;
    int oldest = 0;

    for (i = 0; i < FOCUS_SLOT_MAX; i++) {
        if (focus_man->slot[i].g3dActor == NULL) {
            return &focus_man->slot[i];
        }
        if (focus_man->slot[i].serial < focus_man->slot[oldest].serial) {
            oldest = i;
        }
    }
    func_ov019_0217fa54(&focus_man->slot[oldest]);
    return &focus_man->slot[oldest];
}

// pokebw2's UnionEff_LoadMarks
static void func_ov019_0217fb70(MarkMan *mark_man, HeapID heapId) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(UNION_EFF_ARC_ID, heapId);
    BOOL ret;

    mark_man->res_nsbmd_tomo = GFL_G3DSysReadArcToolResource(arc, UNION_EFF_MARK_TOMO_NSBMD);
    mark_man->res_nsbmd_asonda = GFL_G3DSysReadArcToolResource(arc, UNION_EFF_MARK_ASONDA_NSBMD);
    ret = GFL_G3DResUploadTexData(mark_man->res_nsbmd_tomo);
    GFL_ASSERT(ret);
    ret = GFL_G3DResUploadTexData(mark_man->res_nsbmd_asonda);
    GFL_ASSERT(ret);
    GFL_ArcToolFree(arc);
}

// pokebw2's UnionEff_UnloadMarks
static void func_ov019_0217fbd0(MarkMan *mark_man) {
    GFL_G3DResFreeTexData(mark_man->res_nsbmd_tomo);
    GFL_G3DResFreeTexData(mark_man->res_nsbmd_asonda);
    GFL_G3DResFree(mark_man->res_nsbmd_tomo);
    GFL_G3DResFree(mark_man->res_nsbmd_asonda);
    mark_man->res_nsbmd_tomo = NULL;
    mark_man->res_nsbmd_asonda = NULL;
}

// pokebw2's UnionEff_StartMark
static void func_ov019_0217fc08(MarkMan *mark_man, FieldActor *actor, BOOL asonda) {
    UnionEffSlot *slot;

    GFL_ASSERT(mark_man->res_nsbmd_tomo != NULL);
    GFL_ASSERT(mark_man->res_nsbmd_asonda != NULL);
    slot = func_ov019_0217fd88(mark_man);
    func_ov019_0217fc64(mark_man, slot, actor, mark_man->serial, asonda);
    mark_man->serial++;
}

// pokebw2's UnionEff_InitMarkSlot
static void func_ov019_0217fc64(MarkMan *mark_man, UnionEffSlot *slot, FieldActor *actor, u16 serial, BOOL asonda) {
    MI_CpuFill8(slot, 0, sizeof(UnionEffSlot));
    slot->actor = actor;
    slot->serial = serial;
    if (asonda == FALSE) {
        slot->model = GFL_G3DMdlCreate(mark_man->res_nsbmd_tomo, 0, mark_man->res_nsbmd_tomo);
    } else {
        slot->model = GFL_G3DMdlCreate(mark_man->res_nsbmd_asonda, 0, mark_man->res_nsbmd_asonda);
    }
    slot->g3dActor = GFL_G3DActorCreate(slot->model, NULL, 0);
}

// pokebw2's UnionEff_FreeMarkSlot
static void func_ov019_0217fca4(UnionEffSlot *slot) {
    GFL_G3DActorFree(slot->g3dActor);
    GFL_G3DMdlFree(slot->model);
    MI_CpuFill8(slot, 0, sizeof(UnionEffSlot));
}

// pokebw2's UnionEff_StepMarkSlot
static void func_ov019_0217fcc0(UnionEffSlot *slot) {
}

// pokebw2's UnionEff_DrawMarkSlot
static void func_ov019_0217fcc4(UnionEffSlot *slot, SRTMatrix *mtx) {
    CopyActorWPos(slot->actor, &mtx->translation);
    mtx->translation.y -= FX32_CONST(6.5);
    GFL_G3DSysDrawObjBBoxCull(slot->g3dActor, mtx);
}

// pokebw2's UnionEff_ExitMarks
static void func_ov019_0217fce4(MarkMan *mark_man) {
    int i;

    for (i = 0; i < MARK_SLOT_MAX; i++) {
        if (mark_man->slot[i].g3dActor != NULL) {
            func_ov019_0217fca4(&mark_man->slot[i]);
        }
    }
}

// pokebw2's UnionEff_UpdateMarks
static void func_ov019_0217fd04(MarkMan *mark_man) {
    int i;

    for (i = 0; i < MARK_SLOT_MAX; i++) {
        UnionEffSlot *slot = &mark_man->slot[i];

        if (slot->g3dActor != NULL) {
            if (func_ov010_0216da30(slot->actor) == TRUE) {
                func_ov019_0217fcc0(slot);
            } else {
                func_ov019_0217fca4(slot);
            }
        }
    }
}

// pokebw2's UnionEff_DrawMarks
static void func_ov019_0217fd38(MarkMan *mark_man) {
    SRTMatrix mtx = { { 0, 0, 0 }, { FX32_ONE, FX32_ONE, FX32_ONE }, { 0 } };
    int i;

    MTX_Identity33_(&mtx.rotation);
    for (i = 0; i < MARK_SLOT_MAX; i++) {
        UnionEffSlot *slot = &mark_man->slot[i];

        if (slot->g3dActor != NULL && func_ov010_0216da30(slot->actor) == TRUE) {
            func_ov019_0217fcc4(slot, &mtx);
        }
    }
}

// pokebw2's UnionEff_GetFreeMarkSlot
static UnionEffSlot *func_ov019_0217fd88(MarkMan *mark_man) {
    int i;
    int oldest = 0;

    for (i = 0; i < MARK_SLOT_MAX; i++) {
        if (mark_man->slot[i].g3dActor == NULL) {
            return &mark_man->slot[i];
        }
        if (mark_man->slot[i].serial < mark_man->slot[oldest].serial) {
            oldest = i;
        }
    }
    func_ov019_0217fca4(&mark_man->slot[oldest]);
    return &mark_man->slot[oldest];
}

// pokebw2's UnionEff_AllColosseumMembersHere
BOOL func_ov019_0217fdc8(UnionColosseum *colosseum) {
    int count = 0;
    int i;

    for (i = 0; i < COLOSSEUM_MEMBER_MAX; i++) {
        if (colosseum->members[i].unk26 == 1) {
            count++;
        }
    }
    if (count >= func_0203d1b4()) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionEff_AllColosseumCardsIn
BOOL func_ov019_0217fdf8(UnionColosseum *colosseum) {
    int count = 0;
    int i;

    for (i = 0; i < COLOSSEUM_MEMBER_MAX; i++) {
        if (colosseum->cardReady[i] == 1) {
            count++;
        }
    }
    if (count >= func_0203d1b4()) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionEff_IsColosseumUnk170Set
BOOL func_ov019_0217fe24(UnionColosseum *colosseum) {
    if (colosseum->unk160 == 1) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionEff_ClearColosseumUnk170
void func_ov019_0217fe38(UnionColosseum *colosseum) {
    colosseum->unk160 = 0;
}

static const struct {
    s16 two[2][2];
    s16 four[4][2];
    s16 seatA[5][2];
    s16 seatB[5][2];
// pokebw2's sSpots
} data_ov019_02180794 = {
#include "asset/union_eff_data_ov019_02180794.inc"
};

// pokebw2's UnionEff_GetEntrySpot
BOOL func_ov019_0217fe44(Field *field, u32 kind, u32 *index) {
    FieldPlayer *player = Field_GetPlayer(field);
    FieldActor *actor;
    s16 x;
    s16 z;
    u32 i;

    *index = 0;
    actor = FieldPlayer_GetActor(player);
    x = GetGPosX(actor);
    z = GetGPosZ(actor);
    if (kind == 2) {
        for (i = 0; i < 2; i++) {
            const s16 *spot = data_ov019_02180794.two[i];

            if (x == spot[0] && z == spot[1]) {
                *index = i;
                return TRUE;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            const s16 *spot = data_ov019_02180794.four[i];

            if (x == spot[0] && z == spot[1]) {
                *index = i;
                return TRUE;
            }
        }
    }
    return FALSE;
}

// pokebw2's UnionEff_AllColosseumReady
BOOL func_ov019_0217feb8(UnionColosseum *colosseum) {
    int count = 0;
    int i;
    u8 mask = colosseum->unk16C[2];

    for (i = 0; i < 4; i++) {
        if (colosseum->unk162[i] == 1) {
            count++;
        }
    }
    if (count >= func_0203d1b4() || count >= MATH_CountPopulation(mask)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's UnionEff_AnyColosseumUnk172
BOOL func_ov019_0217fef4(UnionColosseum *colosseum) {
    int i;

    for (i = 0; i < 4; i++) {
        if (colosseum->unk162[i] == 1) {
            return TRUE;
        }
    }
    return FALSE;
}

static inline BOOL UnionEff_IsOnSpot(const s16 (*spots)[2], int count, int x, int z) {
    int i;

    for (i = 0; i < count; i++) {
        if (x == spots[i][0] && z == spots[i][1]) {
            return TRUE;
        }
    }
    return FALSE;
}

// pokebw2's UnionEff_IsOnSeatSpot
BOOL func_ov019_0217ff14(Field *field) {
    const s16 (*spots)[2];
    VecFx32 pos;
    int i;
    int count;

    FieldPlayer_GetWPos(Field_GetPlayer(field), &pos);
    pos.x >>= 12;
    pos.z >>= 12;
    if (Field_GetPlayerStateZoneID(field) == 0x96) {
        spots = data_ov019_02180794.seatA;
        count = NELEMS(data_ov019_02180794.seatA);
    } else {
        spots = data_ov019_02180794.seatB;
        count = NELEMS(data_ov019_02180794.seatB);
    }
    for (i = 0; i < count; i++) {
        if (pos.x == spots[i][0] && pos.z == spots[i][1]) {
            return TRUE;
        }
    }
    return FALSE;
}

void func_ov019_0217ff78(UnionColosseum *colosseum) {
    NetHandle *handle = func_020399a8();
    u8 netId = func_0203d1a4(handle);
    ColosseumChoice choice;
    int i;

    for (i = 0; i < 4; i++) {
        if (netId != i && func_ov012_021751dc(colosseum, i, &choice) == 1) {
            if (func_ov010_021677e4(colosseum->commPlayer, i) == 0) {
                u16 kind;
                if (getTrainerGender((PlayerInfo *)&colosseum->members[i]) == 0) {
                    kind = 1;
                } else {
                    kind = 4;
                }
                func_ov010_021675d0(colosseum->commPlayer, i, kind, (CommPlayerStatus *)&choice);
            } else {
                func_ov010_02167848(colosseum->commPlayer, i, (CommPlayerStatus *)&choice);
            }
        }
    }
}


