// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/musical_event.c, ported by tools/port_file.py.
#include "types.h"
#include "constants/arc.h"
#include "constants/pokemon.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "field/event_mapchange.h"
#include "field/event_sound.h"
#include "field/field.h"
#include "field/field_event.h"
#include "field/field_script.h"
#include "field/field_script_event.h"
#include "field/game_beacon_set.h"
#include "field/musical.h"
#include "field/musical_dressup_sys.h"
#include "field/musical_program.h"
#include "field/musical_stage_sys.h"
#include "field/player_state.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "nitro/rtc.h"
#include "pml/poke_party.h"
#include "save/musical_savedata.h"
#include "save/player_info.h"
#include "save/records.h"
#include "save/save_control.h"
#include "system/game_comm.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/wordset.h"
#include "app/musical/mus_item_data.h"
#include "app/musical/musical_shot_sys.h"
#include "app/musical/musical_system.h"
#include "field/mus_comm_func.h"
#include "gfl/rtc_cache.h"

// The event of a musical show: the dressing room, the stage and the photo, alone with three performers of the
// program or online with up to three other players
struct MusicalEventWork {
    GameSystem *gsys;
    GameData *gameData;
    MusicalCommWork *commWork;
    u32 state;
    u8 subState;
    BOOL online;
    PartyPkm *pkm;
    SaveControl *save;
    GameCommSys *gameComm;
    Ov210Work *ov210;
    // Overlay 211's communication work
    void *comm;
    MusicalDressUpParam *dressUp;
    MusicalStageParam *stage;
    MusicalShotParam *shot;
    MusicalProgram *program;
    MusicalPoke *poke;
    MusicalSave *musicalSave;
    // The stage position of each Pokémon, in entry order
    u8 order[4];
    // The stage positions, from the most points to the fewest
    u8 ranking[4];
    // The stage position of the player's Pokémon
    u8 playerPos;
    BOOL error;
};

GameEventReturnCode func_ov010_02155c0c(GameEvent *event, u32 *state, void *data);
void func_ov010_02155f24(MusicalEventWork *work);
static void func_ov010_02155f4c(MusicalEventWork *work);
static void func_ov010_02155f9c(MusicalEventWork *work);
static void func_ov010_02155fb4(MusicalEventWork *work);
static void func_ov010_02155fc8(MusicalEventWork *work);
static void func_ov010_02156074(MusicalEventWork *work);
void func_ov010_0215611c(MusicalEventWork *work);
static void func_ov010_02156420(MusicalEventWork *work);
static void func_ov010_02156530(MusicalEventWork *work);
static BOOL func_ov010_02156710(GameEvent *event, MusicalEventWork *work);
static BOOL func_ov010_02156770(GameEvent *event, MusicalEventWork *work);
static void func_ov010_021567d8(GameEvent *event, MusicalEventWork *work);
static void func_ov010_02156824(GameEvent *event, MusicalEventWork *work);
void func_ov010_02156888(GameEvent *event, MusicalEventWork *work, u16 scriptId);
u8 func_ov010_021568a8(MusicalEventWork *work, u8 pos);
static void func_ov010_02156b74(MusicalEventWork *work);

static const VecFx32 data_ov010_0216fef0 = {FX32_CONST(232), 0, FX32_CONST(200)};
static const VecFx32 data_ov010_0216ff08 = {FX32_CONST(168), 0, FX32_CONST(200)};
static const VecFx32 data_ov010_0216fefc = {FX32_CONST(136), 0, FX32_CONST(200)};

// The total points from which a number of the program's prize slots is drawn, from 10 slots down
static const u16 data_ov010_0216ff14[10] = {12000, 8000, 5500, 3500, 2000, 1000, 500, 250, 100, 0};

// The props each program can give
static const u8 data_ov010_0216ff28[7][40] = {
#include "asset/musical_event_data_ov010_0216ff28.inc"
};



