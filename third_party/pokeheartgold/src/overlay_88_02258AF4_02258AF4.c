#include "global.h"
struct UnkStruct_ov69_0225D35C_88;

#include "filesystem.h"
#include "vram_transfer_manager.h"
#include "overlay_88_unk_struct_ov69_0225_d35_c_88.h"

s32 ov88_02258C60(void *);

s32 ov88_02258D38(void *);

void ov88_02258AF4(struct UnkStruct_ov69_0225D35C_88 *arg0);

void ov88_02258AF4(UnkStruct_ov69_0225D35C_88 *arg0) {
    NARC_Delete(*((NARC * *)&arg0->unk_1A4));
    GF_DestroyVramTransferManager();
    ov88_02258C60(arg0);
    ov88_02258D38(arg0);
}
