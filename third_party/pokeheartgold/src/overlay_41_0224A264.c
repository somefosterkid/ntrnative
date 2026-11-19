#include "global.h"

#include "constants/heap.h"

#include "pm_string.h"
#include "sprite.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"
#include "unk_0200A090.h"
#include "font.h"
#include "unk_02013534.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

typedef struct {
    Sprite *unk_00;
    SpriteResource *unk_04[4];
    void *unk_14;
    int unk_18;
    int unk_1C;
    NNSG2dPaletteData *unk_20;
    int unk_24;
    int unk_28;
} UnkStruct_ov21_021DDEC8;

void ov41_0224A264(UnkStruct_ov21_021DDEC8 *param0);
void ov41_0224A270(UnkStruct_ov21_021DDEC8 *param0);

void *ov41_0224A258(void *);
void ov41_0224A60C(u32 index, u32 event, void *arg);
s32 ov41_0224A6C4(void *, s32, void *, s32, s32, s32, s32);
s32 ov41_0224A734(void *, s32, void *, UnkStruct_02013534 *, void *, s32, s32, s32, s32);
s32 ov41_0224A7E0(void *, s32, s32, s32, s32, s32);
s32 ov41_0224A7F8(void *);
s32 ov41_0224A888(void *);
void *ov41_0224A928(void *, s32, s32, s32, s32, s32);
s32 ov41_0224A9B0(void *);
void ov41_0224A27C(void *arg0, void *arg1, u8 *arg2);

void ov41_0224A264(UnkStruct_ov21_021DDEC8 *param0) {
    Sprite_SetAnimationFrame(param0->unk_00, GX_OAM_MODE_NORMAL);
}

void ov41_0224A270(UnkStruct_ov21_021DDEC8 *param0) {
    Sprite_SetAnimationFrame(param0->unk_00, GX_OAM_MODE_XLU);
}

void ov41_0224A27C(void *callbackArg, void *arg1, u8 *arg2) {
    void *v0;

    *((void **)&((UnkStruct_0208C06C *)callbackArg)->unk_14.unk_54.unk_0C.unk_06[6]) = arg2;
    *((UnkStruct_02013534 **)&((UnkStruct_0208C06C *)callbackArg)->unk_14.unk_50) = FontSystem_NewInit(1, HEAP_ID_13);
    ov41_0224A7F8(arg1);
    ov41_0224A6C4(callbackArg, 0, arg1, 0x30, 0x90, 0x28, 0x20);
    ov41_0224A7E0(callbackArg + 0x6C, 0, 0x30, 0x98, 0x28, 0x18);
    ov41_0224A6C4(callbackArg + 0x10, 1, arg1, 8, 0x90, 0x28, 0x20);
    ov41_0224A7E0(callbackArg + 0x6C, 1, 8, 0x98, 0x28, 0x18);
    ov41_0224A6C4(callbackArg + 0x20, 2, arg1, 0x60, 0x90, 0x28, 0x2A);
    ov41_0224A7E0(callbackArg + 0x6C, 2, 0x60, 0x9C, 0x28, 0x22);
    ov41_0224A6C4(callbackArg + 0x30, 3, arg1, 0x88, 0x90, 0x28, 0x2A);
    ov41_0224A7E0(callbackArg + 0x6C, 3, 0x88, 0x9C, 0x28, 0x22);
    FontID_Alloc(FONT_SUBSCREEN, HEAP_ID_14);
    v0 = ov41_0224A928(arg1, 0x1B, 0xD7, 0, 9, 5);
    ov41_0224A734(callbackArg + 0x40, 4, arg1, ((UnkStruct_02013534 *)((UnkStruct_0208C06C *)callbackArg)->unk_14.unk_50), v0, 0xB8, 0x90, 0x48, 0x2A);
    ov41_0224A7E0(callbackArg + 0x6C, 4, 0xB8, 0x9C, 0x48, 0x22);
    ov41_0224A9B0(v0);
    FontID_Release(FONT_SUBSCREEN);
    *((s32 *)&((UnkStruct_0208C06C *)callbackArg)->unk_14.unk_4C) = 1;
    ov41_0224A888(arg1);
    ov41_0224A258(callbackArg + 0x20);
    *((TouchHitboxController **)&((UnkStruct_0208C06C *)callbackArg)->unk_14.unk_54.saveData) = TouchHitboxController_Create(callbackArg + 0x6C, 5U, ov41_0224A60C, callbackArg, HEAP_ID_13);
}
