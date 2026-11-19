#include "global.h"

#include "constants/heap.h"
#include "player_data.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_49_unk_struct_ov70_02263910.h"
#include "bg_window.h"
#include "filesystem.h"
#include "list_menu_items.h"
#include "list_menu.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_unk_struct_ov70_0225_ec20.h"
#include "overlay_49_unk_struct_ov70_0225f098.h"
#include "overlay_49_unk_struct_ov70_0225f114.h"
#include "overlay_49_unk_struct_ov70_0225efd4.h"
#include "overlay_49_unk_struct_ov70_0225f208.h"
#include "overlay_49_unk_struct_ov70_02260744.h"
#include "overlay_49_unk_struct_ov70_0225fa84.h"
#include "overlay_49_unk_struct_ov70_0225f350.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"
#include "overlay_49_unk_struct_ov70_0226_c6f8.h"
#include "screen_fade.h"

typedef enum HeapID HeapID;

struct Background;

struct BgConfig;

struct ColoredArrow;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct ListMenu;

struct ListMenuTemplate;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct NARC;

struct GF_2DGfxRawResObj;

struct GF_2DGfxRawResMan;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct TouchScreenActions;

struct TouchScreenHitTableActionStateInternal;

struct PlayerProfile;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov70_0225C894_t;

struct UnkStruct_ov70_0225CA20_t;

struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_02260AD4_t;

struct UnkStruct_ov70_02261E10_t;

struct UnkStruct_ov70_022630A4_t;

struct UnkStruct_ov70_02269204_t;

struct UnkStruct_ov70_0226C6F8_t;

struct WiFiPlazaProfile;

struct Window;

typedef struct UnkStruct_ov70_0225C894_t UnkStruct_ov70_0225C894;

typedef struct UnkStruct_ov70_0225CA20_t UnkStruct_ov70_0225CA20;

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

typedef struct UnkStruct_ov70_02260AD4_t UnkStruct_ov70_02260AD4;

typedef struct UnkStruct_ov70_02261E10_t UnkStruct_ov70_02261E10;

typedef struct UnkStruct_ov70_022630A4_t UnkStruct_ov70_022630A4;

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

typedef u16 charcode_t;

typedef u32 TextColor;

typedef struct ColoredArrow {
    TextColor color;
    String *string;
} ColoredArrow;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;


typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct {
    u32 questionNumber;
    u32 selectedAnswer;
} UnkStruct_ov66_0222E908;

typedef struct {
    u16 unk_00[4];
} UnkStruct_ov66_02230914;

typedef struct {
    u8 pad_0[4];
    u8 unk_4;
    u8 unk_5;
    u16 unk_6;
    u32 unk_8;
} UnkStruct_ov66_02231428;

typedef struct WiFiPlazaProfile {
    s32 unk_00;
    u32 trainerID;
    charcode_t trainerName[7 + 1];
    UnkStruct_ov66_02231428 unk_18;
    UnkStruct_ov66_02231428 unk_1C;
    u16 partySpecies[6];
    u8 partyForms[6];
    u8 partyIsEgg[6];
    u8 trainerGender;
    u8 language;
    u16 trainerAppearanceIdx;
    u16 country;
    u8 region;
    u8 isNationalDexObtained;
    u8 isMainStoryCleared;
    u8 unk_41;
    u8 gameVersion;
    u8 unk_43;
    s64 startTimestamp;
    u8 unk_4C[12];
    s32 unk_58[12];
    u16 unk_88[2];
    UnkStruct_ov66_0222E908 unk_8C;
} WiFiPlazaProfile;

extern u32 ov45_0222A53C(const WiFiPlazaSystem *param0);

const WiFiPlazaProfile *ov45_0222A578(const WiFiPlazaSystem *param0, u32 param1);

WiFiPlazaProfile *ov45_0222A5C0(WiFiPlazaSystem *param0);

BOOL ov45_0222AB28(const WiFiPlazaSystem *param0, u32 param1);

BOOL ov45_0222AB48(const WiFiPlazaSystem *param0, u32 param1);

extern const UnkStruct_ov66_02230914 *ov45_0222AB58(const WiFiPlazaSystem *param0, u32 param1);

extern u32 ov45_0222ADA8(const WiFiPlazaSystem *param0, u32 param1);

