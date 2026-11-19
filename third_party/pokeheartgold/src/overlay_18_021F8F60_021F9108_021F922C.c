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

struct GF_2DGfxResMan;

struct NARC;

struct PokedexData;

struct SpriteResource;

void *ov18_021F9694(u32 species, enum HeapID heapId);

SpriteResource *ov18_021F92AC(GF_2DGfxResMan *mgr, enum HeapID heapID, NARC *narc, s32 fileId, s32 pltt_num, s32 id);

void ov18_021F922C(SpriteResource **res, GF_2DGfxResMan **resMan, enum HeapID heapId, NARC *narc, int charFileId, int plttFileId, int cellFileId, int animFileId, int plttNum, int charResId, int plttResId, int cellResId, int animResId);

void ov18_021F922C(SpriteResource **res, GF_2DGfxResMan **resMan, enum HeapID heapId, NARC *narc, int charFileId, int plttFileId, int cellFileId, int animFileId, int plttNum, int charResId, int plttResId, int cellResId, int animResId) {
    res[GF_GFX_RES_TYPE_CHAR] = AddCharResObjFromOpenNarc(resMan[GF_GFX_RES_TYPE_CHAR], narc, charFileId, TRUE, charResId, NNS_G2D_VRAM_TYPE_2DMAIN, heapId);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(res[GF_GFX_RES_TYPE_CHAR]);
    sub_0200A740(res[GF_GFX_RES_TYPE_CHAR]);

    if (plttFileId != -1) {
        res[GF_GFX_RES_TYPE_PLTT] = ov18_021F92AC(resMan[GF_GFX_RES_TYPE_PLTT], heapId, narc, plttFileId, plttNum, plttResId);
    }

    res[GF_GFX_RES_TYPE_CELL] = AddCellOrAnimResObjFromOpenNarc(resMan[GF_GFX_RES_TYPE_CELL], narc, cellFileId, TRUE, cellResId, GF_GFX_RES_TYPE_CELL, heapId);
    res[GF_GFX_RES_TYPE_ANIM] = AddCellOrAnimResObjFromOpenNarc(resMan[GF_GFX_RES_TYPE_ANIM], narc, animFileId, TRUE, animResId, GF_GFX_RES_TYPE_ANIM, heapId);
}
