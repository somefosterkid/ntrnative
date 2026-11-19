#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "heap.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_ov96_digit_work.h"

void ov96_022195E8(Ov96DigitWork *work);

void ov96_022195E8(Ov96DigitWork *work) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    s32 var_r4;
    s32 var_r5;

    if (work == NULL) {
        GF_AssertFail();
    }
    NARC_Delete(work->narc);
    var_r4 = 0;
    do {
        String_Delete(work->names[var_r4]);
        var_r4 += 1;
    } while (var_r4 < 3);
    Heap_Free(work->scrnBuf);
    var_r5 = 0;
    do {
        RemoveWindow(&work->windows[var_r5]);
        var_r5 += 1;
    } while (var_r5 < 3);
    var_r5 = 0;
    do {
        if (work->sprites[var_r5] == NULL) {
            GF_AssertFail();
        }
        Sprite_DeleteAndFreeResources(work->sprites[var_r5]);
        var_r5 += 1;
    } while (var_r5 < 0x12);
    Heap_Free(work);
#else
    s32 var_r4;
    void *var_r4_p;
    void *var_r5;
    void *var_r6;

    var_r6 = work;
    if (var_r6 == NULL) {
        GF_AssertFail();
    }
    NARC_Delete(*(NARC **)((u8 *)var_r6 + 0x24));
    var_r4 = 0;
    var_r5 = var_r6;
    do {
        String_Delete(*(String **)((u8 *)var_r5 + 0xB0));
        var_r4 += 1;
        var_r5 += 4;
    } while (var_r4 < 3);
    Heap_Free(*(void **)((u8 *)var_r6 + 0x28));
    var_r4 = (s32)var_r6;
    var_r5 = NULL;
    var_r4 += 0x30;
    do {
        RemoveWindow((Window *)var_r4);
        var_r5 += 1;
        var_r4 += (s32)sizeof(Window);
    } while ((s32)var_r5 < 3);
    var_r5 = NULL;
    var_r4_p = var_r6;
    do {
        if (*(ManagedSprite **)((u8 *)var_r4_p + 0x60) == NULL) {
            GF_AssertFail();
        }
        Sprite_DeleteAndFreeResources(*(ManagedSprite **)((u8 *)var_r4_p + 0x60));
        var_r5 += 1;
        var_r4_p += 4;
    } while ((s32)var_r5 < 0x12);
    Heap_Free(var_r6);
#endif
}
