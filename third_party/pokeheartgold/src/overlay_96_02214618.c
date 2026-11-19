#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"

#include "overlay_96_app_02210C98.h"
#include "overlay_96_ov96_course_hud.h"

s32 ov96_021E8318(PokeathlonCourseData *, s32);

s32 ov96_022136A4(Ov96App_02210C98 *, s32);

void ov96_02214618(PokeathlonCourseData *arg0);

void ov96_02214618(PokeathlonCourseData *data) {
    s32 v1;
    Ov96App_02210C98 *v0;

    ov96_021E8A20(PokeathlonCourse_GetDataCopyArea(data) + 0xF0);
    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    v1 = ov96_02214904(v0->unk_750);
    ov96_021E8318(data, v1);
    ov96_022136A4(v0, v1);
}

Ov96CourseHud *ov96_0221464C(enum HeapID heapID, BgConfig *bgConfig, PokeathlonCourseData *course) {
    Ov96CourseHud *hud;
    MsgData *msgData;
    MessageFormat *msgFormat;

    hud = Heap_Alloc(heapID, sizeof(Ov96CourseHud));
    MI_CpuFill8(hud, 0, sizeof(Ov96CourseHud));
    hud->heapID = heapID;
    hud->bgConfig = bgConfig;
    hud->course = course;
    ov96_022148A4(hud);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x135, heapID);
    hud->msgData = msgData;
    msgFormat = MessageFormat_New(heapID);
    hud->msgFormat = msgFormat;
    return hud;
}

void ov96_02214690(Ov96CourseHud *hud) {
    Heap_Free(hud->scrnBuf);
    MessageFormat_Delete(hud->msgFormat);
    DestroyMsgData(hud->msgData);
    RemoveWindow(&hud->nameWindow);
    RemoveWindow(&hud->msgWindow);
    Heap_Free(hud);
    return;
}

void ov96_022146C0(Ov96CourseHud *hud) {
    GfGfxLoader_LoadScrnData(NARC_a_2_3_4, 8, hud->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, hud->heapID);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_4, 9, hud->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 0, hud->heapID);
    hud->scrnBuf = GfGfxLoader_GetScrnData(NARC_a_2_3_4, 0xA, 0, &hud->scrnData, hud->heapID);
    ov96_0221497C(hud, 0);
}
