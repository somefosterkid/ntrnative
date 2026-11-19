#include "global.h"

#include "touchscreen.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "pokemon_types_def.h"
#include "overlay_74_main_menu_util.h"
#include "pokepic.h"

struct Pokemon;
struct Pokepic;
struct PokepicDrawParam;
struct SoftwareSprite;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov22_022596B0;
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
typedef struct {
    Pokepic *unk_00;
    TouchscreenHitbox unk_04;
    UnkStruct_ov22_022596B0 unk_08;
    Pokemon *unk_0C;
} UnkStruct_020298D8;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct {
    int unk_00;
    SoftwareSprite *unk_04;
} UnkStruct_ov22_02255040;
void ov41_02246020(UnkStruct_ov22_02255040 *param0, int *param1, int *param2);
void ov41_02249888(UnkStruct_020298D8 *param0, UnkStruct_ov22_022596B0 *param1);

void ov41_02249BAC(UnkStruct_ov22_02259560 *param0, int *param1, int *param2, int *param3, int *param4);

void ov41_02249BAC(UnkStruct_ov22_02259560 *param0, int *param1, int *param2, int *param3, int *param4) {
    if (param0->unk_04 < 3) {
        UnkStruct_ov22_02255040 *v0 = param0->unk_00;
        ov41_02246020(v0, param1, param3);
        *param2 = *param1;
        *param4 = *param3;
    } else {
        UnkStruct_ov22_022596B0 v1;
        UnkStruct_020298D8 *v2 = param0->unk_00;

        ov41_02249888(v2, &v1);

        *param1 = v1.unk_00;
        *param2 = v1.unk_01;
        *param3 = v1.unk_02;
        *param4 = v1.unk_03;
    }
}
