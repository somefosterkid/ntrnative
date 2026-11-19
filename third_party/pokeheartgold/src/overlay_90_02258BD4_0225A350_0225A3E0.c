#include "global.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"

struct BgConfig;

struct BgTemplate;

struct GraphicsModes;

struct String;

struct Window;


typedef struct String String;

enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};








typedef struct {
    Window unk_00;
    u32 unk_10;
    String *unk_14;
    UnkStruct_ov114_0225CBF0 unk_18;
    s16 unk_2C;
    s16 unk_2E;
} UnkStruct_ov114_0225E1A4;

void ov90_02258CF0(UnkStruct_ov114_0225CBF0 *param0, fx32 param1, fx32 param2, fx32 param3, int param4);

void ov90_0225A60C(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1, s32 param2);

extern const fx32 ov90_0225C228[UnkEnum_ov66_022324D0_12];

extern const fx32 ov90_0225C1F8[UnkEnum_ov66_022324D0_12];

extern const fx32 ov90_0225C204[UnkEnum_ov66_022324D0_12];

void ov90_02258FF0(UnkStruct_ov114_0225CEF0 *param0, const GraphicsModes *param1, const UnkStruct_ov114_02260284 *param2, u32 param3, enum HeapID heapID);

void ov90_0225A3E0(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1);

void ov90_0225A3E0(UnkStruct_ov114_0225E1A4 *param0, UnkStruct_ov114_0225CEF0 *param1) {
    param0->unk_2C = 0;
    param0->unk_2E = 0;

    ov90_02258CF0(&param0->unk_18, ov90_0225C228[param0->unk_10], ov90_0225C1F8[param0->unk_10], ov90_0225C204[param0->unk_10], 8);
    ov90_0225A60C(param0, param1, param0->unk_18.unk_00 >> FX32_SHIFT);
}
