#include "global.h"

#include "constants/heap.h"
#include "sprite_resources_header.h"
#include "overlay_43_palpad.h"
#include "overlay_43_palpad_fns.h"
#include "unk_0202C034.h"
#include "options.h"
#include "filesystem.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "naming_screen.h"
#include "list_menu.h"

struct ApplicationManager;

struct ApplicationManagerTemplate;

struct BgConfig;

struct ColoredArrow;

struct G2dRenderer;

struct ListMenu;

struct ListMenuTemplate;

struct Menu;

struct MessageLoader;

struct NARC;

struct NamingScreenArgs;

struct Options;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct SpriteResourcesHeader;

struct String;

struct StringList;

struct StringTemplate;

struct SysTask;

struct SysTaskManager;

struct System;

struct PlayerProfile;

struct UnkStruct_ov63_0222AE60_t;

struct UnkStruct_ov63_0222BB38_t;

struct WiFiList;

struct Window;

struct WindowTemplate;

typedef struct ApplicationManager ApplicationManager;

typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);

typedef struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} ApplicationManagerTemplate;

struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

typedef u16 charcode_t;



void Heap_Free(void *ptr);

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

typedef struct SysTaskManager SysTaskManager;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};


struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

typedef struct PlayerProfile TrainerInfo;



typedef struct StringTemplate StringTemplate;


typedef struct {
    int unk_00;
    int unk_04[5];
    int unk_18;
    String *unk_1C;
    Options *options;
    int unk_24;
    u32 unk_28;
    u32 unk_2C;
    u32 unk_30;
} UnkStruct_02089438;

typedef struct UnkStruct_ov63_0222AE60_t UnkStruct_ov63_0222AE60;

typedef struct UnkStruct_ov63_0222BB38_t UnkStruct_ov63_0222BB38;

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

typedef struct MessageLoader MessageLoader;

typedef struct ColoredArrow ColoredArrow;

typedef struct StringList StringList;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct Menu Menu;



SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

enum FadeType {
    FADE_TYPE_BRIGHTNESS_OUT = 0,
    FADE_TYPE_BRIGHTNESS_IN = 1,
    FADE_TYPE_DOWNWARD_OUT = 2,
    FADE_TYPE_DOWNWARD_IN = 3,
    FADE_TYPE_UPWARD_OUT = 4,
    FADE_TYPE_UNK_5 = 5,
    FADE_TYPE_UNK_6 = 6,
    FADE_TYPE_UNK_7 = 7,
    FADE_TYPE_UNK_8 = 8,
    FADE_TYPE_UNK_9 = 9,
    FADE_TYPE_UNK_10 = 10,
    FADE_TYPE_UNK_11 = 11,
    FADE_TYPE_UNK_12 = 12,
    FADE_TYPE_UNK_13 = 13,
    FADE_TYPE_UNK_14 = 14,
    FADE_TYPE_UNK_15 = 15,
    FADE_TYPE_CIRCLE_OUT = 16,
    FADE_TYPE_CIRCLE_IN = 17,
    FADE_TYPE_TOP_HALF_CIRCLE_OUT = 18,
    FADE_TYPE_TOP_HALF_CIRCLE_IN = 19,
    FADE_TYPE_UNK_20 = 20,
    FADE_TYPE_UNK_21 = 21,
    FADE_TYPE_UNK_22 = 22,
    FADE_TYPE_UNK_23 = 23,
    FADE_TYPE_UNK_24 = 24,
    FADE_TYPE_UNK_25 = 25,
    FADE_TYPE_UNK_26 = 26,
    FADE_TYPE_UNK_27 = 27,
    FADE_TYPE_UNK_28 = 28,
    FADE_TYPE_UNK_29 = 29,
    FADE_TYPE_UNK_30 = 30,
    FADE_TYPE_UNK_31 = 31,
    FADE_TYPE_UNK_32 = 32,
    FADE_TYPE_UNK_33 = 33,
    FADE_TYPE_UNK_34 = 34,
    FADE_TYPE_UNK_35 = 35,
    FADE_TYPE_UNK_36 = 36,
    FADE_TYPE_UNK_37 = 37,
    FADE_TYPE_UNK_38 = 38,
    FADE_TYPE_UNK_39 = 39,
    FADE_TYPE_CLAMP_OUT = 40,
    FADE_TYPE_CLAMP_IN = 41,
    FADE_TYPE_MAX = 42,
};

typedef void (*Callback)(void *);

typedef void (*UnkCallback)(void);

enum ButtonMode {
    BUTTON_MODE_NORMAL = 0,
    BUTTON_MODE_START_IS_X = 1,
    BUTTON_MODE_SWAP_XY = 2,
    BUTTON_MODE_L_IS_A = 3,
};



enum FadeMode {
    FADE_BOTH_SCREENS = 0,
    FADE_MAIN_THEN_SUB,
    FADE_SUB_THEN_MAIN,
    FADE_MAIN_ONLY,
    FADE_SUB_ONLY,
};

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