GameEvent *func_ov010_02155a98(GameSystem *gsys, GameData *gameData, u8 slot, BOOL online, MusicalCommWork *comm) {
    GameEvent *event;
    MusicalEventWork *work;
    StrBuf *name;
    u16 shots;
    u8 i;
    u8 j;
    u8 other;
    u8 temp;

    GFL_HeapCreateChild(HEAPID_USER, HEAPID_TAIL(HEAPID_MUSICAL_EVENT), 0x42000);
    event = GameEvent_Create(gsys, NULL, func_ov010_02155c0c, sizeof(MusicalEventWork));
    work = GameEvent_GetData(event);
    work->gsys = gsys;
    work->gameData = gameData;
    work->online = online;
    work->save = GameData_GetSaveControl(gameData);
    work->gameComm = GSYS_GetGameCommSystem(gsys);
    work->commWork = comm;
    work->commWork->event = work;
    work->comm = comm->comm;
    work->error = FALSE;
    work->program = NULL;
    name = GFL_StrBufCreate(11, HEAPID_GAMEEVENT);
    work->pkm = PokeParty_GetPkm(GameData_GetParty(work->gameData), slot);
    PokeParty_GetParam(work->pkm, PKM_PARAM_NICKNAME, name);
    func_ov010_0216576c(name);
    GFL_StrBufFree(name);
    work->dressUp = NULL;
    work->stage = NULL;
    work->shot = NULL;
    work->musicalSave = getMusicalInfoBlkAddress(gameData);
    work->ov210 = func_ov112_021f685c(HEAPID_GAMEEVENT);
    if (work->online == FALSE || func_ov132_021fd904(work->commWork->comm) == TRUE) {
        func_ov112_021f68bc(work->ov210, work->save, work->gameData, func_0200a7c8(work->musicalSave), HEAPID_MUSICAL_EVENT);
    }
    if (work->online == TRUE) {
        work->state = 11;
        work->subState = 0;
        func_ov132_021fc848(work->comm, GetGameDataPlayerInfo(work->gameData), func_020185f0(work->pkm), work->gameComm,
            work->ov210, HEAPID_GAMEEVENT);
    } else {
        shots = func_0200a81c(work->musicalSave);
        for (i = 0; i < 4; i++) {
            work->order[i] = i;
        }
        for (j = 0; j < 10; j++) {
            for (i = 0; i < 4; i++) {
                other = GFL_RandomLCAlt(4);
                temp = work->order[i];
                work->order[i] = work->order[other];
                work->order[other] = temp;
            }
        }
        work->playerPos = work->order[0];
        work->program = func_ov010_02157028(HEAPID_GAMEEVENT, work->ov210, shots);
        work->state = 13;
        work->subState = 0;
    }
    return event;
}

