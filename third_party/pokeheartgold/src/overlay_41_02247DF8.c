#include "global.h"

#include "menu_input_state.h"
#include "yes_no_prompt.h"
#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "filesystem_files_def.h"

YesNoResponse ov41_02247DF8(u8 *param_1);

struct Pokemon;
struct Pokepic;
struct PokepicManager;
struct PokemonSpriteTransforms;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
typedef struct Pokemon Pokemon;
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef struct Pokepic Pokepic;
typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;
typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);
typedef struct PokepicTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
} PokepicTemplate;
struct PokemonSpriteTransforms {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};
typedef struct PokemonSpriteShadow {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
} PokemonSpriteShadow;
struct Pokepic {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokepicTemplate template;
    PokepicTemplate templateBackup;
    PokemonSpriteTransforms transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokemonSpriteShadow shadow;
    PokemonSpriteShadow shadowBackup;
    SpriteAnimFrame animFrames[10];
};
typedef struct PokepicManager PokepicManager;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef struct {
    int unk_00;
    SoftwareSprite *unk_04;
} UnkStruct_ov22_02255040;
typedef struct {
    UnkStruct_ov22_02255040 *unk_00;
    int unk_04;
} UnkStruct_ov22_02254DE0;
typedef struct {
    UnkStruct_ov22_02259560 *unk_00;
    int unk_04;
} UnkStruct_ov22_02259484;
typedef struct {
    UnkStruct_ov22_02259484 *unk_00;
    UnkStruct_ov22_02259560 unk_04;
    UnkStruct_ov22_02259560 unk_14;
    int unk_24;
    int unk_28;
} UnkStruct_ov22_02257964_sub1;
typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov22_022596B0;
typedef struct {
    Pokepic *unk_00;
    TouchscreenHitbox unk_04;
    UnkStruct_ov22_022596B0 unk_08;
    Pokemon *unk_0C;
} UnkStruct_020298D8;
typedef struct {
    BgConfig *unk_00;
    enum NarcId narcID;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
} UnkStruct_ov22_022597BC;
typedef struct {
    SoftwareSpriteManager *unk_00;
    SoftwareSpriteChars **unk_04;
    SoftwareSpritePalette **unk_08;
    u8 *unk_0C;
    PokepicManager *unk_10;
    BgConfig *unk_14;
    UnkStruct_ov22_02254DE0 *unk_18;
    UnkStruct_ov22_022597BC unk_1C;
    int unk_48;
    UnkStruct_020298D8 unk_4C;
} UnkStruct_ov22_02257964_sub2;
typedef struct {
    UnkStruct_ov22_02257964_sub1 unk_00;
    UnkStruct_ov22_02257964_sub2 unk_2C;
} UnkStruct_ov22_02257964;
typedef struct {
    SoftwareSpriteManager *unk_00;
    SoftwareSpriteChars **unk_04;
    SoftwareSpritePalette **unk_08;
    u8 *unk_0C;
    PokepicManager *unk_10;
    BgConfig *unk_14;
    UnkStruct_ov22_02254DE0 *unk_18;
    UnkStruct_ov22_02259484 *unk_1C;
    int unk_20;
} UnkStruct_ov22_0225B388;
UnkStruct_ov22_02259560 *ov41_022499F0(UnkStruct_ov22_02259484 *param0, void *param1, int param2);
void ov41_02249A50(UnkStruct_ov22_02259560 *param0, UnkStruct_ov22_02259560 *param1);
void ov41_02247E34(u32 param0, u32 param1, u32 param2, u32 param3, u32 param4, int *param5, int *param6);
void ov41_02247F3C(UnkStruct_ov22_02257964 *param0, UnkStruct_ov22_0225B388 *param1);