extern void ov45_0222ADD8(WiFiPlazaSystem *param0, u32 param1);

void ov45_0222AE08(u32 param0, u32 *param1, u32 *param2);

extern void ov45_0222AE54(WiFiPlazaSystem *param0);

typedef struct UnkStruct_ov70_02269204_t UnkStruct_ov70_02269204;

BOOL ov49_02268968(const UnkStruct_ov70_0226C6F8 *param0, u32 param1, int param2);

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

void GfGfx_EngineBTogglePlanes(u8 layerMask, u8 enable);

enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

BOOL IsPaletteFadeFinished(void);

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

enum TouchScreenButtonState {
    TOUCH_BUTTON_NOT_PRESSED = -1,
    TOUCH_BUTTON_PRESSED,
    TOUCH_BUTTON_RELEASED,
    TOUCH_BUTTON_HELD,
    TOUCH_BUTTON_HELD_OUT_OF_BOUNDS
};

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchscreenHitbox;

typedef void (*TouchScreenHitTableAction)(u32, enum TouchScreenButtonState, void *);

typedef struct TouchScreenHitTableActionStateInternal TouchScreenHitTableActionStateInternal;

typedef struct TouchScreenActions {
    const TouchscreenHitbox *hitTables;
    u32 hitTablesLength;
    TouchScreenHitTableAction action;
    void *hitTableContext;
    TouchScreenHitTableActionStateInternal *actionStates;
} TouchScreenActions;

extern void ov49_0225B444(UnkStruct_ov70_0225DEE8 *param0);

extern void ov49_0225B944(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E9C8 *param1, UnkStruct_ov70_0225E4EC *param2, UnkStruct_ov70_0225F208 *param3, BOOL param4, u32 heapID, const WiFiPlazaProfile *param6, const WiFiPlazaProfile *param7, BOOL param8, BOOL param9, const UnkStruct_ov66_02230914 *param10, BOOL param11);

extern void ov49_0225B99C(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E9C8 *param1, UnkStruct_ov70_0225E4EC *param2);

extern void ov49_0225B9AC(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E4EC *param1, UnkStruct_ov70_0225F208 *param2, u32 heapID, const WiFiPlazaProfile *param4);

extern void ov49_0225B9F0(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E4EC *param1, u32 heapID, u32 param3);

extern void ov49_0225BA20(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E4EC *param1);

extern void ov49_0225BBCC(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E9C8 *param1);

extern BOOL ov49_0225C8D4(UnkStruct_ov70_02260744 *param0, UnkStruct_ov70_0225E4EC *param1, NARC *param2, u32 heapID);

void ov49_0225CB68(UnkStruct_ov70_02260744 *param0);

void ov70_0225E27C(UnkStruct_ov70_0225DEE8 *param0, enum PlazaMinigame param1, u32 param2);

void ov70_0225F2D8(UnkStruct_ov70_0225F208 *param0, enum PlazaMinigame param1, u32 param2);

void ov49_0225CB50(u32 param0, enum TouchScreenButtonState param1, void *param2);

void ov49_0225B518(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225DEE8 *param1, BOOL param2, u32 heapID);

