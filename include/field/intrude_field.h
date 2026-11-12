#ifndef POKEBW_FIELD_INTRUDE_FIELD_H
#define POKEBW_FIELD_INTRUDE_FIELD_H

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "field/intrude_comm.h"

typedef struct IntrudeFieldUnkEntry {
    u32 unk_00_0 : 10;
    u32 unk_00_10 : 10;
    u32 unk_00_20 : 10;
    u32 unk_00_30 : 2;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0a;
} IntrudeFieldUnkEntry;

BOOL func_ov018_0217bed4(u16 zoneId, VecFx32 *param_2);
IntrudeFieldUnkEntry *func_ov018_0217bea8(u32 param_1);
void func_ov018_0217bcdc(IntrudeComm *comm, GameSystem *gsys, u16 param_3, u8 param_4, u8 param_5);
void func_ov018_0217bf1c(IntrudeHighLinkData *data);
u8 func_ov018_0217bf28(IntrudeHighLinkData *data);
GameEvent *func_ov018_0217b4d8(GameSystem *gsys, Field *field, FieldPlayer *param_3, VecFx32 *param_4, s32 param_5,
                               u16 param_6);
void func_ov018_0217ae20(Field *field, GameSystem *gsys, FieldPlayer *player);
u32 func_ov018_0217afc4(IntrudeComm *comm, FieldPlayer *player, u32 *param_3);
GameEvent *func_ov018_0217b410(Field *field, GameSystem *gsys, FieldPlayer *player);
void func_ov018_0217bbcc(Field *field, GameSystem *gsys, IntrudeComm *comm);
void func_ov018_0217bbd8(Field *field, GameSystem *gsys, IntrudeComm *comm);
void func_ov018_0217bc94(Field *field, s32 count);
GameEvent *func_ov018_0217bde0(GameSystem *gsys, IntrudeComm *comm, FieldPlayer *player);
void func_ov018_0217bf84(Field *field);
BOOL func_ov018_0217bfd8(GameSystem *gsys, u16 zoneId);
BOOL func_ov018_0217c070(IntrudeComm *work);
// The event of the hub closing; fieldmap_event.c's call passes the field too (Black's r1), which it doesn't read
GameEvent *func_ov018_0217b810(GameSystem *gsys, Field *field);
void func_ov018_0217c094(IntrudeComm *comm);
u32 func_ov018_0217c0a8(GameCommSys *comm);
void func_ov018_0217c194(Field *field);

#endif // POKEBW_FIELD_INTRUDE_FIELD_H
