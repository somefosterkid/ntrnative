#include "global.h"

#include "constants/heap.h"

#include "library/spl_emitter.h"
#include "library/spl_manager.h"

#include "camera.h"
#include "filesystem_files_def.h"
#include "gf_3d_vramman.h"
#include "heap.h"
#include "overlay_05.h"
#include "unk_02014DA0.h"
#include "particle_system.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "battle/trainer_intro_data.h"

typedef u32 (*SPLTexVRAMAllocFunc)(u32 size, BOOL is4x4comp);

typedef u32 (*SPLPalVRAMAllocFunc)(u32 size, BOOL is4pal);

typedef void (*SPLEmitterCallback)(SPLEmitter *emtr);

void ov05_0221D9F0(UnkStruct_ov10_0221FB28 *arg0);

void ov05_0221D9F0(UnkStruct_ov10_0221FB28 *arg0) {
    arg0->unk_B58 = GF_3DVramMan_Create(arg0->trainerIntroData->heapID, 0, 4, 0, 2, NULL);
    reg_G3X_DISP3DCNT = 8 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    sub_02014DA0();
    arg0->unk_B60 = Heap_Alloc(arg0->trainerIntroData->heapID, 0x4800U);
    arg0->unk_B5C = sub_02014DB4(ov05_0221DB4C, ov05_0221DB70, arg0->unk_B60, 0x4800U, 1, arg0->trainerIntroData->heapID);
    Camera_SetPerspectiveClippingPlane(0x1000, 0x384000, sub_02015524(arg0->unk_B5C));
    sub_0201526C(arg0->unk_B5C, sub_02015264(NARC_a_0_5_9, 2, arg0->trainerIntroData->heapID), 0xAU, 1);
    sub_02015494(arg0->unk_B5C, 0, NULL, NULL);
    sub_02015494(arg0->unk_B5C, 1, NULL, NULL);
    sub_02015494(arg0->unk_B5C, 2, NULL, NULL);
    sub_02015494(arg0->unk_B5C, 3, NULL, NULL);
    sub_02015494(arg0->unk_B5C, 4, NULL, NULL);
}
