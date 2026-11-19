#include "global.h"
struct UnkStruct_ov22_0225A0E4;

#include "heap.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "filesystem.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "gf_gfx_planes.h"
#include "gf_3d_loader.h"
#include "pokepic.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

s32 ov41_02246820(void *);

s32 ov41_02246CB0(UnkStruct_ov22_0225A0E4 *param0);

s32 ov41_02246D2C(void *);

void ov41_02246594(struct UnkStruct_ov22_0225A0E4 *arg0);

struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
void ov41_022465C0(UnkStruct_ov22_0225A0E4 *param0);

void ov41_02246594(UnkStruct_ov22_0225A0E4 *arg0) {
    ov41_02246CB0(arg0);
    ov41_02246820(arg0);
    ov41_02246D2C(arg0);
    Heap_Free(*((void * *)&arg0->unk_04));
    *((void * *)&arg0->unk_04) = NULL;
    Heap_Free(*((void * *)&arg0->unk_10));
    *((void * *)&arg0->unk_10) = NULL;
}

void ov41_022465C0(UnkStruct_ov22_0225A0E4 *param0) {
    FreeBgTilemapBuffer(param0->unk_40, GF_BG_LYR_MAIN_2);
}
