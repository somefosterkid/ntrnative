#include "global.h"

#include "heap.h"
#include "overlay_81.h"
#include "overlay_manager.h"
#include "palette.h"
#include "poke_overlay.h"
#include "render_text.h"
#include "system.h"
#include "overlay_81_battle_factory_app.h"

s32 ov81_02240F08(void *, s32);

/* DS: the callers were built against the wider return type below; the PC takes the definition's (abilint RET) */
#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov81_02240F18(u8);
#else
s32 ov81_02240F18(u8);
#endif

SysTask *ov81_02241BC8(void *);

SysTask *ov81_02243220(void *);

FS_EXTERN_OVERLAY(OVY_80);

s32 ov81_0223E234(OverlayManager *man) {
    OverlayManager *overlayManager;
    void *new_var;
    s32 v7;
    u16 v1;
    void *v0;
    unsigned short *v2;
    s32 v4;
    unsigned short *v5;
    overlayManager = man;
    v0 = OverlayManager_GetData(man);
    if (PaletteData_GetSelectedBuffersBitmask(((BattleFactoryApp *)v0)->plttData) != 0) {
        PaletteData_ScheduleFadeTaskEndIfNoSelectedBuffers(((BattleFactoryApp *)v0)->plttData);
        *((s32 *)&((BattleFactoryApp *)v0)->unk_478) = 0xFF;
        return 0;
    }
    if (ov81_02240F08(v0, 0) == 1) {
        v7 = 0;
        if (ov81_02240F18(((BattleFactoryApp *)v0)->challengeType) > 0) {
            s32 v3;
            v5 = (unsigned short *)v0;
            v3 = 0;
            do {
                v1 = *(u16 *)((u8 *)v5 + offsetof(BattleFactoryApp, selectedIndices));
                v5 += 1;
                *((u16 *)((((void *)((BattleFactoryApp *)v0)->selectedIndicesFinal)) + v3)) = v1;
                v3 += 2;
                v7 += 1;
            } while (v7 < ov81_02240F18(((BattleFactoryApp *)v0)->challengeType));
        }
    } else {
        s32 v6;
        v6 = 0;
        v2 = (unsigned short *)v0;
        v4 = 0;
        do {
            v6 += 1;
            new_var = (((void *)((BattleFactoryApp *)v0)->selectedIndicesFinal)) + v4;
            *((u16 *)new_var) = *(u16 *)((u8 *)v2 + offsetof(BattleFactoryApp, selectedIndices));
            v2 += 1;
            v4 += 2;
        } while (v6 < 2);
        if ((((u32)((*((u8 *)((u8 *)&((BattleFactoryApp *)v0)->numPokeballs + 0x1))) << 0x1C)) >> 0x1F) == 0) {
            *((s16 *)(((void *)((BattleFactoryApp *)v0)->selectedIndicesFinal))) = 0xFF;
            *((s16 *)(((u8 *)(((void *)((BattleFactoryApp *)v0)->selectedIndicesFinal))) + 2)) = 0xFF;
        }
    }
    TextFlags_SetCanTouchSpeedUpPrint(0);
    ov81_02241BC8(((BattleFactoryApp *)v0)->yesNoPrompt);
    ov81_02243220(((BattleFactoryApp *)v0)->inputHandler);
    ov81_02240BB0(v0);
    OverlayManager_FreeData(overlayManager);
    Main_SetVBlankIntrCB(0L, 0L);
    Heap_Destroy(HEAP_ID_100);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_80));
    return 1;
}
