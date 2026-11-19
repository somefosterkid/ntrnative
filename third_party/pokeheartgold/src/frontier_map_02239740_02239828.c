#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "sys_task.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "bg_window.h"
#include "frontier_graphics.h"

struct BattleFrontier;

struct FrontierParticleSystem;

struct G3DPipelineBuffers;

struct PaletteData;

struct SpriteManager;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

typedef struct BattleFrontier BattleFrontier;

typedef struct FrontierParticleSystem FrontierParticleSystem;

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

typedef struct SpriteResourcesHeaderList SpriteResourcesHeaderList;

void ManagedSprite_SetAnimationFrame(ManagedSprite *managedSprite, u16 frame);

void ManagedSprite_SetDrawFlag(ManagedSprite *managedSprite, BOOL draw);

void ManagedSprite_SetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void ov80_0223962C(FrontierGraphics *param0, u16 param1);

ManagedSprite *ov80_0223968C(FrontierGraphics *param0, u16 param1, u16 param2);

void ov80_02239708(FrontierGraphics *param0, u16 param1, int param2);

void ov80_02239AF8(SpriteSystem *param0, SpriteManager *param1, NARC *param2, PaletteData *param3, u16 param4);

typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
    u16 unk_06_0 : 13;
    u16 unk_06_13 : 1;
    u16 unk_06_14 : 1;
    u16 unk_06_15 : 1;
} UnkStruct_ov104_0223D3B0_sub1;

typedef struct {
    u16 unk_00[8];
    UnkStruct_ov104_0223D3B0_sub1 unk_10[8];
} UnkStruct_ov104_0223D3B0;

UnkStruct_ov104_0223D3B0 *sub_02096878(BattleFrontier *frontier);

void sub_02096884(BattleFrontier *frontier);

void ov80_02239828(FrontierGraphics *param0);

void ov80_02239828(FrontierGraphics *param0) {
    int v0;
    NARC *v1;
    UnkStruct_ov104_0223D3B0 *v2;
    ManagedSprite *v3;

    v2 = sub_02096878(param0->frontier);
    v1 = NARC_New((enum NarcId)184, HEAP_ID_101);

    for (v0 = 0; v0 < 8; v0++) {
        if (v2->unk_00[v0] != 0xffff) {
            ov80_02239AF8(param0->spriteSystem, param0->spriteMan, v1, param0->plttData, v2->unk_00[v0]);
            ov80_0223962C(param0, v2->unk_00[v0]);
        }
    }

    for (v0 = 0; v0 < 8; v0++) {
        if (v2->unk_10[v0].unk_06_15 == 1) {
            v3 = ov80_0223968C(param0, v0, v2->unk_10[v0].unk_04);
            ManagedSprite_SetPositionXY(v3, v2->unk_10[v0].unk_00, v2->unk_10[v0].unk_02);
            ManagedSprite_SetDrawFlag(v3, v2->unk_10[v0].unk_06_14);

            ov80_02239708(param0, v0, v2->unk_10[v0].unk_06_13);

            ManagedSprite_SetAnim(v3, v2->unk_10[v0].unk_05);
            ManagedSprite_SetAnimationFrame(v3, v2->unk_10[v0].unk_06_0);
        }
    }

    NARC_Delete(v1);
    sub_02096884(param0->frontier);
}
