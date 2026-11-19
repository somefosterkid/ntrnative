#include "global.h"

#include "camera.h"
#include "gf_gfx_planes.h"
#include "pokepic.h"
#include "unk_02016EDC.h"
#include "unk_0208DE40.h"
#include "pokemon_summary_app.h"

void sub_0208DE40(PokemonSummary *arg0) {
    NNS_G3dInit();
    G3X_Init();
    G3X_InitMtxStack();
    reg_G3X_DISP3DCNT &= 0xFFFFCFFD;
    reg_G3X_DISP3DCNT &= 0xCFFB;
    reg_G3X_DISP3DCNT = 8 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    reg_G3X_DISP3DCNT = 0x10 | (reg_G3X_DISP3DCNT & 0xFFFFCFFF);
    reg_G3X_DISP3DCNT &= 0xCFDF;
    G3X_SetFog(0, GX_FOGBLEND_COLOR_ALPHA, GX_FOGSLOPE_0x8000, 0);
    G3X_SetClearColor(0U, 0, 0x7FFF, 0x3F, 0);
    reg_G3_SWAP_BUFFERS = 0;
    reg_G3_VIEWPORT = 0xBFFF0000;
    GfGfx_EngineATogglePlanes(1U, 1U);
    reg_G2_BG0CNT = 2 | (reg_G2_BG0CNT & ~3);
}

void sub_0208DEDC(PokemonSummary *arg0) {
    NNS_G2dSetupSoftwareSpriteCamera();
    PokepicManager_DrawAll(arg0->pokepicManager);
    reg_G3_SWAP_BUFFERS = 1;
}

s32 sub_02017088(PokepicAnimManager *, u8);

void sub_0208DEFC(PokemonSummary *arg0) {
    Camera_Delete(arg0->camera);
    sub_02017088(arg0->pokepicAnimManager, 0);
    sub_02016F2C(arg0->pokepicAnimManager);
    PokepicManager_Delete(arg0->pokepicManager);
}
