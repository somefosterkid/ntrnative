#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_96_ov96hurdle_hud.h"
#include "pm_string.h"

void ov96_022088AC(Ov96HurdleHud *hud, s32 idx);

void ov96_02208A4C(Ov96HurdleHud *hud);

Ov96HurdleHud *ov96_02207CCC(enum HeapID heapID, BgConfig *bgConfig, PokeathlonCourseData *course);

Ov96HurdleHud *ov96_02207CCC(enum HeapID heapID, BgConfig *bgConfig, PokeathlonCourseData *course) {
    PokeathlonCourseData *data;
    u8 *v1;
    Ov96HurdleHud *hud;
    s32 i;

    data = course;
    hud = Heap_Alloc(heapID, sizeof(Ov96HurdleHud));
    MI_CpuFill8(hud, 0U, sizeof(Ov96HurdleHud));
    hud->heapId = heapID;
    hud->bgConfig = bgConfig;
    hud->course = data;
    ov96_02208A4C(hud);
    hud->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, heapID);
    hud->msgFormat = MessageFormat_New(heapID);
    for (i = 0; i < 3; i++) {
        hud->names[i] = String_New(0xBU, heapID);
    }
    v1 = PokeathlonCourse_GetParticipantUnk04(data, ov96_021E5F24(data));
    for (i = 0; i < 3; i++) {
        CopyU16ArrayToString(hud->names[i], (u16 *)(v1 + 0x12));
        v1 += 0x28;
    }
    ov96_022088AC(hud, 1);
    return hud;
}
