#include "global.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "gf_3d_loader.h"
#include "overlay_41_unk_struct_ov22_02255cb8.h"
#include "bg_window.h"
#include "filesystem.h"
#include "pokepic.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

struct BgConfig;

struct NARC;

struct SoftwareSpriteChars;

struct SoftwareSpriteCharsTemplate;

struct SoftwareSpriteManager;

struct SoftwareSpriteManagerTemplate;

struct SoftwareSpritePalette;

struct SoftwareSpritePaletteTemplate;

typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;

typedef struct SoftwareSpriteManagerTemplate {
    int numSprites;
    int numChars;
    int numPalettes;
    enum HeapID heapID;
} SoftwareSpriteManagerTemplate;

typedef struct SoftwareSpriteCharsTemplate SoftwareSpriteCharsTemplate;

typedef struct SoftwareSpritePaletteTemplate SoftwareSpritePaletteTemplate;

BgConfig *BgConfig_Alloc(enum HeapID heapID);

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void ov41_022467E4(UnkStruct_ov22_0225A0E4 *param0, const SoftwareSpriteManagerTemplate *param1);

void ov41_02246CC0(UnkStruct_ov22_0225A0E4 *param0, enum HeapID heapID, int param2, int param3);

void ov41_02246C90(UnkStruct_ov22_0225A0E4 *param0, enum HeapID heapID);

void ov41_02246A94(UnkStruct_ov22_0225A0E4 *param0);

void ov41_02246A50(UnkStruct_ov22_0225A0E4 *param0);

void ov41_022468FC(UnkStruct_ov22_0225A0E4 *param0);

void ov22_02255410(UnkStruct_ov22_02255CB8 *param0, enum HeapID heapID);

void ov22_0225547C(UnkStruct_ov22_0225A0E4 *param0, const SoftwareSpriteManagerTemplate *param1, enum HeapID heapID);

void ov41_02246D54(UnkStruct_ov22_02255CB8 *param0, int param1, int param2, enum HeapID heapID);

void ov41_02246170(UnkStruct_ov22_0225A0E4 *param0);

void ov41_02246170(UnkStruct_ov22_0225A0E4 *param0) {
    SoftwareSpriteManagerTemplate v0 = {
        .numSprites = 718,
        .numChars = 118,
        .numPalettes = 19,
        .heapID = HEAP_ID_14
    };

    param0->unk_5C = NARC_New((enum NarcId)26, HEAP_ID_14);

    ov41_022467E4(param0, &v0);
    ov41_02246CC0(param0, HEAP_ID_14, 0x2800, 0x20);
    ov41_02246A50(param0);
    ov41_02246C90(param0, HEAP_ID_13);
    ov41_02246A94(param0);

    param0->unk_40 = BgConfig_Alloc(HEAP_ID_14);

    ov41_022468FC(param0);
}
