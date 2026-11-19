#include "global.h"
#include "sprite.h"
#include "bag_cursor.h"
#include "bg_window.h"
#include "camera.h"
#include "field/dynamic_terrain_height.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"
#include "field_types_def.h"
#include "map_matrix.h"
#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "overlay_02_hmcut_in.h"

BOOL ov02_02249774(HMCutIn *cutIn);

BOOL ov02_02249774(HMCutIn *cutIn)
{
    const VecFx32 *pokemonSpritePosition;
    VecFx32 position;

    pokemonSpritePosition = Sprite_GetMatrixPtr(cutIn->pokemonSprite);
    position = *pokemonSpritePosition;
    position.x += cutIn->pokemonSpriteDeltaX;

    if (position.x <= (FX32_ONE * (128 + 32))) {
        position.x = (FX32_ONE * (128 + 64));
        cutIn->state++;
    }

    Sprite_SetMatrix(cutIn->pokemonSprite, &position);
    return FALSE;
}
