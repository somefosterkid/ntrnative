// The PC build compiles src/system/mcss.c through this file (tools/port_sources.py): Smooth motion's tags (libntr's
// simulator/sim_g3tag.h) on the battle's Pokemon sprites. A sprite is a multi-cell animation: func_020155a0 draws one cell's quad
// with matrix 29 and its shadow's with matrix 30 (pokebw2's MCSS_MTX_SPRITE / MCSS_MTX_SHADOW), so the hook on its G3_RestoreMtx
// tags them: 29 the cell (kind 4: owner the MCSS, inst its node in the multi-cell it shows, state the cell), 30 its shadow (kind
// 2, the same inst); 0 (the projection, after the cell) ends the tag, and so does the end of func_02014d2c, the sprites' draw.
// func_020159c8 (pokebw2's MCSSSys_Add) gives a new sprite new ids. Off (the default) each site is one flag test and the command
// stream is unchanged; BWPORT_VCAPTURE_TAGS=1 or SIM_G3TagEnable(1) (Smooth motion on) turns them on. The DS build never sees
// this file.
#pragma push_macro("func_02014d2c")
#pragma push_macro("func_020159c8")
#define func_02014d2c func_02014d2c_game
#define func_020159c8 func_020159c8_game
#include "types.h"   // the game file's headers, before the hook below (their include guards keep them out of it)
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "math.h"
#include "nitro/fx/fx.h"
#include "nitro/fx/fx_const.h"
#include "nitro/fx/fx_trig.h"
#include "nitro/fx/fx_cp.h"
#include "nitro/fx/fx_mtx.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "nnsys/g2d.h"
#include "nnsys/g3d.h"
#include "system/mcss.h"
#include "system/palanm.h"
#include <simulator/sim_g3tag.h>

static void PortMcss_Tag(const MCSS *mcss, int node, const NNSG2dAnimDataSRT *cellSrt, const NNSG2dAnimDataSRT *mcSrt,
                         int mtx);

// G3_RestoreMtx is called only in func_020155a0 (its mcss, node, cellSrt and mcSrt)
#define G3_RestoreMtx(num)                                                                                             \
    ((SIM_G3TagOn ? PortMcss_Tag(mcss, node, cellSrt, mcSrt, (num)) : (void)0), G3_RestoreMtx(num))
#include "../../../../src/system/mcss.c"
#undef G3_RestoreMtx
#pragma pop_macro("func_02014d2c")
#pragma pop_macro("func_020159c8")

// A cell's inst: its node's entry in the sprite's multi-cell data, so a node keeps its id while the sprite shows one multi-cell
// and a pose (another multi-cell: the trainer's throw) is new cells (the data is the sprite's own: a new sprite, a new owner)
static u32 PortMcss_Node(const MCSS *mcss, int node, const NNSG2dAnimDataSRT *mcSrt) {
    return SIM_G3TagId(&mcss->mcAnim.pMultiCellDataBank->pMultiCellDataArray[mcSrt->index].pHierDataArray[node]);
}

static void PortMcss_Tag(const MCSS *mcss, int node, const NNSG2dAnimDataSRT *cellSrt, const NNSG2dAnimDataSRT *mcSrt,
                         int mtx) {
    if (mtx == 29) {
        SIM_G3Tag(SIM_G3TAG_OWNER(SIM_G3TAG_CELL, mcss), PortMcss_Node(mcss, node, mcSrt), cellSrt->index);
    } else if (mtx == 30) {
        SIM_G3Tag(SIM_G3TAG_OWNER(SIM_G3TAG_SHADOW, mcss), PortMcss_Node(mcss, node, mcSrt), cellSrt->index);
    } else {
        SIM_G3Tag(0, 0, 0);
    }
}

void func_02014d2c(MCSSSystem *system) {
    func_02014d2c_game(system);
    if (SIM_G3TagOn) {
        SIM_G3Tag(0, 0, 0);
    }
}

MCSS *func_020159c8(MCSSSystem *system, fx32 x, fx32 y, fx32 z, const MCSSLoadInfo *info) {
    MCSS *mcss = func_020159c8_game(system, x, y, z, info);

    SIM_G3TagDrop(mcss);
    return mcss;
}
