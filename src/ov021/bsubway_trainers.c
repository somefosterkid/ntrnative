// Black's code at ov021 0x021cbd08-0x021cbfb0: the Battle Subway's trainers and their teams. pokebw2 has these
// functions as a file of their own (src/ov033/bsubway_trainers.c, whose name is descriptive), with these five tables
// as its .rodata, laid out after ov021_021ca544.c's.

#include "types.h"
#include "app/ircbattlematch.h"
#include "app/ov306.h"
#include "battle/battle_proc.h"
#include "battle/battle_rec_tool.h"
#include "battle/btl_setup.h"
#include "battle/regulation.h"
#include "constants/directions.h"
#include "constants/pokemon.h"
#include "field/battle_facility.h"
#include "field/bsubway_scr.h"
#include "field/event_wifi_bsubway.h"
#include "field/field.h"
#include "field/field_actor.h"
#include "field/field_gimmick_bsubway.h"
#include "field/player_state.h"
#include "field/zone.h"
#include "gfl/net.h"
#include "gfl/net_system.h"
#include "gfl/overlay.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "save/box.h"
#include "save/event_work.h"
#include "save/player_info.h"
#include "system/game_event.h"
#include "system/iss_switch_sys.h"
#include "system/iss_sys.h"
#include "field/battle_facility.h"
#include "field/bsubway_scr.h"
#include "field/field_actor.h"
#include "gfl/heap.h"
#include "struct_decls.h"
#include "field/field_script.h"
#include "system/game_data.h"
#include "system/game_system.h"
#include "system/vm.h"
#include "gfl/random.h"
#include "pml/poke_party.h"
#include "save/save_control.h"
#include "save/bsubway_save.h"
#include "field/union_oneself.h"
#include "field/field_effect.h"

u16 randFFFFFFFFdivFFFF(BSubwayScrWork *bsw);

BtlSetup *func_ov021_021cbd08(BSubwayScrWork *bsw, GameSystem *gsys) {
    PokeParty *party;
    PokeParty *source;
    PartyPkm *pkm;
    BtlSetup *result;
    s32 i;

    party = PokeParty_Create(0x8004);
    source = func_ov021_021ca210(bsw);
    PokeParty_InitCore(party, bsw->memberCount);
    for (i = 0; i < bsw->memberCount; i++) {
        pkm = PokeParty_GetPkm(source, bsw->memberSlots[i]);
        PokeParty_AddPkm(party, pkm);
    }
    result = SetupTrialHouseBattle(gsys, party, bsw->playMode, bsw->trainers, &bsw->unk2C8[bsw->unkC_5], bsw->memberCount);
    GFL_HeapFree(party);
    return result;
}



s32 func_ov021_021cbd84(s32 param_1) {
    return param_1 + 0x74c;
}


// The trainers of the super trains from the fifth streak on
static const BSubwayTrainerRange data_ov021_021db3cc[1] = {
    { 200, 299 },
};

// The trainers of a normal train's last car, by streak
static const BSubwayTrainerRange data_ov021_021db3d0[2] = {
    { 50, 69 },
    { 70, 89 },
};

// The trainers of a normal train's first six cars, by streak
static const BSubwayTrainerRange data_ov021_021db3d8[3] = {
    { 0, 49 },
    { 30, 69 },
    { 50, 109 },
};

// The trainers of a super train's first six cars, by streak
static const BSubwayTrainerRange data_ov021_021db3e4[4] = {
    { 110, 159 },
    { 140, 179 },
    { 160, 199 },
    { 180, 219 },
};

// The trainers of a super train's last car, by streak
static const BSubwayTrainerRange data_ov021_021db3f4[4] = {
    { 160, 179 },
    { 180, 199 },
    { 200, 219 },
    { 220, 239 },
};

static inline u16 BSubwayScr_PickTrainerInRanges(BSubwayScrWork *bsw, const BSubwayTrainerRange *ranges, u16 level) {
    const BSubwayTrainerRange *range = ranges + level;
    u16 count = range->max - range->min + 1;

    return range->min + randFFFFFFFFdivFFFF(bsw) % count;
}

