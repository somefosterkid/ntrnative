#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "render_window.h"
#include "screen_fade.h"
#include "text.h"
#include "unk_02005D10.h"
#include "overlay_71_trade_receive_phase.h"
#include "overlay_71_trade_sequence_data.h"

void *ov71_02247388(TradeSequenceData *);

MsgData *ov71_0224738C(TradeSequenceData *);

s32 ov71_022473DC(TradeSequenceData *);

s32 ov71_0224B6F0(TradeReceivePhase *, s32);

void sub_0200E948(Window *, u32, u32);

u32 ov71_0224B57C(TradeReceivePhase *param_1, s32 *param_2);

u32 ov71_0224B57C(TradeReceivePhase *param_1, s32 *param_2) {
    MsgData *pMVar1;
    MessageFormat *pMVar2;
    s32 iVar3;
    BOOL BVar4;
    s32 strno;

    switch (*param_2) {
    case 0:
        param_1->timer = 0;
        *param_2 = *param_2 + 1;
        break;
    case 1:
        iVar3 = param_1->timer;
        param_1->timer = iVar3 + 1;
        if (10 < iVar3 + 1) {
            pMVar1 = ov71_0224738C(param_1->sequenceData);
            pMVar2 = ov71_02247388(param_1->sequenceData);
            iVar3 = ov71_022473DC(param_1->sequenceData);
            if (iVar3 == 1) {
                strno = 2;
                iVar3 = 2;
            } else {
                strno = 5;
                iVar3 = 3;
            }
            ReadMsgDataIntoString(pMVar1, strno, param_1->string0);
            StringExpandPlaceholders(pMVar2, param_1->string1, param_1->string0);
            AddTextPrinterParameterized(&param_1->window, FONT_MESSAGE, param_1->string1, 0, 0, 0xff, (PrinterCallback_t)0x0);
            sub_0200E948(&param_1->window, 0x6d, 2);
            CopyWindowToVram(&param_1->window);
            PlayFanfare(0x4a4);
            param_1->timer = 0;
            param_1->fanfarePlaying = 1;
            *param_2 = iVar3;
        }
        break;
    case 2:
        iVar3 = ov71_0224B6F0(param_1, 0x3c);
        if (iVar3 != 0) {
            pMVar1 = ov71_0224738C(param_1->sequenceData);
            pMVar2 = ov71_02247388(param_1->sequenceData);
            ReadMsgDataIntoString(pMVar1, 3, param_1->string0);
            StringExpandPlaceholders(pMVar2, param_1->string1, param_1->string0);
            FillWindowPixelBuffer(&param_1->window, 0xf);
            AddTextPrinterParameterized(&param_1->window, FONT_MESSAGE, param_1->string1, 0, 0, 0xff, (PrinterCallback_t)0x0);
            CopyWindowPixelsToVram_TextMode(&param_1->window);
            param_1->timer = 0;
            *param_2 = *param_2 + 1;
        }
        break;
    case 3:
        iVar3 = ov71_0224B6F0(param_1, 0x3c);
        if (iVar3 != 0) {
            ClearFrameAndWindow2(&param_1->window, 0);
            param_1->timer = 0;
            *param_2 = *param_2 + 1;
        }
        break;
    case 4:
        iVar3 = param_1->timer;
        param_1->timer = iVar3 + 1;
        if (10 < iVar3 + 1) {
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 0x10, 1, HEAP_ID_57);
            *param_2 = *param_2 + 1;
        }
        break;
    case 5:
        BVar4 = IsPaletteFadeFinished();
        if (BVar4 != 0) {
            return 1;
        }
    }
    return 0;
}
