#include "global.h"

#include "constants/gx.h"
#include "constants/pokemon.h"

#include "bg_window.h"
#include "dex_mon_measures.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "palette.h"
#include "pm_string.h"
#include "pokedex_util.h"
#include "pokemon.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "string_util.h"
#include "text.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_18_pokedex_entry_popup.h"

struct BgConfig;

struct BoxPokemon;

struct CAPSULE;

struct G2dRenderer;

struct GF_2DGfxResMan;

struct Mail;

struct MailMessage;

struct NARC;

struct PaletteData;

struct PartyPokemon;

struct PokedexData;

struct PokedexEntryPopup;

struct PokedexEntryPopupObj;

struct Pokepic;

struct PokepicManager;

struct SEAL;

struct SimpleSpriteTemplate;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourcesHeader;

struct SysTask;

struct SysTaskQueue;

struct Window;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

#if defined(SDK_PORT) || defined(NONMATCHING)
u8 ov18_021F9688(u8 type);
#else
u8 ov18_021F9688(int type);
#endif

void ov18_021F9310(SpriteResource **res, GF_2DGfxResMan **resMan, SpriteResourcesHeader *header, int priority);



void *ov18_021F9694(u32 species, enum HeapID heapId);


void ov18_021F9370(PokedexEntryPopup *popup) {
    SimpleSpriteTemplate template;
    SpriteResourcesHeader header;

    ov18_021F9310(popup->objs[1].res, popup->resMan, &header, 1);
    template.spriteList = popup->spriteList;
    template.header = &header;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.priority = 0;
    template.heapID = popup->heapId;
    template.position.x = 168 * FX32_ONE;
    template.position.y = 72 * FX32_ONE;
    popup->objs[1].sprite = Sprite_Create(&template);
    Sprite_SetPalIndexRespectVramOffset(popup->objs[1].sprite, ov18_021F9688(popup->type1));

    // The second icon uses the first icon's palette.
    CreateSpriteResourcesHeader(&header, GF2DGfxResObj_GetResID(popup->objs[2].res[GF_GFX_RES_TYPE_CHAR]), GF2DGfxResObj_GetResID(popup->objs[1].res[GF_GFX_RES_TYPE_PLTT]), GF2DGfxResObj_GetResID(popup->objs[2].res[GF_GFX_RES_TYPE_CELL]), GF2DGfxResObj_GetResID(popup->objs[2].res[GF_GFX_RES_TYPE_ANIM]), -1, -1, 0, 1, popup->resMan[GF_GFX_RES_TYPE_CHAR], popup->resMan[GF_GFX_RES_TYPE_PLTT], popup->resMan[GF_GFX_RES_TYPE_CELL], popup->resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    template.spriteList = popup->spriteList;
    template.header = &header;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.priority = 0;
    template.heapID = popup->heapId;
    template.position.x = 217 * FX32_ONE;
    template.position.y = 72 * FX32_ONE;
    popup->objs[2].sprite = Sprite_Create(&template);
    if (popup->type2 == 0 || popup->type1 == popup->type2) {
        Sprite_SetDrawFlag(popup->objs[2].sprite, FALSE);
    } else {
        Sprite_SetPalIndexRespectVramOffset(popup->objs[2].sprite, ov18_021F9688(popup->type2));
    }
}