u16 func_ov021_021cbd90(BSubwayScrWork *bsw, u16 level, u8 index, u32 mode, u8 side) {
    BOOL super;
    s32 boss;
    u16 trainerId;

    trainerId = 0;
    super = FALSE;
    boss = -1;
    switch (mode) {
    case 5:
    case 6:
    case 7:
    case 8:
        super = TRUE;
        break;
    }
    if (index == 6) {
        if (!super) {
            if (level == 2) {
                boss = 0;
            }
        } else if (level == 6) {
            boss = 1;
        }
    }
    if (boss != -1) {
        switch (mode) {
        case 0:
        case 5:
            if (boss == 0) {
                trainerId = 0x132;
            } else if (boss == 1) {
                trainerId = 0x133;
            }
            break;
        case 1:
        case 6:
            if (boss == 0) {
                trainerId = 0x134;
            } else if (boss == 1) {
                trainerId = 0x135;
            }
            break;
        case 2:
        case 3:
        case 7:
        case 8:
            if (boss == 0) {
                if (side == 0) {
                    trainerId = 0x137;
                } else {
                    trainerId = 0x136;
                }
            } else if (boss == 1) {
                if (side == 0) {
                    trainerId = 0x139;
                } else {
                    trainerId = 0x138;
                }
            }
            break;
        }
    } else if (!super) {
        if (level < 3) {
            if (index < 6) {
                trainerId = BSubwayScr_PickTrainerInRanges(bsw, data_ov021_021db3d8, level);
            } else if (level < 2) {
                trainerId = BSubwayScr_PickTrainerInRanges(bsw, data_ov021_021db3d0, level);
            }
        }
    } else if (level < 4) {
        if (index < 6) {
            trainerId = BSubwayScr_PickTrainerInRanges(bsw, data_ov021_021db3e4, level);
        } else {
            trainerId = BSubwayScr_PickTrainerInRanges(bsw, data_ov021_021db3f4, level);
        }
    } else {
        trainerId = data_ov021_021db3cc[0].min +
                    randFFFFFFFFdivFFFF(bsw) % (data_ov021_021db3cc[0].max - data_ov021_021db3cc[0].min + 1);
    }
    return trainerId;
}


BOOL func_ov021_021cbed8(BSubwayScrWork *bsw, BSubwayTrainer *trainer, u16 trainerId, u32 count, const u16 *species, const u16 *items, BSubwayTeamConfig *config, HeapID heapId) {
    BOOL BVar1;

    BVar1 = func_ov010_02168878(trainer, trainerId, count, species, items, config, heapId);
    return BVar1;
}

u8 func_ov021_021cbefc(u32 value) {
    if (value < 100) {
        return 3;
    }
    if (value < 0x78) {
        return 6;
    }
    if (value < 0x8c) {
        return 9;
    }
    if (value < 0xa0) {
        return 0xc;
    }
    if (value < 0xb4) {
        return 0xf;
    }
    if (value < 200) {
        return 0x12;
    }
    if (value < 0xdc) {
        return 0x15;
    }
    return 0x1f;
}

void func_ov021_021cbf38(BSubwayScrWork *param_1, BSubwayTrainer *trainer, u16 trainerId, BOOL rentalItem, BSubwayTeamConfig *config, HeapID heapId) {
    u8 iv;
    void *ptr;
    s32 i;

    ptr = func_ov010_021688bc(trainer, trainerId, heapId);
    iv = func_ov021_021cbefc(trainerId);
    for (i = 0; i < 2; i++) {
        func_ov010_021684e8(&trainer->pokemon[i], config->files[i], config->id, config->pids[i], iv, i, rentalItem, heapId);
    }
    GFL_HeapFree(ptr);
}


u16 randFFFFFFFFdivFFFF(BSubwayScrWork *bsw) {
    return GFL_RandomLC(0xffffffff) / 0xffff;
}

