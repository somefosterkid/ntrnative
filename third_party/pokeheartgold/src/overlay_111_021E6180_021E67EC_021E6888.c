#include "global.h"

#include "overlay_111.h"
#include "pm_string.h"
#include "text.h"

BOOL ov111_021E6888(SwapMonText *text) {
    if (text->printerId != -1) {
        if (TextPrinterCheckActive(text->printerId) == 0) {
            text->timer = 0;
            String_Delete(text->string);
            text->string = NULL;
            text->printerId = -1;
        }
        return TRUE;
    }
    if (text->timer != 0) {
        if (text->skipDelay == 1) {
            text->timer = 0;
            text->skipDelay = 0;
        } else {
            text->timer--;
            return TRUE;
        }
    }
    return FALSE;
}
