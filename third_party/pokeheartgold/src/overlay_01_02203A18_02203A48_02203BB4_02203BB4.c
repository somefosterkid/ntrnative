#include "global.h"

#include "billboard_gfx_sequence.h"
#include "map_object.h"
#include "overlay_01_02203A18.h"
#include "overlay_01_021F1348.h"
#include "overworld_anim_manager.h"
#include "sprite.h"
#include "unk_02005D10.h"
#include "unk_02023694.h"
#include "vram_transfer_manager.h"

extern void *sub_02020838(void *model, u8 index);

extern void *sub_02020888(void *model, u8 index);

extern u32 sub_02020910(void *model, u32 index);

extern u32 sub_020209AC(void *model, u32 index);

extern const u32 ov01_02209544[14][2];

BOOL ov01_02203BB4(void *param0, UnkOv01_02203A18_Work *work);

BOOL ov01_02203BB4(void *param0, UnkOv01_02203A18_Work *work) {
    VecFx32 vec;
    void *node;
    u32 idx;
    u32 vramAddr;
    work->unk30 = *(UnkOv01_02203A18_Data *)sub_02068D98(param0);
    work->unk8 = MapObject_GetID(work->unk30.unk8);
    work->unkC = MapObject_GetMapID(work->unk30.unk8);
    work->unk28 = 6 << 0xc;
    idx = sub_02068D90(param0);
    work->anim.unk18 = ov01_021F14B4(work->unk30.fieldEffMan, ov01_02209544[idx][0], 1);
    work->anim.unk14 = ov01_021F14B4(work->unk30.fieldEffMan, ov01_02209544[idx][1], 1);
    BillboardGfxSequence_SetData(work->anim.unk14, &work->anim.sequence);
    work->anim.unk10 = NNS_G3dGetTex(work->anim.unk18);
    work->anim.unk24 = 0;
    work->anim.unk26 = 0;
    node = ov01_021F1AD4(work->unk30.fieldEffMan, 0xd);
    work->anim.unk20 = sub_020209AC(node, 0);
    work->anim.unk1C = sub_02020910(node, 0);
    GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_VRAM, work->anim.unk1C, sub_02020838(work->anim.unk10, 0), work->anim.unk20);
    vramAddr = (*(u32 *)((u8 *)node + 0x2c) << 0x10) >> 0xd;
    GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_PLTT, vramAddr, sub_02020888(work->anim.unk10, 0), 0x20);
    sub_02068DB8(param0, &vec);
    work->unk3C = ov01_02203B98(work->unk30.fieldEffMan, &vec);
    sub_02023EA4(work->unk3C, 0);
    PlaySE(1501);
    return 1;
}