GameEventReturnCode func_ov010_02155c0c(GameEvent *event, u32 *state, void *data) {
    MusicalEventWork *work = data;
    u32 program;
    u32 cpus;
    u8 i;
    RivalDataSave *rivals;
    PlayerInfo *info;

    func_ov010_02156bac(work);
    switch (work->state) {
    case 11:
        if (func_ov132_021fd908(work->commWork->comm) == TRUE) {
            program = 0;
            cpus = 0;
            if (func_ov132_021fd904(work->commWork->comm) == TRUE) {
                work->program = func_ov010_02157028(HEAPID_GAMEEVENT, work->ov210, func_0200a81c(work->musicalSave));
                program = func_ov010_02157260(work->program);
                cpus = func_ov010_02157298(work->program);
            }
            func_ov132_021fd918(work->commWork->comm, program, cpus);
            work->state = 12;
            rivals = getHollow_RivalData(work->save);
            for (i = 0; i < 4; i++) {
                if (func_ov132_021fd8d4(work->commWork->comm) != func_ov132_021fd8ec(work->commWork->comm, i)) {
                    info = func_ov132_021fce28(work->commWork->comm, i);
                    if (info != NULL) {
                        func_0200ed54(rivals, getIDAsUInt(info));
                    }
                }
            }
        } else if (work->error == TRUE) {
            work->state = 23;
        }
        break;
    case 12:
        if (func_ov132_021fd83c(work->commWork->comm) == TRUE) {
            work->state = 13;
            for (i = 0; i < 4; i++) {
                work->order[i] = func_ov132_021fd8ec(work->commWork->comm, i);
            }
            work->playerPos = func_ov132_021fd8d4(work->commWork->comm);
            func_ov010_02156b74(work);
        } else if (work->error == TRUE) {
            work->state = 23;
        }
        break;
    case 13:
        func_ov010_021567d8(event, work);
        work->state = 14;
        break;
    case 14:
        func_ov010_02155f24(work);
        func_ov010_02156888(event, work, 3);
        work->state = 0;
        break;
    case 0:
        if (work->error == TRUE && work->subState == 0) {
            work->state = 23;
        } else if (func_ov010_02156770(event, work) == TRUE) {
            work->subState = 0;
            work->state = 1;
        }
        break;
    case 1:
        work->state = 3;
        break;
    case 3:
        func_ov010_02155f9c(work);
        GSYS_QueueProc(work->gsys, OVERLAY_NONE, &data_ov010_02172724, work->dressUp);
        work->state = 4;
        break;
    case 4:
        if (GSYS_GetProcMgrState(work->gsys) == FALSE) {
            func_ov010_02155fb4(work);
            work->state = 16;
        }
        break;
    case 16:
        if (func_ov010_02156710(event, work) == TRUE) {
            work->subState = 0;
            work->state = 17;
            if (work->error == TRUE) {
                work->state = 23;
            }
        }
        break;
    case 17:
        func_ov010_02156888(event, work, 4);
        work->state = 18;
        break;
    case 18:
        if (work->error == TRUE && work->subState == 0) {
            work->state = 23;
        } else if (func_ov010_02156770(event, work) == TRUE) {
            work->subState = 0;
            work->state = 5;
        }
        break;
    case 5:
        func_ov010_02155fc8(work);
        GSYS_QueueProc(work->gsys, OVERLAY_NONE, &data_ov010_02172748, work->stage);
        work->state = 6;
        break;
    case 6:
        if (GSYS_GetProcMgrState(work->gsys) == FALSE) {
            func_ov010_02156074(work);
            work->state = 7;
            if (work->error == TRUE) {
                work->state = 22;
            }
        }
        break;
    case 7:
        func_ov010_0215611c(work);
        GSYS_QueueProc(work->gsys, OVERLAY_NONE, &data_ov112_021f8360, work->shot);
        work->state = 8;
        break;
    case 8:
        if (GSYS_GetProcMgrState(work->gsys) == FALSE) {
            work->state = 19;
            if (work->error == TRUE) {
                work->state = 22;
            }
        }
        break;
    case 19:
        if (func_ov010_02156710(event, work) == TRUE) {
            work->subState = 0;
            work->state = 20;
            func_ov010_02156530(work);
            func_ov010_02156420(work);
        }
        break;
    case 20:
        func_ov010_02156888(event, work, 5);
        work->state = 2;
        break;
    case 2:
        if (work->error == TRUE) {
            work->state = 23;
        } else {
            func_ov010_02155f4c(work);
            func_ov010_02156824(event, work);
            work->state = 9;
        }
        break;
    case 9:
        func_ov010_02156888(event, work, 0x28cc);
        work->state = 10;
        break;
    case 10:
        GFL_HeapDumpOnFailure(HEAPID_GAMEEVENT);
        GFL_HeapDelete(HEAPID_MUSICAL_EVENT);
        return GAMEEVENT_DONE;
    case 22:
        if (func_ov010_02156710(event, work) == TRUE) {
            work->state = 23;
        }
        break;
    case 23:
        func_0200fb38();
        work->state = 24;
        break;
    case 24:
        func_ov010_02155f4c(work);
        func_ov010_02156824(event, work);
        work->state = 25;
        break;
    case 25:
        work->state = 26;
        break;
    case 26:
        func_ov010_02156888(event, work, 0x28cd);
        work->state = 10;
        break;
    }
    return GAMEEVENT_CONTINUE;
}

void func_ov010_02155f24(MusicalEventWork *work) {
    work->poke = func_ov112_021f675c(work->pkm, HEAPID_GAMEEVENT);
    if (work->online != FALSE) {
        func_ov132_021fd904(work->commWork->comm);
        func_ov132_021fd938(work->commWork->comm);
    }
}

