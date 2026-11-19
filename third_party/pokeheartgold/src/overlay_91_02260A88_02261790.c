#include "global.h"
struct UnkStruct_ov115_02261ADC;

#include "bg_window.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "text.h"
#include "unk_0200A090.h"
#include "unk_02013534.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"

void ov91_02261790(void *arg0, struct UnkStruct_ov115_02261ADC *arg1);

void ov91_02261790(void *arg0, UnkStruct_ov115_02261ADC *arg1) {
    TextOBJ_Destroy(*(TextOBJ **)((u8 *)arg0 + 0x48));
    SpriteTransfer_DeletePlttTransferTask(*(SpriteResource **)((u8 *)arg0 + 0x60));
    DestroySingle2DGfxResObj(*((GF_2DGfxResMan * *)&arg1->unk_1AC[1]), *(SpriteResource **)((u8 *)arg0 + 0x60));
    sub_02021B5C(arg0 + 0x50);
    sub_02013938(*(UnkStruct_02013910 **)((u8 *)arg0 + 0x4C));
    String_Delete(*(String **)((u8 *)arg0 + 0x5C));
    Sprite_Delete(*(Sprite **)((u8 *)arg0 + 0x34));
    SpriteTransfer_DeleteCharTransferTask(*(SpriteResource **)arg0);
    SpriteTransfer_DeletePlttTransferTask(*(SpriteResource **)((u8 *)arg0 + 4));
    DestroySingle2DGfxResObj(*((GF_2DGfxResMan * *)&arg1->unk_1AC[0]), *(SpriteResource **)arg0);
    DestroySingle2DGfxResObj(*((GF_2DGfxResMan * *)&arg1->unk_1AC[1]), *(SpriteResource **)((u8 *)arg0 + 4));
    DestroySingle2DGfxResObj(*((GF_2DGfxResMan * *)&arg1->unk_1AC[2]), *(SpriteResource **)((u8 *)arg0 + 8));
    DestroySingle2DGfxResObj(*((GF_2DGfxResMan * *)&arg1->unk_1AC[3]), *(SpriteResource **)((u8 *)arg0 + 0xC));
}

void ov91_02261808(void *arg0, BgConfig **arg1, s32 arg2);

void ov91_02261808(void *arg0, BgConfig **arg1, s32 num) {
    String16_FormatInteger(*(String **)((u8 *)arg0 + 0x5C), num, 2U, PRINTING_MODE_LEADING_ZEROS, 1);
    AddTextWindowTopLeftCorner(*arg1, arg0 + 0x38, 3U, 2U, 0U, 0U);
    AddTextPrinterParameterizedWithColor(arg0 + 0x38, FONT_SYSTEM, *(String **)((u8 *)arg0 + 0x5C), 0U, 0U, 0xFFU, 0xF0E00U, NULL);
    TextOBJ_CopyFromBGWindow(*(TextOBJ **)((u8 *)arg0 + 0x48), *(UnkStruct_02013910 **)((u8 *)arg0 + 0x4C), arg0 + 0x38, *(enum HeapID *)((u8 *)arg0 + 0x9C));
    RemoveWindow(arg0 + 0x38);
    TextOBJ_SetSpritesDrawFlag(*(TextOBJ **)((u8 *)arg0 + 0x48), 1);
    Sprite_SetDrawFlag(*(Sprite **)((u8 *)arg0 + 0x34), 1);
    *(s16 *)((u8 *)arg0 + 0x98) = 1;
    *(s16 *)((u8 *)arg0 + 0x9A) = 0;
}

void ov91_02261890(void *arg0);

void ov91_02261890(void *arg0) {
    u16 v0;

    v0 = *(u16 *)((u8 *)arg0 + 0x98);
    if (v0 != 3) {
        if (v0 == 2) {
            *(s16 *)((u8 *)arg0 + 0x9A) = 0xC;
        }
        *(u16 *)((u8 *)arg0 + 0x98) = 3U;
    }
}

void ov91_022618B0(void *arg0, BgConfig **arg1, s32 arg2);

void ov91_022618B0(void *arg0, BgConfig **arg1, s32 num) {
    if (*(u16 *)((u8 *)arg0 + 0x98) != 3) {
        String16_FormatInteger(*(String **)((u8 *)arg0 + 0x5C), num, 2U, PRINTING_MODE_LEADING_ZEROS, 1);
        AddTextWindowTopLeftCorner(*arg1, arg0 + 0x38, 3U, 2U, 0U, 0U);
        AddTextPrinterParameterizedWithColor(arg0 + 0x38, FONT_SYSTEM, *(String **)((u8 *)arg0 + 0x5C), 0U, 0U, 0xFFU, 0xF0E00U, NULL);
        TextOBJ_CopyFromBGWindow(*(TextOBJ **)((u8 *)arg0 + 0x48), *(UnkStruct_02013910 **)((u8 *)arg0 + 0x4C), arg0 + 0x38, *(enum HeapID *)((u8 *)arg0 + 0x9C));
        RemoveWindow(arg0 + 0x38);
        *(s16 *)((u8 *)arg0 + 0x96) = 0;
    }
}
