#ifndef POKEHEARTGOLD_OVERLAY_18_POKEDEX_ENTRY_POPUP_H
#define POKEHEARTGOLD_OVERLAY_18_POKEDEX_ENTRY_POPUP_H

#include "bg_window.h"
#include "g2d_renderer.h"
#include "heap.h"
#include "pokemon_types_def.h"
#include "save_layout.h"

// PokedexEntryPopup: the Pokedex entry popup task data (ov18_021F8974, Heap_Alloc sizeof = 0x258 on the DS).

struct BgConfig;
struct GF_2DGfxResMan;
struct NARC;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct PaletteData;
struct Pokepic;
struct PokepicManager;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct SysTask;

typedef struct PokedexEntryPopupObj {
    struct Sprite *sprite;
    struct SpriteResource *res[4];
} PokedexEntryPopupObj;

typedef struct PokedexEntryPopup {
    struct BgConfig *bgConfig;
    struct PaletteData *paletteData;
    struct PokepicManager *pokepicManager;
    Pokemon *mon;
    BOOL natDexEnabled;
    enum HeapID heapId;
    struct SysTask *task;
    struct NARC *gfxNarc;
    struct Pokepic *pokepic;
    Window windows[9];
    struct SpriteList *spriteList;
    G2dRenderer renderer;
    struct GF_2DGfxResMan *resMan[4];
    PokedexEntryPopupObj objs[4];
    u16 unk_240;
    u16 palFlashTimer;
    u32 species;
    u32 form;
    u32 type1;
    u32 type2;
    u32 unk_254;
} PokedexEntryPopup;

// DS layout (a host with 8-byte pointers has a bigger one: the allocation uses sizeof)
#ifdef __MWERKS__
LAYOUT_CHECK(PokedexEntryPopup_size, sizeof(PokedexEntryPopup) == 0x258);
LAYOUT_CHECK_OFFSET(PokedexEntryPopup_windows, PokedexEntryPopup, windows, 0x24);
LAYOUT_CHECK_OFFSET(PokedexEntryPopup_spriteList, PokedexEntryPopup, spriteList, 0xB4);
LAYOUT_CHECK_OFFSET(PokedexEntryPopup_renderer, PokedexEntryPopup, renderer, 0xB8);
LAYOUT_CHECK_OFFSET(PokedexEntryPopup_resMan, PokedexEntryPopup, resMan, 0x1E0);
LAYOUT_CHECK_OFFSET(PokedexEntryPopup_objs, PokedexEntryPopup, objs, 0x1F0);
LAYOUT_CHECK_OFFSET(PokedexEntryPopup_unk_240, PokedexEntryPopup, unk_240, 0x240);
LAYOUT_CHECK_OFFSET(PokedexEntryPopup_unk_254, PokedexEntryPopup, unk_254, 0x254);
#endif

void ov18_021F89F8(struct SysTask *task, void *param);
void ov18_021F8AB8(PokedexEntryPopup *popup);
void ov18_021F8B10(PokedexEntryPopup *popup);
void ov18_021F8BEC(PokedexEntryPopup *popup);
void ov18_021F8C0C(PokedexEntryPopup *popup);
s32 ov18_021F8C48(PokedexEntryPopup *popup);
void ov18_021F8C68(PokedexEntryPopup *popup);
void ov18_021F8CCC(PokedexEntryPopup *popup);
void ov18_021F8F10(PokedexEntryPopup *popup);
void ov18_021F8F28(PokedexEntryPopup *popup);
void ov18_021F8F58(void);
void ov18_021F8F60(PokedexEntryPopup *popup);
void ov18_021F8F84(PokedexEntryPopup *popup);
void ov18_021F8FA0(PokedexEntryPopup *popup);
void ov18_021F8FF8(PokedexEntryPopup *popup);
void ov18_021F9054(PokedexEntryPopup *popup);
void ov18_021F9068(PokedexEntryPopup *popup);
void ov18_021F9108(PokedexEntryPopup *popup);
void ov18_021F9150(PokedexEntryPopup *popup);
void ov18_021F91DC(PokedexEntryPopup *popup);
void ov18_021F91F0(PokedexEntryPopup *popup);
void ov18_021F922C(struct SpriteResource **res, struct GF_2DGfxResMan **resMan, enum HeapID heapId, struct NARC *narc, int charFileId, int plttFileId, int cellFileId, int animFileId, int plttNum, int charResId, int plttResId, int cellResId, int animResId);
struct SpriteResource *ov18_021F92AC(struct GF_2DGfxResMan *resMan, enum HeapID heapId, struct NARC *narc, int fileId, int plttNum, int resId);
void ov18_021F9370(PokedexEntryPopup *popup);
void ov18_021F94A0(PokedexEntryPopup *popup);
void ov18_021F94BC(PokedexEntryPopup *popup);
void ov18_021F9508(PokedexEntryPopup *popup);
void ov18_021F9518(PokedexEntryPopup *popup);
void ov18_021F959C(PokedexEntryPopup *popup);
void ov18_021F95CC(PokedexEntryPopup *popup);

#endif // POKEHEARTGOLD_OVERLAY_18_POKEDEX_ENTRY_POPUP_H