static void func_ov010_02155f4c(MusicalEventWork *work) {
    if (work->dressUp != NULL) {
        func_ov010_02156cd8(work->dressUp);
    }
    if (work->stage != NULL) {
        func_ov010_02156f98(work->stage);
    }
    if (work->shot != NULL) {
        GFL_HeapFree(work->shot->shot);
        GFL_HeapFree(work->shot);
    }
    if (work->ov210 != NULL) {
        func_ov112_021f6888(work->ov210);
    }
    if (work->poke != NULL) {
        GFL_HeapFree(work->poke);
    }
    if (work->program != NULL) {
        func_ov010_0215716c(work->program);
    }
    work->commWork->event = NULL;
}

static void func_ov010_02155f9c(MusicalEventWork *work) {
    work->dressUp = func_ov010_02156cac(HEAPID_GAMEEVENT, work->poke, work->save);
    work->dressUp->comm = work->comm;
}

static void func_ov010_02155fb4(MusicalEventWork *work) {
    if (work->online == TRUE) {
        func_ov132_021fd948(work->comm, work->poke);
    }
}

static void func_ov010_02155fc8(MusicalEventWork *work) {
    u8 i;
    u8 cpu;
    MusicalPoke *poke;

    work->stage = func_ov010_02156f68(HEAPID_GAMEEVENT, work->comm);
    if (work->online == FALSE) {
        for (i = 0; i < 4; i++) {
            if (i == 0) {
                func_ov010_02156fc4(work->stage, work->order[i], work->poke);
            } else {
                func_ov010_021572e4(work->program, work->stage, work->order[i], i - 1, HEAPID_GAMEEVENT);
            }
        }
    } else {
        cpu = 0;
        for (i = 0; i < 4; i++) {
            poke = func_ov132_021fd4f8(work->comm, work->order[i]);
            if (poke != NULL) {
                func_ov010_02157000(work->stage, work->order[i], poke);
            } else {
                func_ov010_021572e4(work->program, work->stage, work->order[i], cpu, HEAPID_GAMEEVENT);
                cpu++;
            }
        }
    }
    work->stage->ov210 = work->ov210;
    work->stage->program = work->program;
    func_ov010_02157174(HEAPID_GAMEEVENT, work->program, work->stage);
}

static void func_ov010_02156074(MusicalEventWork *work) {
    MusicalPoke *poke;
    void *items;
    u8 i;
    u8 j;
    u8 slot;

    if (work->online == TRUE) {
        for (i = 0; i < 4; i++) {
            poke = work->stage->pokes[i];
            for (j = 0; j < 2; j++) {
                slot = func_ov132_021fd964(work->comm, i, j);
                if (slot != 0xff) {
                    poke->unk54[slot] = TRUE;
                }
            }
        }
    }
    items = func_ov112_021f4f14(HEAPID_GAMEEVENT);
    for (i = 0; i < 4; i++) {
        poke = work->stage->pokes[i];
        for (j = 0; j < 9; j++) {
            if (poke->unk54[j] == TRUE) {
                u8 kind = func_ov112_021f5140(items, poke->equips[j].itemId);

                poke->points += func_ov010_02157394(work->program, kind);
            }
        }
    }
    func_ov112_021f4f40(items);
}

