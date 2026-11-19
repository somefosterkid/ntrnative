#include "global.h"

#include "constants/heap.h"
#include "error_handling.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "field_system.h"
#include "heap.h"
#include "player_avatar.h"
#include "overlay_02_field3d_effects.h"

void ov02_0224D820(Ov02App_0224D880 *env) {
    int i;

    ov02_0224D7B0(env);
    for (i = 0; i < 16; i++) {
        if (env->slots[i].playing == 0) {
            ov02_0224D698(&env->slots[i], env->fieldSystem->playerAvatar, env->offsetY, env->offsetZ);
            return;
        }
    }
    GF_AssertFail();
}

void ov02_0224D868(Ov02App_0224D880 *env) {
    int i;

    for (i = 0; i < 16; i++) {
        ov02_0224D700(&env->slots[i]);
    }
}

void ov02_0224D880(Field3dObjectTask *task, FieldSystem *fieldSystem, void *data) {
    Ov02App_0224D880 *env = data;
    int i;

    memset(env, 0, sizeof(Ov02App_0224D880));
    HeapExp_FndInitAllocator(&env->allocator, HEAP_ID_FIELD1, 0x20);
    Field3dModel_LoadFromFilesystem(&env->model, NARC_a_1_3_4, 8, HEAP_ID_FIELD1);
    for (i = 0; i < OV02_FIELD3D_EFFECT_ANIM_COUNT; i++) {
        env->animFiles[i] = GfGfxLoader_LoadFromNarc(NARC_a_1_3_4, i + 4, 0, HEAP_ID_FIELD1, 0);
    }
    for (i = 0; i < 16; i++) {
        ov02_0224D73C(&env->slots[i], &env->model, &env->allocator, env->animFiles);
    }
    env->fieldSystem = fieldSystem;
    PlayerAvatar_CopyPositionVector(env->fieldSystem->playerAvatar, &env->lastPlayerPos);
    env->state = 0;
}
