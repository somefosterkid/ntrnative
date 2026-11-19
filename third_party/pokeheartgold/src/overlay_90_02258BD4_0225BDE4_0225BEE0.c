#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"

struct SpriteResourceCollection;

struct UnkStruct_ov114_02025CAD4_t;

typedef struct {
    Sprite *unk_00[2];
    s16 unk_08;
    s16 unk_0A;
    u8 unk_0C[2][192];
    SysTask *unk_18C;
    GXWndPlane unk_190;
    GXWndPlane battlerType;
    int unk_194;
} UnkStruct_ov114_0225FAB8;

struct UnkStruct_ov114_02025CAD4_t {
    u32 unk_00;
    u16 unk_04;
    s16 unk_06;
    SpriteList *unk_08;
    NARC *unk_0C;
    UnkStruct_ov114_0225CCD0 unk_10;
    UnkStruct_ov114_0225CDB4 *unk_28;
    SysTask *unk_2C;
    Sprite *unk_30;
    UnkStruct_ov114_0225FAB8 unk_34;
};

Sprite *ov90_02258EB4(UnkStruct_ov114_0225CDB4 *param0, SpriteList *param1, s32 param2, s32 param3, u16 param4, u32 param5);

void ov90_0225BAD0(UnkStruct_ov114_0225FAB8 *param0, UnkStruct_ov114_0225CDB4 *param1, SpriteList *param2, u32 param3);

BOOL ov90_0225BBD0(UnkStruct_ov114_0225FAB8 *param0, void *);

BOOL ov90_0225BBF0(UnkStruct_ov114_0225FAB8 *param0);

void ov90_0225BD08(UnkStruct_ov114_0225FAB8 *param0);

void ov90_0225BEE0(SysTask *param0, void *param1);

void ov90_0225BEE0(SysTask *param0, void *param1) {
    UnkStruct_ov114_0225CAD4 *v0 = param1;
    BOOL v1;
    u16 v2;
    u16 v3;

    switch (v0->unk_04) {
    case 0:
        break;
    case 1:
        ov90_0225BAD0(&v0->unk_34, v0->unk_28, v0->unk_08, v0->unk_00);
        v0->unk_30 = ov90_02258EB4(v0->unk_28, v0->unk_08, 128, 97, 64, v0->unk_00);
        Sprite_SetPriority(v0->unk_30, 0);
        v0->unk_04++;
        break;
    case 2:
        v1 = ov90_0225BBD0(&v0->unk_34, param1);

        if (v1 == 1) {
            v0->unk_04++;
            v0->unk_06 = 0;
        }
        break;
    case 3:
        v2 = Sprite_GetAnimationFrame(v0->unk_30);
        Sprite_UpdateAnim(v0->unk_30, ((fx32)(((2) > 0) ? ((2) * (1 << 12) + 0.5f) : ((2) * (1 << 12) - 0.5f))));
        v3 = Sprite_GetAnimationFrame(v0->unk_30);

        if (v2 != v3) {
            switch (v3) {
            case 7:
                PlaySE(1501);
                break;
            case 9:
                PlaySE(1501);
                break;
            case 11:
                PlaySE(1501);
                break;
            case 13:
                PlaySE(1501);
                break;
            case 15:
                PlaySE(1602);
                break;
            default:
                break;
            }
        }

        if (v3 == 15) {
            if (v0->unk_06 == 0) {
                Sprite_SetPalOffsetRespectVramOffset(v0->unk_30, 0);
            } else if (v0->unk_06 == 4) {
                Sprite_SetPalOffsetRespectVramOffset(v0->unk_30, 1);
            }

            v0->unk_06++;

            if (v0->unk_06 >= 4 * 2) {
                v0->unk_06 = 0;
            }
        }

        v1 = Sprite_IsAnimated(v0->unk_30);

        if (v1 == 0) {
            v0->unk_04++;
            Sprite_SetPalOffsetRespectVramOffset(v0->unk_30, 0);
        }
        break;
    case 4:
        v1 = ov90_0225BBF0(&v0->unk_34);

        if (v1 == 1) {
            Sprite_Delete(v0->unk_30);
            v0->unk_30 = 0L;
            v0->unk_04++;
            v0->unk_06 = 0;
        }
        break;
    case 5:
        v0->unk_06++;

        if (v0->unk_06 < 2) {
            break;
        }

        v0->unk_04 = 0;

        ov90_0225BD08(&v0->unk_34);
        SysTask_Destroy(v0->unk_2C);
        v0->unk_2C = 0L;
        break;
    default:
        break;
    }
}
