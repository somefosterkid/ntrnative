#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "bg_window.h"
#include "error_handling.h"
#include "font.h"
#include "player_data.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_96_app_0221040c.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
ManagedSprite *ov96_022104D8(SpriteSystem *, SpriteManager *, s16, u8, u16, u16);
#else
ManagedSprite *ov96_022104D8(SpriteSystem *, SpriteManager *, s32, s32, s32, s32);
#endif

extern s16 ov96_0221D14C[][2];

void ov96_0221069C(Ov96App_0221040C *arg0) {
    String *sp20;
    s16 sp1C;
    s32 sp18;
    s32 sp14;
    s32 i;
    String *temp_r0_2;
    ManagedSprite *temp_r0_3;
    s32 var_r4;
    u8 var_r7;

    sp1C = 0;
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    var_r4 = 0;
    i = 0;
    do {
        if (var_r4 == arg0->unk_1C) {
            sp18 = 8;
            var_r7 = 0;
            sp14 = 0x48;
        } else {
            var_r7 = sp1C + 1;
            sp18 = ov96_0221D14C[sp1C][0];
            sp14 = ov96_0221D14C[sp1C][1];
            sp1C = (s16)var_r7;
        }
        temp_r0_2 = PlayerProfile_GetPlayerName_NewString(PokeathlonCourse_GetPlayerProfileFromData(arg0->course, var_r4), arg0->heapId);
        sp20 = temp_r0_2;
        FillWindowPixelBuffer(&arg0->windows[var_r7], 0U);
        AddTextPrinterParameterizedWithColor(&arg0->windows[var_r7], FONT_SYSTEM, sp20, 0U, 0U, 0xFFU, 0xF0E00U, NULL);
        CopyWindowToVram(&arg0->windows[var_r7]);
        String_Delete(temp_r0_2);
        temp_r0_3 = ov96_022104D8(arg0->spriteSystem, arg0->spriteManager, sp18, sp14, (s32)(u16)var_r4, 0x41);
        var_r4 += 1;
        arg0->sprites[i + 3] = temp_r0_3;
        i += 1;
    } while (var_r4 < 4);
}
