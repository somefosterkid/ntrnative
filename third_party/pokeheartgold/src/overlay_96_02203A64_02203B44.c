#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"
#include "overlay_96_ov96_pic_panel.h"
#include "sprite.h"

void ov96_02203B44(u32 param_1, Ov96SpriteMgr *mgr) {
    ov96_021EB2BC(mgr, NARC_a_2_1_7, 0xd, 0x65, 2);
    ov96_021EB2F4(mgr, NARC_a_2_1_7, 10, 0x65, 2, 4);
    ov96_021EB334(mgr, NARC_a_2_1_7, 0xc, 0x65);
    ov96_021EB36C(mgr, NARC_a_2_1_7, 0xb, 0x65);
    return;
}

void ov96_02203B8C(Ov96PicPanel *panel, Ov96SpriteMgr *mgr, Ov96PicPanelMon *mons) {
    s32 iVar1;

    iVar1 = 0;
    do {
        ov96_021EB408(mgr, 3, NNS_G2D_VRAM_TYPE_2DSUB, 0x65, 5);
        iVar1 = iVar1 + 1;
    } while (iVar1 < 3);
    ov96_02203E30(mgr, panel, mons);
    return;
}

void ov96_02203BC0(Ov96PicPanel *panel, s32 index, s32 arg2) {
    ov96_021EB52C(panel->frameObjs[index], 1, arg2);
    return;
}