void func_ov010_0215611c(MusicalEventWork *work) {
    RTCDate date;
    u8 points[4];
    MusicalShot *shot;
    StrBuf *str;
    MsgData *msgData;
    u32 msgId;
    PlayerInfo *info;
    MusicalPoke *poke;
    u8 max = 0;
    u8 value;
    u8 i;
    u8 j;
    u8 pos;
    u8 temp;
    u8 rank;
    u8 slot;

    work->shot = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(MusicalShotParam), FALSE, "musical_event.c", 786);
    work->shot->shot = GFL_HeapAllocate(HEAPID_GAMEEVENT, sizeof(MusicalShot), TRUE, "musical_event.c", 787);
    work->shot->loadComm = FALSE;
    work->shot->comm = work->comm;
    shot = work->shot->shot;
    RTC_GetCachedDate(&date);
    shot->unk0_0 = func_ov010_02157364(work->program);
    shot->year = date.year;
    shot->month = date.month;
    shot->day = date.day;
    str = func_ov010_02156914(work, HEAPID_GAMEEVENT);
    GFL_StrBufStoreString(str, shot->title, 0x25);
    GFL_StrBufFree(str);
    shot->unk0_29 = 0;
    shot->unk1AE = 0x15;
    shot->unk1AF = 1;
    shot->player = work->playerPos;
    for (i = 0; i < 4; i++) {
        value = work->stage->pokes[i]->points;
        if (max < value) {
            max = value;
            shot->tops = 0;
        }
        if (max == value) {
            shot->tops += 1 << i;
        }
    }
    for (pos = 0; pos < 4; pos++) {
        info = NULL;
        poke = work->stage->pokes[pos];
        shot->pokes[pos].species = poke->species;
        shot->pokes[pos].sex = poke->sex;
        shot->pokes[pos].rare = poke->rare;
        shot->pokes[pos].form = poke->form;
        shot->pokes[pos].personality = poke->personality;
        if (pos == work->playerPos) {
            info = GetGameDataPlayerInfo(work->gameData);
        } else if (work->online == TRUE) {
            info = func_ov132_021fce28(work->comm, func_ov010_021568e8(work, pos));
        }
        if (info != NULL) {
            MI_CpuCopy8(info, shot->pokes[pos].name, 0x10);
        } else {
            msgId = func_ov010_02157384(work->program, func_ov010_021568a8(work, func_ov010_021568e8(work, pos)));
            msgData = GFL_MsgSysLoadData(FALSE, 2, 0x4c, HEAPID_GAMEEVENT);
            str = GFL_MsgDataLoadStrbufNew(msgData, msgId);
            GFL_StrBufStoreString(str, shot->pokes[pos].name, 8);
            GFL_StrBufFree(str);
            GFL_MsgDataFree(msgData);
        }
        for (j = 0; j < 8; j++) {
            shot->pokes[pos].equips[j].itemId = 0xff;
            shot->pokes[pos].equips[j].unk2 = 0;
            shot->pokes[pos].equips[j].unk4 = 10;
        }
        for (j = 0; j < 9; j++) {
            if (poke->equips[j].itemId != 0xff) {
                slot = poke->equips[j].slot;
                shot->pokes[pos].equips[slot].itemId = poke->equips[j].itemId;
                shot->pokes[pos].equips[slot].unk2 = poke->equips[j].unk2;
                shot->pokes[pos].equips[slot].unk4 = j;
            }
        }
    }
    work->shot->askSave = TRUE;
    work->shot->save = getAddressOfMusicalDataInfo(GameData_GetSaveControl(GSYS_GetGameData(work->gsys)));
    for (i = 0; i < 4; i++) {
        points[i] = work->stage->pokes[i]->points;
        work->ranking[i] = i;
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4 - (i + 1); j++) {
            if (points[j + 1] > points[j]) {
                temp = points[j + 1];
                rank = work->ranking[j + 1];
                points[j + 1] = points[j];
                work->ranking[j + 1] = work->ranking[j];
                points[j] = temp;
                work->ranking[j] = rank;
            }
        }
    }
}



// Records the show in the save
static void func_ov010_02156420(MusicalEventWork *work) {
    u8 counts[4];
    u8 i;
    u8 points;
    u8 topPoints;
    void *items;
    MusicalPoke *poke;
    GameRecords *records;
    u16 zoneId;
    u8 *count;

    points = func_ov010_02156934(work, work->playerPos);
    topPoints = func_ov010_02156934(work, work->ranking[0]);
    func_0200a768(work->musicalSave);
    if (points == topPoints && points != 0) {
        func_0200a788(work->musicalSave);
    }
    func_0200a7c0(work->musicalSave, points);
    func_0200a800(work->musicalSave, points);
    count = (u8 *)&counts;
    count[0] = 0;
    count[1] = 0;
    count[2] = 0;
    count[3] = 0;
    items = func_ov112_021f4f14(HEAPID_GAMEEVENT);
    poke = work->stage->pokes[work->playerPos];
    for (i = 0; i < 9; i++) {
        if (poke->equips[i].itemId != 0xff) {
            count[(u8)func_ov112_021f5140(items, poke->equips[i].itemId)]++;
        }
    }
    for (i = 0; i < 4; i++) {
        func_0200a7ac(work->musicalSave, i, counts[i]);
    }
    func_ov112_021f4f40(items);
    zoneId = GameData_GetPlayerState(work->gameData)->zoneId;
    FriendshipManagerCalc(work->pkm, 6, zoneId, HEAPID_GAMEEVENT);
    records = GameData_GetRecords(work->gameData);
    if (work->online == FALSE) {
        RecordAddOne(records, 0x72);
        if (points == topPoints && points != 0) {
            RecordAddOne(records, 0x73);
        }
    } else {
        RecordAddOne(records, 0x74);
        if (points == topPoints && points != 0) {
            RecordAddOne(records, 0x75);
        }
    }
    RecordAdd(records, 0x76, points);
}

