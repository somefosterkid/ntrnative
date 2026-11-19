#include "global.h"
#include "bg_window.h"
#include "field/overlay_01_021FB878.h"

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

void StartBrightnessTransition(u16 stepCount, s16 targetBrightness, s16 startBrightness, int planeMask, u32 screenSelect);

BOOL IsBrightnessTransitionActive(u32 screenSelect);

enum SoundHandleType {
    SOUND_HANDLE_TYPE_FIELD_BGM = 0,
    SOUND_HANDLE_TYPE_POKEMON_CRY,
    SOUND_HANDLE_TYPE_FANFARE,
    SOUND_HANDLE_TYPE_SFX_1,
    SOUND_HANDLE_TYPE_SFX_2,
    SOUND_HANDLE_TYPE_SFX_3,
    SOUND_HANDLE_TYPE_SFX_4,
    SOUND_HANDLE_TYPE_BGM,
    SOUND_HANDLE_TYPE_ECHO,

    SOUND_HANDLE_TYPE_COUNT,

    NUM_SFX_HANDLES = SOUND_HANDLE_TYPE_SFX_4 - SOUND_HANDLE_TYPE_SFX_1 + 1,
};

void GF_SndHandleSetTempoRatio(enum SoundHandleType handleType, int tempoRatio);

BOOL PlaySE(u16 seqID);

typedef struct {
    Field3dObject unk_00[3];
    Field3dModel unk_168[3];
    Field3DModelAnimation unk_198[6];
    u16 unk_210;
    u16 unk_212;
    Field3DModelAnimation unk_214[1];
    fx32 unk_228;
    u16 unk_22C;
    u16 unk_22E;
} UnkStruct_ov115_02263DF8;

typedef struct {
    Field3dObject unk_00[3];
    Field3dModel unk_168[3];
    Field3DModelAnimation unk_198[3];
    fx32 unk_1D4;
    u8 unk_1D8;
    u8 unk_1D9;
    s16 unk_1DA;
    fx32 unk_1DC;
    fx32 unk_1E0;
} UnkStruct_ov115_02264FA0;

typedef struct {
    u16 unk_00;
    u8 unk_02;
    s8 unk_03;
} UnkStruct_ov115_022658E0;

void ov91_02260364(UnkStruct_ov115_02263DF8 *param0, BOOL param1);

void ov91_022611D0(UnkStruct_ov115_02264FA0 *param0);

void ov91_02261200(UnkStruct_ov115_02264FA0 *param0, fx32 param1);

void ov91_02261A00(UnkStruct_ov115_022658E0 *param0, UnkStruct_ov115_02264FA0 *param1, UnkStruct_ov115_02263DF8 *param2);

void ov91_02261A00(UnkStruct_ov115_022658E0 *param0, UnkStruct_ov115_02264FA0 *param1, UnkStruct_ov115_02263DF8 *param2) {
    fx32 v0;
    u32 v1;

    if (param0->unk_02 == 0) {
        return;
    }

    switch (param0->unk_00) {
    case 0:
        param0->unk_03++;

        v0 = (param0->unk_03 * ((fx32)(((25) > 0) ? ((25) * (1 << 12) + 0.5f) : ((25) * (1 << 12) - 0.5f)))) / 58;
        v0 += ((fx32)(((1) > 0) ? ((1) * (1 << 12) + 0.5f) : ((1) * (1 << 12) - 0.5f)));

        ov91_02261200(param1, v0);

        v1 = (param0->unk_03 * 150) / 58;
        v1 += 256;

        GF_SndHandleSetTempoRatio((enum SoundHandleType)(7), v1);

        if (12 == param0->unk_03) {
            PlaySE(1415);
        }

        if (param0->unk_03 >= 58) {
            param0->unk_00++;
        }
        break;
    case 1:
        StartBrightnessTransition(4, 16, 0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, (1 << SCREEN_MAIN));
        param0->unk_00++;
        break;
    case 2:
        if (IsBrightnessTransitionActive(1 << SCREEN_MAIN) == 1) {
            ov91_022611D0(param1);
            ov91_02261200(param1, ((fx32)(((2) > 0) ? ((2) * (1 << 12) + 0.5f) : ((2) * (1 << 12) - 0.5f))));
            ov91_02260364(param2, 1);
            param0->unk_00++;
        }
        break;
    case 3:
        StartBrightnessTransition(14, 0, 16, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, (1 << SCREEN_MAIN));
        param0->unk_00++;
        break;
    case 4:
        if (IsBrightnessTransitionActive(1 << SCREEN_MAIN) == 1) {
            param0->unk_00++;
        }
        break;
    case 5:
        param0->unk_02 = 0;
        param0->unk_00 = 0;
        break;
    }
}
