#include "global.h"

#include "overlay_45_wifi_plaza_api.h"
#include "constants/heap.h"

#include "screen_fade.h"
#include "system.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_49_unk_struct_ov70_02263910.h"
#include "bg_window.h"
#include "pm_string.h"
#include "filesystem.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "list_menu.h"
#include "overlay_manager.h"
#include "overlay_45_wifi_plaza_system.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "screen_fade_internal.h"
#include "touch_hitbox_controller.h"
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

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct ListMenuCursor;

struct G2dRenderer;

struct G3DPipelineBuffers;

struct ListMenu;

struct ListMenuTemplate;

struct MsgData;

struct NARC;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct TouchHitboxController;

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

void TextFlags_SetCanABSpeedUpPrint(BOOL val);

void TextFlags_SetAutoScrollParam(int flags);

void TextFlags_SetCanTouchSpeedUpPrint(BOOL val);

typedef struct ListMenuCursor ListMenuCursor;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void *OverlayManager_CreateAndGetData(OverlayManager *appMan, u32 size, enum HeapID heapID);

void *OverlayManager_GetArgs(OverlayManager *appMan);


u32 ov45_0222A53C(const WiFiPlazaSystem *param0);


BOOL ov45_0222AADC(const WiFiPlazaProfile *profile);

typedef struct {
    SaveData *saveData;
    int unk_04;
    int unk_08;
    UnkStruct_ov70_02263910 unk_0C;
    int unk_18;
    WiFiPlazaSystem *unk_1C;
} UnkStruct_ov66_02230E68;

UnkStruct_ov70_0225C894 *ov49_02258958(enum HeapID heapID);

#if defined(SDK_PORT) || defined(NONMATCHING)
UnkStruct_ov70_0225CA20 *ov49_02258AB4(u32 param0, s16 param1, const UnkStruct_ov70_0225C894 *param2, u32 heapID, u32 heapID2);
#else
UnkStruct_ov70_0225CA20 *ov49_02258AB4(u32 param0, u32 param1, const UnkStruct_ov70_0225C894 *param2, u32 heapID, u32 heapID2);
#endif

UnkStruct_ov70_02260AD4 *ov49_0225CB78(u32 heapID);

#if defined(SDK_PORT) || defined(NONMATCHING)
UnkStruct_ov70_02261E10 *ov49_0225DF18(s8 param0, s8 param1, const UnkStruct_ov70_0225C894 *param2, enum HeapID heapID, u32 heapID2);
#else
UnkStruct_ov70_02261E10 *ov49_0225DF18(int param0, int param1, const UnkStruct_ov70_0225C894 *param2, enum HeapID heapID, u32 heapID2);
#endif

UnkStruct_ov70_022630A4 *ov49_0225EEAC(UnkStruct_ov70_0225DEE8 *param0, enum HeapID heapID);

