#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_96_ov96_mon_icon_work.h"

Ov96MonIconWork *ov96_021FFF3C(enum HeapID heapID, BgConfig *bgConfig, PokeathlonCourseData *course) {
    PokeathlonCourseData *data;
    s32 v0;
    s32 v4;
    u8 *v1;
    Ov96MonIconWork *work;

    data = course;
    work = Heap_Alloc(heapID, sizeof(Ov96MonIconWork));
    MI_CpuFill8(work, 0U, sizeof(Ov96MonIconWork));
    work->heapID = heapID;
    work->bgConfig = bgConfig;
    work->course = data;
    ov96_02200DF8(work);
    work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, heapID);
    work->msgFormat = MessageFormat_New(heapID);
    v0 = 0;
    do {
        work->names[v0] = String_New(0xBU, heapID);
        v0 += 1;
    } while (v0 < 3);
    v1 = PokeathlonCourse_GetParticipantUnk04(data, ov96_021E5F24(data));
    v4 = 0;
    do {
        CopyU16ArrayToString(work->names[v4], (u16 *)(v1 + 0x12));
        v1 += 0x28;
        v4 += 1;
    } while (v4 < 3);
    ov96_02200BD8(work, 1);
    ov96_02200C40(work);
    work->counter[0] = 1;
    work->counter[1] = 2;
    return work;
}
