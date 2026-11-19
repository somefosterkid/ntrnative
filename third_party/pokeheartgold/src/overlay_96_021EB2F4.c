#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "overlay_96_ov96_sprite_mgr.h"
#include "unk_0200A090.h"

void ov96_021EB2F4(Ov96SpriteMgr *mgr, s32 narcId, s32 fileId, s32 resId, s32 vram, u8 plttNum) {
    SpriteResource **slot;

    slot = ov96_021EB5EC(mgr, resId, GF_GFX_RES_TYPE_PLTT);
    *slot = AddPlttResObjFromNarc(mgr->resMan[GF_GFX_RES_TYPE_PLTT], (NarcId)narcId, fileId, 0, resId, vram, (u32)plttNum, mgr->heapID);
}
