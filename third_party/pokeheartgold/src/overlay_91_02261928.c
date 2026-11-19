#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_91.h"
#include "unk_02005D10.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct FontOAM;

struct UnkStruct_02012B20_t;

typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;

void sub_02013728(FontOAM *param0);

void TextOBJ_SetSpritesDrawFlag(FontOAM *param0, BOOL param1);

typedef struct {
    fx32 unk_00;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov115_02261304;

typedef struct {
    SpriteResource *unk_00[4];
    SpriteResourcesHeader unk_10;
    Sprite *unk_34;
    Window unk_38;
    FontOAM *unk_48;
    UnkStruct_02012B20 *unk_4C;
    UnkStruct_02021AC8 unk_50;
    String *unk_5C;
    SpriteResource *unk_60;
    UnkStruct_ov115_02261304 unk_64;
    UnkStruct_ov115_02261304 unk_7C;
    s16 unk_94;
    s16 unk_96;
    u16 unk_98;
    s16 unk_9A;
    enum HeapID heapID;
} UnkStruct_ov115_02265788;

BOOL ov91_0225D46C(UnkStruct_ov115_02261304 *param0);

void ov91_02261928(UnkStruct_ov115_02265788 *param0);

void ov91_02261928(UnkStruct_ov115_02265788 *param0) {
    switch (param0->unk_98) {
    case 0:
        break;
    case 1:
        if (param0->unk_94 < 8) {
            param0->unk_94++;
        } else {
            param0->unk_98 = 2;
        }
        break;
    case 2:
        break;
    case 3:
        if (param0->unk_9A > 0) {
            param0->unk_9A--;
            break;
        }

        if (param0->unk_94 > 0) {
            param0->unk_94--;
        } else {
            param0->unk_98 = 0;
            TextOBJ_SetSpritesDrawFlag(param0->unk_48, 0);
            Sprite_SetDrawFlag(param0->unk_34, 0);
        }
        break;
    }

    if (param0->unk_96 < 4) {
        param0->unk_96++;
        param0->unk_7C.unk_10 = param0->unk_96;
        ov91_0225D46C(&param0->unk_7C);
    }

    {
        param0->unk_64.unk_10 = param0->unk_94;
        ov91_0225D46C(&param0->unk_64);
    }

    {
        VecFx32 v0;

        v0.x = param0->unk_64.unk_00;
        v0.y = param0->unk_7C.unk_00;

        Sprite_SetMatrix(param0->unk_34, &v0);
        sub_02013728(param0->unk_48);
    }
}

void ov91_022619E8(void *arg0) {
    *(s16 *)arg0 = 0;
    *(s8 *)((u8 *)arg0 + 2) = 1;
    *(s8 *)((u8 *)arg0 + 3) = 0;
    PlaySE(0x586U);
}
