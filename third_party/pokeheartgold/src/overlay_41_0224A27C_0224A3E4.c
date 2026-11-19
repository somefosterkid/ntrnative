#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "touchscreen.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"

struct FontOAM;

struct PokemonSpriteManager;

struct ResourceCollection;

struct SoftwareSpriteChars;

struct SoftwareSpriteManager;

struct SoftwareSpritePalette;

struct SpriteList;

struct SpriteResourceCollection;

struct TouchScreenActions;

struct UnkStruct_02012744_t;

typedef struct SpriteList SpriteList;

typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;

struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
};

typedef struct UnkStruct_ov22_02259C58_tag {
    Sprite *unk_00;
    int unk_04;
    void *unk_08;
    void (*unk_0C)(struct UnkStruct_ov22_02259C58_tag *param0, void *param1);
} UnkStruct_ov22_02259C58;

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

typedef struct FontOAM FontOAM;

typedef struct {
    UnkStruct_ov22_02259C58 unk_00;
    FontOAM *unk_10;
    UnkStruct_02021AC8 unk_14;
} UnkStruct_ov22_0225A154;

typedef struct TouchScreenActions TouchScreenActions;

void TouchHitboxController_Destroy(TouchScreenActions *buttonAction);

typedef struct {
    UnkStruct_ov22_02259C58 unk_00[4];
    UnkStruct_ov22_0225A154 unk_40;
    BOOL unk_60;
    UnkStruct_02012744 *unk_64;
    TouchScreenActions *unk_68;
    TouchscreenHitbox unk_6C[5];
} UnkStruct_ov22_02259D2C;

typedef struct PokemonSpriteManager PokemonSpriteManager;

typedef struct ResourceCollection ResourceCollection;

typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;

typedef struct SpriteResourceCollection SpriteResourceCollection;

void ov41_0224A1A8(UnkStruct_ov22_02259C58 *param0);

void ov41_0224A1C0(UnkStruct_ov22_0225A154 *param0);

void ov41_02246388(UnkStruct_ov22_0225A0E4 *param0, int param1);

void ov41_0224639C(UnkStruct_ov22_0225A0E4 *param0, int param1);

void sub_020135AC(UnkStruct_02012744 *param0);

void ov41_0224A3E4(UnkStruct_ov22_02259D2C *param0, UnkStruct_ov22_0225A0E4 *param1);

void ov41_0224A3E4(UnkStruct_ov22_02259D2C *param0, UnkStruct_ov22_0225A0E4 *param1) {
    int v0;

    for (v0 = 0; v0 < 5; v0++) {
        ov41_02246388(param1, v0);
        ov41_0224639C(param1, v0);
        ObjCharTransfer_ResetTransferTasksByResID(v0);
    }

    ObjPlttTransfer_FreeTaskByID(0);
    ObjPlttTransfer_FreeTaskByID(1);

    for (v0 = 0; v0 < 4; v0++) {
        ov41_0224A1A8(&param0->unk_00[v0]);
    }

    sub_02021B5C(&param0->unk_40.unk_14);
    ov41_0224A1C0(&param0->unk_40);
    sub_020135AC(param0->unk_64);
    TouchHitboxController_Destroy(param0->unk_68);

    param0->unk_68 = 0L;
}
