// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/sp_poke_gimmick.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_SP_POKE_GIMMICK_H
#define BWPORT_FIELD_SP_POKE_GIMMICK_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"

// The work block 1 of the gimmick, 4 bytes. Its events read and write these three bytes, named by their offsets
typedef struct {
    u8 unk_0;
    u8 unk_1;
    u8 unk_2;
} SpPokeGimmickWork;

// Black's NPC data, at 0x24 of the ov059 gimmick block that func_ov059_021f3cfc reads: the actor and uid it sets
struct SpPokeGimmickNpc {
    u16 actorId;
    u16 uid;
};

// Black's gimmick block 1 of ov059 (0x28 bytes): a jump's mode, start, end, height, frames and frame, then the NPC data
typedef struct {
    u32 mode;
    VecFx32 start;
    VecFx32 end;
    fx32 height;
    u16 frames;
    u16 frame;
    SpPokeGimmickNpc *npc;
} SpPokeGimmickBlock;

// pokebw2's SpPokeGimmickA_End
void func_ov042_021f380c(Field *field);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's SpPokeGimmickA_Init
void func_ov042_021f36c0(Field *field);
void func_ov042_021f3828(Field *field);
GameEvent *func_ov042_021f3834(GameSystem *gsys, u32 state, u8 flag);
GameEvent *func_ov042_021f3888(GameSystem *gsys, u32 state);

#endif // BWPORT_FIELD_SP_POKE_GIMMICK_H
