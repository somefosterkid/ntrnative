#include "global.h"

#include "error_handling.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_96_text_box.h"

u32 ov96_021EE830(Ov96TextBox *arg0);

u32 ov96_021EE830(Ov96TextBox *arg0) {
    s32 printerId;

    printerId = arg0->printerId;
    if (printerId != -1) {
        if (TextPrinterCheckActive((u8)printerId) == 0) {
            arg0->counter = 0x14;
            String_Delete(arg0->string);
            arg0->string = NULL;
            arg0->printerId = -1;
        }
        arg0->busy = TRUE;
    } else {
        if (printerId != -1) {
            GF_AssertFail();
        }
        if (arg0->counter != 0) {
            if (arg0->unk_28 == 0) {
                arg0->busy = FALSE;
            } else {
                arg0->counter--;
                arg0->busy = (arg0->counter != 0);
            }
        } else {
            arg0->busy = FALSE;
        }
    }
    return arg0->busy;
}
