#include "global.h"
struct UnkStruct_ov69_0225D35C;
#include "gf_3d_vramman.h"
#include "overlay_48_unk_struct_ov69_0225_d35_c.h"

void ov48_02259798(void);

void ov48_02259750(struct UnkStruct_ov69_0225D35C *arg0, enum HeapID arg1);

void ov48_02259750(UnkStruct_ov69_0225D35C *arg0, enum HeapID arg1) {
    *((GF3DVramMan * *)&arg0->unk_1A4) = GF_3DVramMan_Create(arg1, 0, 2, 0, 4, ov48_02259798);
    NNS_G3dGlbLightVector(GX_LIGHTID_0, 0, 0, 0xFFFFF001);
}
