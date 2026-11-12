// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov139/print_msg.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// print_msg.c, a guessed name: prints strings into a window through the print queue, aligned left, right or centered

#include "app/ui/print_msg.h"
#include "types.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "gfl/msg.h"
#include "gfl/str.h"
#include "system/printsys.h"
#include "system/wordset.h"
#include "text/system/btl_main_12.h"

static u16 func_ov067_021b6c4c(const StrBuf *strbuf, Font *font, u32 align);

void func_ov067_021b6ae4(PrintWindow *window, PrintQueue *queue, u16 x, u16 y, const StrBuf *strbuf, Font *font,
                         u16 color, u32 align) {
    x -= func_ov067_021b6c4c(strbuf, font, align);
    func_0201c724(queue, BmpWin_GetBitmap(window->window), x, y, strbuf, font, color);
    window->flushPending = TRUE;
}

// Prints "number1/number2" with the slash centered at x
void func_ov067_021b6b2c(PrintWindow *window, PrintQueue *queue, Font *font, u16 x, u16 y, u16 color, s32 number1,
                         s32 number2, HeapID heapId) {
    MsgData *msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_BTL_MAIN_12, heapId);
    WordSet *wordSet = GFL_WordSetSystemCreateDefault(heapId);
    StrBuf *buf = GFL_StrBufCreate(10, heapId);
    StrBuf *label = GFL_MsgDataLoadStrbufNew(msgData, BtlMain12_Text_000);
    u16 width = GFL_FontGetBlockWidth(label, font, 0);
    StrBuf *numberFormat;

    func_ov067_021b6ae4(window, queue, x, y, label, font, color, PRINT_ALIGN_CENTER);
    x -= func_ov067_021b6c4c(label, font, PRINT_ALIGN_CENTER);
    GFL_StrBufFree(label);

    numberFormat = GFL_MsgDataLoadStrbufNew(msgData, BtlMain12_Text_001);
    WordSetNumber(wordSet, 0, number1, 8, 0, TRUE);
    GFL_WordSetFormatStrbuf(wordSet, buf, numberFormat);
    func_ov067_021b6ae4(window, queue, x, y, buf, font, color, PRINT_ALIGN_RIGHT);
    GFL_StrBufFree(numberFormat);

    numberFormat = GFL_MsgDataLoadStrbufNew(msgData, BtlMain12_Text_002);
    WordSetNumber(wordSet, 0, number2, 8, 0, TRUE);
    GFL_WordSetFormatStrbuf(wordSet, buf, numberFormat);
    func_ov067_021b6ae4(window, queue, x + width, y, buf, font, color, PRINT_ALIGN_LEFT);
    GFL_StrBufFree(numberFormat);

    GFL_StrBufFree(buf);
    GFL_WordSetSystemFree(wordSet);
    GFL_MsgDataFree(msgData);
}

// The offset to subtract from x: the whole width to align right, half of it to center
// pokebw2's GetStrWidthForAlign
static u16 func_ov067_021b6c4c(const StrBuf *strbuf, Font *font, u32 align) {
    if (align == PRINT_ALIGN_RIGHT) {
        return GFL_FontGetBlockWidth(strbuf, font, 0);
    }
    if (align == PRINT_ALIGN_CENTER) {
        return GFL_FontGetBlockWidth(strbuf, font, 0) >> 1;
    }
    return 0;
}
