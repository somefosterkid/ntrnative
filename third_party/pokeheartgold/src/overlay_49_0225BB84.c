#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "list_menu.h"
#include "pm_string.h"
#include "touch_hitbox_controller.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

extern u32 ov49_022697F4;

struct G3DPipelineBuffers;
struct SpriteList;
struct SpriteResourceCollection;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct {
    u32 unk_00;
    Window unk_04[5];
    Sprite *unk_54;
    SpriteResource *unk_58[4];
    Sprite *unk_68;
    SpriteResource *unk_6C[4];
    Sprite *unk_7C[12];
    SpriteResource *unk_AC[4];
    SpriteResource *unk_BC[12];
    SpriteResource *unk_EC[4];
    SpriteResource *unk_FC[4];
} UnkStruct_ov70_0225FA84;
void ov49_0225BF80(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E9C8 *param1, UnkStruct_ov70_0225E4EC *param2);
void ov49_0225BBA8(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E9C8 *param1, UnkStruct_ov70_0225E4EC *param2);

void ov49_0225BB84(UnkStruct_ov70_0225FA84 *param_1, UnkStruct_ov70_0225E4EC *param_2);

void ov49_0225BB84(UnkStruct_ov70_0225FA84 *param_1, UnkStruct_ov70_0225E4EC *param_2) {
    WindowTemplate *template;
    Window *window;
    s32 iVar1;

    template = (WindowTemplate *)&ov49_022697F4;
    iVar1 = 0;
    window = param_1->unk_04;
    do {
        AddWindow(param_2->unk_00, window, template);
        iVar1 = iVar1 + 1;
        template = template + 1;
        window = window + 1;
    } while (iVar1 < 5);
    return;
}

void ov49_0225BBA8(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E9C8 *param1, UnkStruct_ov70_0225E4EC *param2) {
    if (param0->unk_54 != 0L) {
        ov49_0225BF80(param0, param1, param2);
    }

    {
        int v0;

        for (v0 = 0; v0 < 5; v0++) {
            RemoveWindow(&param0->unk_04[v0]);
        }
    }
}
