#include "global.h"

#include "sys_task.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite_system.h"

struct BgConfig;
struct Camera;
struct G3DPipelineBuffers;
struct NARC;
struct PaletteData;
struct SpriteManager;
struct SpriteSystem;
void Heap_Free(void *ptr);
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

void ov92_0225EB68(void *ptr);
BOOL ov92_0225EB70(UnkStruct_ov116_02262A44 *param0);

void ov92_0225EB68(void *ptr) {
    Heap_Free(ptr);
}

BOOL ov92_0225EB70(UnkStruct_ov116_02262A44 *param0) {
    if ((--param0->unk_08) <= 0) {
        return 0;
    }

    return 1;
}
