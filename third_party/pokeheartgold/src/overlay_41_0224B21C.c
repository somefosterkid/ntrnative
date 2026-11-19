#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_0200A090.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_41_unk_struct_ov22_0225ab54.h"

struct SpriteResourceCollection;

typedef struct {
    u32 unk_00;
    u8 unk_04;
    u8 padding_05[3];
} UnkStruct_02095B28;

typedef struct {
    UnkStruct_02095B28 unk_00;
    UnkStruct_02095B28 unk_08;
    u8 unk_10[4];
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
} UnkStruct_02095C60;

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    int unk_0C;
    int unk_10;
} UnkStruct_ov22_0225AD68;

typedef struct {
    BOOL unk_00;
    Sprite *unk_04[2];
    VecFx32 unk_0C[2];
    UnkStruct_ov22_0225AD68 unk_24;
    UnkStruct_ov22_0225AD68 unk_38;
    UnkStruct_ov22_0225AD68 unk_4C;
} UnkStruct_ov22_0225AE9C;

void ov41_0224AD84(Window *param0);

void ov41_0224B084(SpriteResource **param0, SpriteResourceCollection **param1);

void ov41_0224B21C(UnkStruct_ov22_0225AB54 *param0, SpriteResourceCollection **param1);

s32 ov41_0224B270(u8 *);
s32 ov41_0224B298(void *);
s32 ov41_0224B310();
s32 ov41_0224B450(u8 *);
void ov41_0224B250(u8 *arg0);

void ov41_0224B21C(UnkStruct_ov22_0225AB54 *param0, SpriteResourceCollection **param1) {
    if (param0->unk_24) {
        SysTask_Destroy(param0->unk_24);
    }

    if (param0->unk_28) {
        SysTask_Destroy(param0->unk_28);
    }

    ov41_0224B084(param0->unk_00, param1);
    ov41_0224AD84(param0->unk_18);

    memset(param0, 0, sizeof(UnkStruct_ov22_0225AB54));
}

void ov41_0224B250(u8 *arg0) {
    ov41_0224B310();
    ov41_0224B270(arg0);
    ov41_0224B298(arg0);
    ov41_0224B450(arg0 + 0x30);
}
