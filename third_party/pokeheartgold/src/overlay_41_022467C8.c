#include "global.h"

#include "gf_gfx_planes.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "pm_string.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "unk_struct_ov66_022342_f4.h"
#include "pokepic.h"
#include "ppw_lobby.h"

void ov41_022467C8(void);

struct PPW_LobbyVipRecord;
struct PokepicManager;
struct GF_2DGfxRawResMan;
struct SoftwareSpriteChars;
struct SoftwareSpriteCharsTemplate;
struct SoftwareSpriteManager;
struct SoftwareSpriteManagerTemplate;
struct SoftwareSpritePalette;
struct SoftwareSpritePaletteTemplate;
struct Sprite;
struct SpriteList;
struct SpriteResourceCollection;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef struct SoftwareSpriteManagerTemplate SoftwareSpriteManagerTemplate;
struct SoftwareSpriteManagerTemplate {
    int numSprites;
    int numChars;
    int numPalettes;
    enum HeapID heapID;
};
typedef struct SoftwareSpriteCharsTemplate SoftwareSpriteCharsTemplate;
typedef struct SoftwareSpritePaletteTemplate SoftwareSpritePaletteTemplate;
SoftwareSpriteManager *sub_02015DDC(const SoftwareSpriteManagerTemplate *template);
void sub_02015E20(void *ptr);
void sub_02015E64(Sprite *gfxElem);
typedef struct ChooseStarterRotation {
    fx32 verticalOffset;
    fx32 verticalOffsetMultiplier;
    int frameCountMax;
    int frameCount;
} ChooseStarterRotation;
typedef struct ChooseStarterCursor {
    Sprite *sprite;
    SpriteResource *resources[6];
    VecFx32 position;
    SysTask *movementTask;
    ChooseStarterRotation starterRotation;
} ChooseStarterCursor;
struct SoftwareSpriteCharsTemplate {
    SoftwareSpriteManager *softSpriteMan;
    NNSG2dCharacterData *charsData;
};
SoftwareSpriteChars *sub_02015EA0(const SoftwareSpriteCharsTemplate *template);
struct SoftwareSpritePaletteTemplate {
    SoftwareSpriteManager *softSpriteMan;
    NNSG2dPaletteData *paletteData;
    u32 paletteSlot;
};
SoftwareSpritePalette *sub_02015F1C(const SoftwareSpritePaletteTemplate *template);
void ov41_022467E4(UnkStruct_ov22_0225A0E4 *param0, const SoftwareSpriteManagerTemplate *param1);
void ov41_02246820(UnkStruct_ov66_022342F4 *param0);
void ov41_02246830(ChooseStarterCursor *cursor);
void ov41_0224683C(UnkStruct_ov22_0225A0E4 *param0, SoftwareSpriteCharsTemplate *param1, int param2);
void ov41_0224689C(UnkStruct_ov22_0225A0E4 *param0, SoftwareSpritePaletteTemplate *param1, int param2);

void ov41_022467C8(void) {
    NNS_GfdResetFrmTexVramState();
    NNS_GfdResetFrmPlttVramState();
}

void ov41_022467D4(void);

void ov41_022467D4(void) {
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    NNS_G2dInitOamManagerModule();
}

void ov41_022467E4(UnkStruct_ov22_0225A0E4 *param0, const SoftwareSpriteManagerTemplate *param1) {
    param0->unk_00 = sub_02015DDC(param1);
    param0->unk_04 = Heap_Alloc(param1->heapID, sizeof(SoftwareSpriteChars *) * (100 + 18));
    param0->unk_08 = (100 + 18);
    param0->unk_0C = 0;
    param0->unk_10 = Heap_Alloc(param1->heapID, sizeof(SoftwareSpritePalette *) * (1 + 18));
    param0->unk_14 = (1 + 18);
    param0->unk_18 = 0;
    param0->unk_1C = 1;
}

void ov41_02246820(UnkStruct_ov66_022342F4 *param0) {
    sub_02015E20(param0->lobbyVipRecord);
    param0->lobbyVipRecord = 0L;
}

void ov41_02246830(ChooseStarterCursor *cursor) {
    sub_02015E64(cursor->sprite);
}

void ov41_0224683C(UnkStruct_ov22_0225A0E4 *param0, SoftwareSpriteCharsTemplate *param1, int param2) {
    int v0;

    for (v0 = 0; v0 < param2; v0++) {
        ((param0->unk_0C < param0->unk_08) ? (void)0 : GF_AssertFail());

        if (param1[v0].charsData != 0L) {
            param0->unk_04[param0->unk_0C] = sub_02015EA0(param1 + v0);
        } else {
            param0->unk_04[param0->unk_0C] = 0L;
        }

        param0->unk_0C++;
    }
}

void ov41_0224689C(UnkStruct_ov22_0225A0E4 *param0, SoftwareSpritePaletteTemplate *param1, int param2) {
    int v0;

    for (v0 = 0; v0 < param2; v0++) {
        ((param0->unk_18 < param0->unk_14) ? (void)0 : GF_AssertFail());

        if (param1[v0].paletteData != 0L) {
            param0->unk_10[param0->unk_18] = sub_02015F1C(param1 + v0);
        } else {
            param0->unk_10[param0->unk_18] = 0L;
        }

        param0->unk_18++;
    }
}
