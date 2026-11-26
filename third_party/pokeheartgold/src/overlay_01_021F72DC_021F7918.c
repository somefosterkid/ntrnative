#include "global.h"

#include "map_object.h"
#include "overlay_01_021F72DC.h"
#include "overlay_01_021F944C.h"
#include "overlay_01_022053EC.h"
#include "billboard.h"
#include "billboard_gfx_sequence.h"

void ov01_021F8C88(LocalMapObject *, Sprite *);

s32 ov01_02205808(s32, LocalMapObject *, Sprite *);

struct MapObject;
typedef struct MapObject MapObject;
void sub_02023EE0(Billboard *billboard, int animNum);
int sub_02023F04(Billboard *billboard, fx32 numFrames);
void sub_02023F40(Billboard *billboard, fx32 animFrameNum);
typedef struct {
    u16 unk_00;
    u16 unk_02;
    fx32 unk_04;
} UnkStruct_ov5_021ED01C;
int ov01_021FA44C(int param0);
typedef struct {
    s16 unk_00;
    s8 unk_02;
    s8 unk_03;
    Billboard *unk_04;
    UnkStruct_ov5_021ED01C unk_08;
} UnkStruct_ov5_021EB2EC;
void ov01_021F8C30(Billboard *param0);
void ov01_021F796C(MapObject *mapObj, Billboard *param1, UnkStruct_ov5_021EB2EC *param2, int param3);

void ov01_021F7918(LocalMapObject *obj) {
    unsigned char new_var;
    Sprite *sprite;
    u8 *v1;
    v1 = sub_0205F40C(obj);
    new_var = 0x17;
    if (ov01_021FA2D4(obj) != 1) {
        sprite = *((Sprite **)v1);
        if (sprite != 0L) {
            u8 v0;
            v0 = *((u8 *)(((u8 *)v1) + 0x17));
            if ((((u32)(v0 << 0x1F)) >> 0x1F) == 0) {
                *((u8 *)(((u8 *)v1) + 0x17)) = (u8)(1 | ((*((u8 *)(((u8 *)v1) + new_var))) & (~1)));
                if (ov01_0220553C(obj) != 0) {
                    ov01_02205808(1, obj, sprite);
                }
            }
            ov01_021FA3E8(obj, sprite);
            ov01_021F8C88(obj, sprite);
        }
    }
}

void ov01_021F796C(MapObject *mapObj, Billboard *param1, UnkStruct_ov5_021EB2EC *param2, int param3) {
    if (param3 != param2->unk_02) {
        int v0 = ov01_021FA44C(param3);

        sub_02023EE0(param1, v0);
        sub_02023F40(param1, 0);
    } else {
        ov01_021F8C30(param1);
    }

    sub_02023F04(param1, 0);
}
