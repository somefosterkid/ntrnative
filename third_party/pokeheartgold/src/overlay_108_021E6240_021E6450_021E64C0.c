#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "pm_string.h"
#include "safari_zone.h"
#include "sprite_system.h"
#include "system.h"
#include "unk_02005D10.h"
#include "yes_no_prompt.h"
#include "overlay_108_safari_area_customizer.h"

// one layout for the Safari area customizer app (OverlayManager data, 0x184EC bytes: SafariAreaCustomizer_Init);
// every member's offset is used by ov108_021E5F38 / 021E64C0 / 021E7F7C / 021E81A8 (or 021E6450 / 021E7EB0 in src)
int ov108_021E6450(SafariAreaCustomizer *data);

void ov108_021E78C0(SafariAreaCustomizer *param_1, s32 param_2, s32 param_3, s32 param_4);

void ov108_021E78F4(SafariAreaCustomizer *arg0, s32 arg1, u32 arg2);

void ov108_021E7EB0(SafariAreaCustomizer *base);

s32 ov108_021E64C0(SafariAreaCustomizer *arg0);

s32 ov108_021E64C0(SafariAreaCustomizer *arg0) {
    s32 sp4;
    u32 sp0;
    u8 var_r4;
    u8 var_r6;

    sp0 = arg0->cursor;
    if (2 & gSystem.newKeys) {
        PlaySE(0x5DC);
        return 2;
    }
    if (1 & gSystem.newKeys) {
        if (sp0 >= 6U) {
            PlaySE(0x5DC);
            return 2;
        }
        if (sp0 + arg0->page * 6 == arg0->areaSet.areas[arg0->areaIdx].area_no) {
            return 0;
        }
        return ov108_021E6450(arg0);
    }
    if (!(0xF0 & gSystem.newKeys)) {
        return 0;
    }
    var_r6 = (s32)sp0 % 3;
    var_r4 = (s32)sp0 / 3;
    sp4 = 0;
    if ((0x10 & gSystem.newKeys) && var_r4 < 2) {
        if (var_r6 == 2) {
            if (var_r4 == 0) {
                var_r4 ^= 1;
                var_r6 = (var_r6 + 1) % 3;
            } else if (var_r4 < 2 && arg0->page < 1) {
                sp4 = 1;
                arg0->unk_184E2_1 = 0;
                var_r4 ^= sp4;
                var_r6 = (var_r6 + 1) % 3;
            }
        } else {
            var_r6 = (var_r6 + 1) % 3;
        }
    } else if ((0x20 & gSystem.newKeys) && var_r4 < 2) {
        if (var_r6 == 0) {
            if (var_r4 == 1) {
                var_r4 ^= 1;
                var_r6 = (var_r6 + 2) % 3;
            } else if (var_r4 < 2 && arg0->page != 0) {
                sp4 = 1;
                arg0->unk_184E2_1 = 1;
                var_r4 ^= sp4;
                var_r6 = (var_r6 + 2) % 3;
            }
        } else {
            var_r6 = (var_r6 + 2) % 3;
        }
    } else if (0x40 & gSystem.newKeys) {
        var_r4 = (var_r4 + 2) % 3;
    } else if (0x80 & gSystem.newKeys) {
        var_r4 = (var_r4 + 1) % 3;
    }
    arg0->cursor = var_r6 + var_r4 * 3;
    ov108_021E78F4(arg0, 1, arg0->cursor);
    if (sp4 != 0) {
        PlaySE(0x5E1);
        ov108_021E78C0(arg0, 1, 0, 0);
        return 3;
    }
    if (sp0 != arg0->cursor) {
        PlaySE(0x5E5);
    }
    ov108_021E7EB0(arg0);
    return 0;
}
