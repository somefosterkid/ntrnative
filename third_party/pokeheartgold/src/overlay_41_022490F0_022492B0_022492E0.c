#include "global.h"

#include "sprite.h"
#include "sys_task.h"
#include "system.h"
#include "unk_02009D48.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "filesystem_files_def.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "bg_window.h"
#include "overlay_41_unk_struct_ov22_02257964.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_0225ab54.h"
#include "screen_fade_internal.h"
#include "overlay_41_unk_struct_ov22_02258ee4.h"
#include "overlay_41_unk_struct_ov22_02258A48.h"
#include "touchscreen.h"
#include "constants/heap.h"
#include "overlay_41_unk_struct_ov22_022597BC.h"

typedef struct System System;

struct Pokepic;

struct PokepicShadow;

struct PokepicDrawParam;

struct SoftwareSprite;

struct SoftwareSpriteChars;

struct SoftwareSpriteManager;

struct SoftwareSpritePalette;

struct SpriteAnimFrame;

struct SysTask;

struct SysTaskQueue;

struct System;

struct UnkStruct_ov22_02258A48_t;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);


typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;

void ov41_022482B8(UnkStruct_ov22_02257964 *param0, int *param1, int *param2);

void ov41_02249B94(UnkStruct_ov22_02259560 *param0, int *param1, int *param2);

void ov41_02249BAC(UnkStruct_ov22_02259560 *param0, int *param1, int *param2, int *param3, int *param4);

void ov41_022495A4(UnkStruct_ov22_02258A48 *param0, int param1, int param2);

void ov41_022492E0(UnkStruct_ov22_02258A48 *param0);

void ov41_022492E0(UnkStruct_ov22_02258A48 *param0) {
    UnkStruct_ov22_02258EE4 *v0 = param0->unk_00;
    int v1, v2;
    int v3, v4;
    int v5, v6;
    int v7, v8;
    int v9, v10;

    if (v0->unk_10 != 0L) {
        if ((gSystem.touchX != 0xffff) && (gSystem.touchX != 0xffff)) {
            ov41_02249BAC(v0->unk_10, &v7, &v9, &v8, &v10);
            ov41_02249B94(v0->unk_10, &v1, &v2);

            v3 = gSystem.touchX - v0->unk_14;
            v4 = gSystem.touchY - v0->unk_18;

            v5 = v3 + v7;
            v6 = v4 + v8;
            v1 -= v9;
            v2 -= v10;

            if (v5 <= (136 + 2)) {
                v3 = (136 + 2) - v7;
            } else {
                if (v3 + v1 >= ((136 + 2) + (112 - (2 * 2)))) {
                    v3 = ((136 + 2) + (112 - (2 * 2))) - v1;
                }
            }

            if (v6 <= (16 + 2)) {
                v4 = (16 + 2) - v8;
            } else {
                if (v4 + v2 >= ((16 + 2) + (129 - (2 * 2)))) {
                    v4 = ((16 + 2) + (129 - (2 * 2))) - v2;
                }
            }

            ov41_022495A4(param0, v3, v4);
            ov41_022482B8(v0->unk_04, &v7, &v8);

            v3 += v7;
            v4 += v8;

            ov41_022495A4(param0, v3, v4);
        }
    }
}
