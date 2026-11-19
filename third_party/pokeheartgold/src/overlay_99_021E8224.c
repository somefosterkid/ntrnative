#include "global.h"
struct PokeathlonMedals;

#include "error_handling.h"
#include "message_format.h"
#include "heap.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "screen_fade.h"
#include "system.h"
#include "bg_window.h"
#include "overlay_99_pokeathlon_event_record.h"
#include "overlay_99_pokeathlon_medals.h"
#include "overlay_98_ov98text_sys.h"

s32 ov99_021E7150(void *);

s32 ov99_021E718C(void *);

s32 ov99_021E724C(void *);

void ov99_021E8224(struct PokeathlonMedals *arg0);

s32 ov98_0221F090(void);
void ov99_021E856C(void *);
s32 ov99_021E8590(void *);
s32 ov99_021E860C(void *);
s32 ov99_021E8788(void *);
s32 ov99_021E8818(void *);
s32 ov99_021E88D4(void *);
s32 ov99_021E8C04(void *);
s32 ov99_021E93DC(void *, void *);
FS_EXTERN_OVERLAY(OVY_98);
extern WindowTemplate ov99_021EA45C[];
s32 PokeathlonEventRecord_Init(OverlayManager *man, s32 *state);
s32 ov98_0221E684(void *, void *, s32);
s32 ov98_0221F0EC(void);
s32 ov99_021E875C(void *);
s32 ov99_021E9418(void *);
s32 PokeathlonEventRecord_Exit(OverlayManager *man, s32 *state);

void ov99_021E8224(PokeathlonMedals *arg0) {
    MessageFormat *messageFormat;
    int new_var2;
    u32 v0;
    char new_var;
    messageFormat = ov98_0221EEFC(arg0->textContext);
    ov98_0221EC24(arg0->textContext, 0xB, 0x7E, 3);
    ov98_0221EC08(arg0->textContext, 0, 0x7F, 1, 0);
    ov98_0221EBEC(arg0->textContext, 4, 0, 1, 0, 4);
    v0 = 0;
    new_var = 1;
    do {
        BufferPokeathlonMedalName(messageFormat, 0U, ((u32)v0) & 0xFF);
        ov98_0221EBD8(arg0->textContext, v0 - -6, 4, 0);
        v0 += new_var;
    } while (((s32)v0) < 5);
    new_var2 = 0x80;
    BufferIntegerAsString(messageFormat, 0U, ov99_021E718C(arg0), 3U, PRINTING_MODE_RIGHT_ALIGN, new_var);
    ov98_0221EC08(arg0->textContext, new_var, new_var2, new_var, 0);
    if (ov99_021E7150(arg0) != 0) {
        ov98_0221EC08(arg0->textContext, 2, ov99_021E724C(arg0) + 0x71, new_var, 0);
    }
}

u16 ov99_021E82E4(s32 *param_1, u32 param_2);

u16 ov99_021E82E4(s32 *param_1, u32 param_2) {
    if (param_1 == 0L) {
        GF_AssertFail();
    }
    if ((9 + 1) <= param_2) {
        GF_AssertFail();
    }
    return *((u16 *)((*((u8 **)((*param_1) + 8))) + (param_2 * 2)));
}

s32 PokeathlonEventRecord_Init(OverlayManager *man, s32 *state) {
    void *v0;
    void *v1;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_98), OVY_LOAD_ASYNC);
    v0 = OverlayManager_GetArgs(man);
    if (v0 == NULL) {
        GF_AssertFail();
    }
    Heap_Create(HEAP_ID_3, HEAP_ID_132, 0x30000U);
    v1 = OverlayManager_CreateAndGetData(man, sizeof(PokeathlonEventRecord), HEAP_ID_132);
    MI_CpuFill8(v1, 0U, sizeof(PokeathlonEventRecord));
    ((PokeathlonEventRecord *)v1)->heapId = HEAP_ID_132;
    ov99_021E93DC(v1, v0);
    ov98_0221F090();
    ov99_021E8590(v1);
    ov99_021E860C(v1);
    ((PokeathlonEventRecord *)v1)->textContext = ov98_0221EABC(((PokeathlonEventRecord *)v1)->heapId, *((void **)&((PokeathlonEventRecord *)v1)->bgConfig), 0x13, ov99_021EA45C, 0x13A);
    ov99_021E88D4(v1);
    ov99_021E8788(v1);
    ov99_021E8C04(v1);
    ov99_021E8818(v1);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    Main_SetVBlankIntrCB(ov99_021E856C, v1);
    return 1;
}

s32 PokeathlonEventRecord_Exit(OverlayManager *man, s32 *state) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    ov99_021E9418(*(void **)v0);
    ov98_0221E684(*((void **)&((PokeathlonEventRecord *)v0)->spriteContext), ((PokeathlonEventRecord *)v0)->sprites, 0x25);
    ov98_0221EB84(((PokeathlonEventRecord *)v0)->textContext, 0x13);
    ov99_021E875C(v0);
    ov98_0221F0EC();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_132);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    return 1;
}
