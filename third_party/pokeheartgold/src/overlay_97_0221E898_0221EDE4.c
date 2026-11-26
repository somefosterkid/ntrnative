#include "global.h"
#include "heap.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_97_course_sub_screen.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 sub_0208B85C(void *, s32, s32, s8, s32);
#else
s32 sub_0208B85C(void *, s32, s32, u32, s32);
#endif

void ov97_0221EDE4(void *arg0, void *arg1, void *arg2, void *arg3);

void ov97_0221EDE4(void *arg0, void *arg1, void *arg2, void *arg3) {
    *(s8 *)((u8 *)arg3 + 0x28) = sub_0208B85C(arg3 + 0x29, *(u8 *)arg0, *(u8 *)arg1, (u32) (*(u16 *)((u8 *)arg1 + 6) << 0x1D) >> 0x1D, (s32) *(s8 *)arg2);
    *(s8 *)((u8 *)arg3 + 0x30) = sub_0208B85C(arg3 + 0x31, *(u8 *)((u8 *)arg0 + 3), *(u8 *)((u8 *)arg1 + 3), (u32) (*(u16 *)((u8 *)arg1 + 6) << 0x14) >> 0x1D, (s32) *(s8 *)((u8 *)arg2 + 1));
    *(s8 *)((u8 *)arg3 + 0x38) = sub_0208B85C(arg3 + 0x39, *(u8 *)((u8 *)arg0 + 4), *(u8 *)((u8 *)arg1 + 4), (u32) (*(u16 *)((u8 *)arg1 + 6) << 0x11) >> 0x1D, (s32) *(s8 *)((u8 *)arg2 + 2));
    *(s8 *)((u8 *)arg3 + 0x40) = sub_0208B85C(arg3 + 0x41, *(u8 *)((u8 *)arg0 + 2), *(u8 *)((u8 *)arg1 + 2), (u32) (*(u16 *)((u8 *)arg1 + 6) << 0x17) >> 0x1D, (s32) *(s8 *)((u8 *)arg2 + 3));
    *(s8 *)((u8 *)arg3 + 0x48) = sub_0208B85C(arg3 + 0x49, *(u8 *)((u8 *)arg0 + 1), *(u8 *)((u8 *)arg1 + 1), (u32) (*(u16 *)((u8 *)arg1 + 6) << 0x1A) >> 0x1D, (s32) *(s8 *)((u8 *)arg2 + 4));
}

Ov97CourseSubScreen *ov97_0221EE84(enum HeapID heapID);

Ov97CourseSubScreen *ov97_0221EE84(enum HeapID heapID) {
    Ov97CourseSubScreen *screen;

    screen = Heap_Alloc(heapID, sizeof(Ov97CourseSubScreen));
    MI_CpuFill8(screen, 0U, sizeof(Ov97CourseSubScreen));
    screen->heapID = heapID;
    screen->sprites.heapID = heapID;
    return screen;
}

s32 ov97_0221F14C(BgConfig *, enum HeapID);

s32 ov97_0221F294(Ov97CourseSubScreen *);

s32 ov97_0221F7DC(Ov97CourseSubScreenSprites *);

s32 ov97_0221F9E0(Ov97CourseSubScreenSprites *);

void ov97_0221EEA4(Ov97CourseSubScreen *screen, BgConfig *bgConfig, s16 courseId, s16 arg3);

void ov97_0221EEA4(Ov97CourseSubScreen *screen, BgConfig *bgConfig, s16 courseId, s16 arg3) {
    screen->bgConfig = bgConfig;
    ov97_0221F14C(bgConfig, screen->heapID);
    AddWindowParameterized(screen->bgConfig, &screen->windows[0], 4U, 1U, 1U, 0x11U, 2U, 0xFU, 1U);
    AddWindowParameterized(screen->bgConfig, &screen->windows[1], 4U, 0x16U, 1U, 9U, 2U, 0xFU, 0x23U);
    AddWindowParameterized(screen->bgConfig, &screen->windows[2], 5U, 1U, 0xFU, 8U, 2U, 0xFU, 0x35U);
    AddWindowParameterized(screen->bgConfig, &screen->windows[3], 5U, 9U, 0xFU, 3U, 2U, 0xFU, 0x45U);
    AddWindowParameterized(screen->bgConfig, &screen->windows[4], 5U, 0xDU, 5U, 0xAU, 0xAU, 0xFU, 0x4BU);
    AddWindowParameterized(screen->bgConfig, &screen->windows[5], 5U, 1U, 0x13U, 0xCU, 4U, 0xFU, 0xAFU);
    screen->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x116, screen->heapID);
    screen->msgFormat = MessageFormat_New(screen->heapID);
    screen->string = String_New(0xBU, screen->heapID);
    screen->courseId = courseId;
    screen->unk_6A = arg3;
    ov97_0221F7DC(&screen->sprites);
    ov97_0221F9E0(&screen->sprites);
    ov97_0221F294(screen);
    GfGfx_EngineBTogglePlanes(2U, 0U);
    GfGfx_EngineBTogglePlanes(4U, 0U);
}
