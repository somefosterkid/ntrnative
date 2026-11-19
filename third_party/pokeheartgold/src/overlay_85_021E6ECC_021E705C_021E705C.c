#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "palette.h"
#include "pm_string.h"
#include "render_window.h"
#include "overlay_85_ov109_021D0F70.h"
#include "unk_struct_0209_c194.h"

s32 ov85_021E730C(void *, s32);

extern WindowTemplate ov85_021EA4FC;

void ov85_021E705C(UnkStruct_ov109_021D0F70 *arg0);

void ov85_021E705C(UnkStruct_ov109_021D0F70 *arg0) {
    UnkStruct_ov109_021D24F8 *temp_r4;
    s32 var_r4;

    temp_r4 = &arg0->unk_C9C;
    LoadUserFrameGfx1(arg0->unk_D84, GF_BG_LYR_SUB_0_F, 1U, 0xFU, 0U, HEAP_ID_102);
    LoadUserFrameGfx2(arg0->unk_D84, GF_BG_LYR_SUB_0_F, 0xAU, 0xEU, arg0->unk_CC->unk_14.unk_04, HEAP_ID_102);
    PaletteData_LoadNarc(arg0->unk_D9C, NARC_a_0_3_8, (s32)sub_0200E640(arg0->unk_CC->unk_14.unk_04), HEAP_ID_102, PLTTBUF_MAIN_BG, 0x20U, 0xE0U);
    PaletteData_LoadNarc(arg0->unk_D9C, NARC_graphic_font, 8, HEAP_ID_102, PLTTBUF_MAIN_BG, 0x20U, 0xF0U);
    temp_r4->unk_04 = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xCE, HEAP_ID_102);
    temp_r4->unk_08 = MessageFormat_New(HEAP_ID_102);
    AddWindow(arg0->unk_D84, &temp_r4->unk_0C[0], &ov85_021EA4FC);
    temp_r4->unk_6C = String_New(0x100U, HEAP_ID_102);
    var_r4 = 0;
    do {
        arg0->unk_2C.unk_6C[var_r4] = String_New(8U, HEAP_ID_102);
        var_r4 += 1;
    } while (var_r4 < 5);
    ov85_021E730C(arg0, arg0->unk_CC->unk_08);
}
