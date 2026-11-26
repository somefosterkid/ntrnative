#include "global.h"

#include "constants/heap.h"

#include "overlay_96_ov96_pic_panel.h"
#include "pokemon.h"
#include "sprite.h"

void ov96_02203E30(Ov96SpriteMgr *mgr, Ov96PicPanel *panel, Ov96PicPanelMon *mons) {
    u8 bVar1;
    s32 iVar3;
    Ov96PicPanelMon *puVar5;
    s32 iStack_24;
    VecFx32 VStack_20;
    iVar3 = 0;
    do {
        ov96_021EB408(mgr, 3, NNS_G2D_VRAM_TYPE_2DSUB, 0x65, 2);
        iVar3 = iVar3 + 1;
    } while (iVar3 < 3);
    iVar3 = 0;
    iStack_24 = 0x10;
    puVar5 = mons;
    do {
        panel->picObjs[iVar3] = ov96_021EB4F4(mgr, 0x65, 2);
        VStack_20.z = 0;
        VStack_20.x = (iStack_24 & 0xFFFFFFFFu) << 0xc;
        bVar1 = GetMonPicHeightBySpeciesGenderForm(puVar5->species, puVar5->gender, 0, (u8)puVar5->form, puVar5->personality);
        VStack_20.y = (bVar1 + 0x178) * 0x1000;
        ov96_021EB588(panel->picObjs[iVar3], &VStack_20);
        ov96_021EB564(panel->picObjs[iVar3], iVar3 + 1);
        ov96_021EB52C(panel->picObjs[iVar3], 1, 1);
        ov96_021EB630(panel->picObjs[iVar3], 0x13 - iVar3);
        panel->frameObjs[iVar3] = ov96_021EB4F4(mgr, 0x65, 5);
        ov96_021EB564(panel->frameObjs[iVar3], 0);
        VStack_20.y = VStack_20.y + (-0x10000);
        ov96_021EB588(panel->frameObjs[iVar3], &VStack_20);
        ov96_021EB630(panel->frameObjs[iVar3], 4);
        puVar5 = puVar5 + 1;
        iStack_24 = iStack_24 + 0x32;
        iVar3 = iVar3 + 1;
    } while (iVar3 < 3);
    ov96_02203DCC(panel, mons);
    ov96_02203F50(mgr, panel);
    ov96_02203F0C(panel);
}

void ov96_02203F0C(Ov96PicPanel *panel) {
    u32 uVar1;

    uVar1 = 0;
    do {
        DC_FlushRange(panel->pics[uVar1].charData, 0xc80);
        GXS_LoadOBJ(panel->pics[uVar1].charData, panel->charVramOffs[uVar1], 0xc80);
        DC_FlushRange(panel->pics[uVar1].plttData, 0x20);
        GXS_LoadOBJPltt(panel->pics[uVar1].plttData, panel->plttVramOffs[uVar1], 0x20);
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 3);
}
