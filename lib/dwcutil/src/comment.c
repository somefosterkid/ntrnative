// libdwcutil's comment.c (Black's ov016 0x021bd04c-0x021bd170): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"

static u8 data_ov016_021de50c;
void DWCi_COMMENTlInit(void);

void DWCi_COMMENTlInit(void) {
    data_ov016_021de50c = 0;
}

u32 DWCi_COMMENTlDispEx(u32 messageId, s32 n, s32 num) {
    NNSG2dCharCanvas *canvas;
    u16 *msg;
    u32 fontFlag;
    if (data_ov016_021de50c != 0) {
        return 0;
    }
    DWCi_CMNlLoadResource("char/jtNull.nsc.l", (u8 *)GXS_LoadBG0Scr);
    reg_G2S_DB_BG0OFS = 0x1920000;
    canvas = DWCi_FNTlFormBgEx(1, 0);
    msg = DWCi_MSGlGetEx(data_ov016_021de598, messageId, n, num);
    fontFlag = DWCi_CMNlFontFlag();
    DWCi_FNTlDrawRect(canvas, 0x14, 0, 0xd8, 0x40, 2, fontFlag, msg);
    DWCi_FNTlRenewBg((s32)canvas);
    data_ov016_021de50c = 1;
    return 1;
}

u32 DWCi_COMMENTlDelete(void) {
    if (data_ov016_021de50c != 0) {
        DWCi_FNTlDeleteBgLcd(1);
        data_ov016_021de50c = 0;
        return 1;
    }
    return 0;
}
