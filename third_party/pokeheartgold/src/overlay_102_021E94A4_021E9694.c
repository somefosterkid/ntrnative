#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "screen_fade.h"
#include "sys_task.h"
#include "overlay_102_app_021e937c.h"

typedef struct UnkStruct_ov102_021E9694_param1 {
    void *unk_0;
    u8 unk_4[0x10];
    s32 unk_14;
} UnkStruct_ov102_021E9694_param1;

void ov102_021E94A4(void *arg0);

void ov102_021EA380(void *arg0);

void ov102_021EB1E0(void *arg0);

s32 ov102_021E937C(void *);

s32 ov102_021EB6C8(void *);

s32 ov102_021EBCBC(void *);

SysTask *ov102_021EC240(void *);

void ov102_021E9694(SysTask *param0, UnkStruct_ov102_021E9694_param1 *param1);

void ov102_021E9694(SysTask *param0, UnkStruct_ov102_021E9694_param1 *param1) {
    s32 v0;
    s32 v3;
    void *ptr;
    void *v1;
    Ov102App_021E937C *v2;

    v0 = param1->unk_14;
    v2 = param1->unk_0;
    switch (v0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 5, 1, HEAP_ID_35);
        param1->unk_14 = (s32)(param1->unk_14 + 1);
        return;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            ov102_021EA380(*(void **)((u8 *)(param1->unk_0) + 0x1E0));
            ov102_021EB1E0(*(void **)((u8 *)(param1->unk_0) + 0x1E4));
            ov102_021EB6C8(*(void **)((u8 *)(param1->unk_0) + 0x1E8));
            ov102_021EBCBC(*(void **)((u8 *)(param1->unk_0) + 0x1EC));
            ov102_021EC240(*(void **)((u8 *)(param1->unk_0) + 0x1F0));
            ov102_021E937C(param1->unk_0);
            v3 = 0;
            do {
                ptr = v2->unk_1D0[v3];
                if (ptr != NULL) {
                    Heap_Free(ptr);
                }
                v1 = v2->unk_1D8[v3];
                if (v1 != NULL) {
                    Heap_Free(v1);
                }
                v3 += 1;
            } while (v3 < 2);
            FontID_Release(FONT_SUBSCREEN);
            FreeBgTilemapBuffer(v2->unk_20, 0U);
            FreeBgTilemapBuffer(v2->unk_20, 1U);
            FreeBgTilemapBuffer(v2->unk_20, 2U);
            FreeBgTilemapBuffer(v2->unk_20, 3U);
            FreeBgTilemapBuffer(v2->unk_20, 4U);
            FreeBgTilemapBuffer(v2->unk_20, 5U);
            reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
            ov102_021E94A4(param1);
        }
        return;
    }
}
