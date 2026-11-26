// The PC build compiles src/ov112/musical_mcss.c through this file (tools/port_sources.py): Smooth motion's tags (libntr's
// simulator/sim_g3tag.h) on the musical's Pokemon sprites, as port/extern/src/system/mcss.c on the battle's: the hook on
// func_ov112_021f5edc's G3_RestoreMtx tags a cell at matrix 29 (kind 4: owner the sprite, inst its node in the multi-cell it
// shows, state the cell) and ends the tag at 0 (the projection, after the cell: the marks' callbacks draw the props between
// cells); the end of func_ov112_021f56bc, the sprites' draw, ends it too. func_ov112_021f611c gives a new sprite new ids. Off
// (the default) each site is one flag test and the command stream is unchanged; BWPORT_VCAPTURE_TAGS=1 or SIM_G3TagEnable(1)
// (Smooth motion on) turns them on. The DS build never sees this file.
#pragma push_macro("func_ov112_021f56bc")
#pragma push_macro("func_ov112_021f611c")
#define func_ov112_021f56bc func_ov112_021f56bc_game
#define func_ov112_021f611c func_ov112_021f611c_game
#include "types.h"   // the game file's headers, before the hook below (their include guards keep them out of it)
#include "app/musical/musical_mcss.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_vec.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "nnsys/g3d.h"
#include "pml/personal.h"
#include "pml/poke_party.h"
#include "pml/poke_graphic.h"
#include "system/mcss.h"
#include <simulator/sim_g3tag.h>

static void PortMusMcss_Tag(const MusicalMcss *mcss, u32 node, const NNSG2dAnimDataSRT *cellSrt, const NNSG2dAnimDataSRT *mcSrt,
                            int mtx);

// G3_RestoreMtx is called only in func_ov112_021f5edc (its mcss, node, cellSrt and mcSrt)
#define G3_RestoreMtx(num)                                                                                             \
    ((SIM_G3TagOn ? PortMusMcss_Tag(mcss, node, cellSrt, mcSrt, (num)) : (void)0), G3_RestoreMtx(num))
#include "../../../../src/ov112/musical_mcss.c"
#undef G3_RestoreMtx
#pragma pop_macro("func_ov112_021f56bc")
#pragma pop_macro("func_ov112_021f611c")

// A cell's inst: its node's entry in the sprite's multi-cell data, so a node keeps its id while the sprite shows one multi-cell
// and a pose (another multi-cell: the trainer's throw) is new cells (the data is the sprite's own: a new sprite, a new owner)
static u32 PortMusMcss_Node(const MusicalMcss *mcss, u32 node, const NNSG2dAnimDataSRT *mcSrt) {
    return SIM_G3TagId(&mcss->mcAnim.pMultiCellDataBank->pMultiCellDataArray[mcSrt->index].pHierDataArray[node]);
}

static void PortMusMcss_Tag(const MusicalMcss *mcss, u32 node, const NNSG2dAnimDataSRT *cellSrt, const NNSG2dAnimDataSRT *mcSrt,
                            int mtx) {
    if (mtx == 29) {
        SIM_G3Tag(SIM_G3TAG_OWNER(SIM_G3TAG_CELL, mcss), PortMusMcss_Node(mcss, node, mcSrt), cellSrt->index);
    } else {
        SIM_G3Tag(0, 0, 0);
    }
}

void func_ov112_021f56bc(MusicalMcssSys *sys, MusicalMcssCellCallback callback) {
    func_ov112_021f56bc_game(sys, callback);
    if (SIM_G3TagOn) {
        SIM_G3Tag(0, 0, 0);
    }
}

MusicalMcss *func_ov112_021f611c(MusicalMcssSys *sys, fx32 x, fx32 y, fx32 z, MCSSLoadInfo *info, void *work,
                                 BOOL loadAtVBlank) {
    MusicalMcss *mcss = func_ov112_021f611c_game(sys, x, y, z, info, work, loadAtVBlank);

    SIM_G3TagDrop(mcss);
    return mcss;
}
