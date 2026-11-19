#include "global.h"

#include "constants/sndseq.h"

#include "battle_castle_app_sprite.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "item.h"
#include "message_format.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "party.h"
#include "party_menu.h"
#include "player_avatar.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "render_window.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_02013FDC.h"
#include "unk_02030A98.h"
#include "unk_02034354.h"
#include "unk_0205BFF0.h"
#include "vram_transfer_manager.h"
#include "yes_no_prompt.h"

struct BoxPokemon;

struct PokedexData;

struct PokepicTemplate;

struct Sprite;

struct UnkStruct_02014E30;

extern const UnkStruct_02014E30 ov83_0224818C;

void ov83_02247668(BattleCastleAppSprite *obj, BoxPokemon *boxMon, int species, u32 personality);

void ov83_02247668(BattleCastleAppSprite *obj, BoxPokemon *boxMon, int species, u32 personality) {
    NNSG2dCharacterData *charData;
    PokepicTemplate pokepicTemplate;
    UnkStruct_02014E30 rect = ov83_0224818C;
    u32 location;
    void *buf;
    void *data;
    u32 narcId;
    u32 palId;

    buf = Heap_AllocAtEnd(HEAP_ID_107, 0xC80);
    if (species != SPECIES_NONE) {
        GetBoxmonSpriteCharAndPlttNarcIds(&pokepicTemplate, boxMon, 2, FALSE);
        sub_02014510((NarcId)pokepicTemplate.narcID, pokepicTemplate.charDataID, HEAP_ID_107, &rect, buf, personality, FALSE, 2, species);
        narcId = pokepicTemplate.narcID;
        palId = pokepicTemplate.palDataID;
    } else {
        data = GfGfxLoader_GetCharData(NARC_a_1_8_4, 39, TRUE, &charData, HEAP_ID_107);
        MI_CpuCopy32(charData->pRawData, buf, 0xC80);
        Heap_Free(data);
        narcId = NARC_a_1_8_4;
        palId = 61;
    }
    location = NNS_G2dGetImageLocation(Sprite_GetImageProxy(obj->sprite), NNS_G2D_VRAM_TYPE_2DSUB);
    DC_FlushRange(buf, 0xC80);
    GXS_LoadOBJ(buf, location, 0xC80);
    GfGfxLoader_GXLoadPal((NarcId)narcId, palId, GF_PAL_LOCATION_SUB_OBJ, (enum GFPalSlotOffset)NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(obj->sprite), NNS_G2D_VRAM_TYPE_2DSUB), 0x20, HEAP_ID_107);
    Heap_Free(buf);
}
