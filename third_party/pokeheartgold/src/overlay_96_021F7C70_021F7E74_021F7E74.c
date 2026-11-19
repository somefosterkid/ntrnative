#include "global.h"

#include "constants/heap.h"

#include "overlay_96_unk_struct_ov96_021_f7934.h"
#include "save.h"

struct RecordMixedRNG;

struct SaveData;

typedef struct RecordMixedRNG RecordMixedRNG;

RecordMixedRNG *Save_SealCase_Get(SaveData *saveData);

void *ov73_021E7960(RecordMixedRNG *param0);

void *ov73_021EA2B4(SaveData *saveData, enum HeapID heapID, u32 param2);

void *ov59_021D3088(SaveData *saveData, enum HeapID heapID, u32 param2);

void *ov73_021EA348(SaveData *saveData, enum HeapID heapID, u32 param2);

BOOL ov96_021F7E74(void *param0, u8 *state);

void *PokeathlonCourse_GetHeapAllocPtr4(void *param0);

BOOL IsFanfarePlaying(void);

int PokeathlonCourse_GetMode(void *param0);

int ov96_021F85F4(void *param0);

void ov96_021EE830(void *param0);

int ov96_021EEA80(void *param0);

void *ov96_021EE97C(void *param0);

int ov96_021F83DC(void *param0);

BOOL ov96_021F81CC(UnkStruct_ov96_021F8094 *param0, void *param1);

void *PokeathlonCourse_GetGraphicsSystem(void *param0);

void *ov96_021E94EC(void *param0, u32 param1);

void *PokeathlonCourse_GetPlayerProfileFromData(void *param0, u32 param1);

void BufferPlayersName(void *param0, u32 param1, void *param2);

void StartBrightnessTransition(u16 param0, s16 param1, s16 param2, u32 param3, u32 param4);

void UpdateMainScreenBrightnessSurface(u32 param0, u32 param1);

BOOL IsBrightnessTransitionActive(u32 param0);

u32 PokeathlonCourse_GetField3D8_ForCurrentParticipant(void *param0);

s32 ov96_021E95D8(void *param0, u32 param1, u32 param2);

void ov96_021F910C(u8 *param0, s32 param1, u32 param2);

int ov96_021F83FC(void *param0);

u8 ov96_021F7E64(void *param0);

void BeginNormalPaletteFade(u32 param0, u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6);

void GF_SndStartFadeOutBGM(u32 param0, u32 param1);

BOOL IsPaletteFadeFinished(void);

void ov96_021EE944(void *param0);

void ov96_021EE8CC(void *param0, s32 param1);

BOOL ov96_021F7E74(void *param0, u8 *state) {
    int r4 = 0;
    BOOL ret;
    UnkStruct_ov96_021F7934 *v0;
    void *v10;
    u32 v9;
    void *arr[4];
    int i;
    s32 v;

    v0 = PokeathlonCourse_GetHeapAllocPtr4(param0);
    ret = r4;
    IsFanfarePlaying();
    if (PokeathlonCourse_GetMode(param0) == 1) {
        r4 = ov96_021F85F4(param0);
    }
    ov96_021EE830(v0->unk_18);
    if (ov96_021EEA80(v0->unk_18) == 0 && r4 == 0) {
        v10 = ov96_021EE97C(v0->unk_18);
        v9 = -1;

        switch (*state) {
        case 0:
            v9 = ov96_021F83DC(param0);
            (*state)++;
            break;
        case 1:
            if (ov96_021F81CC(&v0->unk_1C, param0)) {
                for (i = 0; i < 4; i++) {
                    arr[i] = ov96_021E94EC(PokeathlonCourse_GetGraphicsSystem(param0), (u8)i);
                }
                if (*((u8 *)arr[0] + 9) == *((u8 *)arr[1] + 9)) {
                    v9 = 0xfe;
                } else {
                    void *profile = PokeathlonCourse_GetPlayerProfileFromData(param0, *(u32 *)arr[0]);
                    BufferPlayersName(v10, 0, profile);
                    v9 = 0xfd;
                }
                (*state)++;
            }
            break;
        case 2:
            StartBrightnessTransition(16, -8, 0, 0x3f, 1);
            UpdateMainScreenBrightnessSurface(0x3f, 0x3f);
            reg_G2_BLDALPHA = 16;
            (*state)++;
            break;
        case 3:
            if (IsBrightnessTransitionActive(1) == 0) {
                break;
            }
            v9 = 0xff;
            (*state)++;
            break;
        case 4:
            for (i = 0; i < 4; i++) {
                r4 = PokeathlonCourse_GetField3D8_ForCurrentParticipant(param0);
                v = ov96_021E95D8(PokeathlonCourse_GetGraphicsSystem(param0), r4, i);
                if (v > 0x3e7) {
                    v = 0x3e7;
                }
                ov96_021F910C(v0->unk_14, v, (u8)i);
            }
            StartBrightnessTransition(16, 0, 16, 0x3f, 2);
            (*state)++;
            break;
        case 5:
            if (IsBrightnessTransitionActive(2) == 0) {
                break;
            }
            (*state)++;
            break;
        case 6:
            v9 = ov96_021F83FC(param0);
            v0->unk_A8 = 1;
            (*state)++;
            break;
        case 7:
            if (PokeathlonCourse_GetMode(param0) != 0) {
                if (ov96_021F7E64(param0) == 0) {
                    break;
                }
            }
            BeginNormalPaletteFade(0, 0, 0, 0x7fff, 12, 1, v0->heapID);
            GF_SndStartFadeOutBGM(0, 12);
            (*state)++;
            break;
        case 8:
            if (IsPaletteFadeFinished() == 0) {
                break;
            }
            ov96_021EE944(v0->unk_18);
            ret = 1;
            break;
        }

        if (v9 != -1) {
            ov96_021EE8CC(v0->unk_18, v9);
            v0->unk_A4++;
        }
    }
    return ret;
}
