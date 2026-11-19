#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_108_safari_area_customizer.h"

extern const u32 ov108_021EA724[2];

void ov108_021E7C5C(SafariAreaCustomizer *arg0);

void ov108_021E7C5C(SafariAreaCustomizer *arg0) {
    s32 i;
    Window *w;

    if (arg0->unk_184E3 != 0) {
        for (i = 0; i < 5; i++) {
            w = &arg0->windows[5 + i * 2];
            ReadMsgDataIntoString(arg0->msgData, i + 10, arg0->string);
            AddTextPrinterParameterizedWithColor(w, FONT_SYSTEM, arg0->string, 0, 0, 0, ov108_021EA724[i % 2], (PrinterCallback_t)0x0);
            CopyWindowToVram(w);
        }
    }
}