BOOL IsPaletteFadeFinished(void);

typedef struct {
    s16 unk_00;
    s16 unk_02;
    u8 unk_04;
    u8 unk_05;
} UnkStruct_ov64_0223221C;

typedef struct {
    void *unk_00[2];
    NNSG2dScreenData *unk_08[2];
} UnkStruct_ov64_022300E4;

typedef struct {
    String *unk_00;
    String *unk_04;
} UnkStruct_ov64_0222E060_sub1;

typedef struct {
    const u16 *unk_00;
    BOOL unk_04;
} UnkStruct_ov64_0222E060_sub2;

typedef struct {
    u32 unk_00;
    u8 unk_04[32];
} UnkStruct_ov64_022301B4;

typedef struct {
    UnkStruct_ov64_0223221C unk_00;
    Window unk_08;
    UnkStruct_ov63_0222BB38 *unk_18[8];
} UnkStruct_ov64_02230074;

typedef struct {
    Window unk_00;
    Window unk_10;
    void *unk_20;
    NNSG2dScreenData *unk_24;
} UnkStruct_ov64_02230620;

typedef struct {
    void *unk_00[7];
    NNSG2dScreenData *unk_1C[7];
} UnkStruct_ov64_02230F60;

typedef struct {
    UnkStruct_ov64_0223221C unk_00;
    u32 unk_08;
    Window *unk_0C[7];
    u32 unk_28[7];
} UnkStruct_ov64_02230F98;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_ov64_0223222E;

u32 ov43_0222A358(PalPadShared *param0, String *param1, String *param2);

void ov43_0222AAA4(PalPadGfx *param0, u64 param1);

void ov43_0222B1FC(PalPadMenu *param0, PalPadShared *param1, PalPadGfx *param2);

BOOL ov64_0222EE84(PalPadMenu *param0, PalPadShared *param1, PalPadGfx *param2);

void ov43_0222B440(PalPadMenu *param0, PalPadShared *param1);

void ov43_0222B458(PalPadMenu *param0, PalPadGfx *param1);

void ov43_0222B4BC(PalPadMenu *param0, PalPadGfx *param1, u32 param2, enum HeapID heapID);

BOOL ov64_0222F038(const PalPadMenu *param0);

void ov64_0222F050(PalPadMenu *param0);

BOOL ov43_0222B574(PalPadMenu *param0, PalPadShared *param1);

void ov64_0222DF48(PalPadWork *param0, enum HeapID heapID);

void ov64_0222E040(PalPadShared *param0, void *param1, enum HeapID heapID);

void ov64_0222E1A4(PalPadGfx *param0, const PalPadShared *param1, enum HeapID heapID);

void ov64_0222E3D8(PalPadGfx *param0, enum HeapID heapID);

void ov64_0222E5D8(PalPadGfx *param0, enum HeapID heapID);

void ov64_0222E620(PalPadGfx *param0, const PalPadShared *param1, enum HeapID heapID);

void ov43_0222AB20(PalPadGfx *param0, SaveData *saveData, u32 param2, enum HeapID heapID);

void ov43_0222AB5C(PalPadGfx *param0, SaveData *saveData, u32 param2, enum HeapID heapID);

