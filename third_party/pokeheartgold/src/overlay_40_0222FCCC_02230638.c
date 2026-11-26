#include "global.h"
struct UnkStruct_0208C06C;

#include "bg_window.h"
#include "font.h"
#include "overlay_40.h"
#include "save_misc_data.h"
#include "sprite_system.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_40_unk_struct_ov62_02241204.h"

void ov40_0222FF74(UnkStruct_ov62_02233F74 *param0, UnkStruct_0208C06C *param1);

void ov40_02230424(UnkStruct_ov62_02233F74 *param0, UnkStruct_0208C06C *param1);

void ov40_02230638(UnkStruct_ov62_02233F74 *arg0, UnkStruct_0208C06C *arg1);

void ov40_02230638(UnkStruct_ov62_02233F74 *arg0, UnkStruct_0208C06C *arg1) {
    ov40_0222FF74(arg0, arg1);
    ov40_02230424(arg0, arg1);
}

s32 ov40_02230398(void *, void *);

void ov40_0223064C(void *arg0, struct UnkStruct_0208C06C *arg1);

void ov40_0223064C(void *arg0, UnkStruct_0208C06C *arg1) {
    void *v0;
    s32 v1;
    void *v2;

    v0 = arg0;
    SpriteManager_UnloadPlttObjById(arg1->unk_14.unk_08, 0x186A0U);
    SpriteManager_UnloadCellObjById(arg1->unk_14.unk_08, 0x186A0U);
    SpriteManager_UnloadAnimObjById(arg1->unk_14.unk_08, 0x186A0U);
    v1 = 0;
    v2 = v0;
    do {
        if (*(ManagedSprite **)((u8 *)v2 + 0xC) != NULL) {
            SpriteManager_UnloadCharObjById(arg1->unk_14.unk_08, v1 + 0x186A0);
            Sprite_DeleteAndFreeResources(*(ManagedSprite **)((u8 *)v2 + 0xC));
            *(ManagedSprite **)((u8 *)v2 + 0xC) = NULL;
        }
        v1 += 1;
        v2 += 4;
    } while (v1 < 0xC);
    ov40_02230398(v0, arg1);
}

void ov40_022306A0(void *arg0, s32 arg1);

void ov40_022306A0(void *arg0, s32 flag) {
    ManagedSprite *managedSprite;
    s32 v0;
    void *v1;

    v1 = arg0;
    v0 = 0;
    do {
        managedSprite = *(ManagedSprite **)((u8 *)v1 + 0xC);
        if (managedSprite != NULL) {
            ManagedSprite_SetDrawFlag(managedSprite, flag);
        }
        v0 += 1;
        v1 += 4;
    } while (v0 < 0xC);
}

s32 ov40_022306C0(Window *arg0, String *arg1);

s32 ov40_022306C0(Window *window, String *string) {
    u32 v0;
    u32 v1;

    v1 = FontID_String_GetWidth(FONT_SYSTEM, string, 0U);
    v0 = (GetWindowWidth(window) * 8) - v1;
    return (s32)(v0 + (v0 >> 0x1F)) >> 1;
}

void ov40_022306E0(UnkStruct_0208C06C *arg0);

void ov40_022306E0(UnkStruct_0208C06C *arg0) {
    arg0->unk_834 = 0;
    arg0->unk_836 = 0;
}

s32 ov40_022306F0(void *arg0) {
    u32 a2;
    u32 a3;
    u8 a4;
    SAVE_MISC_DATA *saveMiscData;

    saveMiscData = Save_Misc_Get(((SaveData *)((UnkStruct_0208C06C *)arg0)->saveData));
    sub_0202AC38(saveMiscData, ((UnkStruct_0208C06C *)arg0)->unk_86C + 2, &a2, &a3, &a4);
    sub_0202AC60(saveMiscData, ((UnkStruct_0208C06C *)arg0)->unk_86C + 2, -1U, -1U, (u8)(s32)a4);
    return 1;
}

void ov40_02230738(void) {
    SetBgPriority(0U, 1U);
    SetBgPriority(1U, 3U);
    SetBgPriority(2U, 0U);
    SetBgPriority(3U, 1U);
    SetBgPriority(4U, 1U);
    SetBgPriority(5U, 3U);
    SetBgPriority(6U, 0U);
    SetBgPriority(7U, 1U);
}

s32 sub_020878B0(void *, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 sub_020878B8(void *, s16, s16);
#else
s32 sub_020878B8(void *, s32, s32);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 sub_02087948(void *, s16, s16);
#else
s32 sub_02087948(void *, s32, s32);
#endif

s32 sub_020879E0(void *, s32);

void ov40_0223077C(void *arg0, void *arg1, s32 arg2, s32 arg3);

void ov40_0223077C(void *arg0, void *arg1, s32 arg2, s32 arg3) {
    if (*(s32 *)((u8 *)arg0 + 0x44) == 0) {
        sub_020879E0(arg1, 1);
    }
    sub_020878B0(arg1, 1);
    sub_02087948(arg1, arg2, arg3);
    sub_020878B8(arg1, arg2, arg3);
}
