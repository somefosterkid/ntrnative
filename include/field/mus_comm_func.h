// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/mus_comm_func.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_MUS_COMM_FUNC_H
#define BWPORT_FIELD_MUS_COMM_FUNC_H

#include "types.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "struct_decls.h"

// The communication work of a show, whose layout only mus_comm_func.c needs
typedef struct MusCommWork MusCommWork;

void func_ov132_021fc644(HeapID heapId, GameSystem *gsys, GameCommSys *comm, u16 value);
void func_ov132_021fc684(MusCommWork *comm);
// Its GameCommSys callbacks for GAME_COMM_NO_MUSICAL (see game_comm.c)
void *func_ov132_021fc694(u32 *seq, void *param);
BOOL func_ov132_021fc6ec(u32 *seq, void *param, void *work);
BOOL func_ov132_021fc7dc(u32 *seq, void *param, void *work);
void func_ov132_021fc804(MusCommWork *comm);
void func_ov132_021fc81c(MusCommWork *comm);
// Sends a timing number to synchronize on, and whether the others reached it (or the connection failed)
void func_ov132_021fcdec(MusCommWork *comm, u8 timing);
BOOL func_ov132_021fce00(MusCommWork *comm, u8 timing);
PlayerInfo *func_ov132_021fce28(MusCommWork *comm, u8 index);
BoxPkm *func_ov132_021fce44(MusCommWork *comm, u8 index);
MusicalPoke *func_ov132_021fd4f8(MusCommWork *comm, u8 index);
// pokebw2's func_ov211_021f00c8 and func_ov211_021f04a0
void func_ov132_021fd52c(MusCommWork *comm);
BOOL func_ov132_021fd904(MusCommWork *comm);
// Whether the sound data has all arrived
BOOL func_ov132_021fd6a4(MusCommWork *comm);
BOOL func_ov132_021fd83c(MusCommWork *comm);
BOOL func_ov132_021fd844(MusCommWork *comm);
BOOL func_ov132_021fd8c4(MusCommWork *comm, u8 equip);
u8 func_ov132_021fd8d4(MusCommWork *comm);
u8 func_ov132_021fd8ec(MusCommWork *comm, u8 index);
u16 *func_ov132_021fd8f8(MusCommWork *comm, u8 index);
BOOL func_ov132_021fd908(MusCommWork *comm);
void func_ov132_021fd918(MusCommWork *comm, u32 a1, u32 a2);
void func_ov132_021fd938(MusCommWork *comm);
void func_ov132_021fd948(MusCommWork *comm, MusicalPoke *poke);
u8 func_ov132_021fd964(MusCommWork *comm, u8 index, u8 a2);
void func_ov132_021fd974(MusCommWork *comm, u8 pos, u8 equip);
u8 func_ov132_021fd9a0(MusCommWork *comm, u8 pos);
void func_ov132_021fd9d0(MusCommWork *comm, u8 pos);
u8 func_ov132_021fd9fc(MusCommWork *comm);
void func_ov132_021fda18(MusCommWork *comm);
void func_ov132_021fda24(MusCommWork *comm, u8 pos, u8 equip, u32 result);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov132_021fc7f8(u32 *seq, void *param, void *work);
void func_ov132_021fc848(MusCommWork *comm, PlayerInfo *info, BoxPkm *pkm, GameCommSys *gameComm, Ov210Work *ov210,
                         HeapID heapId);
BOOL func_ov132_021fd910(MusCommWork *comm);
u32 func_ov132_021fd954(MusCommWork *comm);
u32 func_ov132_021fd95c(MusCommWork *comm);
const GFLNetInitData *func_ov132_021fda6c(GameData *gameData);

#endif // BWPORT_FIELD_MUS_COMM_FUNC_H
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/mus_comm_func.h: the lines in use so
// far.
#ifndef BWPORT_FIELD_MUS_COMM_FUNC_H

#include "types.h"

// The communication work of a show, whose layout only mus_comm_func.c needs

// Sends a timing number to synchronize on, and whether the others reached it (or the connection failed)
// Whether the sound data has all arrived

#endif // BWPORT_FIELD_MUS_COMM_FUNC_H
