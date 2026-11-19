#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "bg_window.h"
#include "font.h"
#include "font_types_def.h"
#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "text.h"
#include "unk_0200A090.h"

struct OverlayManager;
typedef u32 TextColor;
void ov37_021E7478(Window *param0, int param1, u32 param2, UnkStruct_02095EAC *param3);

void ov37_021E6418(UnkStruct_02095EAC *param0, OverlayManager *appMan);
void ov37_021E6540(UnkStruct_02095EAC *param0);
void ov37_021E657C(Sprite *param0, int param1, int param2);

void ov37_021E6418(UnkStruct_02095EAC *param0, OverlayManager *appMan) {
    AddWindowParameterized(param0->unk_00, &param0->unk_33C, 0, 2, 1, 27, 4, 13, 1 + (18 + 12) + 9);
    FillWindowPixelBuffer(&param0->unk_33C, 0xf0f);
    AddWindowParameterized(param0->unk_00, &param0->unk_32C, 1, 1, 2, 30, 15, 0, 1);
    FillWindowPixelBuffer(&param0->unk_32C, 0x202);
    AddWindowParameterized(param0->unk_00, &param0->unk_34C, 1, 25, 21, 7, 2, 13, 1 + 30 * 15);
    FillWindowPixelBuffer(&param0->unk_34C, 0x0);

    {
        u32 v0 = FontID_String_GetCenterAlignmentX(FONT_MESSAGE, param0->unk_28, 0, (7 - 1) * 8) + 2;
        AddTextPrinterParameterizedWithColor(&param0->unk_34C, FONT_MESSAGE, param0->unk_28, v0, 0, 0, ((TextColor)(((7 & 0xFF) << 16) | ((1 & 0xFF) << 8) | ((0 & 0xFF) << 0))), 0L);
    }

    {
        int v1;

        for (v1 = 0; v1 < 5; v1++) {
            AddWindowParameterized(param0->unk_00, &param0->unk_2DC[v1], 4, 24 / 8 + 2, 32 / 8 + v1 * 4 - 1, 10, 2, 13, 1 + v1 * (10 * 2));
            FillWindowPixelBuffer(&param0->unk_2DC[v1], 0);
        }

        ov37_021E7478(param0->unk_2DC, 0, ((TextColor)(((14 & 0xFF) << 16) | ((13 & 0xFF) << 8) | ((15 & 0xFF) << 0))), param0);
    }
}

void ov37_021E6540(UnkStruct_02095EAC *param0) {
    int v0;

    for (v0 = 0; v0 < 5; v0++) {
        RemoveWindow(&param0->unk_2DC[v0]);
    }

    RemoveWindow(&param0->unk_34C);
    RemoveWindow(&param0->unk_32C);
    RemoveWindow(&param0->unk_33C);
}

void ov37_021E657C(Sprite *param0, int param1, int param2) {
    VecFx32 v0;

    v0.x = ((fx32)(((param1) > 0) ? ((param1) * (1 << 12) + 0.5f) : ((param1) * (1 << 12) - 0.5f)));
    v0.y = ((fx32)(((param2 - 8) > 0) ? ((param2 - 8) * (1 << 12) + 0.5f) : ((param2 - 8) * (1 << 12) - 0.5f)));
    v0.z = 0;

    Sprite_SetMatrix(param0, &v0);
}
