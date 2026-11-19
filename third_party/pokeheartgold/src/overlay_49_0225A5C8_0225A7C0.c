#include "global.h"
struct UnkStruct_ov70_0225E4EC;

#include "sprite.h"
#include "vram_transfer_manager.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"

void ov49_0225A7C0(struct UnkStruct_ov70_0225E4EC *arg0);

void ov49_0225A7C0(UnkStruct_ov70_0225E4EC *arg0) {
    SpriteList_RenderAndAnimateSprites(arg0->unk_04);
    thunk_UpdateCellTransferStateManager();
}
