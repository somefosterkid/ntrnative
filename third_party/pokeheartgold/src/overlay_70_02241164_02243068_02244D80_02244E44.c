#include "global.h"

#include "text.h"
#include "unk_02037C94.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_02238E50(void *, u32, u32);

void ov70_02244FA4(GTSApplicationState *, MsgData *, u32, u32, u32);

u32 ov70_02244E44(GTSApplicationState *param_1, u32 param_2, u32 param_3, u32 param_4);

u32 ov70_02244E44(GTSApplicationState *param_1, u32 param_2, u32 param_3, u32 param_4) {
    u8 uVar1;

    switch (param_1->wfcDisconnectMessageIndex) {
    case 0:
        ov70_02244FA4(param_1, param_1->gtsMessageLoader, 0xba, 1, 0xf0f);
        param_1->wfcDisconnectMessageIndex = param_1->wfcDisconnectMessageIndex + 1;
        break;
    case 1:
        uVar1 = TextPrinterCheckActive((u8)param_1->textPrinter);
        if (uVar1 == 0) {
            sub_0203946C();
            DWC_CleanupInet();
            param_1->wfcDisconnectMessageIndex = param_1->wfcDisconnectMessageIndex + 1;
        }
        break;
    case 2:
        ov70_02244FA4(param_1, param_1->gtsMessageLoader, 0xbb, 1, 0xf0f);
        param_1->wfcDisconnectMessageIndex = param_1->wfcDisconnectMessageIndex + 1;
        break;
    case 3:
        uVar1 = TextPrinterCheckActive((u8)param_1->textPrinter);
        if (uVar1 == 0) {
            param_1->wfcDisconnectMessageIndex = param_1->wfcDisconnectMessageIndex + 1;
        }
        break;
    default:
        param_1->wfcDisconnectMessageFrameDelay = param_1->wfcDisconnectMessageFrameDelay + 1;
        if (0x1e < param_1->wfcDisconnectMessageFrameDelay) {
            ov70_02238E50(param_1, 0, 0);
            param_1->currentScreenInstruction = 0xb;
        }
    }
    return 3;
}
