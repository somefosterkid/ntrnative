#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sprite.h"
#include "text.h"

void sub_0202BE60(u32 *, String *);

u16 sub_0202BE98(void *);

void ov41_0224BE80(void **param_1);

void ov41_0224BE80(void **param_1) {
    u16 wordno;
    MsgData *msgData;
    MessageFormat *messageFormat;
    String *pSVar1;
    u32 uVar2;
    String *src;
    VecFx32 VStack_24;

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xd7, HEAP_ID_13);
    if (msgData == NULL) {
        GF_AssertFail();
    }
    messageFormat = MessageFormat_New(HEAP_ID_13);
    Sprite_SetAnimCtrlSeq((Sprite *)param_1[0x66], 5);
    VStack_24.x = 0x30000;
    VStack_24.y = 0x90000;
    VStack_24.z = 0;
    Sprite_SetMatrix((Sprite *)param_1[0x66], &VStack_24);
    pSVar1 = String_New(0xc, HEAP_ID_13);
    sub_0202BE60(*param_1, pSVar1);
    uVar2 = FontID_String_GetWidth(FONT_SYSTEM, pSVar1, 0);
    AddTextPrinterParameterizedWithColor((Window *)param_1[0x67], FONT_SYSTEM, pSVar1, 0x80 - (s32)uVar2 / 2, 7, 0, 0x10200, (PrinterCallback_t)0x0);
    String_Delete(pSVar1);
    wordno = sub_0202BE98(*param_1);
    BufferECWord(messageFormat, 0, wordno);
    pSVar1 = String_New(200, HEAP_ID_13);
    src = NewString_ReadMsgData(msgData, 0x2d);
    StringExpandPlaceholders(messageFormat, pSVar1, src);
    uVar2 = FontID_String_GetWidth(FONT_SYSTEM, pSVar1, 0);
    AddTextPrinterParameterizedWithColor((Window *)param_1[0x67], FONT_SYSTEM, pSVar1, 0x80 - (s32)uVar2 / 2, 0x1b, 0, 0x10200, (PrinterCallback_t)0x0);
    String_Delete(pSVar1);
    String_Delete(src);
    DestroyMsgData(msgData);
    MessageFormat_Delete(messageFormat);
    return;
}
