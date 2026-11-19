#include "global.h"

#include "gf_gfx_planes.h"
#include "heap.h"
#include "screen_fade.h"
#include "system.h"
#include "unk_020210A0.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "overlay_103_mail_box_work.h"
#include "overlay_103_mail_box_view.h"

void ov103_021EC9B4(void *);

s32 ov103_021EC9D8(void);

s32 ov103_021EC9E8(void *);

s32 ov103_021ECC1C(void *);

s32 ov103_021ECD18(void *);

s32 ov103_021ECD68(void *);

s32 ov103_021ECE80(void *);

s32 ov103_021ECEEC(void *);

s32 ov103_021ED00C(void *);

s32 ov103_021ED0C0(void *);

s32 ov103_021EDEA8(void *);

s32 ov103_021EE390(void *);

s32 ov103_021EE550(void *);

s32 ov103_021EE644(void *);

s32 ov103_021EEA48(void *);

s32 ov103_021ED314(MailBoxWork *arg);

s32 ov103_021ED47C(MailBoxWork *arg0);

s32 ov103_021ED314(MailBoxWork *arg) {
    MailBoxView *view;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_EngineASetPlanes(0U);
    GfGfx_EngineBSetPlanes(0U);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    EnableTouchPad();
    InitializeTouchPad(4);
    reg_GX_POWCNT &= 0xFFFF7FFF;
    Heap_Create(HEAP_ID_3, HEAP_ID_157, 0x60000U);
    view = Heap_Alloc(HEAP_ID_157, sizeof(MailBoxView));
    arg->view = view;
    MI_CpuFill8(view, 0U, sizeof(MailBoxView));
    ov103_021ED00C(arg);
    ov103_021EC9D8();
    ov103_021EC9E8(arg);
    ov103_021ECC1C(arg);
    ov103_021ECD18(arg);
    ov103_021EE390(arg);
    ov103_021ECD68(arg);
    ov103_021EDEA8(arg);
    ov103_021ECE80(arg);
    ov103_021EEA48(arg);
    ov103_021ECE18(arg->view);
    ov103_021EE550(arg);
    ov103_021EE644(arg);
    ov103_021ED0C0(arg);
    ov103_021ECEEC(arg);
    Main_SetVBlankIntrCB(ov103_021EC9B4, arg);
    return arg->nextState;
}

s32 ov103_021ECBBC(void *);

s32 ov103_021ECE0C(void *);

s32 ov103_021ECE94(void *);

s32 ov103_021ECF48(void *);

s32 ov103_021ED0A0(void *);

s32 ov103_021EDF50(void *);

s32 ov103_021EE3C0(void *);

s32 ov103_021EEAA0(void *);

s32 ov103_021ED3E8(MailBoxWork *arg0);

s32 ov103_021ED3E8(MailBoxWork *arg0) {
    Main_SetVBlankIntrCB(NULL, NULL);
    ov103_021ECF48(arg0);
    ov103_021ECE54(arg0->view);
    ov103_021EEAA0(arg0);
    ov103_021ECE94(arg0);
    ov103_021EDF50(arg0);
    ov103_021ECE0C(arg0);
    ov103_021EE3C0(arg0);
    ov103_021ECBBC(arg0);
    ov103_021ED0A0(arg0);
    DisableTouchPad();
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    GfGfx_EngineASetPlanes(0U);
    GfGfx_EngineBSetPlanes(0U);
    Heap_Free(arg0->view);
    arg0->view = NULL;
    Heap_Destroy(HEAP_ID_157);
    return arg0->nextState;
}

s32 ov103_021ED460(MailBoxWork *arg0);

s32 ov103_021ED460(MailBoxWork *arg0) {
    if (IsPaletteFadeFinished() == 1) {
        return arg0->view->wipeNextState;
    }
    return 2;
}

s32 ov103_021ED47C(MailBoxWork *arg0) {
    if (TextPrinterCheckActive(arg0->view->textPrinterId) == 0) {
        if (arg0->view->waitForInput == 0) {
            return arg0->nextState;
        }
        if (System_GetTouchNew() == 1) {
            PlaySE(0x5DCU);
            return arg0->nextState;
        }
        if (3 & gSystem.newKeys) {
            PlaySE(0x5DCU);
            return arg0->nextState;
        }
        goto error;
    }
error:
    return 3;
}