// Draws the props the show gives away to the audience
static void func_ov010_02156530(MusicalEventWork *work) {
    u8 *props;
    u32 total;
    u8 i;
    u8 count;
    u8 numProps;
    u8 program;
    u32 winner;
    u8 prop;
    u8 index;
    u8 temp;
    u8 order[10];
    u8 j;
    u16 *otherTotal;
    u32 chance;
    MusicalSaveUnk1E0 *entry;

    props = GFL_HeapAllocate(HEAPID_GAMEEVENT, 40, TRUE, "musical_event.c", 1017);
    total = 0;
    for (i = 0; i < 10; i++) {
        entry = func_0200a750(work->musicalSave, i);
        entry->unk0 = 4;
        entry->unk1 = 0;
        entry->unk2 = 0;
    }
    if (work->online == FALSE) {
        total = func_0200a81c(work->musicalSave) + func_ov010_02156934(work, work->playerPos);
    } else {
        for (i = 0; i < 4; i++) {
            otherTotal = func_ov132_021fd8f8(work->comm, func_ov010_021568e8(work, i));
            if (func_ov132_021fd4f8(work->comm, i) != NULL) {
                total += *otherTotal;
                total += func_ov010_02156934(work, i);
            }
        }
    }
    for (i = 0; i < 10; i++) {
        if (data_ov010_0216ff14[i] <= total) {
            count = 10 - i;
            break;
        }
    }
    program = func_ov010_0215736c(work->program);
    if (program < 7) {
        numProps = 0;
        for (i = 0; i < 40; i++) {
            prop = data_ov010_0216ff28[program][i];
            if (func_0200a644(work->musicalSave, prop) == FALSE) {
                props[numProps] = prop;
                numProps++;
            }
        }
    }
    winner = func_ov010_021572c0(work->program);
    for (i = 0; i < 10; i++) {
        order[i] = i;
    }
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            index = GFL_RandomLCAlt(10);
            temp = order[j];
            order[j] = order[index];
            order[index] = temp;
        }
    }
    for (i = 0; i < count; i++) {
        entry = func_0200a750(work->musicalSave, order[i]);
        chance = func_ov010_02156934(work, work->playerPos);
        entry->unk0 = winner;
        if ((u8)GFL_RandomLCAlt(100) < chance && numProps != 0) {
            index = GFL_RandomLCAlt(numProps);
            entry->unk1 = 1;
            entry->unk2 = props[index];
            for (; index < numProps - 1; index++) {
                props[index] = props[index + 1];
            }
            numProps--;
        }
    }
    GFL_HeapFree(props);
}

// Goes back to the field
static BOOL func_ov010_02156710(GameEvent *event, MusicalEventWork *work) {
    GameData_GetFieldSoundSystem(GSYS_GetGameData(work->gsys));
    switch (work->subState) {
    case 0:
        GameEvent_ChainNext(event, EventBGMPop_CreateEx(work->gsys, 0, 60));
        work->subState++;
        break;
    case 1:
        GameEvent_ChainNext(event, EventFieldOpen_CreateHeadless(work->gsys));
        work->subState++;
        break;
    case 2:
        work->subState++;
        break;
    case 3:
        GameEvent_ChainNext(event, EventBGMFadeWait_Create(work->gsys));
        work->subState++;
        break;
    case 4:
        return TRUE;
    }
    return FALSE;
}

