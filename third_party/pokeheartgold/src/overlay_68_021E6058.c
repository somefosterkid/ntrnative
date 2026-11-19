#include "global.h"

#include "text.h"
#include "overlay_68_move_relearner.h"

s32 ov68_021E6058(MoveRelearner *arg0);

s32 ov68_021E6058(MoveRelearner *arg0) {
    if (TextPrinterCheckActive(arg0->textPrinterId) == 0) {
        return arg0->nextState;
    }
    return 2;
}
