#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "error_handling.h"

struct BgConfig;

struct Options;

struct SpriteResource;

struct String;

struct YesNoTouchMenuParams;

enum BgLayer {
    BG_LAYER_MAIN_0 = 0,
    BG_LAYER_MAIN_1,
    BG_LAYER_MAIN_2,
    BG_LAYER_MAIN_3,
    BG_LAYER_SUB_0,
    BG_LAYER_SUB_1,
    BG_LAYER_SUB_2,
    BG_LAYER_SUB_3,

    BG_LAYER_MAX,
};

typedef struct YesNoTouchMenuParams {
    BgConfig *bgConfig;
    enum BgLayer bgLayer;
    u32 baseTile;
    u32 palette;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 unk_12;
    u8 unk_13;
} YesNoTouchMenuParams;

void ov37_021E68AC(UnkStruct_02095EAC *param0);

int ov37_021E76A0(int param0);

void ov37_021E7844(UnkStruct_02095EAC *param0, int param1);

BOOL ov37_021E7880(UnkStruct_02095EAC *param0, YesNoTouchMenuParams *param1);

int ov37_021E6CC0(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6CC0(UnkStruct_02095EAC *param0, int param1) {
    if (ov37_021E76A0(param0->unk_30)) {
        YesNoTouchMenuParams v0;
        v0.bgConfig = param0->unk_00;
        v0.bgLayer = BG_LAYER_MAIN_0;
        v0.baseTile = 180;
        v0.palette = 8;
        v0.tilemapLeft = 25;
        v0.tilemapTop = 6;
        v0.unk_13 = 0;
        if (ov37_021E7880(param0, &v0) == 0) {
            GF_AssertFail();
        }
        ov37_021E7844(param0, 14);
    }
    ov37_021E68AC(param0);
    return param1;
}
