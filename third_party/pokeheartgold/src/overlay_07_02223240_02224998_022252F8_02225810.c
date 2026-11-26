#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sys_task.h"
#include "overlay_07_palette_fade.h"

// the DS layout of FadePokemonSpriteContext (overlay_07_02223240_02224998_022252F8_02225838.c): the host pointers make the
// members sit later than their DS offsets (4 / 0x30)
typedef struct UnkStruct_ov07_02225810_ptr {
    BattleAnimScriptFuncCommon common;
    BattleAnimSpriteInfo spriteInfo;
    PaletteFadeContext *paletteFade;
} UnkStruct_ov07_02225810_ptr;

s32 ov07_02222EE8(s32 *);

s32 ov07_02222EF8(void *);

void ov07_02225810(SysTask *arg0, void *param);

void ov07_02225810(SysTask *arg0, void *param) {
    UnkStruct_ov07_02225810_ptr *ptr = param;
    if (ov07_02222EE8((s32 *)ptr->paletteFade) == 0) {
        ov07_02222EF8(ptr->paletteFade);
        ov07_0221C448(ptr->common.battleAnimSys, arg0);
        Heap_Free(ptr);
    }
}
