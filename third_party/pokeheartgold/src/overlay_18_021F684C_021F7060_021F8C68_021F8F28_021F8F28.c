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


void ov18_021F8F28(PokedexEntryPopup *popup) {
    ObjCharTransferTemplate template = ov18_021FBD50.objCharTransferTemplate;
    template.heapID = popup->heapId;
    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_64K, GX_OBJVRAMMODE_CHAR_1D_32K);
}
