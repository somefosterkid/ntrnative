#include "global.h"
struct WifiBattleTowerAppState;
#include "system.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

extern void *_021EA940;

void *ov73_021E83F4(s32 unused, u32 size, s32 alignment);

int ov73_021E8440(void);
void ov73_021E844C(struct WifiBattleTowerAppState *arg0, s32 arg1, s32 arg2);

void ov73_021E8100(void *);
s32 ov73_021E8128(void);
s32 ov73_021E8168(void);
s32 ov73_021E8198(void *);
s32 ov73_021E82D4(void *);
void ov73_021E8454(void *arg0);

void *ov73_021E83F4(s32 unused, u32 size, s32 alignment) {
    u32 state;
    void *v0;

    state = OS_DisableInterrupts();
    v0 = NNS_FndAllocFromExpHeapEx(*(NNSiFndHeapHead **)((u8 *)(&_021EA940) + 4), size, alignment);
    OS_RestoreInterrupts(state);
    return v0;
}

void ov73_021E841C(s32 unused1, void *ptr, u32 unused2);

void ov73_021E841C(s32 unused1, void *ptr, u32 unused2) {
    u32 state;

    if (ptr != NULL) {
        state = OS_DisableInterrupts();
        NNS_FndFreeToExpHeap(*(NNSiFndHeapHead **)((u8 *)(&_021EA940) + 4), ptr);
        OS_RestoreInterrupts(state);
    }
}

int ov73_021E8440(void) {
    return WM_LINK_LEVEL_3 - DWC_GetLinkLevel();
}

void ov73_021E844C(WifiBattleTowerAppState *arg0, s32 arg1, s32 arg2) {
    *((s32 *)&arg0->exitMode) = arg1;
    *((s32 *)&arg0->unused_18) = arg2;
}

void ov73_021E8454(void *arg) {
    ov73_021E8128();
    ov73_021E8168();
    ov73_021E8198(arg);
    ov73_021E82D4(arg);
    Main_SetVBlankIntrCB(ov73_021E8100, arg);
}
