#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "screen_fade.h"
#include "sound.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"

s32 ov96_021EF280(s32, s32);

s32 ov96_021EF924(UnkStruct_ov96_021EF610 *arg0);

s32 ov96_021EFA3C(void *);

s32 ov96_021EFACC(void *);

BOOL ov96_021EFD9C(PokeathlonCourseData *arg0, u8 *arg1);

BOOL ov96_021F008C(PokeathlonCourseData *arg0, u8 *arg1);

BOOL ov96_021EFD9C(PokeathlonCourseData *data, u8 *arg1) {
    s32 v1;
    s32 flag;
    u8 v0;
    void *v2;

    v2 = PokeathlonCourse_GetHeapAllocPtr4(data);
    ov96_021EF924(v2);
    v0 = *arg1;
    switch (v0) {
    case 0:
        PlayBGM(0x470U);
        *arg1 += 1;
        break;
    case 1:
        if (ov96_021EF280(0, 0) != 0) {
            BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, 1, 1, *((enum HeapID *)&((UnkStruct_ov96_021EF610 *)v2)->unk_C));
            *arg1 += 1;
        }
        break;
    case 2:
        if (ov96_021EF280(0, 1) != 0) {
            ov96_021EFA3C(v2);
            *arg1 += 1;
        }
        break;
    case 3:
        if (((UnkStruct_ov96_021EF610 *)v2)->unk_2C != 0) {
            ov96_021EFACC(v2);
        } else if (ov96_021EF280(0, 3) != 0) {
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, 0xC, 1, *((enum HeapID *)&((UnkStruct_ov96_021EF610 *)v2)->unk_C));
            ManagedSprite_SetAnimateFlag(*((ManagedSprite **)&((UnkStruct_ov96_021EF610 *)v2)->unk_40), 1);
            if (*((s32 *)&((UnkStruct_ov96_021EF610 *)v2)->unk_24) == 0) {
                flag = 1;
            } else {
                flag = 0;
            }
            ManagedSprite_SetDrawFlag(*((ManagedSprite **)&((UnkStruct_ov96_021EF610 *)v2)->unk_44), flag);
            *arg1 += 1;
        }
        break;
    case 4:
        if (IsPaletteFadeFinished() != 0) {
            if (*((s32 *)&((UnkStruct_ov96_021EF610 *)v2)->unk_24) != 0) {
                ((UnkStruct_ov96_021EF610 *)v2)->unk_1C = 0;
                *arg1 += 1;
            } else {
                PokeathlonCourse_SetStateField07(data, 1U);
            }
        }
        break;
    case 5:
        v1 = ((UnkStruct_ov96_021EF610 *)v2)->unk_1C + 1;
        ((UnkStruct_ov96_021EF610 *)v2)->unk_1C = v1;
        if (v1 >= 0xB4) {
            PokeathlonCourse_SetStateField07(data, 4U);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

s32 ov96_021EFB20(PokeathlonCourseData *, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
BOOL ov96_021EFB50(PokeathlonCourseData *);
#else
void *ov96_021EFB50(PokeathlonCourseData *);
#endif

s32 ov96_021EFD08(PokeathlonCourseData *);

extern TouchscreenHitbox ov96_0221BA20;

BOOL ov96_021EFEB8(PokeathlonCourseData *arg0, u8 *arg1);

BOOL ov96_021EFEB8(PokeathlonCourseData *data, u8 *arg1) {
    s32 v1;
    u8 v0;

    ov96_021EF924(PokeathlonCourse_GetHeapAllocPtr4(data));
    v0 = *arg1;
    switch (v0) {
    case 0:
        if (IsPaletteFadeFinished() != 0) {
            v1 = TouchscreenHitbox_FindRectAtTouchNew(&ov96_0221BA20);
            if (v1 != -1) {
                switch (v1) {
                case 0:
                    PlaySE(0x5DCU);
                    ov96_021EFB20(data, 2);
                    break;
                case 1:
                    PokeathlonCourse_SetStateField07(data, 4U);
                    break;
                default:
                    GF_AssertFail();
                    break;
                }
                *arg1 += 1;
            }
        }
        break;
    case 1:
        if (ov96_021EFB50(data) != 0) {
            ov96_021EFD08(data);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

extern TouchscreenHitbox ov96_0221BA2C;

BOOL ov96_021EFF3C(PokeathlonCourseData *arg0, u8 *arg1);

BOOL ov96_021EFF3C(PokeathlonCourseData *data, u8 *arg1) {
    s32 v1;
    u8 v0;
    void *v2;

    v2 = PokeathlonCourse_GetHeapAllocPtr4(data);
    ov96_021EF924(v2);
    v0 = *arg1;
    switch (v0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, 4, 1, *((enum HeapID *)&((UnkStruct_ov96_021EF610 *)v2)->unk_C));
        *arg1 += 1;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            v1 = TouchscreenHitbox_FindRectAtTouchNew(&ov96_0221BA2C);
            if (v1 == 0) {
                PlaySE(0x5DCU);
                ov96_021EFB20(data, 3);
                *arg1 += 1;
            } else if (v1 == 1) {
                PlaySE(0x5DCU);
                ov96_021EFB20(data, 1);
                *arg1 += 1;
            }
        }
        break;
    case 2:
        if (ov96_021EFB50(data) != 0) {
            ov96_021EFD08(data);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

extern TouchscreenHitbox ov96_0221BA38;

BOOL ov96_021EFFE4(PokeathlonCourseData *arg0, u8 *arg1);

BOOL ov96_021EFFE4(PokeathlonCourseData *data, u8 *arg1) {
    s32 v1;
    u8 v0;
    void *v2;

    v2 = PokeathlonCourse_GetHeapAllocPtr4(data);
    ov96_021EF924(v2);
    v0 = *arg1;
    switch (v0) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0x7FFFU, 4, 1, *((enum HeapID *)&((UnkStruct_ov96_021EF610 *)v2)->unk_C));
        *arg1 += 1;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            v1 = TouchscreenHitbox_FindRectAtTouchNew(&ov96_0221BA38);
            if (v1 == 0) {
                PlaySE(0x5DCU);
                ov96_021EFB20(data, 2);
                *arg1 += 1;
            } else if (v1 == 1) {
                PlaySE(0x5DCU);
                ov96_021EFB20(data, 1);
                *arg1 += 1;
            }
        }
        break;
    case 2:
        if (ov96_021EFB50(data) != 0) {
            ov96_021EFD08(data);
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return 0;
}

BOOL ov96_021F008C(PokeathlonCourseData *data, u8 *arg1) {
    u8 v0;
    void *v1;

    v1 = PokeathlonCourse_GetHeapAllocPtr4(data);
    ov96_021EF924(v1);
    v0 = *arg1;
    switch (v0) {
    case 0:
        if (PokeathlonCourse_GetCurrentParticipantIndex(data) == 0) {
            PlaySE(0x8E4U);
        } else {
            PlaySE(0x8D6U);
        }
        GF_SndStartFadeOutBGM(0U, 0x18U);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7FFFU, 0x18, 1, *((enum HeapID *)&((UnkStruct_ov96_021EF610 *)v1)->unk_C));
        *arg1 += 1;
    default:
        break;
    case 1:
        if (IsPaletteFadeFinished() != 0) {
            return 1;
        }
        break;
    }
    return 0;
}
