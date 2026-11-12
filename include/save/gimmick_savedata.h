// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/gimmick_savedata.h: the lines in use so
// far.
#ifndef BWPORT_SAVE_GIMMICK_SAVEDATA_H
#define BWPORT_SAVE_GIMMICK_SAVEDATA_H

#include "types.h"
#include "struct_decls.h"

#define GIMMICK_WORK_MAX 0x20

// Black 2's; matching code confirms the offsets of id, work and unk84, and its size in Black
struct GimmickState {
    // -1 for none
    s32 id;
    // The gimmick's own data
    u32 work[GIMMICK_WORK_MAX];
    u8 unk84[0x5];
    u8 unk89[0xb];
}; // size: 0x94

void GimmickState_StoreSave(const GimmickState *gimmick, SaveControl *save);
void GimmickState_LoadSave(SaveControl *save, GimmickState *gimmick);

// The two areas after the user data. The ID is not checked
void *func_0200b3a8(GimmickState *gimmick, u32 gimmickId);

u32 GimmickSave_GetBlockSize(void);
void GimmickSave_Init(GimmickState *gimmick);
void GimmickState_Reset(GimmickState *gimmick);
// Resets the state for a gimmick and returns its user data
void *GimmickState_SetID(GimmickState *gimmick, u32 gimmickId);

#endif // BWPORT_SAVE_GIMMICK_SAVEDATA_H
