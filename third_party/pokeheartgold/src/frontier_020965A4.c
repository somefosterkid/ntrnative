#include "global.h"

#include "error_handling.h"
#include "frontier.h"
#include "overlay_manager.h"
#include "frontier_work.h"

s32 FrontierSystem_AddTask(void *, int, s32);

void *FrontierSystem_Create(void *, s32, int);

s32 Frontier_CreateMap(void *);

s32 Frontier_LoadOverlays(void);

s32 sub_02096780(void *);

s32 sub_02096884(void *);

s32 Frontier_Init(OverlayManager *man) {
    void *v1;
    void *dest;
    void *v0;

    Frontier_LoadOverlays();
    dest = OverlayManager_CreateAndGetData(man, sizeof(FrontierWork), HEAP_ID_FIELD2);
    MI_CpuFill8(dest, 0U, sizeof(FrontierWork));
    sub_02096780(dest);
    sub_02096884(dest);
    v0 = OverlayManager_GetArgs(man);
    *(void **)dest = v0;
    if (v0 == NULL) {
        GF_AssertFail();
    }
    v1 = FrontierSystem_Create(dest, 0xB, *(u8 *)((u8 *)(*(void **)dest) + 0x20));
    *((void **)&((FrontierWork *)dest)->system) = v1;
    FrontierSystem_AddTask(v1, *(u8 *)((u8 *)(*(void **)dest) + 0x20), 0);
    Frontier_CreateMap(dest);
    return 1;
}
