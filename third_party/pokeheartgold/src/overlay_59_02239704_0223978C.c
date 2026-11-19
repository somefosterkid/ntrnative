#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"

void ov59_0223978C(Ov59ApricornApp *arg0);

void ov59_0223978C(Ov59ApricornApp *arg0) {
    s32 i;

    for (i = 0; i < 7; i++) {
        String_Delete(arg0->apricornNames[i]);
    }
    for (i = 0; i < 0x16; i++) {
        String_Delete(arg0->unk_78[i]);
    }
    String_Delete(arg0->unk_74);
    String_Delete(arg0->unk_6C);
    String_Delete(arg0->unk_68);
    String_Delete(arg0->unk_64);
    MessageFormat_Delete(arg0->msgFormat);
    DestroyMsgData(arg0->msgData);
    FontID_Release(FONT_UNK_4);
}
