#include "global.h"

#include "pm_string.h"
#include "unk_0200A090.h"
#include "filesystem_files_def.h"
#include "bg_window.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_41_unk_struct_ov22_0225ab54.h"
#include "overlay_41_unk_struct_ov22_0225af68.h"
#include "overlay_41_unk_struct_ov22_0225A428.h"

struct BgConfig;
struct SpriteList;
struct SpriteResourceCollection;
struct SysTask;
struct SysTaskQueue;
struct Window;
void ov41_0224AD7C(UnkStruct_ov22_022597BC *param0);
void ov41_0224AD84(Window *param0);
void ov41_0224AFD4(UnkStruct_ov22_0225A914 *param0, SpriteResourceCollection **param1);
void ov41_0224B21C(UnkStruct_ov22_0225AB54 *param0, SpriteResourceCollection **param1);

void ov41_0224AB40(UnkStruct_ov22_0225A428 *param0);

void ov41_0224AB40(UnkStruct_ov22_0225A428 *param0) {
    if (param0->unk_138 & 2) {
        ov41_0224AD7C(&param0->unk_00);
        param0->unk_138 &= ~2;
    }

    if (param0->unk_138 & 4) {
        ov41_0224AD84(param0->unk_2C);
        param0->unk_138 &= ~4;
    }

    if (param0->unk_138 & 8) {
        ov41_0224AFD4(&param0->unk_38, param0->unk_34);
        param0->unk_138 &= ~8;
    }

    if (param0->unk_138 & 16) {
        ov41_0224B21C(&param0->unk_A0, param0->unk_34);
        param0->unk_138 &= ~16;
    }

    if (param0->unk_138 & 32) {
        ov41_0224AD84(param0->unk_134);
        param0->unk_138 &= ~32;
    }

    memset(param0, 0, sizeof(UnkStruct_ov22_0225A428));
}
