#include "global.h"

#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"

void ov49_0225C78C(UnkStruct_ov70_0225FA84 *arg0, UnkStruct_ov70_0225E4EC *arg1);

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
    s16 unk_04;
    s16 unk_06;
} UnkStruct_ov70_0226D844;
extern u32 ov49_02269764;
extern u32 ov49_022699AC;
UnkStruct_ov70_0226D844 *ov49_0225C828(s32 param0, s32 param1, s32 param2, u32 param3);

void ov49_0225C78C(UnkStruct_ov70_0225FA84 *arg0, UnkStruct_ov70_0225E4EC *arg1) {
    SpriteResource *plttResObj;
    s32 v1;
    s32 v3;

    v3 = 0;
    do {
        if (arg0->unk_BC[v3] != NULL) {
            Sprite_Delete(arg0->unk_7C[v3]);
            arg0->unk_7C[v3] = NULL;
            SpriteTransfer_DeleteCharTransferTask(arg0->unk_BC[v3]);
            DestroySingle2DGfxResObj(*((GF_2DGfxResMan * *)&arg1->unk_194[0]), arg0->unk_BC[v3]);
            arg0->unk_BC[v3] = NULL;
        }
        v3 += 1;
    } while (v3 < 0xC);
    v1 = 0;
    do {
        plttResObj = arg0->unk_AC[v1];
        if (plttResObj != NULL) {
            SpriteTransfer_DeletePlttTransferTask(plttResObj);
            DestroySingle2DGfxResObj(*((GF_2DGfxResMan * *)&arg1->unk_194[1]), arg0->unk_AC[v1]);
            DestroySingle2DGfxResObj(*((GF_2DGfxResMan * *)&arg1->unk_194[2]), arg0->unk_EC[v1]);
            DestroySingle2DGfxResObj(*((GF_2DGfxResMan * *)&arg1->unk_194[3]), arg0->unk_FC[v1]);
            arg0->unk_AC[v1] = NULL;
        }
        v1 += 1;
    } while (v1 < 4);
}

UnkStruct_ov70_0226D844 *ov49_0225C828(s32 param0, s32 param1, s32 param2, u32 param3) {
    if (param1 == param2) {
        return (UnkStruct_ov70_0226D844 *)(&ov49_02269764 + param3 * 2);
    }
    return (UnkStruct_ov70_0226D844 *)(&ov49_022699AC + param0 * 2);
}
