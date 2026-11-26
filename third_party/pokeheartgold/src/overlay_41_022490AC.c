#include "global.h"
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
#include "overlay_41_unk_struct_ov22_02258A48.h"
#include "touchscreen.h"
#include "constants/heap.h"
#include "overlay_41_unk_struct_ov22_022597BC.h"

void ov41_022490AC(void);

struct Pokepic;
struct PokepicDrawParam;
struct SoftwareSprite;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SysTask;
struct SysTaskQueue;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSprite SoftwareSprite;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
void ov41_02249418(UnkStruct_ov22_02258EE4 *param0);
void ov41_02249480(UnkStruct_ov22_02258EE4 *param0, int param1, int param2, int param3, int param4, int param5);
void ov41_022490B0(UnkStruct_ov22_02258A48 *param0);

void ov41_022490AC(void) {
}

void ov41_022490B0(UnkStruct_ov22_02258A48 *param0) {
    UnkStruct_ov22_02258EE4 *v0 = param0->unk_00;

    if (v0->unk_10) {
        ((v0->unk_10->unk_04 == 1) ? (void)0 : GF_AssertFail());

        ov41_02249480(v0, 4, v0->unk_1C, v0->unk_1E, v0->unk_10->unk_04, v0->unk_21);
        ov41_02249418(v0);
    }
}
