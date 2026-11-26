// Black 2's and White 2's battle sprites (port/extern/src/system/mcss.c's twin): pokebw2's src/system/mcss.c compiled through
// this file (tools/port_sources.py --b2w2), with Smooth motion's tags (libntr's simulator/sim_g3tag.h). A sprite is a multi-cell
// animation: MCSS_DrawQuad draws one cell's quad with MCSS_MTX_SPRITE and its shadow's with MCSS_MTX_SHADOW, so the hook on its
// G3_RestoreMtx tags them: the sprite's matrix the cell (kind 4: owner the MCSS, inst its node in the multi-cell it shows, state
// the cell), the shadow's its shadow (kind 2, the same inst); 0 (the projection, after the cell) ends the tag, and so does the
// end of MCSSSys_Draw. MCSSSys_Add gives a new sprite new ids. Off (the default) each site is one flag test and the command
// stream is unchanged; BWPORT_VCAPTURE_TAGS=1 or SIM_G3TagEnable(1) (Smooth motion on) turns them on. The DS build never sees
// this file.
#pragma push_macro("MCSSSys_Draw")
#pragma push_macro("MCSSSys_Add")
#define MCSSSys_Draw MCSSSys_Draw_game
#define MCSSSys_Add MCSSSys_Add_game
#include "types.h"   // the game file's headers, before the hook below (their include guards keep them out of it)
#include "constants/arc.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/heap.h"
#include "gfl/tcb.h"
#include "math.h"
#include "nitro/fx.h"
#include "nitro/gx.h"
#include "nitro/mi.h"
#include "nnsys/g2d.h"
#include "nnsys/g3d.h"
#include "system/mcss.h"
#include "system/palanm.h"
#include <simulator/sim_g3tag.h>

static void PortMcss_Tag(const MCSS *mcss, int node, const NNSG2dAnimDataSRT *cellSrt, const NNSG2dAnimDataSRT *mcSrt,
                         int mtx);

// G3_RestoreMtx is called only in MCSS_DrawQuad (its mcss, node, cellSrt and mcSrt)
#define G3_RestoreMtx(num)                                                                                             \
    ((SIM_G3TagOn ? PortMcss_Tag(mcss, node, cellSrt, mcSrt, (num)) : (void)0), G3_RestoreMtx(num))
#include "../../../../third_party/pokebw2/src/system/mcss.c"
#undef G3_RestoreMtx
#pragma pop_macro("MCSSSys_Draw")
#pragma pop_macro("MCSSSys_Add")

// A cell's inst: its node's entry in the sprite's multi-cell data, so a node keeps its id while the sprite shows one multi-cell
// and a pose (another multi-cell: the trainer's throw) is new cells (the data is the sprite's own: a new sprite, a new owner)
static u32 PortMcss_Node(const MCSS *mcss, int node, const NNSG2dAnimDataSRT *mcSrt) {
    return SIM_G3TagId(&mcss->mcAnim.pMultiCellDataBank->pMultiCellDataArray[mcSrt->index].pHierDataArray[node]);
}

static void PortMcss_Tag(const MCSS *mcss, int node, const NNSG2dAnimDataSRT *cellSrt, const NNSG2dAnimDataSRT *mcSrt,
                         int mtx) {
    if (mtx == MCSS_MTX_SPRITE) {
        SIM_G3Tag(SIM_G3TAG_OWNER(SIM_G3TAG_CELL, mcss), PortMcss_Node(mcss, node, mcSrt), cellSrt->index);
    } else if (mtx == MCSS_MTX_SHADOW) {
        SIM_G3Tag(SIM_G3TAG_OWNER(SIM_G3TAG_SHADOW, mcss), PortMcss_Node(mcss, node, mcSrt), cellSrt->index);
    } else {
        SIM_G3Tag(0, 0, 0);
    }
}

void MCSSSys_Draw(MCSSSystem *system) {
    MCSSSys_Draw_game(system);
    if (SIM_G3TagOn) {
        SIM_G3Tag(0, 0, 0);
    }
}

MCSS *MCSSSys_Add(MCSSSystem *system, fx32 x, fx32 y, fx32 z, const MCSSLoadInfo *info) {
    MCSS *mcss = MCSSSys_Add_game(system, x, y, z, info);

    SIM_G3TagDrop(mcss);
    return mcss;
}
