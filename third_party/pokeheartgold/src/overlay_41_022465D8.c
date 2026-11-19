#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"

struct PokemonSpriteManager;
struct ResourceCollection;
struct SoftwareSpriteChars;
struct SoftwareSpriteManager;
struct SoftwareSpritePalette;
struct SpriteList;
struct SpriteResourceCollection;
typedef struct ResourceCollection ResourceCollection;
typedef struct SoftwareSpriteChars SoftwareSpriteChars;
typedef struct SoftwareSpritePalette SoftwareSpritePalette;
typedef struct SoftwareSpriteManager SoftwareSpriteManager;
typedef struct PokemonSpriteManager PokemonSpriteManager;
void PokepicManager_DrawAll(PokemonSpriteManager *monSpriteMan);
void ov41_02246830(UnkStruct_ov22_0225A0E4 *param0);

void ov41_022465D8(UnkStruct_ov22_0225A0E4 *param0, int param1, int param2, u16 param3, const VecFx32 *param4);

void ov41_022465D8(UnkStruct_ov22_0225A0E4 *param0, int param1, int param2, u16 param3, const VecFx32 *param4) {
    G3_Identity();
    G3_PushMtx();

    {
        NNS_G2dSetupSoftwareSpriteCamera();
        G3_Translate(param1 * ((fx32)0x0000000000001000L), param2 * ((fx32)0x0000000000001000L), 0);

        {
            G3_RotZ(FX_SinIdx(param3), FX_CosIdx(param3));
            G3_Scale(param4->x, param4->y, param4->z);
        }

        G3_Translate(-param1 * ((fx32)0x0000000000001000L), -param2 * ((fx32)0x0000000000001000L), 0);
        G3_PushMtx();

        {
            if (param0->unk_1C) {
                ov41_02246830(param0);
            }

            if (param0->unk_2C) {
                PokepicManager_DrawAll(param0->unk_20);
            }
        }
        G3_PopMtx(1);
    }
    G3_PopMtx(1);
}
