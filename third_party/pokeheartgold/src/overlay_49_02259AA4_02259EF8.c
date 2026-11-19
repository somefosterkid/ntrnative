#include "global.h"

#include "heap.h"
#include "overlay_49.h"
#include "overlay_manager.h"
#include "system.h"
#include "unk_02005D10.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

s32 ov49_02258994(void *);

s32 ov49_02258B20(void *);

s32 ov49_0225A7D0(void *);

s32 ov49_0225AB14(void *);

s32 ov49_0225ACBC(void *);

s32 ov49_0225AE4C(void *, void *);

s32 ov49_0225B0D8(void *);

s32 ov49_0225B200(void *);

SysTask *ov49_0225B244(void *);

s32 ov49_0225B35C(void *);

s32 ov49_0225B4E4(void *, void *, void *);

s32 ov49_0225CBDC(void *);

s32 ov49_0225E2B4(void *);

s32 ov49_0225EEF8(void *);

s32 ov49_0226535C(void *);

s32 ov49_02268850(void *);

s32 ov49_02259EF8(OverlayManager *man, int *state) {
    UnkStruct_ov70_0225DEE8 *ptr;

    ptr = OverlayManager_GetData(man);
    *(s32 *)((u8 *)(OverlayManager_GetArgs(man)) + 0x18) = (s32)ptr->unk_01;
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    ov49_02268850(ptr->unk_440);
    ov49_0226535C(ptr->unk_438);
    ov49_0225EEF8(ptr->unk_454);
    ov49_0225B4E4(&ptr->unk_1E8, &ptr->unk_37C, &ptr->unk_3C);
    ov49_0225B35C(&ptr->unk_340);
    ov49_0225AB14(&ptr->unk_35C);
    ov49_0225ACBC(&ptr->unk_37C);
    ov49_0225AE4C(&ptr->unk_39C, &ptr->unk_3C);
    ov49_0225B244(&ptr->unk_3F4);
    ov49_0225B200(&ptr->unk_404);
    ov49_0225B0D8(&ptr->unk_428);
    ov49_0225CBDC(ptr->unk_450);
    ov49_0225E2B4(ptr->unk_448);
    ov49_02258B20(ptr->unk_444);
    ov49_02258994(ptr->unk_44C);
    ov49_0225A7D0(&ptr->unk_3C);
    Heap_Free(ptr);
    Heap_Destroy(HEAP_ID_119);
    Heap_Destroy(HEAP_ID_120);
    sub_0200616C(0);
    sub_02006300(0);
    return 1;
}
