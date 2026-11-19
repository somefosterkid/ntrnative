#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "field/field_sprite_manager.h"

#include "bg_window.h"
#include "gf_3d_loader.h"
#include "pokemon_types_def.h"
#include "seal_case.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"






enum PokemonPreviewState {
    PREVIEW_STATE_WAIT = 0,
    PREVIEW_STATE_REMOVE,
    PREVIEW_STATE_SET_ANIM,
    PREVIEW_STATE_WAIT_ANIM,
};

typedef struct PokemonPreview {
    FieldSpriteManager spriteManager;
    ManagedSprite *managedSprite;
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 x;
    u8 y;
    u8 state;
} PokemonPreview;

void sub_0200F9DC(PokemonPreview *preview);

void sub_0200F54C(SysTask *task, void *data);

void sub_0200F5C4(void *arg0, s8 arg1, s8 arg2, s8 arg3, enum HeapID arg4);

void sub_0200F54C(SysTask *task, void *data) {
    PokemonPreview *preview = data;

    switch (preview->state) {
    case PREVIEW_STATE_REMOVE:
        sub_0200F9DC(preview);
        Sprite_DeleteAndFreeResources(preview->managedSprite);
        FieldSpriteManager_ReleaseWithoutResDat(&preview->spriteManager);
        DestroySysTaskAndEnvironment(task);
        return;

    case PREVIEW_STATE_SET_ANIM:
        preview->state = 3;
        Sprite_SetAnimCtrlSeq(preview->managedSprite->sprite, 1);
        break;

    case PREVIEW_STATE_WAIT_ANIM:
        if (Sprite_GetAnimationFrame(preview->managedSprite->sprite) == 6) {
            preview->state = PREVIEW_STATE_WAIT;
        }
        break;
    }

    Sprite_UpdateAnim(preview->managedSprite->sprite, FX32_ONE);
    SpriteList_RenderAndAnimateSprites(preview->spriteManager.spriteList);
}

void sub_0200F5C4(void *arg0, s8 arg1, s8 arg2, s8 arg3, enum HeapID heapID) {
    void *v0;

    v0 = SysTask_GetData(CreateSysTaskAndEnvironment(sub_0200F54C, sizeof(PokemonPreview), 0U, heapID));
    *(s8 *)((u8 *)v0 + 0x16F) = 0;
    *(void **)((u8 *)v0 + 0x168) = arg0;
    *(s8 *)((u8 *)v0 + 0x16C) = arg1;
    *(s8 *)((u8 *)v0 + 0x16D) = arg2;
    *(s8 *)((u8 *)v0 + 0x16E) = arg3;
}
