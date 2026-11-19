#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct FontOAM;

void FontOAM_Delete(FontOAM *param0);

void ov93_02261FB0(UnkStruct_ov117_02265EB0 *param0);

void ov93_02261FB0(UnkStruct_ov117_02265EB0 *param0) {
    FontOAM_Delete(param0->unk_00);
    sub_02021B5C(&param0->unk_04);
    param0->unk_00 = NULL;
}

void sub_020136B4(FontOAM *fontOAM, int x, int y);

void sub_02013794(const FontOAM *fontOAM, int *x, int *y);

void ov93_02261FC8(UnkStruct_ov117_02265EC8 *param0);

void ov93_02261FC8(UnkStruct_ov117_02265EC8 *param0) {
    int v0, v1;
    int v2 = 16 * 5;
    int v3, v4, v5, v6;

    for (v0 = 0; v0 < 6; v0++) {
        v1 = param0->unk_180[v0];

        if (v1 < v2 - 16) {
            v5 = v2 - 16 - v1;
            v6 = v5 - v2;
        } else if (v1 < 160 - 32) {
            v6 = v2 - 16 - (v1 - v2);
            v5 = v6 - v2;
        } else {
            v5 = v2 - 16 - (v1 - 160);
            v6 = v5 - v2;
        }

        sub_02013794(param0->unk_04[v0][0].unk_00, &v3, &v4);
        sub_020136B4(param0->unk_04[v0][0].unk_00, v3, 168 - v5);
        sub_020136B4(param0->unk_04[v0][1].unk_00, v3, 168 - v6);
    }
}
