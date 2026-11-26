#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "overlay_102_app_021eb1e0.h"

extern WindowTemplate ov102_021EC808;

void ov102_021EB46C(Ov102App_021EB1E0 *arg0, BgConfig *arg1);

void ov102_021EB46C(Ov102App_021EB1E0 *arg0, BgConfig *arg1) {
    s32 i;

    i = 0;
    do {
        AddWindow(arg1, &arg0->unk_24[i], &(&ov102_021EC808)[i]);
        arg0->unk_18[i] = NewString_ReadMsgData(arg0->unk_14, i);
        i++;
    } while ((u32)i < 3U);
    PutWindowTilemap(&arg0->unk_24[0]);
    FillWindowPixelBuffer(&arg0->unk_24[0], 0xEU);
    PutWindowTilemap(&arg0->unk_24[1]);
    FillWindowPixelBuffer(&arg0->unk_24[1], 0xEU);
}
