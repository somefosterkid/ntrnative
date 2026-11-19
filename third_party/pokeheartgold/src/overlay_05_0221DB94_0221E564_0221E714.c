#include "global.h"

#include "bg_window.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "pm_string.h"
#include "string_util.h"
#include "unk_0200CE7C.h"

void ov05_0221E714(UnkStruct_ov10_0221FB28 *arg0, MsgData *arg1, MessagePrinter *arg2, MessageFormat *arg3, String *arg4, u32 arg5);

void ov05_0221E714(UnkStruct_ov10_0221FB28 *arg0, MsgData *arg1, MessagePrinter *arg2, MessageFormat *arg3, String *arg4, u32 arg5) {
    FontSpecialChars_DrawPartyScreenText(arg2, 1, (u32)arg0->unk_214[arg5].unk_0C, 3, PRINTING_MODE_LEFT_ALIGN, &arg0->unk_10[arg5 * 4 + 1], 0, 2);
    return;
}
