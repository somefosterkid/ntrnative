#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "font_types_def.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "text.h"
#include "heap.h"
#include "overlay_102_easy_chat_app.h"
#include "mail_misc.h"
#include "touch_hitbox_controller.h"

typedef struct UnkStruct_ov102_021EC20C_Heap_Alloc {
    void *unk_0;
    struct EasyChatApp *unk_4;
    struct EasyChatWordList *unk_8;
    void *unk_C;
    void *unk_10;
    s32 unk_14;
    s32 unk_18;
    u8 unk_1C[0x10];
    s32 unk_2C;
} UnkStruct_ov102_021EC20C_Heap_Alloc;

struct EasyChatApp;

struct EasyChatArgs;

struct EasyChatWordList;

struct TouchHitboxController;

struct UnkStruct_ov20_021D2128_t;

struct UnkStruct_ov20_021D4210_t;

typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;

typedef struct EasyChatWordList EasyChatWordList;

typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);

void ov102_021E8FFC(const EasyChatApp *easyChatApp, u32 param1, String *param2);

typedef struct UnkStruct_ov20_021D4210_t UnkStruct_ov20_021D4210;

typedef struct {
    BgConfig *unk_00;
    u32 unk_04;
    int unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    fx32 unk_14;
    u16 unk_18;
    u16 unk_1A;
    SysTask *unk_1C;
} UnkStruct_ov20_021D4E8C;

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    SysTask *unk_1C;
} UnkStruct_ov20_021D4FF0;

typedef u32 TextColor;

struct UnkStruct_ov20_021D4210_t {
    UnkStruct_ov20_021D2128 *unk_00;
    const EasyChatApp *unk_04;
    const EasyChatWordList *unk_08;
    BgConfig *unk_0C;
    Window unk_10;
    Window unk_20;
    Sprite *unk_30;
    Sprite *unk_34;
    Sprite *unk_38;
    String *unk_3C;
    UnkStruct_ov20_021D4FF0 unk_40;
    UnkStruct_ov20_021D4E8C unk_60;
    u32 unk_80;
    GXWndPlane unk_84;
    int unk_88;
    int unk_8C;
    int unk_90;
};

void ov102_021EC13C(UnkStruct_ov20_021D4210 *param0, u32 param1, u32 param2);

void *ov102_021EA268(void *);
void *ov102_021EA26C(void *);
void *ov102_021EC20C(void *arg0, struct EasyChatApp *arg1, struct EasyChatWordList *arg2);

void ov102_021EC13C(UnkStruct_ov20_021D4210 *param0, u32 param1, u32 param2) {
    ov102_021E8FFC(param0->unk_04, param1, param0->unk_3C);

    if (param2 <= 240) {
        AddTextPrinterParameterizedWithColor(&param0->unk_10, FONT_SYSTEM, param0->unk_3C, 0 + (param1 & 1) * 112, param2, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    } else {
        u32 v0 = 256 - param2;

        FillWindowPixelBuffer(&param0->unk_20, 0);
        AddTextPrinterParameterizedWithColor(&param0->unk_20, FONT_SYSTEM, param0->unk_3C, 0, 0, 0xFF, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
        BlitBitmapRectToWindow(&param0->unk_10, param0->unk_20.pixelBuffer, 0, 0, 12 * 8, 2 * 8, 0 + (param1 & 1) * 112, param2, 12 * 8, v0);
        BlitBitmapRectToWindow(&param0->unk_10, param0->unk_20.pixelBuffer, 0, v0, 12 * 8, 2 * 8, 0 + (param1 & 1) * 112, 0, 12 * 8, (2 * 8) - v0);
    }
}

void *ov102_021EC20C(void *arg0, struct EasyChatApp *arg1, struct EasyChatWordList *arg2) {
    UnkStruct_ov102_021EC20C_Heap_Alloc *v0;

    v0 = Heap_Alloc(HEAP_ID_35, sizeof(UnkStruct_ov102_021EC20C_Heap_Alloc));
    v0->unk_0 = arg0;
    v0->unk_4 = arg1;
    v0->unk_8 = arg2;
    v0->unk_C = ov102_021EA268(arg0);
    v0->unk_10 = ov102_021EA26C(arg0);
    v0->unk_14 = 0;
    v0->unk_18 = 0;
    v0->unk_2C = 0;
    return v0;
}