void ov49_0225F1A8(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225F1F0(UnkStruct_ov70_0225DEE8 *param0);

typedef struct UnkStruct_ov70_02269204_t UnkStruct_ov70_02269204;

UnkStruct_ov70_02269204 *ov49_022652E8(WiFiPlazaSystem *param0, UnkStruct_ov70_0225C894 *param1, UnkStruct_ov70_02260AD4 *param2, UnkStruct_ov70_0225CA20 *param3, enum HeapID heapID, u32 heapID2);

UnkStruct_ov70_0226C6F8 *ov49_02268764(u32 heapID, UnkStruct_ov70_0225DEE8 *param1);

typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;

enum VramManagerType {
    VRAM_MANAGER_TYPE_LINKED_LIST = 0,
    VRAM_MANAGER_TYPE_FRAME,
};

enum TextureVramSize {
    TEXTURE_VRAM_SIZE_128K = 1,
    TEXTURE_VRAM_SIZE_256K,
    TEXTURE_VRAM_SIZE_384K,
    TEXTURE_VRAM_SIZE_512K,
};

enum PaletteVramSize {
    PALETTE_VRAM_SIZE_16K = 1,
    PALETTE_VRAM_SIZE_32K,
    PALETTE_VRAM_SIZE_48K,
    PALETTE_VRAM_SIZE_64K,
    PALETTE_VRAM_SIZE_80K,
    PALETTE_VRAM_SIZE_96K,
};

typedef void (*G3DPipelineSetupCallback)(void);

G3DPipelineBuffers *GF_3DVramMan_Create(enum HeapID heapID, enum VramManagerType texVramManagerType, enum TextureVramSize texVramSize, enum VramManagerType plttVramManagerType, enum PaletteVramSize plttVramSize, G3DPipelineSetupCallback cb);

static inline G3DPipelineBuffers *G3DPipeline_Init(enum HeapID heapID, enum TextureVramSize texVramSize, enum PaletteVramSize plttVramSize, G3DPipelineSetupCallback cb) {
    return GF_3DVramMan_Create(heapID, VRAM_MANAGER_TYPE_LINKED_LIST, texVramSize, VRAM_MANAGER_TYPE_LINKED_LIST, plttVramSize, cb);
}

BOOL Heap_Create(enum HeapID parent, enum HeapID child, u32 size);

PlayerProfile *Save_PlayerData_GetProfile(SaveData *saveData);

void Main_SetVBlankIntrCB(Callback cb, void *data);

void HBlankInterruptDisable(void);

void ov49_0225A5C8(void *param0);

void ov49_0225A5EC(UnkStruct_ov70_0225E4EC *param0, SaveData *saveData, u32 heapID);

void ov49_0225AAC8(UnkStruct_ov70_0225E9C8 *param0, UnkStruct_ov70_0225E4EC *param1, SaveData *saveData, u32 heapID);

void ov49_0225ACA8(UnkStruct_ov70_0225E9C8 *param0, UnkStruct_ov70_0225E4EC *param1, SaveData *saveData, u32 heapID);

void ov49_0225AD20(UnkStruct_ov70_0225EC20 *param0, UnkStruct_ov70_0225E4EC *param1, u32 heapID);

void ov49_0225B0D4(UnkStruct_ov70_0225EFD4 *param0, UnkStruct_ov70_0225E4EC *param1, u32 heapID);

void ov49_0225B198(UnkStruct_ov70_0225F098 *param0, UnkStruct_ov70_0225F208 *param1, u32 heapID);

void ov49_0225B214(UnkStruct_ov70_0225F114 *param0, UnkStruct_ov70_0225E4EC *param1, u32 heapID);

void ov49_0225B308(UnkStruct_ov70_0225F208 *param0, u32 heapID);

void ov49_0225B450(UnkStruct_ov70_0225F350 *param0, const UnkStruct_ov70_02263910 *param1, UnkStruct_ov70_0225E4EC *param2, PlayerProfile *param3, u32 heapID);

void ov49_0225B9AC(UnkStruct_ov70_0225F350 *param0, UnkStruct_ov70_0225E4EC *param1, UnkStruct_ov70_0225F208 *param2, u32 heapID, const WiFiPlazaProfile *param4);

void ov70_0225E27C(UnkStruct_ov70_0225DEE8 *param0, enum PlazaMinigame param1, u32 param2);

void ov70_0225F2D8(UnkStruct_ov70_0225F208 *param0, enum PlazaMinigame param1, u32 param2);

void ov49_0225CB50(u32 param0, enum TouchScreenButtonState param1, void *param2);

int ov49_02259AA4(OverlayManager *appMan, int *param1);

int ov49_02259AA4(OverlayManager *appMan, int *param1) {
    UnkStruct_ov70_0225DEE8 *v0;
    UnkStruct_ov66_02230E68 *v1 = OverlayManager_GetArgs(appMan);

    Heap_Create(HEAP_ID_3, HEAP_ID_119, 0x3a000);
    Heap_Create(HEAP_ID_3, (enum HeapID)120, 0x3d000);

    v0 = OverlayManager_CreateAndGetData(appMan, sizeof(UnkStruct_ov70_0225DEE8), HEAP_ID_119);
    memset(v0, 0, sizeof(UnkStruct_ov70_0225DEE8));

    v0->unk_38 = &v1->unk_0C;
    v0->unk_34 = v1->unk_1C;
    v0->unk_02 = ov45_0222A53C(v0->unk_34);

    ((ov45_0222A53C(v0->unk_34) != 0xffffffff) ? (void)0 : GF_AssertFail());

    TextFlags_SetCanABSpeedUpPrint(1);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(0);

    v0->unk_458 = Save_PlayerData_GetProfile(v1->saveData);
    ov49_0225A5EC(&v0->unk_3C, v1->saveData, HEAP_ID_119);
    v0->unk_44C = ov49_02258958(HEAP_ID_119);

    {
        u32 v2;

        v2 = PlayerProfile_GetTrainerGender(v0->unk_458);
        v0->unk_444 = ov49_02258AB4(24, v2, v0->unk_44C, HEAP_ID_119, (enum HeapID)120);
    }

    v0->unk_448 = ov49_0225DF18(v1->unk_04, v1->unk_08, v0->unk_44C, HEAP_ID_119, (enum HeapID)120);
    v0->unk_450 = ov49_0225CB78(HEAP_ID_119);

    ov49_0225AAC8(&v0->unk_35C, &v0->unk_3C, v1->saveData, HEAP_ID_119);
    ov49_0225ACA8(&v0->unk_37C, &v0->unk_3C, v1->saveData, HEAP_ID_119);
    ov49_0225AD20(&v0->unk_39C, &v0->unk_3C, HEAP_ID_119);
    ov49_0225B214(&v0->unk_3F4, &v0->unk_3C, HEAP_ID_119);
    ov49_0225B0D4(&v0->unk_428, &v0->unk_3C, HEAP_ID_119);
    ov49_0225B308(&v0->unk_340, HEAP_ID_119);
    ov49_0225B198(&v0->unk_404, &v0->unk_340, HEAP_ID_119);
    ov49_0225B450(&v0->unk_1E8, v0->unk_38, &v0->unk_3C, v0->unk_458, HEAP_ID_119);

    {
        const WiFiPlazaProfile *v3;

        v3 = ov45_0222A5C0(v0->unk_34);

        if (ov45_0222AADC(v3) == 1) {
            ov49_0225B9AC(&v0->unk_1E8, &v0->unk_3C, &v0->unk_340, HEAP_ID_119, v3);
        }
    }

    v0->unk_454 = ov49_0225EEAC(v0, HEAP_ID_119);

    ov49_0225F1A8(v0);
    ov49_0225F1F0(v0);

    v0->unk_438 = ov49_022652E8(v0->unk_34, v0->unk_44C, v0->unk_450, v0->unk_444, HEAP_ID_119, (enum HeapID)120);
    v0->unk_440 = ov49_02268764(HEAP_ID_119, v0);

    Main_SetVBlankIntrCB(ov49_0225A5C8, v0);
    HBlankInterruptDisable();

    return 1;
}

void ov49_0225A038(UnkStruct_ov70_0225DEE8 *param0, BOOL param1);

typedef enum PPW_LOBBY_ERROR {
    PPW_LOBBY_ERROR_NONE,
    PPW_LOBBY_ERROR_UNKNOWN,
    PPW_LOBBY_ERROR_ALLOC,
    PPW_LOBBY_ERROR_SESSION,
    PPW_LOBBY_ERROR_STATS_SCHEDULE,
    PPW_LOBBY_ERROR_STATS_VIP,
    PPW_LOBBY_ERROR_STATS_CHECKPROFILE,
    PPW_LOBBY_ERROR_STATS_QUESTIONNAIRE,
    PPW_LOBBY_ERROR_MAX
} PPW_LOBBY_ERROR;

int ov45_0222A1FC(const WiFiPlazaSystem *param0);

BOOL ov45_0222A33C(const WiFiPlazaSystem *param0);

BOOL ov45_0222D844(void);

PPW_LOBBY_ERROR ov45_0222E7CC(void);

BOOL sub_020393C8(void);

BOOL sub_020397FC(void);

void ov49_0225A98C(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225AA2C(UnkStruct_ov70_0225DEE8 *param0);

void ov49_0225AB44(UnkStruct_ov70_0225E9C8 *param0, const String *param1);

BOOL ov49_0225AC5C(const UnkStruct_ov70_0225E9C8 *param0);

void ov49_0225AC74(UnkStruct_ov70_0225E9C8 *param0);

void ov49_0225B014(UnkStruct_ov70_0225EC20 *param0, u16 *param1, u16 *param2);

void ov49_0225B124(UnkStruct_ov70_0225EFD4 *param0);

void ov49_0225B200(UnkStruct_ov70_0225F098 *param0);

void ov49_0225B284(UnkStruct_ov70_0225F114 *param0, UnkStruct_ov70_0225F208 *param1);

void ov49_0225B2C0(UnkStruct_ov70_0225F114 *param0, UnkStruct_ov70_0225F208 *param1, int param2);

void ov49_0225B2F0(UnkStruct_ov70_0225F114 *param0, UnkStruct_ov70_0225F208 *param1, int param2);

String *ov49_0225B388(UnkStruct_ov70_0225F208 *param0, int param1, u32 param2);

u8 ov49_0225B898(const UnkStruct_ov70_0225F350 *param0);

int ov49_02259C90(OverlayManager *appMan, int *param1);

int ov49_02259C90(OverlayManager *appMan, int *param1) {
    UnkStruct_ov70_0225DEE8 *v0;
    UnkStruct_ov66_02230E68 *v1;
    BOOL v2;

    v0 = OverlayManager_GetData(appMan);
    v1 = OverlayManager_GetArgs(appMan);

    switch (*param1) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, (((0) << 10) | ((0) << 5) | (0)), 6, 1, HEAP_ID_119);
        v0->unk_03 = 1;
        (*param1)++;
        break;
    case 1:
        v2 = IsPaletteFadeFinished();

        if (v2 == 1) {
            v0->unk_03 = 0;
            (*param1)++;
        }
        break;
    case 2:
        if (v0->unk_04_0 == 1) {
            if ((ov45_0222D844() == 1) || (ov45_0222A1FC(v0->unk_34) != 0)) {
                v0->unk_04_4 = 1;
            }
        }

        if (v0->unk_07 == 0) {
            if (ov45_0222A33C(v0->unk_34) == 1) {
                v0->unk_06 = 1;
                ov49_0225A038(v0, 8);
            }
        }

        if ((v0->unk_00 == 1) || (v0->unk_04_4 == 1) || (v0->unk_06 == 1)) {
            if (v0->unk_04_4) {
                (*param1) = 3;
            } else if (v0->unk_06 == 1) {
                (*param1) = 5;
            } else {
                (*param1) = 7;
            }
        }
        break;
    case 3: {
        ov49_0225AC74(&v0->unk_35C);
        ov49_0225B014(&v0->unk_39C, 0L, 0L);
        ov49_0225B124(&v0->unk_428);
        ov49_0225B200(&v0->unk_404);

        if (sub_020393C8()) {
            ov49_0225B284(&v0->unk_3F4, &v0->unk_340);
        } else if (sub_020397FC()) {
            ov49_0225B2C0(&v0->unk_3F4, &v0->unk_340, ov45_0222E7CC());
        } else {
            ov49_0225B2F0(&v0->unk_3F4, &v0->unk_340, ov45_0222A1FC(v0->unk_34));
        }
        (*param1) = 4;
    } break;
    case 4:
        if (gSystem.newKeys & 0x0001) {
            (*param1) = 7;
        }
        break;
    case 5: {
        ov49_0225AC74(&v0->unk_35C);
        ov49_0225B014(&v0->unk_39C, 0L, 0L);
        ov49_0225B124(&v0->unk_428);
        ov49_0225B200(&v0->unk_404);
        ov49_0225AB44(&v0->unk_35C, ov49_0225B388(&v0->unk_340, 1, 70));
        (*param1) = 6;
        v0->unk_08 = 60;
    } break;
    case 6:
        if (ov49_0225AC5C(&v0->unk_35C)) {
            v0->unk_08--;

            if (v0->unk_08 <= 0) {
                (*param1) = 7;
            }
        }
        break;
    case 7: {
        u32 v3;

        v3 = ov49_0225B898(&v0->unk_1E8);

        if (v3 == 5) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((0) << 10) | ((0) << 5) | (0)), 6, 1, HEAP_ID_119);
            v0->unk_03 = 1;
            (*param1)++;
            break;
        }

        if (v3 == 0) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((0) << 10) | ((0) << 5) | (0)), 6, 1, HEAP_ID_119);
            v0->unk_03 = 1;
            (*param1)++;
            break;
        }
    } break;
    case 8:
        v2 = IsPaletteFadeFinished();

        if (v2 == 1) {
            v0->unk_03 = 0;
            return 1;
        }
        break;
    }

    ov49_0225A98C(v0);
    ov49_0225AA2C(v0);

    return 0;
}