void ov49_0225B518(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225DEE8 *param1, BOOL param2, u32 heapID) {
    WiFiPlazaSystem *v0;
    UnkStruct_ov70_0225E4EC *v1;
    UnkStruct_ov70_0225F208 *v2;
    BOOL v3;

    v0 = param1->unk_34;
    v1 = &param1->unk_3C;
    v2 = &param1->unk_340;

    switch (param0->unk_00) {
    case 0:
        if (param0->unk_06 == 1) {
            param0->unk_00++;
        }
        break;
    case 1:
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((31) << 10) | ((31) << 5) | (31)), (HeapID)(4), 1, (HeapID)(heapID));
        param0->unk_00++;
        break;
    case 2:
        if (IsPaletteFadeFinished() == 1) {
            {
                const WiFiPlazaProfile *v4;

                v4 = ov45_0222A5C0(v0);
                ov49_0225B9AC(param0, v1, &param1->unk_340, heapID, v4);
                param0->unk_01 = 4;
            }

            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, 1);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, 1);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 1);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, 1);
            param0->unk_00++;
        }
        break;
    case 3:
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((31) << 10) | ((31) << 5) | (31)), 6, 1, (HeapID)(heapID));
        param0->unk_00++;
        break;
    case 4:
        if (IsPaletteFadeFinished() == 1) {
            param0->unk_00++;
        }
        break;
    case 5:
        if (param2 == 1) {
            break;
        }

        switch (param0->unk_01) {
        case 1:
            v3 = ov49_0225C8D4(&param0->unk_114, v1, param0->unk_14C, heapID);

            if (v3 == 1) {
                ov49_0225B444(param1);
            }
            break;
        case 4:
            break;
        case 3:
            if (param0->unk_153) {
                if (ov49_02268968(param1->unk_440, param0->unk_150, param0->unk_152) == 0) {
                    param0->unk_153 = 0;
                    ov49_0225CB68(&param0->unk_114);
                }
            }

            v3 = ov49_0225C8D4(&param0->unk_114, v1, param0->unk_14C, heapID);

            if (v3 == 1) {
                ov45_0222ADD8(v0, ov45_0222A53C(v0));
                ov45_0222AE54(v0);

                {
                    u32 v5, v6, v7;

                    v5 = ov45_0222ADA8(v0, ov45_0222A53C(v0));
                    ov45_0222AE08(v5, &v6, &v7);

                    param0->unk_153 = 1;
                    param0->unk_150 = v6;
                    param0->unk_152 = v7;
                }
            }
            break;
        case 2:
            ov49_0225BBCC(&param0->unk_08, &param1->unk_37C);
            break;
        default:
            break;
        }

        if (param0->unk_02 != 0) {
            param0->unk_00++;
        }
        break;
    case 6:
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_OUT, (((0) << 10) | ((0) << 5) | (0)), 3, 1, (HeapID)(heapID));
        param0->unk_00++;
        break;
    case 7:
        if (IsPaletteFadeFinished() == 1) {
            switch (param0->unk_01) {
            case 1:
            case 4:
            case 3:
                ov49_0225BA20(param0, v1);
                break;
            case 2:
                ov49_0225B99C(param0, &param1->unk_37C, v1);
                break;
            }

            param0->unk_00++;
        }
        break;
    case 8: {
        const WiFiPlazaProfile *v8;
        BOOL v9;
        BOOL v10;
        const UnkStruct_ov66_02230914 *v11;

        switch (param0->unk_02) {
        case 1:
        case 4:
            v8 = ov45_0222A5C0(v0);
            ov49_0225B9AC(param0, v1, &param1->unk_340, heapID, v8);
            break;
        case 2: {
            BOOL v12;
            const WiFiPlazaProfile *v13;

            v13 = ov45_0222A5C0(v0);

            if (param0->unk_03 == ov45_0222A53C(v0)) {
                v8 = ov45_0222A5C0(v0);
                v12 = 1;
            } else {
                v8 = ov45_0222A578(v0, param0->unk_03);
                v12 = 0;
            }

            v9 = ov45_0222AB28(v0, param0->unk_03);

            if (param0->unk_04 == 1) {
                v10 = ov45_0222AB48(v0, param0->unk_03);
                v11 = ov45_0222AB58(v0, param0->unk_03);

                if (v11 == 0L) {
                    v10 = 0;
                }
            } else {
                v10 = 0;
                v11 = 0L;
            }

            ov49_0225B944(param0, &param1->unk_37C, v1, v2, v12, heapID, v8, v13, v9, v10, v11, 1);
        } break;
        case 3: {
            u32 v14;
            u32 v15;
            u32 v16;

            v14 = ov45_0222ADA8(v0, ov45_0222A53C(v0));

            if (v14 == 0xffffffff) {
                ((0) ? (void)0 : GF_AssertFail());

                v14 = 0;
            }

            ov45_0222AE08(v14, &v15, &v16);
            ov49_0225B9F0(param0, v1, heapID, v16);
        } break;
        }

        param0->unk_02 = 0;
        param0->unk_00++;
    } break;
    case 9:
        BeginNormalPaletteFade(FADE_SUB_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((0) << 10) | ((0) << 5) | (0)), 3, 1, (HeapID)(heapID));

        if (param0->unk_01 == 2) {
            ov49_0225BBCC(&param0->unk_08, &param1->unk_37C);
        }

        param0->unk_00++;
        break;
    case 10:
        if (IsPaletteFadeFinished() == 1) {
            param0->unk_00 = 5;
        }
        break;
    }
}
