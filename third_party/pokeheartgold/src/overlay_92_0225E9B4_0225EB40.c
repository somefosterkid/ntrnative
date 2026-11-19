#include "global.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "heap.h"
#include "palette.h"
#include "sprite_system.h"

struct G3DPipelineBuffers;

typedef struct {
    u8 unk_00;
    u8 padding_01[3];
} UnkStruct_ov116_022649E4_sub1;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct {
    NARC *unk_00;
    NARC *unk_04;
    SpriteSystem *unk_08;
    SpriteManager *unk_0C;
    BgConfig *unk_10;
    PaletteData *unk_14;
    G3DPipelineBuffers *unk_18;
    Camera *camera;
    u8 padding_20[4];
    NNSFndAllocator unk_24;
    UnkStruct_ov116_022649E4_sub1 unk_2C;
} UnkStruct_ov116_022649E4;

typedef struct {
    u8 padding_00[4];
    int unk_04;
    s16 unk_08;
    u8 padding_0A;
    UnkStruct_ov116_022649E4 *unk_0C;
} UnkStruct_ov116_02262A44;

UnkStruct_ov116_02262A44 *ov92_0225EB40(int param0, UnkStruct_ov116_022649E4 *param1);

UnkStruct_ov116_02262A44 *ov92_0225EB40(int param0, UnkStruct_ov116_022649E4 *param1) {
    UnkStruct_ov116_02262A44 *v0 = Heap_Alloc(HEAP_ID_113, sizeof(UnkStruct_ov116_02262A44));

    memset(v0, 0, sizeof(UnkStruct_ov116_02262A44));

    v0->unk_04 = param0;
    v0->unk_08 = ((30 * 40) - 0);
    v0->unk_0C = param1;

    return v0;
}