void ov64_0222EA68(PalPadMenu *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

int ov64_0222F0C4(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222BB20(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

BOOL ov43_0222C024(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov64_0222FA70(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

BOOL ov64_0222FBA4(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov64_0222FC1C(PalPadRoster *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);


void ov43_0222C550(PalPadRoster *param0, PalPadGfx *param1, u32 param2, enum HeapID heapID);

void ov43_0222C918(UnkStruct_ov64_02230074 *param0, PalPadShared *param1, PalPadGfx *param2, UnkStruct_ov63_0222AE60 *param3, const UnkStruct_ov64_022300E4 *param4, const UnkStruct_ov64_022301B4 *param5, u32 param6, enum HeapID heapID);

void ov43_0222CA50(UnkStruct_ov64_02230074 *param0, UnkStruct_ov63_0222AE60 *param1, PalPadGfx *param2, const UnkStruct_ov64_022300E4 *param3, u32 param4, WiFiList *param5, u32 param6, enum HeapID heapID);

void ov64_02230444(PalPadNaming *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

int ov43_0222CBB4(PalPadNaming *param0, PalPadWork *param1, PalPadShared *param2, PalPadGfx *param3, enum HeapID heapID);

NamingScreenArgs *ov64_022305DC(PalPadNaming *param0, PalPadShared *param1, enum HeapID heapID);

void ov64_02230620(UnkStruct_ov64_02230620 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov64_02230680(UnkStruct_ov64_02230620 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov64_022308DC(PalPadDetails *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

int ov64_02230904(PalPadDetails *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222D584(PalPadDetails *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov64_02230BD8(PalPadDetails *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222DE14(UnkStruct_ov64_02230F98 *param0, PalPadGfx *param1, u32 param2, u32 param3, const UnkStruct_ov64_0223222E *param4, enum HeapID heapID);

void ov64_02231164(UnkStruct_ov64_02230F98 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov64_02231528(UnkStruct_ov64_02230F98 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222E220(UnkStruct_ov64_02230F98 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222E5B4(UnkStruct_ov64_02230F98 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222E78C(UnkStruct_ov64_02230F98 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov43_0222E904(UnkStruct_ov64_02230F98 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

void ov64_02231E94(UnkStruct_ov64_02230F98 *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

BOOL ov64_0223217C(PalPadShared *param0, enum HeapID heapID);

int ov43_0222AE64(PalPadMenu *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID);

BOOL System_GetTouchNew(void);

void ov43_0222B55C(PalPadMenu *);

void ov43_0222B408(PalPadMenu *, PalPadGfx *);


int ov43_0222AE2C(PalPadShared *param0, PalPadGfx *param1);

BOOL ov43_0222B374(PalPadMenu *param0, PalPadShared *param1, PalPadGfx *param2);

BOOL ov43_0222B534(PalPadMenu *param0, PalPadShared *param1);

int ov43_0222AE64(PalPadMenu *param0, PalPadShared *param1, PalPadGfx *param2, enum HeapID heapID) {
    BOOL v0;

    switch (param1->subState) {
    case 0:
        ov43_0222B1FC(param0, param1, param2);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_CIRCLE_IN, FADE_TYPE_CIRCLE_IN, (((0) << 11) | ((0) << 5) | (0)), 6, 1, heapID);
        param1->subState = 1;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            param1->subState = 3;
        }
        break;
    case 2:
        ov43_0222B1FC(param0, param1, param2);
        param1->subState = 3;
        break;
    case 3:
        v0 = ov43_0222B374(param0, param1, param2);

        if (v0 == 0) {
            break;
        }

        param0->timer = 2;
        param1->subState = 4;
        break;
    case 4:
        if (param0->timer-- == 0) {
            param0->timer = 0;

            if (ov43_0222B574(param0, param1) == 1) {
                ov43_0222B440(param0, param1);

                if ((param0->selection == 3) || (param0->selection == 1)) {
                    param1->subState = 5;
                } else {
                    ov43_0222B458(param0, param2);
                    return 1;
                }
            } else {
                if (param0->selection == 1) {
                    ov43_0222B4BC(param0, param2, 58, heapID);
                    param1->subState = 11;
                }
            }
        }
        break;
    case 5:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_CIRCLE_OUT, FADE_TYPE_CIRCLE_OUT, (((0) << 11) | ((0) << 5) | (0)), 6, 1, heapID);
        param1->subState++;
        break;
    case 6:
        if (IsPaletteFadeFinished() == 1) {
            return 1;
        }
        break;
    case 7:
        ov43_0222B1FC(param0, param1, param2);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_CIRCLE_IN, FADE_TYPE_CIRCLE_IN, (((0) << 11) | ((0) << 5) | (0)), 6, 1, heapID);
        param1->subState = 8;
        break;
    case 8:
        if (IsPaletteFadeFinished()) {
            u64 v1;
            TrainerInfo *v2;

            v2 = PlayerProfile_New(heapID);

            Save_Profile_PlayerName_Set(v2, String_cstr(param1->name));
            BufferPlayersName(param2->msgFormat, 0, v2);
            Heap_Free(v2);

            v1 = String_atoi(param1->code, &v0);

            if (v0) {
                ov43_0222AAA4(param2, v1);
                ov43_0222B4BC(param0, param2, 55, heapID);
                param1->subState = 9;
            } else {
                ov43_0222B4BC(param0, param2, 56, heapID);
                param1->subState = 11;
            }
        }
        break;
    case 9:
        if (ov43_0222B534(param0, param1) == 0) {
            break;
        }

        ov43_0222ADB8(param1, param2, 0);
        param1->subState = 10;
        break;
    case 10:
        switch (ov43_0222AE2C(param1, param2)) {
        case 1: {
            u32 v4;
            static const u8 v5[3] = {
                0,
                56,
                59,
            };

            v4 = ov43_0222A358(param1, param1->code, param1->name);

            if (v4 == 0) {
                ov43_0222B55C(param0);
                param1->subState = 3;
            } else {
                ov43_0222B4BC(param0, param2, v5[v4], heapID);
                param1->subState = 11;
            }
        } break;
        case 2:
            ov43_0222B55C(param0);
            param1->subState = 3;
            break;
        }
        break;
    case 11:
        if (ov43_0222B534(param0, param1) == 0) {
            break;
        }

        if ((gSystem.newKeys & 0x0001) || (gSystem.newKeys & 0x0002) || System_GetTouchNew()) {
            ov43_0222B55C(param0);
            ov43_0222B408(param0, param2);
            param1->subState = 3;
        }

        break;
    }
    return 0;
}
