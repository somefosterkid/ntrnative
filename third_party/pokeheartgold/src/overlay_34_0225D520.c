#include "global.h"

#include "msgdata.h"
#include "overlay_01.h"
#include "overlay_34.h"
#include "pm_string.h"
#include "text.h"

void ov34_0225D520(Ov34Work *work) {
    String *string;

    string = NewString_ReadMsgData(work->msgData, 0xDB);
    AddTextPrinterParameterizedWithColor(&work->titleWindow, FONT_SYSTEM, string, 0U, 0U, 0U, 0xF0200U, NULL);
    String_Delete(string);
}

void ov34_0225D558(Ov34Work *work, BgConfig *bgConfig) {
    if ((0x400 & gSystem.newKeys) && (ov01_021F6B10(work->fieldSystem) == 1)) {
        work->state = 3;
    }
    ov34_0225E58C(work);
    ov34_0225E4F8(work);
    ov34_0225DE04(work);
    ov34_0225E164(work);
    ov34_0225DD04(work);
    ov34_0225E630(work);
}
