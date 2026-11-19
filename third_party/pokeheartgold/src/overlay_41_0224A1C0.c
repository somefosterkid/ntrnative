#include "global.h"
struct UnkStruct_ov22_0225A154;

#include "unk_02013534.h"
#include "sprite.h"
#include "touch_hitbox_controller.h"
#include "overlay_41_unk_struct_ov22_0225a154.h"

s32 ov41_0224A1A8();

void ov41_0224A1C0(struct UnkStruct_ov22_0225A154 *arg0);

typedef struct UnkStruct_ov22_02259C58_tag {
    Sprite *unk_00;
    int unk_04;
    void *unk_08;
    void (*unk_0C)(struct UnkStruct_ov22_02259C58_tag *param0, void *param1);
} UnkStruct_ov22_02259C58;
void ov41_0224A1DC(UnkStruct_ov22_02259C58 *param0, int param1);

void ov41_0224A1C0(UnkStruct_ov22_0225A154 *arg0) {
    ov41_0224A1A8();
    FontOAM_Delete(*((TextOBJ * *)&arg0->unk_10));
    memset(arg0, 0, sizeof(UnkStruct_ov22_0225A154));
}

void ov41_0224A1DC(UnkStruct_ov22_02259C58 *param0, int param1) {
    if (param1 == param0->unk_04) {
        param0->unk_0C(param0, param0->unk_08);
    }
}
