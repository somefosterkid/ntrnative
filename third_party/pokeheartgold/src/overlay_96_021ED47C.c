#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "error_handling.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "pm_string.h"
#include "poke_overlay.h"
#include "sprite_system.h"
#include "system.h"
#include "unk_0203A3B0.h"
#include "overlay_96_app_021ed1d0.h"

s32 ov96_021ED47C(PokeathlonCourseData *arg0);

s32 ov96_021ED660(PokeathlonCourseData *arg0, s32 arg1, s32 arg2);

s32 ov96_021ED47C(PokeathlonCourseData *data) {
    Ov96App_021ED1D0 *work = PokeathlonCourse_GetHeapAllocPtr4(data);

    SpriteSystem_DrawSprites(work->spriteManager);
    return 1;
}

s32 ov96_021E9C0C(UnkStruct_ov96_021E9A78 *);

s32 ov96_021EC51C(Ov96App_021ED1D0 *);

SysTask *ov96_021ECBF4(Ov96PlayerSlot *);

s32 ov96_021ED09C(void *);

s32 ov96_021EE808(void *);

FS_EXTERN_OVERLAY(OVY_98);

s32 ov96_021ED48C(PokeathlonCourseData *arg0);

s32 ov96_021ED48C(PokeathlonCourseData *data) {
    Ov96App_021ED1D0 *v0;

    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    ov96_021ED09C(v0->unk_90);
    ov96_021ECBF4(v0->playerSlots);
    ov96_021EE808(v0->textBox);
    FreeBgTilemapBuffer(v0->bgConfig, 0U);
    FreeBgTilemapBuffer(v0->bgConfig, 1U);
    FreeBgTilemapBuffer(v0->bgConfig, 4U);
    FreeBgTilemapBuffer(v0->bgConfig, 5U);
    Heap_Free(v0->bgConfig);
    ov96_021EA894(v0->spriteSet);
    ov96_021E9C0C(v0->resSrc);
    sub_0203A914();
    ov96_021EC51C(v0);
    PokeathlonCourse_FreePtr4HeapAlloc(data);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    Heap_Destroy(HEAP_ID_135);
    return 1;
}

MessageFormat *ov96_021EE97C(void *);

void ov96_021ED524(PokeathlonCourseData *arg0, s32 arg1, s32 arg2, u32 arg3);

void ov96_021ED524(PokeathlonCourseData *data, s32 index, s32 arg2, u32 arg3) {
    u32 fieldno;
    String *string;
    u8 *v1;
    Ov96App_021ED1D0 *v0;

    fieldno = arg3;
    v0 = PokeathlonCourse_GetHeapAllocPtr4(data);
    v1 = PokeathlonCourse_GetParticipantUnk04(data, index);
    string = String_New(0xBU, v0->heapId);
    CopyU16ArrayToString(string, (u16 *)(&v1[0x28 * arg2] + 0x12));
    BufferString(ov96_021EE97C(v0->textBox), fieldno, string, 2, 1, 0);
    String_Delete(string);
}

s32 ov96_021EDF3C(MessageFormat *, s32, s32, s32);

void ov96_021ED578(PokeathlonCourseData *arg0, s32 arg1, s32 arg2);

void ov96_021ED578(PokeathlonCourseData *data, s32 index, s32 arg2) {
    MessageFormat *messageFormat;
    Ov96App_021ED1D0 *work = PokeathlonCourse_GetHeapAllocPtr4(data);

    messageFormat = ov96_021EE97C(work->textBox);
    BufferPlayersName(messageFormat, 0U, PokeathlonCourse_GetPlayerProfileFromData(data, index));
    ov96_021EDF3C(messageFormat, arg2, 3, 1);
}

void ov96_021ED5AC(PokeathlonCourseData *arg0, s32 arg1, s32 arg2);

void ov96_021ED5AC(PokeathlonCourseData *data, s32 index, s32 arg2) {
    MessageFormat *messageFormat;
    Ov96App_021ED1D0 *work = PokeathlonCourse_GetHeapAllocPtr4(data);

    messageFormat = ov96_021EE97C(work->textBox);
    BufferPlayersName(messageFormat, 0U, PokeathlonCourse_GetPlayerProfileFromData(data, index));
    ov96_021ED524(data, index, arg2, 1);
}

void ov96_021ED5E0(PokeathlonCourseData *arg0);

void ov96_021ED5E0(PokeathlonCourseData *data) {
    u8 *v0;
    PokeathlonTally *v1;

    v0 = PokeathlonCourse_GetDataCopyArea(data);
    v1 = ov96_021E8A20(v0 + 0xF0);
    if (data == NULL) {
        GF_AssertFail();
    }
    if (v0 == NULL) {
        GF_AssertFail();
    }
    if (v1 == NULL) {
        GF_AssertFail();
    }
    MATH_CountPopulation((u32)(v1->pairs[0].word1 << 5) >> 0x14);
}

s32 ov96_021ED618(PokeathlonCourseData *arg0, s32 arg1, s32 arg2);

s32 ov96_021ED618(PokeathlonCourseData *data, s32 arg1, s32 arg2) {
    s32 v1;
    u8 *v0;
    PokeathlonTally *v2;

    v1 = arg2 + (arg1 * 3);
    v0 = PokeathlonCourse_GetDataCopyArea(data);
    v2 = ov96_021E8A20(v0 + 0xF0);
    if (data == NULL) {
        GF_AssertFail();
    }
    if (v0 == NULL) {
        GF_AssertFail();
    }
    if (v2 == NULL) {
        GF_AssertFail();
    }
    if (v1 >= 0xC) {
        GF_AssertFail();
    }
    return ((u32)(v2->pairs[0].word1 << 5) >> 0x14) & (1 << v1);
}

s32 ov96_021ED660(PokeathlonCourseData *data, s32 arg1, s32 arg2) {
    s32 v1;
    u8 *v0;
    unsigned int new_var;
    PokeathlonTally *v2;
    v1 = arg2 + (arg1 * 3);
    v0 = PokeathlonCourse_GetDataCopyArea(data);
    v2 = ov96_021E8A20(v0 + 0xF0);
    if (data == 0L) {
        GF_AssertFail();
    }
    if (v0 == 0L) {
        GF_AssertFail();
    }
    if (v2 == 0L) {
        GF_AssertFail();
    }
    if (v1 >= 0xC) {
        GF_AssertFail();
    }
    new_var = (u32)((v2->pairs[1].word0) * 0x10);
    return (new_var >> 0x14) & (1 << v1);
}
