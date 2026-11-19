#include "global.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "font.h"

struct BgConfig;

struct CharTransferAllocation;

struct TextOBJ;

struct SpriteManager;

struct String;

struct UnkStruct_02012744_t;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

typedef u32 TextColor;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    fx32 unk_04;
} UnkStruct_ov117_02266BEC;

typedef struct {
    s32 unk_00[3];
} UnkStruct_ov117_02266ED4;

extern const UnkStruct_ov117_02266ED4 ov93_02262FD4[5];

extern const UnkStruct_ov117_02266BEC ov93_02262CEC[3];

void ov93_02261EB8(BgConfig *param0, SpriteManager *param1, UnkStruct_02012744 *param2, UnkStruct_ov117_02265EB0 *param3, const String *param4, enum Font param5, TextColor param6, int param7, int param8, int param9, int param10, int param11, int param12, int param13, int param14);

void ov93_0225FD8C(int param0, int param1, int param2, fx32 *param3, fx32 *param4);

void ov93_0225FD8C(int param0, int param1, int param2, fx32 *param3, fx32 *param4) {
    fx32 v0;

    v0 = ov93_02262CEC[param1].unk_04 * param2 / ov93_02262FD4[param0].unk_00[param1];
    v0 += 0x300;

    *param3 = v0;
    *param4 = v0;

    if (v0 > (FX32_ONE)) {
        *param3 += FX_Mul((v0 - (FX32_ONE)), (FX32_CONST(1.1)));
    }
}
