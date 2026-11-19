#include "global.h"

#include "constants/heap.h"

#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_106_motion_blur.h"

struct MotionBlur;
void ov01_021F44B4(MotionBlur **motionBlur, GXDispMode displayMode, GXBGMode bgMode, GXBG0As bg0Mode);
typedef struct FieldMotionBlur {
    MotionBlur *motionBlur;
} FieldMotionBlur;

void ov01_021F46DC(FieldMotionBlur **fieldMotionBlur);

void ov01_021F46DC(FieldMotionBlur **fieldMotionBlur) {
    ov01_021F44B4(&(*fieldMotionBlur)->motionBlur, GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_3D);
    GX_SetBankForBG(GX_VRAM_BG_128_C);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1 | GX_PLANEMASK_BG2 | GX_PLANEMASK_BG3, 1);
    Heap_FreeExplicit(HEAP_ID_FIELD1, *fieldMotionBlur);
}
