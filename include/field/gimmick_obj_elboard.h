// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/gimmick_obj_elboard.h: the lines in
// use so far.
#ifndef BWPORT_FIELD_GIMMICK_OBJ_ELBOARD_H
#define BWPORT_FIELD_GIMMICK_OBJ_ELBOARD_H

#include "types.h"
#include "gfl/g3d.h"
#include "nitro/fx/fx.h"
#include "system/game_event.h"
#include "struct_decls.h"

struct ElboardInit {
    u16 heapId;
    // The most messages the board holds
    u8 capacity;
    u8 unk03;
    u8 unk04;
    G3DActor *actor;
};

// Black 2's, not checked against Black
struct ElboardMessageArg {
    u16 kind;
    u16 padding;
    const char *name;
    const char *plName;
    u32 messageArc;
    u32 messageFile;
    u32 messageId;
    WordSet *wordSet;
};

void func_ov048_021f472c(Elboard *state, ElboardMessageArg *arg);
void func_ov048_021f474c(Elboard *state);
void func_ov048_021f4784(Elboard *state, u32 value);
void func_ov048_021f4b40(ElboardMessage *message);
void func_ov048_021f4b54(ElboardMessage *message);
void func_ov048_021f4b7c(Elboard *state, ElboardMessage *message, u32 amount);
void func_ov048_021f4bf0(ElboardMessage *message);
BOOL func_ov048_021f4c10(void *dest, u32 arcId, u32 fileId);
GameEventReturnCode func_ov048_021f4c20(GameEvent *event, u32 *state, void *data);
GameEvent *func_ov048_021f4dbc(GameSystem *gsys, Field *field, u32 duration);
BOOL func_ov048_021f4de4(GimmickGateBoardEntry *entry, u32 arc, u32 index);
BOOL func_ov048_021f4e3c(GimmickGateBoardEntry *entry);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
Elboard *func_ov048_021f4698(ElboardInit *init);
s32 func_ov048_021f47b0(Elboard *state);
u16 func_ov048_021f47b4(Elboard *state);
u8 func_ov048_021f47b8(Elboard *state);
void func_ov048_021f47bc(Elboard *state, fx32 time);
ElboardMessage *func_ov048_021f4948(Elboard *state, const ElboardMessageArg *arg, u32 index);
BOOL func_ov048_021f4df4(GimmickGateBoardEntry *entry, u16 zone);

#endif // BWPORT_FIELD_GIMMICK_OBJ_ELBOARD_H
