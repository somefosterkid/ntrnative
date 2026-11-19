#include "global.h"

#include "unk_02035900.h"
#include "unk_020379A0.h"
#include "comm_tool.h"


// Copies a 0x46-byte comm payload (u16[0x23], e.g. BattleTower commPayload / FrontierFsys unk83e) into
// slot a0 and sends command 0x14. Callers with a smaller source over-read it: the ov44 callers pass one
// s16 local, ov49_02262028 passes &work->unk_0C of a 0x44-byte work (0x14 bytes past its end). The DS
// does the same, so the copy size stays; the source is not a larger embedded object.
u32 sub_02037C0C(u32 a0, s16 *a1) {
    if (_021D414C != 0) {
        MI_CpuCopy8(a1, _021D414C->unk_18[a0], 0x46);
        sub_020376E0(0x14, _021D414C->unk_18[a0]);
        return 1;
    }
    return 0;
}

u16 *sub_02037C44(s32 a0) {
    if (_021D414C->hasRecievedTempData[a0] != 0) {
        return (u16 *)_021D414C->unk_18[a0];
    }
    return NULL;
}
