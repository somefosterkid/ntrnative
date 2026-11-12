// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv_clact.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTLV_CLACT_H
#define BWPORT_BATTLE_BTLV_CLACT_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "gfl/heap.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "gfl/clact.h"

// pokebw2's BtlvClact_AddActor
int func_ov094_02201af8(BtlvClact *wk, u32 arcId, u32 fileId, s16 x, s16 y, fx32 scaleX, fx32 scaleY);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlvClact_Create
BtlvClact *func_ov094_02201a8c(TCBManager *tcbManager, HeapID heapId);
// pokebw2's BtlvClact_Delete
void func_ov094_02201ac4(BtlvClact *wk);
// pokebw2's BtlvClact_Draw
void func_ov094_02201af0(BtlvClact *wk);
// pokebw2's BtlvClact_StartMove
void func_ov094_02201cd0(BtlvClact *wk, int index, int type, ClActorPos *pos, int frames, int wait, int count);
// pokebw2's BtlvClact_StartScale
void func_ov094_02201d5c(BtlvClact *wk, int index, int type, VecFx32 *scale, int frames, int wait, int count);
// pokebw2's BtlvClact_SetAnimSeq
void func_ov094_02201da0(BtlvClact *wk, int index, u32 sequence);
// pokebw2's BtlvClact_StartPalFade
void func_ov094_02201db4(BtlvClact *wk, int index, u8 param3, u8 param4, s8 param5, u16 param6);
// pokebw2's BtlvClact_DeleteActor
void func_ov094_02201ca4(BtlvClact *wk, int index);
// pokebw2's BtlvClact_AddActorEx
int func_ov094_02201b20(BtlvClact *wk, u32 arcId, u32 charFileId, u32 plttFileId, u32 cellFileId, u32 animFileId,
                         s16 x, s16 y, fx32 scaleX, fx32 scaleY);

#endif // BWPORT_BATTLE_BTLV_CLACT_H
