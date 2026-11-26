#include "global.h"

#include "frontier/frontier.h"

#include "bg_window.h"
#include "frontier_map.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "palette.h"
#include "render_text.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_020210A0.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "overlay_80_unk_struct_ov104_0223_c634.h"
#include "frontier_graphics.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

s32 ov80_0222ADB4(void *, void *, int);

s32 ov80_0223927C(void *);

SysTask *ov80_0223937C(void *);

s32 ov80_022393E8(void *);

s32 ov80_02239980(void *);

struct BattleFrontier;
struct FrontierParticleSystem;
struct G3DPipelineBuffers;
struct UnkStruct_ov63_0222AE60_t;
struct UnkStruct_ov63_0222BB38_t;
struct UnkStruct_ov63_0222BCE8_t;
struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222BEC0_t;
struct UnkStruct_ov63_0222D1C0_t;
struct UnkStruct_ov63_0222D77C_t;
struct UnkStruct_ov63_0222D848_t;
typedef struct BattleFrontier BattleFrontier;
typedef struct UnkStruct_ov63_0222BCE8_t UnkStruct_ov63_0222BCE8;
typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;
typedef struct UnkStruct_ov63_0222AE60_t UnkStruct_ov63_0222AE60;
typedef struct UnkStruct_ov63_0222BB38_t UnkStruct_ov63_0222BB38;
typedef struct {
    const UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov63_0222BB38 *unk_04;
    u16 unk_08;
    u16 unk_0A;
    u16 unk_0C;
    u16 unk_0E;
    u16 unk_10;
    u8 unk_12;
    u8 unk_13;
} UnkStruct_ov63_0222CE44;
typedef struct {
    UnkStruct_ov63_0222AE60 *unk_00;
    UnkStruct_ov63_0222CE44 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov63_0222CD2C;
typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222D160;
typedef struct UnkStruct_ov63_0222D1C0_t UnkStruct_ov63_0222D1C0;
typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;
typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;
typedef struct FrontierParticleSystem FrontierParticleSystem;
typedef struct {
    SysTask *unk_00;
    s16 unk_04;
    s16 unk_06;
    s8 unk_08;
    s8 unk_09;
    s8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov104_0223319C;
typedef struct {
    SysTask *unk_00;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    s16 unk_08;
    s8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov104_022331E8;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    s16 unk_02[7];
} UnkStruct_ov104_0223D8F0;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    s16 unk_06;
    s16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
    u8 unk_0C;
    UnkStruct_ov104_0223D8F0 unk_0E;
} UnkStruct_ov104_0223D570;
typedef struct {
    ManagedSprite *unk_00[8];
    u16 unk_20[8];
    u32 unk_30;
    u16 unk_34[8];
} UnkStruct_ov104_0223D3B0_1;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
s32 ov42_02228188(const UnkStruct_ov63_0222BEC0 *param0, int param1);
BOOL ov42_022291F4(const UnkStruct_ov63_0222CE44 *param0);
void ov80_02239740(FrontierGraphics *param0);
void ov80_022389C4(FrontierGraphics *param0);

void FrontierMap_Free(void *ptr) {
    Frontier_GetLaunchArgs(*(void **)((u8 *)ptr + 8));
    ov80_0222ADB4(ptr, ptr + 0x90, *(u8 *)((u8 *)ptr + 0xC1));
    ov80_0223927C(ptr);
    GfGfx_EngineATogglePlanes(1U, 0U);
    GfGfx_EngineATogglePlanes(2U, 0U);
    FreeBgTilemapBuffer(*(BgConfig **)ptr, 1U);
    FreeBgTilemapBuffer(*(BgConfig **)ptr, 2U);
    FreeBgTilemapBuffer(*(BgConfig **)ptr, 3U);
    ToggleBgLayer(4U, 0U);
    FreeBgTilemapBuffer(*(BgConfig **)ptr, 4U);
    ov80_022393E8(ptr);
    ov80_02239980(*(void **)((u8 *)ptr + 0x10));
    GF_DestroyVramTransferManager();
    PaletteData_FreeBuffers(*(PaletteData **)((u8 *)ptr + 4), PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(*(PaletteData **)((u8 *)ptr + 4), PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(*(PaletteData **)((u8 *)ptr + 4), PLTTBUF_MAIN_OBJ);
    PaletteData_FreeBuffers(*(PaletteData **)((u8 *)ptr + 4), PLTTBUF_SUB_OBJ);
    PaletteData_Free(*(PaletteData **)((u8 *)ptr + 4));
    Heap_Free(*(BgConfig **)ptr);
    SysTask_Destroy(*(SysTask **)((u8 *)ptr + 0x94));
    SysTask_Destroy(*(SysTask **)((u8 *)ptr + 0x98));
    SysTask_Destroy(*(SysTask **)((u8 *)ptr + 0x9C));
    SysTask_Destroy(*(SysTask **)((u8 *)ptr + 0xA0));
    ov80_0223937C(*(void **)((u8 *)ptr + 0xC));
    DisableTouchPad();
    Heap_Free(ptr);
    reg_GX_DISPCNT &= 0xFFFF1FFF;
    reg_GXS_DB_DISPCNT &= 0xFFFF1FFF;
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    Heap_Destroy(HEAP_ID_101);
    TextFlags_SetCanABSpeedUpPrint(0);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    sub_0203A914();
    MIi_CpuClear16(0x7FFFU, (void *)HW_BG_PLTT, 0x200U);
    MIi_CpuClear16(0x7FFFU, (void *)HW_OBJ_PLTT, 0x200U);
    MIi_CpuClear16(0x7FFFU, (void *)HW_DB_BG_PLTT, 0x200U);
    MIi_CpuClear16(0x7FFFU, (void *)HW_DB_OBJ_PLTT, 0x200U);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
}

void ov80_022389C4(FrontierGraphics *param0) {
    int v0;

    {
        UnkStruct_ov104_0223C634 *v1;

        for (v0 = 0; v0 < 32; v0++) {
            v1 = sub_0209686C(param0->frontier, v0);

            if (v1->unk_00 != 0L) {
                v1->unk_08.unk_0A = ov42_02228188(v1->unk_00, 6);
                v1->unk_08.unk_02 = ov42_02228188(v1->unk_00, 5);
                v1->unk_08.unk_06 = ov42_02228188(v1->unk_00, 0);
                v1->unk_08.unk_08 = ov42_02228188(v1->unk_00, 1);
                v1->unk_08.unk_0B = ov42_022291F4(v1->sprite);
            }
        }
    }

    ov80_02239740(param0);
}
