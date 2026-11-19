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

struct BgTemplate;

struct BoxPokemon;

struct CAPSULE;

struct G2dRenderer;

struct GF_2DGfxResMan;

struct Mail;

struct MailMessage;

struct NARC;

struct ObjCharTransferTemplate;

struct PaletteData;

struct PartyPokemon;

struct PokedexData;

struct PokedexEntryPopup;

struct PokedexEntryPopupObj;

struct PokedexEntryPopupRodata;

struct Pokepic;

struct PokepicManager;

struct SEAL;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SysTask;

struct SysTaskQueue;

struct Window;

struct WindowTemplate;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

typedef struct PokedexEntryPopupRodata {
    ObjCharTransferTemplate objCharTransferTemplate;
    BgTemplate bgTemplate2;
    BgTemplate bgTemplate1;
    BgTemplate bgTemplate3;
    WindowTemplate windowTemplates[9];
    u8 typePalIndex[20];
    u32 typeIconCharId[18];
} PokedexEntryPopupRodata;

extern const PokedexEntryPopupRodata ov18_021FBD50;



void *ov18_021F9694(u32 species, enum HeapID heapId);


void ov18_021F8B10(PokedexEntryPopup *popup) {
    SetBgPriority(GF_BG_LYR_MAIN_0, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);

    {
        BgTemplate template = ov18_021FBD50.bgTemplate1;
        InitBgFromTemplate(popup->bgConfig, GF_BG_LYR_MAIN_1, &template, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, popup->heapId);
    }
    {
        BgTemplate template = ov18_021FBD50.bgTemplate2;
        InitBgFromTemplate(popup->bgConfig, GF_BG_LYR_MAIN_2, &template, GF_BG_TYPE_TEXT);
    }
    {
        BgTemplate template = ov18_021FBD50.bgTemplate3;
        InitBgFromTemplate(popup->bgConfig, GF_BG_LYR_MAIN_3, &template, GF_BG_TYPE_TEXT);
    }

    GfGfxLoader_LoadCharDataFromOpenNarc(popup->gfxNarc, 19, popup->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 1, popup->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(popup->gfxNarc, 20, popup->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 1, popup->heapId);
    PaletteData_LoadOpenNarc(popup->paletteData, popup->gfxNarc, 18, popup->heapId, PLTTBUF_MAIN_BG, 0, 0);
}
