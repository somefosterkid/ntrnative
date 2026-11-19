#include "global.h"
struct UnkStruct_ov22_0225A0E4;

#include "filesystem.h"
#include "heap.h"
#include "pokepic.h"
#include "unk_02026E30.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_02255cb8.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

s32 ov41_02246820(void *);

s32 ov41_02246A20(void *);

s32 ov41_02246A7C(void *);

s32 ov41_02246B34(void *);

s32 ov41_02246CB0(UnkStruct_ov22_0225A0E4 *param0);

s32 ov41_02246D2C(void *);

void ov41_022461D0(struct UnkStruct_ov22_0225A0E4 *arg0);

struct PokepicManager;
struct GF_2DGfxRawResMan;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SpriteList;
struct SpriteResourceCollection;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
void sub_02015EF4(SoftwareSpriteManager *softSpriteMan);
void sub_02015F64(SoftwareSpriteManager *softSpriteMan);
void ov41_0224626C(UnkStruct_ov22_0225A0E4 *param0);

void ov41_022461D0(UnkStruct_ov22_0225A0E4 *arg0) {
    ov41_02246CB0(arg0);
    ov41_02246820(arg0);
    ov41_02246A20(arg0);
    Heap_Free(*((void * *)&arg0->unk_40));
    ov41_02246D2C(arg0);
    ov41_02246B34(arg0);
    ov41_02246A7C(arg0);
    NARC_Delete(arg0->unk_5C);
    Heap_Free(*((void * *)&arg0->unk_04));
    *((void * *)&arg0->unk_04) = NULL;
    Heap_Free(*((void * *)&arg0->unk_10));
    *((void * *)&arg0->unk_10) = NULL;
}

s32 ov41_02246830(void *);

s32 ov41_02246B5C(void *);

void ov41_0224621C(struct UnkStruct_ov22_0225A0E4 *arg0);

void ov41_0224621C(UnkStruct_ov22_0225A0E4 *arg0) {
    Thunk_G3X_Reset();
    NNS_G2dSetupSoftwareSpriteCamera();
    if (*((s32 *)&arg0->unk_1C) != 0) {
        ov41_02246830(arg0);
    }
    if (*((s32 *)&arg0->unk_2C) != 0) {
        PokepicManager_DrawAll(*((PokepicManager * *)&arg0->unk_20));
    }
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    ov41_02246B5C(arg0);
}

s32 ov41_0224683C(void *, void *, s32);

s32 ov41_0224689C(void *, void *, s32);

void ov41_02246250(void *arg0, void *arg1);

void ov41_02246250(void *arg0, void *arg1) {
    ov41_0224683C(arg0, *(void **)arg1, *((s32 *)&((UnkStruct_ov22_02255CB8 *)arg1)->unk_04));
    ov41_0224689C(arg0, *((void **)&((UnkStruct_ov22_02255CB8 *)arg1)->unk_08), *((s32 *)&((UnkStruct_ov22_02255CB8 *)arg1)->unk_0C));
}

void ov41_0224626C(UnkStruct_ov22_0225A0E4 *param0) {
    sub_02015EF4(param0->unk_00);
    sub_02015F64(param0->unk_00);
}
