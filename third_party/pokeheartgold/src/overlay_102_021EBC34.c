#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sys_task.h"
#include "overlay_102_easy_chat_app.h"
#include "mail_misc.h"
#include "touch_hitbox_controller.h"

void *ov102_021E8F68(void *);

BgConfig *ov102_021EA268(void *);

struct EasyChatApp;
struct EasyChatWordList;

void *ov102_021EBC34(void *arg0, struct EasyChatApp *arg1, struct EasyChatWordList *arg2);

struct EasyChatApp;
struct EasyChatArgs;
struct EasyChatWordList;
struct TouchHitboxController;
struct UnkStruct_ov20_021D2128_t;
struct UnkStruct_ov20_021D4210_t;
typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;
typedef struct EasyChatWordList EasyChatWordList;
typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);
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
void ov102_021EBCBC(UnkStruct_ov20_021D4210 *param0);

void *ov102_021EBC34(void *arg0, struct EasyChatApp *arg1, struct EasyChatWordList *arg2) {
    void *v0;

    v0 = Heap_Alloc(HEAP_ID_35, 0x98U);
    *(void **)v0 = arg0;
    *(struct EasyChatApp **)((u8 *)v0 + 4) = arg1;
    *(struct EasyChatWordList **)((u8 *)v0 + 8) = arg2;
    *(BgConfig **)((u8 *)v0 + 0xC) = ov102_021EA268(arg0);
    *(String **)((u8 *)v0 + 0x3C) = String_New(0x20U, HEAP_ID_35);
    AddWindowParameterized(*(BgConfig **)((u8 *)v0 + 0xC), v0 + 0x10, 2U, 4U, 0U, 0x1AU, 0x20U, 0xCU, 1U);
    AddWindowParameterized(*(BgConfig **)((u8 *)v0 + 0xC), v0 + 0x20, 2U, 0U, 0U, 0xCU, 4U, 0xCU, 1U);
    *(s32 *)((u8 *)v0 + 0x30) = 0;
    *(s32 *)((u8 *)v0 + 0x34) = 0;
    *(s32 *)((u8 *)v0 + 0x38) = 0;
    *(void **)((u8 *)v0 + 0x94) = ov102_021E8F68(*(void **)((u8 *)v0 + 4));
    return v0;
}

void ov102_021EBCBC(UnkStruct_ov20_021D4210 *param0) {
    if (param0->unk_30) {
        Sprite_Delete(param0->unk_30);
    }

    if (param0->unk_34) {
        Sprite_Delete(param0->unk_34);
    }

    if (param0->unk_38) {
        Sprite_Delete(param0->unk_38);
    }

    if (param0->unk_3C) {
        String_Delete(param0->unk_3C);
    }

    RemoveWindow(&(param0->unk_20));
    RemoveWindow(&(param0->unk_10));
    Heap_Free(param0);
}