// Leaves the field
static BOOL func_ov010_02156770(GameEvent *event, MusicalEventWork *work) {
    Field *field;

    GameData_GetFieldSoundSystem(GSYS_GetGameData(work->gsys));
    field = GSYS_GetField(work->gsys);
    switch (work->subState) {
    case 0:
        work->subState++;
        break;
    case 1:
        GameEvent_ChainNext(event, EventBGMPushWait_Create(work->gsys, 30));
        work->subState++;
        break;
    case 2:
        GameEvent_ChainNext(event, CreateFieldCloseEvent(work->gsys, field));
        work->subState++;
        break;
    case 3:
        GameEvent_ChainNext(event, func_02029c64(work->gsys));
        work->subState++;
        break;
    case 4:
        return TRUE;
    }
    return FALSE;
}

// Warps to the dressing room, to the player's place in it
static void func_ov010_021567d8(GameEvent *event, MusicalEventWork *work) {
    Field *field = GSYS_GetField(work->gsys);
    VecFx32 pos = data_ov010_0216fefc;

    pos.x += work->playerPos * FX32_CONST(32);
    GameEvent_ChainNext(event, EventMapChange_CreateGridDefault(work->gsys, field, 78, &pos, 0));
}

// Warps back to the musical hall
static void func_ov010_02156824(GameEvent *event, MusicalEventWork *work) {
    Field *field = GSYS_GetField(work->gsys);
    VecFx32 pos = data_ov010_0216fef0;
    VecFx32 onlinePos = data_ov010_0216ff08;
    GameEvent *next;

    if (work->online == TRUE) {
        next = EventMapChange_CreateGridDefault(work->gsys, field, 77, &onlinePos, 1);
    } else {
        next = EventMapChange_CreateGridDefault(work->gsys, field, 77, &pos, 1);
    }
    GameEvent_ChainNext(event, next);
}

void func_ov010_02156888(GameEvent *event, MusicalEventWork *work, u16 scriptId) {
    GameEvent *script = EventScriptCall_Create(work->gsys, scriptId, NULL, HEAPID_FIELDMAP);

    EventScriptCall_GetWork(script);
    GameEvent_ChainNext(event, script);
}

// The number of performers of the program before an entry
u8 func_ov010_021568a8(MusicalEventWork *work, u8 pos) {
    u8 i;
    u8 count = 0;

    for (i = 0; i < pos; i++) {
        if (work->comm == NULL) {
            if (i != 0) {
                count++;
            }
        } else if (func_ov132_021fce28(work->comm, i) == NULL) {
            count++;
        }
    }
    return count;
}

u8 func_ov010_021568e0(MusicalEventWork *work) {
    return work->playerPos;
}

u8 func_ov010_021568e8(MusicalEventWork *work, u8 pos) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (pos == work->order[i]) {
            return i;
        }
    }
    return 0;
}

u8 func_ov010_02156908(MusicalEventWork *work) {
    return func_ov010_021572c0(work->program);
}

StrBuf *func_ov010_02156914(MusicalEventWork *work, HeapID heapId) {
    MsgData *msgData = GFL_MsgDataCreateFromHandle(work->ov210->msgArc, heapId);
    StrBuf *str = GFL_MsgDataLoadStrbufNew(msgData, 0);

    GFL_MsgDataFree(msgData);
    return str;
}

u8 func_ov010_02156934(MusicalEventWork *work, u8 pos) {
    u16 points = work->stage->pokes[pos]->points;

    if (points < 0x100) {
        return points;
    }
    return 0xff;
}

u16 func_ov010_02156954(MusicalEventWork *work) {
    u16 max = 0;
    u8 i;

    for (i = 0; i < 4; i++) {
        if (max < work->stage->pokes[i]->points) {
            max = work->stage->pokes[i]->points;
        }
    }
    return max;
}

u16 func_ov010_02156978(MusicalEventWork *work) {
    u16 min = 0xff;
    u8 i;

    for (i = 0; i < 4; i++) {
        if (min > work->stage->pokes[i]->points) {
            min = work->stage->pokes[i]->points;
        }
    }
    return min;
}

u8 func_ov010_0215699c(MusicalEventWork *work, u8 rank) {
    return work->ranking[rank];
}

