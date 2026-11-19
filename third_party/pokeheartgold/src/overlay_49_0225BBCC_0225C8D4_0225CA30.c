#include "global.h"

#include "error_handling.h"
#include "gf_gfx_loader.h"
#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"

void ov49_0225CAD4(UnkStruct_ov70_02260744 *, UnkStruct_ov70_0225E4EC *, u32, NARC *, u32 arg4);

void ov49_0225CA30(UnkStruct_ov70_02260744 *arg0, UnkStruct_ov70_0225E4EC *arg1, u32 arg2, NARC *arg3, enum HeapID arg4);

void ov49_0225CA30(UnkStruct_ov70_02260744 *arg0, UnkStruct_ov70_0225E4EC *arg1, u32 arg2, NARC *arg3, enum HeapID arg4) {
    NARC *sp10;

    sp10 = arg3;
    if (arg2 >= 3U) {
        GF_AssertFail();
    }
    GfGfxLoader_LoadScrnDataFromOpenNarc(sp10, 0x37, arg1->unk_00, GF_BG_LYR_SUB_2_F, 0U, 0U, 0, arg4);
    arg0->unk_08 = 1;
    arg0->unk_0A = (u16)arg2;
    GfGfxLoader_LoadCharDataFromOpenNarc(sp10, arg0->unk_0A + 0x5D, arg1->unk_00, GF_BG_LYR_SUB_2, 0x140U, 0U, 0, arg4);
    if (arg0->unk_02 == 0) {
        ov49_0225CAD4(arg0, arg1, 0, sp10, arg4);
        return;
    }
    ov49_0225CAD4(arg0, arg1, 2, sp10, arg4);
}
