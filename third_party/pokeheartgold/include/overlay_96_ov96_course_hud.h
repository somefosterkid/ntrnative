#ifndef POKEHEARTGOLD_OVERLAY_96_OV96_COURSE_HUD_H
#define POKEHEARTGOLD_OVERLAY_96_OV96_COURSE_HUD_H

#include "constants/heap.h"

#include "bg_window.h"

struct MessageFormat;
struct MsgData;
struct NNSG2dScreenData;
struct PokeathlonCourseData;
struct Sprite;

// Sub screen HUD of the Pokeathlon touch-screen course (ov96_0221464C allocates it, 0x50 bytes; the
// Ov96App_02210C98 work keeps it at +0x750): player name window, seconds counter digits and the time-out message.
typedef struct Ov96CourseHud {
    enum HeapID heapID;
    struct PokeathlonCourseData *course;
    struct Sprite *digits[2]; // 0x08 tens, ones
    struct BgConfig *bgConfig;
    Window nameWindow;
    Window msgWindow;
    struct MsgData *msgData;
    struct MessageFormat *msgFormat;
    struct NNSG2dScreenData *scrnData;
    void *scrnBuf;
    int unk_44;
    BOOL timeUpShown;
    void *unk_4C;
} Ov96CourseHud; // 0x50

struct Ov96CourseHud *ov96_0221464C(enum HeapID heapID, struct BgConfig *bgConfig, struct PokeathlonCourseData *course);
void ov96_02214690(Ov96CourseHud *hud);
void ov96_022146C0(Ov96CourseHud *hud);
void ov96_02214718(void *arg0, Ov96CourseHud *hud, s32 arg2, s32 arg3);
void ov96_022147FC(Ov96CourseHud *hud);
void ov96_022148A4(Ov96CourseHud *hud);
void ov96_022148E8(Ov96CourseHud *hud, s32 arg1, s32 arg2);
u32 ov96_02214904(Ov96CourseHud *hud);
void ov96_0221490C(Ov96CourseHud *hud, s32 arg1);
void ov96_0221497C(Ov96CourseHud *hud, s32 arg1);

#endif // POKEHEARTGOLD_OVERLAY_96_OV96_COURSE_HUD_H