u8 func_ov010_021569a4(MusicalEventWork *work, u8 pos) {
    u8 best;
    u8 i;
    u8 max;
    BOOL tie;
    MusicalPoke *poke;

    max = 0;
    best = 0;
    tie = FALSE;
    i = 0;
    poke = work->stage->pokes[pos];

    while (i < 4) {
        if (max < poke->unk4C[i]) {
            tie = FALSE;
            best = i;
            max = poke->unk4C[i];
        } else if (max == poke->unk4C[i]) {
            tie = TRUE;
        }
        i++;
    }
    if (tie == TRUE) {
        best = 4;
    }
    return best;
}

u8 func_ov010_021569ec(MusicalEventWork *work, u8 pos) {
    u8 index = func_ov010_021568e8(work, pos);
    PlayerInfo *info;

    if (pos == work->playerPos) {
        return 0;
    }
    if (work->online == TRUE) {
        info = func_ov132_021fce28(work->comm, index);
        if (info != NULL) {
            if (getTrainerGender(info) == 0) {
                return 1;
            }
            return 4;
        }
    }
    return func_ov010_02157374(work->program, func_ov010_021568a8(work, index));
}

void func_ov010_02156a3c(MusicalEventWork *work, u8 pos, WordSet *wordSet, u32 wordIndex) {
    BOOL done = FALSE;
    u8 index = func_ov010_021568e8(work, pos);
    PlayerInfo *info;
    u32 msgId;
    MsgData *msgData;
    StrBuf *name;

    if (pos == work->playerPos) {
        copyVarForText(wordSet, wordIndex, GetGameDataPlayerInfo(work->gameData));
        done = TRUE;
    } else if (work->online == TRUE) {
        info = func_ov132_021fce28(work->comm, index);
        if (info != NULL) {
            copyVarForText(wordSet, wordIndex, info);
            done = TRUE;
        }
    }
    if (!done) {
        msgId = func_ov010_02157384(work->program, func_ov010_021568a8(work, index));
        msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_9, HEAPID_GAMEEVENT);
        name = GFL_MsgDataLoadStrbufNew(msgData, msgId);
        func_0201ee0c(wordSet, wordIndex, name, 0, 1, 2);
        GFL_StrBufFree(name);
        GFL_MsgDataFree(msgData);
    }
}

void func_ov010_02156ad4(MusicalEventWork *work, u8 pos, WordSet *wordSet, u32 wordIndex) {
    BOOL done = FALSE;
    u8 index = func_ov010_021568e8(work, pos);
    BoxPkm *pkm;
    u32 msgId;
    MsgData *msgData;
    StrBuf *name;

    if (pos == work->playerPos) {
        GetGameDataPlayerInfo(work->gameData);
        loadPokemonNicknameToStrbuf(wordSet, wordIndex, work->pkm);
        done = TRUE;
    } else if (work->online == TRUE) {
        pkm = func_ov132_021fce44(work->comm, index);
        if (pkm != NULL) {
            loadBoxPokemonNameToStrbuf(wordSet, wordIndex, pkm);
            done = TRUE;
        }
    }
    if (!done) {
        msgId = func_ov010_02157384(work->program, func_ov010_021568a8(work, index));
        msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0098, HEAPID_GAMEEVENT);
        name = GFL_MsgDataLoadStrbufNew(msgData, msgId);
        func_0201ee0c(wordSet, wordIndex, name, 0, 1, 2);
        GFL_StrBufFree(name);
        GFL_MsgDataFree(msgData);
    }
}

static void func_ov010_02156b74(MusicalEventWork *work) {
    u32 program = func_ov132_021fd954(work->comm);
    u32 cpus = func_ov132_021fd95c(work->comm);

    if (work->program == NULL) {
        work->program = func_ov010_02157028(HEAPID_GAMEEVENT, work->ov210, 0);
    }
    func_ov010_02157278(work->program, program);
    func_ov010_021572a8(work->program, cpus);
}


void func_ov010_02156bac(MusicalEventWork *work) {
    if (work->online == TRUE && func_0200fb24() && work->error == FALSE) {
        work->error = TRUE;
    }
}

BOOL func_ov010_02156bcc(MusicalEventWork *work) {
    return work->error;
}
