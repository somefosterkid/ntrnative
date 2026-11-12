#ifndef FIELD_OV018_0217C458_H
#define FIELD_OV018_0217C458_H

#include "types.h"
#include "struct_decls.h"

struct IntrudeCommEvent;

// The player turning round on the spot as an Entralink warp starts or ends (func_ov018_0217c658/c688), or
// stepping out beside a double of itself (func_ov018_0217c758/c820)
typedef struct Ov018WarpSpin {
    GameSystem *gsys;
    Field *field;
    s16 unk_08;
    // The frames of a turn: down to 1 before the warp, back up to 0xf after it
    s16 unk_0a;
    u16 unk_0c;
    u8 unk_0e;
    u8 unk_0f;
    u8 state;
    // Which of the four directions the player faces now
    u8 unk_11;
    // Set once the warp is made
    u8 unk_12;
    u8 direction;
    // The double c758 makes, NULL while there is none
    FieldActor *unk_14;
} Ov018WarpSpin;

GameEvent *func_ov018_0217ca1c(GameSystem *gsys);
GameEvent *func_ov018_0217ca84(GameSystem *gsys, u32 param_2);
GameEvent *func_ov018_0217c458(GameSystem *, Field *, IntrudeComm *, u16);
void func_ov018_0217c658(Ov018WarpSpin *, GameSystem *, Field *, u16, u8, u8);
u32 func_ov018_0217c688(Ov018WarpSpin *, IntrudeComm *);
void func_ov018_0217c758(Ov018WarpSpin *param_1, GameSystem *gsys, Field *field, IntrudeComm *param_4, u16 param_5, u8 param_6, u8 param_7, u16 param_8, s32 param_9);
u32 func_ov018_0217c820(Ov018WarpSpin *param_1, IntrudeComm *param_2);
u32 func_ov018_0217c914(IntrudeComm *work, struct IntrudeCommEvent *param_2, u8 *state);

#endif // FIELD_OV018_0217C458_H
