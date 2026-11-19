#include "global.h"
#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "filesystem_files_def.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "overlay_41_unk_struct_ov22_02257964.h"

s32 ov41_02249BE8(void *, s32, s32);

void ov41_02248120(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

struct Pokepic;
struct PokepicDrawParam;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
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
void ov41_02248164(UnkStruct_ov22_02257964 *param0, int param1);
void ov41_02248158(UnkStruct_ov22_02257964 *param0);

void ov41_02248120(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 v0;
    s32 v1;

    v0 = 0 - arg2;
    v1 = 0 - arg1;
    ov41_02249BE8(arg0 + 4, v1, v0);
    ov41_02249BE8(arg0 + 4, arg3, arg4);
    ov41_02249BE8(arg0 + 0x14, v1, v0);
    ov41_02249BE8(arg0 + 0x14, arg3, arg4);
}

void ov41_02248158(UnkStruct_ov22_02257964 *param0) {
    ov41_02248164(param0, -1);
}
