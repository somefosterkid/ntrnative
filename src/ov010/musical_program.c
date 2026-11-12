// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/musical_program.c, which is the same code
// in Black 2/White 2's overlay 12, with the musical's data in Black's overlay 112.
#include "types.h"
#include "app/musical/mus_item_data.h"
#include "app/musical/musical_system.h"
#include "field/musical_program.h"
#include "field/musical_stage_sys.h"
#include "gfl/heap.h"
#include "gfl/random.h"

// A performer of the program, other than the players
typedef struct {
    u16 species;
    // The trainer class shown for the performer
    u8 trainerClass;
    // The message of the performer's name
    u8 name;
    u8 unk4;
    u8 form;
    u16 unk6;
    // The photos the player must have taken before the performer appears
    u16 minShots;
    u16 unkA;
    struct {
        u16 itemId;
        u8 slot;
        u8 unk3;
    } equips[8];
} MusicalNpc;

// The program's data, which overlay 112 loads
typedef struct {
    u8 unk0;
    // The points the audience has for each kind of prop
    u8 points[4];
    // The points the program shares out among the kinds of prop at random, by fives
    u8 bonus;
    // The program's list of props to give away
    u8 propList;
    u8 unk7[5];
    MusicalNpc npcs[6];
} MusicalProgramData;

struct MusicalProgram {
    u8 points[4];
    // The points of each prop of a kind, the kind's points shared out among the props worn
    u8 propPoints[4];
    // The performers, as indexes in the data
    u8 npcs[3];
    MusicalProgramData *data;
};

MusicalProgram *func_ov010_02157028(HeapID heapId, Ov210Work *ov210, u16 shots) {
    MusicalProgram *program;
    u8 i;
    u8 left;
    u8 add;
    u8 count;
    u8 a;
    u8 b;
    u8 temp;
    u8 candidates[6];
    s8 *list;

    program = GFL_HeapAllocate(heapId, sizeof(MusicalProgram), FALSE, "musical_program.c", 100);
    program->data = ov210->unk4;
    for (i = 0; i < 4; i++) {
        program->points[i] = 0;
        program->propPoints[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        program->points[i] = program->data->points[i];
    }
    left = program->data->bonus / 5;
    while (left != 0) {
        add = GFL_RandomMTRange(3) + 1;
        if (add > left) {
            add = left;
        }
        program->points[GFL_RandomMTRange(4)] += add * 5;
        left -= add;
    }
    list = candidates;
    count = 0;
    list[0] = 0;
    list[1] = 0;
    list[2] = 0;
    list[3] = 0;
    list[4] = 0;
    list[5] = 0;
    for (i = 0; i < 6; i++) {
        if (program->data->npcs[i].minShots <= shots) {
            list[count] = i;
            count++;
        }
    }
    if (count < 3) {
        candidates[0] = 0;
        candidates[1] = 1;
        candidates[2] = 2;
        count = 3;
    }
    for (i = 0; i < 30; i++) {
        a = GFL_RandomMTRange(count);
        b = GFL_RandomMTRange(count);
        temp = candidates[a];
        candidates[a] = candidates[b];
        candidates[b] = temp;
    }
    for (i = 0; i < 3; i++) {
        program->npcs[i] = candidates[i];
    }
    return program;
}

void func_ov010_0215716c(MusicalProgram *program) {
    GFL_HeapFree(program);
}

// Shares out each kind's points among the props of that kind on the stage, and gives the Pokémon their points
void func_ov010_02157174(HeapID heapId, MusicalProgram *program, MusicalStageParam *stage) {
    MusItemData *items = func_ov112_021f4f14(heapId);
    u8 k;
    u8 i;
    u8 j;
    u8 kind;
    u8 value;
    u8 totals[4];
    u8 counts[4][4];
    MusicalPoke *poke;
    MusicalPoke *dst;

    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            counts[i][j] = 0;
        }
        totals[j] = 0;
    }
    for (i = 0; i < 4; i++) {
        poke = stage->pokes[i];
        for (k = 0; k < 9; k++) {
            if (poke->equips[k].itemId != 0xff) {
                kind = func_ov112_021f5140(items, poke->equips[k].itemId);
                counts[i][kind]++;
                totals[kind]++;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        program->propPoints[i] = program->points[i] / totals[i];
    }
    for (i = 0; i < 4; i++) {
        dst = stage->pokes[i];
        for (k = 0; k < 4; k++) {
            value = program->propPoints[k] * counts[i][k];
            dst->points += value;
            dst->unk4C[k] = value;
        }
    }
    func_ov112_021f4f40(items);
}

u8 func_ov010_0215725c(MusicalProgram *program, u8 kind) {
    return program->points[kind];
}

u32 func_ov010_02157260(MusicalProgram *program) {
    return program->points[0] + (program->points[1] << 8) + (program->points[2] << 16) + (program->points[3] << 24);
}

void func_ov010_02157278(MusicalProgram *program, u32 value) {
    program->points[0] = value;
    program->points[1] = (value & 0xff00) >> 8;
    program->points[2] = (value & 0xff0000) >> 16;
    program->points[3] = (value & 0xff000000) >> 24;
}

u32 func_ov010_02157298(MusicalProgram *program) {
    return program->npcs[0] + (program->npcs[1] << 8) + (program->npcs[2] << 16);
}

void func_ov010_021572a8(MusicalProgram *program, u32 value) {
    program->npcs[0] = value;
    program->npcs[1] = (value & 0xff00) >> 8;
    program->npcs[2] = (value & 0xff0000) >> 16;
}

// The kind of prop with the most points
u8 func_ov010_021572c0(MusicalProgram *program) {
    u8 i;
    u8 max = 0;
    u8 best = 0;

    for (i = 0; i < 4; i++) {
        if (max < program->points[i]) {
            max = program->points[i];
            best = i;
        }
    }
    return best;
}

// Puts a performer of the program on the stage
void func_ov010_021572e4(MusicalProgram *program, MusicalStageParam *stage, u8 pos, u8 cpu, HeapID heapId) {
    MusicalNpc *npc = &program->data->npcs[program->npcs[cpu]];
    u8 i;

    func_ov010_02156fd0(stage, pos, npc->species, npc->form,
                        npc->trainerClass + (npc->name << 8) + (npc->unk4 << 16) + (npc->form << 24), npc->unk6,
                        heapId);
    for (i = 0; i < 8; i++) {
        if (npc->equips[i].itemId != 0x1fe && npc->equips[i].itemId != 0x1ff) {
            func_ov010_0215700c(stage, pos, npc->equips[i].slot, npc->equips[i].itemId, 0, i);
        }
    }
}

u8 func_ov010_02157364(MusicalProgram *program) {
    return program->data->unk0;
}

u8 func_ov010_0215736c(MusicalProgram *program) {
    return program->data->propList;
}

u8 func_ov010_02157374(MusicalProgram *program, u8 cpu) {
    return program->data->npcs[program->npcs[cpu]].trainerClass;
}

u8 func_ov010_02157384(MusicalProgram *program, u8 cpu) {
    return program->data->npcs[program->npcs[cpu]].name;
}

u8 func_ov010_02157394(MusicalProgram *program, u8 kind) {
    return program->propPoints[kind];
}
