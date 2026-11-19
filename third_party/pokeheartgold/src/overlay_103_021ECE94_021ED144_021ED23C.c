#include "global.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "mail.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021ED23C(MailBoxWork *arg0);

void ov103_021ED23C(MailBoxWork *arg0) {
    NARC *temp_r4;
    u8 temp_r0;

    temp_r4 = NARC_New(NARC_a_0_7_9, HEAP_ID_157);
    temp_r0 = Mail_GetType(arg0->view->mails[arg0->mailIndex]);
    GfGfxLoader_LoadCharDataFromOpenNarc(temp_r4, temp_r0 + 0xC, arg0->view->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_157);
    GfGfxLoader_GXLoadPalFromOpenNarc(temp_r4, (s32)temp_r0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40U, HEAP_ID_157);
    GfGfxLoader_LoadScrnDataFromOpenNarc(temp_r4, temp_r0 + 0x18, arg0->view->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_157);
    NARC_Delete(temp_r4);
}
