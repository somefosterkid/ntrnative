#include "global.h"
struct SpriteResourceManager;

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "field/field_sprite_manager.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02009D48.h"
#include "pokemon.h"
#include "pokepic.h"
#include "sprite_resource_manager.h"

typedef struct PokemonPreview {
    FieldSpriteManager spriteManager;
    ManagedSprite *managedSprite;
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 x;
    u8 y;
    u8 state;
} PokemonPreview;

extern const ManagedSpriteTemplate _020F5C78;

void sub_0200F684(PokemonPreview *preview, u8 x, u8 y);

s32 sub_0200F748(void *, PokepicTemplate *);
void sub_0200F6D4(struct SpriteResourceManager *arg0, u16 arg1, u8 arg2);
void sub_0200F714(struct SpriteResourceManager *arg0, Pokemon *arg1);

void sub_0200F684(PokemonPreview *preview, u8 x, u8 y) {
    ManagedSpriteTemplate template = _020F5C78;
    template.x = (x + 5) * 8;
    template.y = (y + 5) * 8;

    preview->managedSprite = FieldSpriteManager_CreateManagedSprite(&preview->spriteManager, &template);

    SpriteList_RenderAndAnimateSprites(preview->spriteManager.spriteList);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, TRUE);
}

void sub_0200F6D4(SpriteResourceManager *arg0, u16 species, u8 gender) {
    PokepicTemplate pokepicTemplate;
    PokepicManager *pokepicManager;

    pokepicManager = PokepicManager_Create((enum HeapID) arg0->heapID);
    GetMonSpriteCharAndPlttNarcIdsEx(&pokepicTemplate, species, gender, 2U, 0U, 0U, 0U);
    sub_0200F748(arg0, &pokepicTemplate);
    PokepicManager_Delete(pokepicManager);
}

void sub_0200F714(SpriteResourceManager *arg0, Pokemon *mon) {
    PokepicTemplate pokepicTemplate;
    PokepicManager *pokepicManager;

    pokepicManager = PokepicManager_Create((enum HeapID) arg0->heapID);
    GetPokemonSpriteCharAndPlttNarcIds(&pokepicTemplate, mon, 2U);
    sub_0200F748(arg0, &pokepicTemplate);
    PokepicManager_Delete(pokepicManager);
}
