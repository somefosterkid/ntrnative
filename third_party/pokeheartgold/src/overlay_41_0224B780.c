#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "bg_window.h"
#include "gf_3d_loader.h"
#include "filesystem.h"
#include "pokepic.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_41_unk_struct_ov22_02257964.h"
#include "overlay_41_unk_struct_ov22_0225b1bc.h"
#include "overlay_41_unk_struct_ov22_0225b4e4.h"

struct BgConfig;
struct NARC;
struct PhotoAccessory;
struct PhotoPokemon;
struct Pokemon;
struct Pokepic;
struct PokepicManager;
struct PokepicDrawParam;
struct GF_2DGfxRawResMan;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SpriteList;
struct SpriteResourceCollection;
typedef struct PhotoAccessory PhotoAccessory;
typedef struct PhotoPokemon PhotoPokemon;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
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
typedef struct {
    SoftwareSpriteManager *unk_00;
    SoftwareSpriteChars **unk_04;
    SoftwareSpritePalette **unk_08;
    u8 *unk_0C;
    PokepicManager *unk_10;
    BgConfig *unk_14;
    UnkStruct_ov22_02254DE0 *unk_18;
    UnkStruct_ov22_02259484 *unk_1C;
    int unk_20;
} UnkStruct_ov22_0225B388;
void ov41_02247F3C(UnkStruct_ov22_02257964 *param0, UnkStruct_ov22_0225B388 *param1);
void ov41_02247FAC(UnkStruct_ov22_02257964 *param0, Pokemon *param1, PokepicTemplate *param2, int param3, int param4, int param5, int param6);
BOOL ov41_02248044(UnkStruct_ov22_02257964 *param0, int param1, int param2, int param3, int param4);
s8 sub_0202BEDC(const PhotoPokemon *photoMon);
u8 sub_0202BEE4(const PhotoPokemon *photoMon);
u8 sub_0202BEEC(const PhotoPokemon *photoMon);
u32 sub_0202BEFC(const PhotoAccessory *accessory);
u32 sub_0202BF00(const PhotoAccessory *accessory);
u8 sub_0202BF04(const PhotoAccessory *accessory);
s8 sub_0202BF08(const PhotoAccessory *accessory);
void ov41_0224825C(UnkStruct_ov22_02257964 *param0, int param1, enum HeapID heapID);

void ov41_0224B780(UnkStruct_ov22_0225B1BC *param0, const UnkStruct_ov22_0225B4E4 *param1);
void ov41_0224B848(UnkStruct_ov22_0225B1BC *param0, const UnkStruct_ov22_0225B4E4 *param1);

void ov41_0224B780(UnkStruct_ov22_0225B1BC *param0, const UnkStruct_ov22_0225B4E4 *param1) {
    UnkStruct_ov22_0225B388 v0;
    int xPos, yPos, priority;

    memset(&v0, 0, sizeof(UnkStruct_ov22_0225B388));

    v0.unk_00 = param0->unk_00.unk_00;
    v0.unk_04 = param0->unk_00.unk_04;
    v0.unk_08 = param0->unk_00.unk_10;
    v0.unk_0C = param0->unk_00.unk_30;
    v0.unk_10 = param0->unk_00.unk_20;
    v0.unk_14 = param0->unk_00.unk_40;
    v0.unk_18 = param0->unk_18C;
    v0.unk_1C = &param0->unk_184;
    v0.unk_20 = 21;

    ov41_02247F3C(&param0->unk_190, &v0);

    PokepicTemplate v4;

    xPos = sub_0202BEE4(param1->photoMon);
    yPos = sub_0202BEEC(param1->photoMon);
    priority = sub_0202BEDC(param1->photoMon);

    ov41_02247FAC(&param0->unk_190, param0->unk_234, &v4, xPos, yPos, priority, param1->heapID);

    {
        int v5;
        int v6, v7;
        int v8;
        s8 v9;

        for (v5 = 0; v5 < param1->unk_54; v5++) {
            v8 = sub_0202BEFC(param1->unk_04[v5]);
            v6 = sub_0202BF00(param1->unk_04[v5]);
            v7 = sub_0202BF04(param1->unk_04[v5]);
            v9 = sub_0202BF08(param1->unk_04[v5]);

            ov41_02248044(&param0->unk_190, v8, v6, v7, v9);
        }
    }
}

void ov41_0224B848(UnkStruct_ov22_0225B1BC *param0, const UnkStruct_ov22_0225B4E4 *param1) {
    ov41_0224825C(&param0->unk_190, param1->unk_5C, param1->heapID);
}