YesNoResponse ov41_02247DF8(u8 *param_1) {
    YesNoResponse YVar1;
    MenuInputState MVar2;

    YVar1 = YesNoPrompt_HandleInput(*((YesNoPrompt **)&((UnkStruct_ov22_0225A0E4 *)param_1)->unk_6B8));
    switch (YVar1) {
    case YESNORESPONSE_WAIT:
        return (YesNoResponse)(YESNORESPONSE_NO_TOUCH | YESNORESPONSE_YES);
    case YESNORESPONSE_YES:
        YVar1 = (YesNoResponse)(YESNORESPONSE_NO_TOUCH | YESNORESPONSE_NO);
        break;
    case YESNORESPONSE_NO:
        YVar1 = (YesNoResponse)(YESNORESPONSE_NO_TOUCH | YESNORESPONSE_YES_TOUCH);
        break;
    }
    MVar2 = YesNoPrompt_IsInTouchMode(*((YesNoPrompt **)&((UnkStruct_ov22_0225A0E4 *)param_1)->unk_6B8));
    *(MenuInputState *)(param_1 + 0x6ec) = MVar2;
    return YVar1;
}

void ov41_02247E34(u32 param0, u32 param1, u32 param2, u32 param3, u32 param4, int *param5, int *param6) {
    u32 v0, v1;
    u32 v2, v3;

    v0 = param2 + ((param3 - param2) / 3);
    v1 = param2 + ((param3 - param2) / 3) * 2;
    v2 = param0 + ((param1 - param0) / 3);
    v3 = param0 + ((param1 - param0) / 3) * 2;

    switch (param4) {
    case 0:
        *param5 = param2;
        *param6 = param0;
        break;
    case 1:
        *param5 = param2;
        *param6 = v2;
        break;
    case 2:
        *param5 = param2;
        *param6 = v3;
        break;
    case 3:
        *param5 = param2;
        *param6 = param1;
        break;
    case 4:
        *param5 = v0;
        *param6 = param0;
        break;
    case 5:
        *param5 = v0;
        *param6 = v2;
        break;
    case 6:
        *param5 = v0;
        *param6 = v3;
        break;
    case 7:
        *param5 = v0;
        *param6 = param1;
        break;
    case 8:
        *param5 = v1;
        *param6 = param0;
        break;
    case 9:
        *param5 = v1;
        *param6 = v2;
        break;
    case 10:
        *param5 = v1;
        *param6 = v3;
        break;
    case 11:
        *param5 = v1;
        *param6 = param1;
        break;
    case 12:
        *param5 = param3;
        *param6 = param0;
        break;
    case 13:
        *param5 = param3;
        *param6 = v2;
        break;
    case 14:
        *param5 = param3;
        *param6 = v3;
        break;
    case 15:
        *param5 = param3;
        *param6 = param1;
        break;
    }
}

void ov41_02247F3C(UnkStruct_ov22_02257964 *param0, UnkStruct_ov22_0225B388 *param1) {
    param0->unk_2C.unk_00 = param1->unk_00;
    param0->unk_2C.unk_04 = param1->unk_04;
    param0->unk_2C.unk_08 = param1->unk_08;
    param0->unk_2C.unk_0C = param1->unk_0C;
    param0->unk_2C.unk_10 = param1->unk_10;
    param0->unk_2C.unk_14 = param1->unk_14;
    param0->unk_2C.unk_18 = param1->unk_18;
    param0->unk_2C.unk_48 = 0;
    param0->unk_00.unk_00 = param1->unk_1C;
    param0->unk_00.unk_04.unk_08 = &param0->unk_00.unk_04;
    param0->unk_00.unk_04.unk_0C = &param0->unk_00.unk_04;
    param0->unk_00.unk_14.unk_08 = &param0->unk_00.unk_14;
    param0->unk_00.unk_14.unk_0C = &param0->unk_00.unk_14;
    param0->unk_00.unk_24 = 0;
    param0->unk_00.unk_28 = param1->unk_20;

    {
        UnkStruct_ov22_02259560 *v0;

        v0 = ov41_022499F0(param0->unk_00.unk_00, &param0->unk_2C.unk_4C, 3);
        ov41_02249A50(v0, &param0->unk_00.unk_14);
    }
}
