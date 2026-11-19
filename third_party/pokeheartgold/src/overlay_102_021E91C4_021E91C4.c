#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "text.h"
#include "unk_02009D48.h"
#include "unk_02013534.h"

void ov102_021E91C4(u8 *param_1);

void ov102_021E91C4(u8 *param_1) {
    UnkStruct_02013534 *pUVar1;
    UnkStruct_02013910 *pUVar2;
    s32 iStack_4c;
    u32 size;
    String *dest;
    String *dest_00;
    u32 x;
    TextOBJ *pTVar4;
    u32 y;
    u32 n;
    MsgData *msgData;
    TextOBJTemplate TStack_48;

    pUVar1 = FontSystem_NewInit(2, HEAP_ID_35);
    *(UnkStruct_02013534 **)(param_1 + 0x1fc) = pUVar1;
    InitWindow((Window *)(param_1 + 0x204));
    AddTextWindowTopLeftCorner(*(BgConfig **)(param_1 + 0x20), (Window *)(param_1 + 0x204), 9, 4, 0, 0);
    FillWindowPixelBufferText_AssumeTileSize32((Window *)(param_1 + 0x204), 0);
    pUVar2 = sub_02013910((Window *)(param_1 + 0x204), HEAP_ID_35);
    *(UnkStruct_02013910 **)(param_1 + 0x200) = pUVar2;
    size = sub_02013948(*(UnkStruct_02013910 **)(param_1 + 0x200), NNS_G2D_VRAM_TYPE_2DMAIN);
    TStack_48.fontSystem = *(UnkStruct_02013534 **)(param_1 + 0x1fc);
    TStack_48.window = (Window *)(param_1 + 0x204);
    TStack_48.spriteList = *(SpriteList **)(param_1 + 0x24);
    TStack_48.plttResourceProxy = (NNSG2dImagePaletteProxy *)(param_1 + 0x198);
    TStack_48.unk_20 = 3;
    TStack_48.unk_24 = 0;
    TStack_48.vram = 1;
    TStack_48.x = 0;
    TStack_48.y = 0;
    TStack_48.heapID = HEAP_ID_35;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x11b, HEAP_ID_35);
    dest = String_New(0x15, HEAP_ID_35);
    dest_00 = String_New(0x15, HEAP_ID_35);
    iStack_4c = 0;
    do {
        u32 uVar3;
        sub_02021AC8(size, 1, NNS_G2D_VRAM_TYPE_2DMAIN, (UnkStruct_02021AC8 *)(param_1 + 0x218 + iStack_4c * 0x10));
        TStack_48.offset = *(u32 *)(param_1 + iStack_4c * 0x10 + 0x21c);
        TStack_48.sprite = NULL;
        ReadMsgDataIntoString(msgData, iStack_4c + 0xb, dest);
        uVar3 = String_CountLines(dest);
        FillWindowPixelBufferText_AssumeTileSize32((Window *)(param_1 + 0x204), 0);
        n = 0;
        y = 0x20 - uVar3 * 0x10;
        y >>= 1;
        if (n < uVar3) {
            do {
                String_GetLineN(dest_00, dest, n);
                x = FontID_String_GetCenterAlignmentX(FONT_UNK_4, dest_00, 0, 0x48);
                AddTextPrinterParameterizedWithColor((Window *)(param_1 + 0x204), FONT_UNK_4, dest_00, x, y, 0xff, 0xe0f00, (PrinterCallback_t)0x0);
                n = n + 1;
                y = y + 0x10;
            } while (n < uVar3);
        }
        pTVar4 = TextOBJ_Create(&TStack_48, *(UnkStruct_02013910 **)(param_1 + 0x200));
        *(TextOBJ **)(param_1 + iStack_4c * 0x10 + 0x214) = pTVar4;
        TextOBJ_SetSpritesDrawFlag(*(TextOBJ **)(param_1 + iStack_4c * 0x10 + 0x214), 1);
        TextOBJ_SetPaletteNum(*(TextOBJ **)(param_1 + iStack_4c * 0x10 + 0x214), 0);
        sub_020136B4(*(TextOBJ **)(param_1 + iStack_4c * 0x10 + 0x214), iStack_4c * 0x88 + 0x18, 6);
        iStack_4c = iStack_4c + 1;
    } while (iStack_4c < 2);
    String_Delete(dest_00);
    String_Delete(dest);
    DestroyMsgData(msgData);
    return;
}
