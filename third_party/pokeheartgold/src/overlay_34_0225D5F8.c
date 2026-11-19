#include "global.h"

#include "gf_gfx_loader.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_34.h"

void ov34_0225D5F8(Ov34Work *work) {
    work->needsRedraw = 0;
    work->count = 0;
    work->prevCount = 0;
    work->scroll = 0;
    work->scrollbarVisible = 0;
    work->msgFormat = MessageFormat_New(HEAP_ID_FIELD1);
    work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x2E2, HEAP_ID_FIELD1);
    work->scrnBuf = GfGfxLoader_GetScrnData(NARC_a_0_7_3, 3, 1, &work->scrnData, HEAP_ID_FIELD1);
}
