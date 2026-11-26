#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
struct UnkStruct_ov14_boxapp;

s32 ov14_021F1090(void *, s32);

struct UnkStruct_ov14_boxsys;
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);

s32 ov14_021F3488(void *, s32, s32);

void ov14_021F6654(struct UnkStruct_ov14_boxsys *param_1, s32 param_2);

void ov14_021F2734(struct UnkStruct_ov14_boxapp *arg0);

void ov14_021F2734(UnkStruct_ov14_boxapp *arg0) {
    ov14_021F6654(arg0->sys, 0x27);
    ov14_021F2A18(arg0->sys, 9, 0);
    ov14_021F3488(arg0, 1, 1);
    ov14_021F1090(arg0, 0x3F);
}

enum PokemonContestType {
    CONTEST_TYPE_COOL = 0,
    CONTEST_TYPE_BEAUTY = 1,
    CONTEST_TYPE_CUTE = 2,
    CONTEST_TYPE_SMART = 3,
    CONTEST_TYPE_TOUGH = 4,
    CONTEST_TYPE_MAX = 5,
};

u32 ov14_021F027C(enum PokemonContestType contestType, s8 competitionType);

u32 ov14_021F2760(enum PokemonContestType contestType);

u32 ov14_021F2760(enum PokemonContestType contestType) {
    return ov14_021F027C(contestType, 2);
}

u32 ov14_021F0F0C(enum PokemonContestType contestType, int competitionType);

u32 ov14_021F276C(enum PokemonContestType contestType);

u32 ov14_021F276C(enum PokemonContestType contestType) {
    return ov14_021F0F0C(contestType, 2);
}
