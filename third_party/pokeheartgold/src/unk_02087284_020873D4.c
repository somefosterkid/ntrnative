#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "struct_defs/resdat_narc_structs.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "sys_task.h"

struct ManagedSprite;

struct NARC;

struct PaletteData;

struct SelectedPaletteData;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

typedef struct {
    int unk_00;
    enum HeapID heapID;
    int unk_08;
    int unk_0C;
    fx32 unk_10;
} UnkStruct_0208B284;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

extern void ManagedSprite_GetPositionXYWithSubscreenOffset(ManagedSprite *managedSprite, s16 *outX, s16 *outY, fx32 offset);

void ManagedSprite_AddSpritePrecisePositionXY(ManagedSprite *managedSprite, fx32 x, fx32 y);

void ManagedSprite_SetPositionFxXYWithSubscreenOffset(ManagedSprite *managedSprite, fx32 x, fx32 y, fx32 offset);

void ManagedSprite_GetPositionFxXYWithSubscreenOffset(ManagedSprite *managedSprite, fx32 *outX, fx32 *outY, fx32 offset);

typedef struct {
    NARC *unk_00;
    SpriteSystem *unk_04;
    SpriteManager *unk_08;
    PaletteData *unk_0C;
} UnkStruct_0208B878_sub1;

typedef struct {
    ManagedSprite *unk_00;
    s16 unk_04;
    s16 unk_06;
    int unk_08;
    int unk_0C;
} UnkStruct_0208B878_sub2;

typedef struct {
    UnkStruct_0208B878_sub1 unk_00;
    UnkStruct_0208B284 unk_10;
    UnkStruct_0208B878_sub2 unk_24[12];
    int unk_E4;
    int unk_E8;
    int unk_EC;
    BOOL unk_F0;
    SysTask *unk_F4;
    int unk_F8;
    int unk_FC;
    int unk_100;
    int unk_104;
} UnkStruct_0208B878;

fx32 GF_SinDeg(u16 degrees);

fx32 GF_CosDeg(u16 degrees);

extern BOOL sub_020872DC(s16 param0, s16 param1, f32 param2, f32 param3, f32 *param4, f32 *param5, f32 param6, s16 param7);

void sub_020873D4(SysTask *param0, void *param1);

void sub_020873D4(SysTask *param0, void *param1) {
    UnkStruct_0208B878 *v0 = param1;

    if (v0->unk_F0 == 0) {
        return;
    }

    {
        int v1;
        int v2 = 0;
        f32 v3;
        f32 v4;
        fx32 v5;
        fx32 v6;
        f32 v7;
        f32 v8;
        BOOL v9;

        for (v1 = 0; v1 < v0->unk_10.unk_00; v1++) {
            if (v1 != 0) {
                v0->unk_24[v1].unk_04 = v0->unk_24[v1 - 1].unk_04;
                v0->unk_24[v1].unk_06 = v0->unk_24[v1 - 1].unk_06;
            }

            ManagedSprite_GetPositionFxXYWithSubscreenOffset(v0->unk_24[v1].unk_00, &v5, &v6, v0->unk_10.unk_10);

            v3 = ((f32)((v5) / (f32)(1 << 12)));
            v4 = ((f32)((v6) / (f32)(1 << 12)));

            v9 = sub_020872DC(v0->unk_24[v1].unk_04, v0->unk_24[v1].unk_06, v3, v4, &v7, &v8, (8.0f - ((v1 - v2) * 0.5f)) / 2, 16);

            if (v9 && (v0->unk_24[v1].unk_08 == 0)) {
                ManagedSprite_AddSpritePrecisePositionXY(v0->unk_24[v1].unk_00, ((fx32)(((v7) > 0) ? ((v7) * (1 << 12) + 0.5f) : ((v7) * (1 << 12) - 0.5f))), ((fx32)(((v8) > 0) ? ((v8) * (1 << 12) + 0.5f) : ((v8) * (1 << 12) - 0.5f))));
            } else {
                {
                    int v11;
                    s16 v12, v13;
                    fx32 v14, v15;

                    switch (v0->unk_24[v1].unk_08) {
                    case 0:
                        ManagedSprite_GetPositionXYWithSubscreenOffset(v0->unk_24[v1].unk_00, &v12, &v13, v0->unk_10.unk_10);

                        if (v0->unk_E8 == 0xFF) {
                            v0->unk_E8 = v1;
                            v11 = FX_Atan2Idx(((fx32)(((v0->unk_24[0].unk_06 - v13) > 0) ? ((v0->unk_24[0].unk_06 - v13) * (1 << 12) + 0.5f) : ((v0->unk_24[0].unk_06 - v13) * (1 << 12) - 0.5f))), ((fx32)(((v0->unk_24[0].unk_04 - v12) > 0) ? ((v0->unk_24[0].unk_04 - v12) * (1 << 12) + 0.5f) : ((v0->unk_24[0].unk_04 - v12) * (1 << 12) - 0.5f))));
                            v0->unk_24[v1].unk_0C = v11;
                        } else {
                            v0->unk_24[v1].unk_0C = v0->unk_24[v0->unk_E8].unk_0C - ((720 / v0->unk_10.unk_00) * v0->unk_EC);
                        }

                        v0->unk_24[v1].unk_0C %= 720;
                        v0->unk_EC++;
                        v0->unk_24[v1].unk_08++;
                        break;
                    case 1:
                        v0->unk_24[v1].unk_0C += (8 / 2);
                        v0->unk_24[v1].unk_0C %= 720;
                        v14 = (v0->unk_24[0].unk_04 << 12) + (GF_SinDeg(v0->unk_24[v1].unk_0C) * v0->unk_FC);
                        v15 = (v0->unk_24[0].unk_06 << 12) + (GF_CosDeg(v0->unk_24[v1].unk_0C) * v0->unk_100);
                        ManagedSprite_SetPositionFxXYWithSubscreenOffset(v0->unk_24[v1].unk_00, v14, v15, v0->unk_10.unk_10);
                        break;
                    default:
                        break;
                    }
                }
                v2++;
            }
        }
    }
}
