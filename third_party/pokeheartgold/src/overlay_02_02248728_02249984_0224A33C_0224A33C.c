#include "global.h"
#include "sprite_resources_header.h"
#include "field_system_def.h"
#include "overlay_02_hmcut_in.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_74_main_menu_util.h"
#include "sprite.h"

struct HMCutIn;

struct Pokemon;

struct PokemonSpriteTemplate;

struct Sprite;

struct SpriteList;

struct SpriteListTemplate;

struct SpriteResourcesHeader;

Sprite *Sprite_Create(const SimpleSpriteTemplate *template);

void *ov02_0224A7B8(Pokemon *mon, PokemonSpriteTemplate *spriteTemplate, enum HeapID heapID);

void *ov02_0224A800(PokemonSpriteTemplate *spriteTemplate, enum HeapID heapID);

Sprite *ov02_0224A33C(HMCutIn *cutIn, const VecFx32 *position, u32 charResourceID, u32 paletteResourceID, u32 cellResourceID, u32 animResourceID, int headerPriority, int listPriority);

Sprite *ov02_0224A33C(HMCutIn *cutIn, const VecFx32 *position, u32 charResourceID, u32 paletteResourceID, u32 cellResourceID, u32 animResourceID, int headerPriority, int listPriority) {
    SpriteResourcesHeader resourceHeader;
    SimpleSpriteTemplate spriteListTemplate;
    Sprite *sprite;

    if (animResourceID == 4) {
        CreateSpriteResourcesHeader(
            &resourceHeader,
            charResourceID,
            paletteResourceID,
            cellResourceID,
            -1,
            -1,
            -1,
            0,
            headerPriority,
            cutIn->charLocation,
            cutIn->paletteLocation,
            cutIn->cellLocation,
            0L,
            0L,
            0L);
    } else {
        CreateSpriteResourcesHeader(
            &resourceHeader,
            charResourceID,
            paletteResourceID,
            cellResourceID,
            animResourceID,
            -1,
            -1,
            0,
            headerPriority,
            cutIn->charLocation,
            cutIn->paletteLocation,
            cutIn->cellLocation,
            cutIn->animLocation,
            0L,
            0L);
    }

    spriteListTemplate.spriteList = cutIn->spriteList;
    spriteListTemplate.header = &resourceHeader;
    spriteListTemplate.position = *position;
    spriteListTemplate.priority = listPriority;
    spriteListTemplate.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    spriteListTemplate.heapID = HEAP_ID_FIELD1;

    sprite = Sprite_Create(&spriteListTemplate);
    ((sprite != 0L) ? (void)0 : GF_AssertFail());

    return sprite;
}
