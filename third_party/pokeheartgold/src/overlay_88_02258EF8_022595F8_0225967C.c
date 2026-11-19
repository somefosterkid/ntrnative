#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

void ov88_0225983C(Window *, u16, u32);

extern u32 _02259A60;

extern u32 ov88_02259980;

extern u32 ov88_02259A68;

void ov88_0225967C(Window *param_1, void **param_2, enum HeapID param_3);

void ov88_0225967C(Window *param_1, void **param_2, enum HeapID param_3) {
    u8 uVar1;
    u16 uVar2;
    s32 iStack_1c;
    MsgData *msgData;
    String *dest;
    String *dest_00;
    u32 uVar4;
    u8 *puVar5;
    u32 y;
    u32 n;
    Window *pWStack_44;
    u32 uVar3;
    u32 uStack_2c;
    u32 *puStack_30;
    u16 *puStack_34;
    Window *pWStack_38;

    memset(param_1, 0, 7 * sizeof(Window));
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x2f5, param_3);
    dest = String_New(0x80, param_3);
    iStack_1c = 0;
    puVar5 = (unsigned char *)(&ov88_02259980);
    puStack_30 = (u32 *)&ov88_02259A68;
    puStack_34 = (u16 *)&_02259A60;
    pWStack_38 = param_1 + 4;
    pWStack_44 = param_1;
    do {
        ov88_0225983C(pWStack_38, *puStack_34, *puStack_30);
        AddWindowParameterized((BgConfig *)*param_2, pWStack_44, 0, *puVar5, puVar5[1], puVar5[2], puVar5[3], puVar5[6], *(u16 *)(puVar5 + 4));
        FillWindowPixelBuffer(pWStack_44, 0);
        ReadMsgDataIntoString(msgData, iStack_1c, dest);
        uVar3 = String_CountLines(dest);
        y = (u32)puVar5[8];
        uVar2 = String_GetLength(dest);
        dest_00 = String_New(uVar2 + 1, param_3);
        n = 0;
        if (n < uVar3) {
            do {
                String_GetLineN(dest_00, dest, n);
                uVar1 = puVar5[9];
                switch (uVar1) {
                case 0:
                    uStack_2c = (u32)puVar5[7];
                    break;
                case 1:
                    uVar4 = FontID_String_GetWidthMultiline(FONT_SYSTEM, dest_00, 0);
                    uStack_2c = (u32)puVar5[7] - (uVar4 + 1 >> 1);
                    break;
                case 2:
                    uVar4 = FontID_String_GetWidthMultiline(FONT_SYSTEM, dest_00, 0);
                    uStack_2c = puVar5[7] - uVar4;
                    break;
                }
                AddTextPrinterParameterizedWithColor(pWStack_44, FONT_SYSTEM, dest_00, uStack_2c, y, 0xff, *(u32 *)(puVar5 + 0xc), (PrinterCallback_t)0x0);
                n = n + 1;
                y = y + 0x10;
            } while (n < uVar3);
        }
        String_Delete(dest_00);
        ScheduleWindowCopyToVram(pWStack_44);
        puVar5 = puVar5 + 0x10;
        puStack_30 = puStack_30 + 1;
        puStack_34 = puStack_34 + 1;
        pWStack_38 = (Window *)&pWStack_38->pixelBuffer;
        pWStack_44 = pWStack_44 + 1;
        iStack_1c = iStack_1c + 1;
    } while (iStack_1c < 4);
    String_Delete(dest);
    DestroyMsgData(msgData);
    return;
}
