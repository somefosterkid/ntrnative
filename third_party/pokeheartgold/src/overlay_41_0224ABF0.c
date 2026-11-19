#include "global.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "filesystem_files_def.h"
#include "bg_window.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_41_unk_struct_ov22_0225ab54.h"

s32 ov41_0224B250(void *);

void ov41_0224ABF0(void *arg0);

struct BgConfig;
struct SpriteList;
struct SpriteResourceCollection;
struct SysTask;
struct SysTaskQueue;
struct Window;
typedef struct {
    BgConfig *unk_00;
    enum NarcId narcID;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
} UnkStruct_ov22_022597BC;
typedef struct {
    SpriteResource *unk_00[4];
    Sprite *unk_10[20];
    int unk_60;
    int unk_64;
} UnkStruct_ov22_0225A914;
typedef struct {
    UnkStruct_ov22_022597BC unk_00;
    Window *unk_2C;
    SpriteList *unk_30;
    SpriteResourceCollection **unk_34;
    UnkStruct_ov22_0225A914 unk_38;
    UnkStruct_ov22_0225AB54 unk_A0;
    Window *unk_134;
    int unk_138;
    int unk_13C;
    u32 unk_140;
    String *unk_144;
} UnkStruct_ov22_0225A428;
s32 ov41_0224AD90(Window *param0, int param1, int param2, int param3, int param4, int param5, u32 param6);
s32 ov41_0224ADD8(Window *param0, int param1, int param2, int param3, int param4, int param5, u32 param6, String **param7);
void ov41_0224AF8C(UnkStruct_ov22_0225A914 *param0, int param1);
void ov41_0224AC08(UnkStruct_ov22_0225A428 *param0, int param1, int param2, int param3);
s32 ov41_0224AC40(UnkStruct_ov22_0225A428 *param0, int param1, int param2, int param3);
void ov41_0224AC80(UnkStruct_ov22_0225A428 *param0);
void ov41_0224AC98(UnkStruct_ov22_0225A428 *param0, int param1);

void ov41_0224ABF0(void *arg0) {
    if (0x10 & *((s32 *)&((UnkStruct_ov22_0225A428 *)arg0)->unk_138)) {
        ov41_0224B250(arg0 + 0xA0);
    }
}

void ov41_0224AC08(UnkStruct_ov22_0225A428 *param0, int param1, int param2, int param3) {
    ((param0->unk_138 & 4) ? (void)0 : GF_AssertFail());
    ov41_0224AD90(param0->unk_2C, param1, param2, param3, 0, 0, 0xFF);
}

s32 ov41_0224AC40(UnkStruct_ov22_0225A428 *param0, int param1, int param2, int param3) {
    ((param0->unk_138 & 4) ? (void)0 : GF_AssertFail());
    return ov41_0224ADD8(param0->unk_2C, param1, param2, param3, 0, 0, param0->unk_140, &param0->unk_144);
}

void ov41_0224AC80(UnkStruct_ov22_0225A428 *param0) {
    String_Delete(param0->unk_144);
    param0->unk_144 = 0L;
}

void ov41_0224AC98(UnkStruct_ov22_0225A428 *param0, int param1) {
    ov41_0224AF8C(&param0->unk_38, param1);
}
