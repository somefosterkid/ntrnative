#include "global.h"

#include "gf_gfx_planes.h"
#include "system.h"
#include "bg_window.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

void ov40_0222BC44(u8 arg0);

extern BgTemplate ov40_02244D00;
void ov40_0222BC54(UnkStruct_0208C06C *arg0);

void ov40_0222BC44(u8 arg0) {
    gSystem.screensFlipped = arg0;
    GfGfx_SwapDisplay();
}

void ov40_0222BC54(UnkStruct_0208C06C *arg0) {
    InitBgFromTemplate(arg0->unk_14.unk_10, 2U, &ov40_02244D00, 0U);
}
