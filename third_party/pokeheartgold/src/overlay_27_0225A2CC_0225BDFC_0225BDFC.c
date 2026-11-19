#include "global.h"

#include "constants/heap.h"

#include "overlay_2/overlay_02_gear_phone.h"

#include "bg_window.h"
#include "field_types_def.h"
#include "font.h"
#include "pm_string.h"
#include "sprite.h"
#include "unk_02092BE8.h"
#include "overlay_27_ov27task.h"

void ov27_0225B630(Window *, s32);

void ov27_0225BB38(Window *, String *, s32);

void ov27_0225BEB0(Ov27PhoneIcon *);

void ov27_0225BDFC(Ov27PhoneIcon *param_1);

void ov27_0225BDFC(Ov27PhoneIcon *param_1) {
    BOOL BVar1;
    String *string;
    int width;
    int iVar3;
    BVar1 = GearPhoneRingManager_IsRinging(param_1->gearPhone);
    if (param_1->unk_1C == 0) {
        if (BVar1 == 1 && param_1->unk_1C == 0) {
            string = GetPhoneBookEntryName(param_1->gearPhone, HEAP_ID_8);
            width = FontID_String_GetWidth(FONT_SYSTEM, string, 0);
            iVar3 = width / 8 + 2;
            if (9 < iVar3) {
                iVar3 = 9;
            }
            AddWindowParameterized(param_1->bgConfig, &param_1->window, 5, 0xb, 0x13, (u8)iVar3, 2, 4, 0xc0);
            ov27_0225B630(&param_1->window, 1);
            ov27_0225BB38(&param_1->window, string, 1);
            String_Delete(string);
            Sprite_TryChangeAnimSeq(param_1->sprite, 5);
            param_1->unk_1C = 1;
        }
    } else {
        if (BVar1 != 0) {
            Sprite_TryChangeAnimSeq(param_1->sprite, 5);
            return;
        }
        Sprite_TryChangeAnimSeq(param_1->sprite, 0);
        ov27_0225BEB0(param_1);
        param_1->unk_1C = 0;
    }
}
