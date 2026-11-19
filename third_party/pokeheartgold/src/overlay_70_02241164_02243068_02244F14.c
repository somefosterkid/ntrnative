#include "global.h"

#include "text.h"
#include "overlay_70_gtsapplication_state.h"

s32 ov70_02244F14(GTSApplicationState *arg0);

s32 ov70_02244F14(GTSApplicationState *arg0) {
    if (TextPrinterCheckActive((u8) * &arg0->textPrinter) == 0) {
        arg0->currentScreenInstruction = (s32) * &arg0->nextScreenInstruction;
    }
    return 3;
}
