#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "screen_fade.h"
#include "sound.h"
#include "heap.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "overlay_96_app_021ed1d0.h"

s32 ov96_021ED0C8(u32 *);

BOOL ov96_021EC1B0(PokeathlonCourseData *arg0, u8 *arg1);

s32 ov96_021EAC5C(Ov96SpriteSetElem *, s32);
s32 ov96_021EAD78(Ov96SpriteSetElem *);
void ov96_021EC218(SysTask *arg0, void *arg1);
void ov96_021EC298(Ov96App_021ED1D0 *arg0, s32 arg1);
s32 ov96_021EC2E0(Ov96App_021ED1D0 *arg0);

BOOL ov96_021EC1B0(PokeathlonCourseData *data, u8 *arg1) {
    Ov96App_021ED1D0 *work;
    u8 v0;

    work = PokeathlonCourse_GetHeapAllocPtr4(data);
    ov96_021ED0C8(work->unk_90);
    v0 = *arg1;
    switch (v0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7FFFU, 0x1E, 1, PokeathlonCourse_GetHeapID(data));
        GF_SndStartFadeOutBGM(0U, 0x1EU);
        *arg1 += 1;
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            return 1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

void ov96_021EC218(SysTask *task, void *ptr) {
    Ov96CryTask *cry;
    s32 v1;
    Ov96SpriteSetElem *v2;
    u16 v0;

    cry = ptr;
    v0 = cry->state;
    v2 = cry->elem;
    switch (v0) {
    case 0:
        ov96_021EAC0C(v2, 2);
        ov96_021EAC5C(v2, 0x10);
        cry->state = (u16)(cry->state + 1);
        return;
    case 1:
        v1 = cry->timer;
        cry->timer = (s32)(v1 + 1);
        if (v1 > 0x3C) {
            cry->state = (u16)(cry->state + 1);
            return;
        }
        return;
    case 2:
        if ((ov96_021EAD78(v2) == 0) && (IsCryFinished() == 0)) {
            ov96_021EAC0C(v2, 1);
            ov96_021EAC5C(v2, 0);
            *cry->running = 0;
            Heap_Free(ptr);
            SysTask_Destroy(task);
            return;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
}

void ov96_021EC298(Ov96App_021ED1D0 *arg0, s32 arg1) {
    Ov96CryTask *v0;

    v0 = Heap_AllocAtEnd(arg0->heapId, sizeof(Ov96CryTask));
    MI_CpuFill8(v0, 0U, sizeof(Ov96CryTask));
    arg0->cryTaskData = v0;
    v0->elem = ov96_021EAA04(arg0->spriteSet, arg1);
    arg0->cryRunning = 1;
    v0->running = &arg0->cryRunning;
    arg0->cryTask = SysTask_CreateOnMainQueue(ov96_021EC218, v0, 1U);
}

s32 ov96_021EC2E0(Ov96App_021ED1D0 *arg0) {
    return arg0->cryRunning;
}
