#include "global.h"

#include "bg_window.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "frontier_graphics.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

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
    ManagedSprite *unk_00[8];
    u16 unk_20[8];
    u32 unk_30;
    u16 unk_34[8];
} UnkStruct_ov104_0223D3B0_1;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222CC3C;
typedef struct {
    UnkStruct_ov63_0222CC3C unk_00;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_ov63_0222CCB8;
void ov42_02228068(UnkStruct_ov63_0222BE18 *param0, const UnkStruct_ov63_0222CCB8 *param1);
typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov63_0222D894;
BOOL ov42_02228C80(const UnkStruct_ov63_0222BCE8 *param0, const UnkStruct_ov63_0222BE18 *param1, const UnkStruct_ov63_0222D894 *param2, UnkStruct_ov63_0222CCB8 *param3);
BOOL ov42_02229A08(UnkStruct_ov63_0222D77C *param0, UnkStruct_ov63_0222CCB8 *param1);
BOOL ov42_02229AC8(UnkStruct_ov63_0222D848 *param0, UnkStruct_ov63_0222D894 *param1);

void ov80_02238C78(FrontierGraphics *param0);

void ov80_02238C78(FrontierGraphics *param0) {
    UnkStruct_ov63_0222D894 v0;
    UnkStruct_ov63_0222CCB8 v1;
    BOOL v2;

    while (ov42_02229A08(param0->unk_30, &v1) == 1) {
        ov42_02228068(param0->unk_14, &v1);
    }

    while (ov42_02229AC8(param0->unk_2C, &v0) == 1) {
        v2 = ov42_02228C80(param0->unk_18, param0->unk_14, &v0, &v1);

        if (v2 == 1) {
            ov42_02228068(param0->unk_14, &v1);
        }
    }
}
