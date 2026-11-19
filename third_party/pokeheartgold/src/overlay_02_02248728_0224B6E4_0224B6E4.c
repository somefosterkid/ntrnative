#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"

#include "overworld_anim_manager.h"
#include "struct_defs/resdat_narc_structs.h"

#include "field/area_data.h"
#include "field/area_light.h"
#include "field/dynamic_terrain_height.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"
#include "field/overlay_01_02204004.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "camera.h"
#include "gf_3d_loader.h"
#include "igt.h"
#include "map_matrix.h"
#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "sav_chatot.h"
#include "save.h"
#include "seal_case.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "overlay_02_hmcut_in.h"

void ov02_0224B6E4(OverworldAnimManager *param0, void *hmCutInWithPlayer2);

void ov02_0224B6E4(OverworldAnimManager *param0, void *hmCutInWithPlayer2) {
    fx32 playerSpriteY, windowMinY, windowMaxY;
    const VecFx32 *playerSpritePosition;
    HMCutInWithPlayer2 *wrapper = hmCutInWithPlayer2;

    playerSpritePosition = Sprite_GetMatrixPtr(wrapper->playerSprite);
    playerSpriteY = playerSpritePosition->y;
    windowMinY = wrapper->cutIn->windowY1;
    windowMaxY = wrapper->cutIn->windowY2;

    if (wrapper->cutIn->forceDrawPlayerSprite == FALSE) {
        if (playerSpriteY - (FX32_ONE * 8) >= windowMinY && playerSpriteY + (FX32_ONE * 8) <= windowMaxY) {
            Sprite_SetDrawFlag(wrapper->playerSprite, TRUE);
        } else {
            Sprite_SetDrawFlag(wrapper->playerSprite, FALSE);
        }
    } else {
        Sprite_SetDrawFlag(wrapper->playerSprite, TRUE);
    }
}
