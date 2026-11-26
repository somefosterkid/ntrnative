#include "global.h"
#include "font.h"
#include "heap.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "overlay_manager.h"
#include "system.h"
#include "unk_0200B150.h"
#include "bg_window.h"
#include "party.h"
#include "save_arrays.h"
#include "text.h"
#include "unk_0203A3B0.h"
#include "overlay_97_course_sub_screen.h"
#include "overlay_97_reception.h"


s32 ov97_0221F020(void *);

s32 sub_02093354(void *);

s32 ov97_0221E69C(OverlayManager *arg0);

s32 ov97_0221E69C(OverlayManager *arg0) {
    void *temp_r4;

    temp_r4 = OverlayManager_GetData(arg0);
    Main_SetVBlankIntrCB(NULL, NULL);
    ov97_0221F020(*(void **)((u8 *)temp_r4 + 0xC));
    sub_02093354(*(void **)((u8 *)temp_r4 + 8));
    FontID_Release(FONT_SUBSCREEN);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    Heap_Free(*(void **)temp_r4);
    Heap_Free(temp_r4);
    return 1;
}

s32 ov97_0221E700(OverlayManager *);

s32 ov97_0221E6DC(OverlayManager *arg0);

s32 ov97_0221E6DC(OverlayManager *arg0) {
    void **temp_r4;

    temp_r4 = OverlayManager_GetArgs(arg0);
loop_1:
    if (ov97_0221E700(arg0) != 0) {
        return 1;
    }
    if (*(s32 *)((u8 *)(*temp_r4) + 4) != 0) {
        return 0;
    }
    goto loop_1;
}

s32 ov97_0221E7F4(void);

s32 ov97_0221E814(BgConfig *);

s32 ov97_0221E834(void);

s32 ov97_0221E864(void *);

void ov97_0221E88C(void *);

Ov97CourseSubScreen *ov97_0221EE84(enum HeapID);

s32 ov97_0221EEA4(Ov97CourseSubScreen *, BgConfig *, s16, s16);

void *sub_020932E0(s32, u8, s8);

s32 sub_02093440(void *, BgConfig *, PCStorage *, Party *, s32, s32, s32, s32, s32 *, s32 *, void *, s32 *, void *);

extern s32 ov97_0221E91C;

extern s32 ov97_0221E97C;

extern s32 ov97_0221EC14;

s32 ov97_0221E700(OverlayManager *arg0) {
    PCStorage *temp_r7_2;
    s32 var_r6;
    void **temp_r7;
    Ov97Reception *temp_r0;
    void *temp_r5;

    temp_r7 = OverlayManager_GetArgs(arg0);
    temp_r5 = *(void **)temp_r7;
    temp_r0 = OverlayManager_CreateAndGetData(arg0, sizeof(Ov97Reception), HEAP_ID_POKEATHLON);
    memset(temp_r0, 0, sizeof(Ov97Reception));
    temp_r0->unk_0 = BgConfig_Alloc(HEAP_ID_POKEATHLON);
    temp_r0->unk_2C = (SaveData *) *(SaveData **)temp_r5;
    temp_r0->unk_30 = (s32) *(s32 *)((u8 *)temp_r7 + 0x84);
    ov97_0221E7F4();
    temp_r0->unk_8 = sub_020932E0(0x5C, 3, 0x13);
    temp_r0->unk_C = ov97_0221EE84(HEAP_ID_POKEATHLON);
    ov97_0221E814(temp_r0->unk_0);
    ov97_0221E834();
    ov97_0221E864(temp_r0);
    var_r6 = 0;
    if (*(s32 *)((u8 *)(*(void **)temp_r7) + 4) == 0) {
        var_r6 = 1;
    }
    temp_r7_2 = SaveArray_PCStorage_Get(*(SaveData **)temp_r5);
    sub_02093440(temp_r0->unk_8, temp_r0->unk_0, temp_r7_2, SaveArray_Party_Get(*(SaveData **)temp_r5), 0, 0, var_r6, 0x12, &ov97_0221E91C, &ov97_0221E97C, (void *)&temp_r0->unk_10, &ov97_0221EC14, temp_r0);
    sub_0203A994(2);
    ov97_0221EEA4(temp_r0->unk_C, temp_r0->unk_0, (u8) temp_r0->unk_30, *(u8 *)((u8 *)temp_r5 + 0xF));
    Main_SetVBlankIntrCB(ov97_0221E88C, temp_r0);
    temp_r0->unk_4 = 0;
    ResetAllTextPrinters();
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_POKEATHLON);
    FontID_Alloc(FONT_SUBSCREEN, HEAP_ID_POKEATHLON);
    return 1;
}
