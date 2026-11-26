#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"
#include "struct_defs/resdat_narc_structs.h"

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
};

extern u32 ov88_02259944;

void ov88_02258C60(UnkStruct_ov69_0225D35C *param0);

void ov88_02258C60(UnkStruct_ov69_0225D35C *param0) {
    s32 iVar1;
    u32 *puVar2;

    puVar2 = (u32 *)&ov88_02259944;
    iVar1 = 0;
    do {
        FreeBgTilemapBuffer(param0->unk_00, (u8)*puVar2);
        iVar1 = iVar1 + 1;
        puVar2 = puVar2 + 1;
    } while (iVar1 < 5);
    Heap_Free(param0->unk_00);
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    return;
}
