#include "global.h"

#include "constants/sndseq.h"

#include "field_system.h"
#include "heap.h"
#include "map_object.h"
#include "sprite.h"
#include "task.h"
#include "unk_02005D10.h"
#include "vram_transfer_manager.h"
#include "overlay_01_02203A18.h"

void *sub_02020838(NNSG3dResTex *texture, const u8 index);

BOOL ov01_02203DF8(UnkOv01_02203A18_Anim *anim);

BOOL ov01_02203DF8(UnkOv01_02203A18_Anim *anim) {
    anim->unk24++;
    if (anim->unk24 >= anim->sequence.startFrame[anim->unk26]) {
        anim->unk26++;
        anim->unk24 = 0;
        if (anim->unk26 >= anim->sequence.seqCount) {
            return TRUE;
        }
        GF_CreateNewVramTransferTask(NNS_GFD_DST_3D_TEX_VRAM, anim->unk1C, sub_02020838(anim->unk10, anim->sequence.textureIdx[anim->unk26]), anim->unk20);
    }
    return FALSE;
}
