#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "filesystem_files_def.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_02257964.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_0225ab54.h"
#include "overlay_41_unk_struct_ov22_02258ee4.h"

typedef struct UnkStruct_ov41_02249390_v1 {
    u8 unk_0[0x4];
    void *unk_4;
    u8 unk_8[0x8];
    void *unk_10;
} UnkStruct_ov41_02249390_v1;

s32 ov41_0224825C(void *, s32, s32);

s32 ov41_022482A8(void *);

void ov41_02249390(void **arg0);

struct Pokepic;
struct PokepicDrawParam;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SysTask;
struct SysTaskQueue;
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
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
int ov41_0224895C(UnkStruct_ov22_02257F50 *param0, int param1);
void ov41_02249A90(UnkStruct_ov22_02259560 *param0, int param1);
void ov41_02249B44(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);
typedef struct UnkStruct_ov22_02258A48_t {
    void *unk_00;
    void (*unk_04)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_08)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_0C)(struct UnkStruct_ov22_02258A48_t *param0);
    void (*unk_10)(struct UnkStruct_ov22_02258A48_t *param0);
    s16 unk_14;
    s16 unk_16;
    u8 unk_18;
    u8 padding[3];
} UnkStruct_ov22_02258A48;
void ov41_02249B94(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);
void ov41_022493BC(UnkStruct_ov22_02258EE4 *param0, UnkStruct_ov22_02259560 *param1, int param2, int param3, int param4, int param5);
void ov41_02249418(UnkStruct_ov22_02258EE4 *param0);
void ov41_0224942C(UnkStruct_ov22_02258A48 *param0, int *param1, int *param2, int *param3, int *param4);

void ov41_02249390(void **arg0) {
    s32 *v2;
    void *v0;
    UnkStruct_ov41_02249390_v1 *v1;

    v1 = *arg0;
    v0 = v1->unk_10;
    if (v0 != NULL) {
        if (*(s32 *)((u8 *)v0 + 4) != 1) {
            GF_AssertFail();
        }
        v2 = *(s32 **)(v1->unk_10);
        ov41_022482A8(v1->unk_4);
        ov41_0224825C(v1->unk_4, *v2, 0xE);
    }
}

void ov41_022493BC(UnkStruct_ov22_02258EE4 *param0, UnkStruct_ov22_02259560 *param1, int param2, int param3, int param4, int param5) {
    int v0, v1;

    ov41_02249B44(param1, &v0, &v1);

    param0->unk_10 = param1;
    param0->unk_1C = v0;
    param0->unk_1E = v1;
    param0->unk_20 = param2;
    param0->unk_14 = param3;
    param0->unk_18 = param4;

    if (param2 == 0) {
        param0->unk_21 = ov41_0224895C(param0->unk_08, param0->unk_10->unk_04);
    } else {
        param0->unk_21 = 0;
    }

    param0->unk_30 = 1;

    ov41_02249A90(param1, param5);
}

void ov41_02249418(UnkStruct_ov22_02258EE4 *param0) {
    param0->unk_10 = 0L;
    param0->unk_1C = 0;
    param0->unk_1E = 0;
    param0->unk_20 = 0;
    param0->unk_21 = 0;
}

void ov41_0224942C(UnkStruct_ov22_02258A48 *param0, int *param1, int *param2, int *param3, int *param4) {
    UnkStruct_ov22_02258EE4 *v0 = param0->unk_00;
    int v1, v2;
    int v3, v4;

    ov41_02249B94(v0->unk_10, &v1, &v2);
    ov41_02249B44(v0->unk_10, &v3, &v4);

    *param1 = v4;
    *param2 = v4 + v2;
    *param3 = v3;
    *param4 = v3 + v1;
}
