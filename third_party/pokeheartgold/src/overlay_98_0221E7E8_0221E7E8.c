#include "global.h"

#include "constants/gx.h"
#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_3d_loader.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_98.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "pokemon_types_def.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02013FDC.h"
#include "wild_encounter.h"
#include "overlay_98_ov98text_sys.h"

struct Background;

struct BgConfig;

struct G2dRenderer;

struct GF_2DGfxRawResMan;

struct GF_2DGfxRawResObj;

struct GF_2DGfxResHeader;

struct GF_2DGfxResMan;

struct GF_2DGfxResObjList;

struct GraphicsBanks;

struct ManagedSprite;

struct MessageFormat;

struct MessageFormatAttrs;

struct MessageFormatFields;

struct MsgData;

struct NARC;

struct OamCharTransferParam;

struct OamManagerParam;

struct Ov98MonInfo;

struct Ov98Rodata;

struct Ov98TextSys;

struct PaletteBuffer;

struct PaletteData;

struct PokepicTemplate;

struct SelectedPaletteData;

struct Sprite;

struct SpriteList;

struct SpriteManager;

struct SpriteResource;

struct SpriteResourceHeaderList;

struct SpriteResourcesHeader;

struct SpriteSystem;

struct String;

struct UnkStruct_02014E30;

struct Window;

struct WindowTemplate;

struct _MAT;

struct _MAT_ENTRY;

typedef struct Ov98MonInfo {
    u32 species : 9;
    u32 form : 5;
    u32 gender : 2;
    u32 shiny : 1;
    u32 personality;
} Ov98MonInfo;

typedef struct Ov98Rodata {
    u32 vramTypes[2];
    UnkStruct_02014E30 monPicRect;
    OamCharTransferParam oamTransfer;
    OamManagerParam oamManager;
    TouchscreenHitbox hitbox;
    u32 powersOfTen[5];
    u16 statThresholds[10][2];
    s32 rankThresholds[10];
    GraphicsBanks banks;
} Ov98Rodata;

extern const Ov98Rodata _0221F194;

void ov98_0221E7E8(ManagedSprite *sprite, const Ov98MonInfo *mon, BOOL isMain, BOOL grayscale, enum HeapID heapId);

void ov98_0221E7E8(ManagedSprite *sprite, const Ov98MonInfo *mon, BOOL isMain, BOOL grayscale, enum HeapID heapId) {
    PokepicTemplate pokepic;
    UnkStruct_02014E30 rect;
    void *charBuf;

    if (mon->species == 0) {
        ManagedSprite_SetDrawFlag(sprite, 0);
        return;
    }

    GetMonSpriteCharAndPlttNarcIdsEx(&pokepic, mon->species, mon->gender, 2, mon->shiny, mon->form, mon->personality);
    rect = _0221F194.monPicRect;
    charBuf = Heap_AllocAtEnd(heapId, 0xC80);
    sub_02014510((NarcId)pokepic.narcID, pokepic.charDataID, heapId, &rect, charBuf, mon->personality, 0, 2, mon->species);
    ov98_0221E9FC((struct Sprite **)(sprite), charBuf, 0xC80);
    ov98_0221EA4C((struct Sprite **)(sprite), (NarcId)pokepic.narcID, pokepic.palDataID, isMain, grayscale, heapId);
    ManagedSprite_SetDrawFlag(sprite, 1);
    Heap_Free(charBuf);
}
