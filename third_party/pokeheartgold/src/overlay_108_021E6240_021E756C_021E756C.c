#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "obj_char_transfer.h"
#include "pm_string.h"
#include "sprite_system.h"
#include "text.h"
#include "unk_02013534.h"
#include "overlay_108_safari_area_customizer.h"

void ov108_021E756C(SafariAreaCustomizer *param_1);

void ov108_021E756C(SafariAreaCustomizer *param_1) {
    UnkStruct_02013910 *pUVar2;
    u8 bVar1;
    u32 size;
    u32 xpos;
    TextOBJ *pTVar4;
    TextOBJTemplate TStack_40;

    pUVar2 = sub_02013910(&param_1->textWindow, param_1->heapID);
    param_1->unk_3A0 = pUVar2;
    size = sub_02013948(param_1->unk_3A0, NNS_G2D_VRAM_TYPE_2DMAIN);
    bVar1 = GetWindowWidth(&param_1->textWindow);
    xpos = ((((u32)bVar1) * 8) - FontID_String_GetWidth(FONT_SYSTEM, param_1->unk_310[0], 0)) >> 1;
    AddTextPrinterParameterizedWithColor(&param_1->textWindow, FONT_SYSTEM, param_1->unk_310[0], xpos, 0, 0xff, 0xb0600, (PrinterCallback_t)0);
    TStack_40.fontSystem = param_1->unk_39C;
    TStack_40.window = &param_1->textWindow;
    TStack_40.spriteList = SpriteManager_GetSpriteList(param_1->spriteManager);
    TStack_40.plttResourceProxy = SpriteManager_FindPlttResourceProxy(param_1->spriteManager, 0);
    TStack_40.unk_20 = 3;
    TStack_40.unk_24 = 0x80;
    TStack_40.x = 200;
    TStack_40.vram = 1;
    TStack_40.y = 0xac;
    TStack_40.heapID = param_1->heapID;
    sub_02021AC8(size, 0, NNS_G2D_VRAM_TYPE_2DMAIN, &param_1->unk_3A8);
    TStack_40.offset = param_1->unk_3A8.offset;
    TStack_40.sprite = 0L;
    pTVar4 = TextOBJ_Create(&TStack_40, param_1->unk_3A0);
    param_1->unk_3A4 = pTVar4;
    TextOBJ_SetSpritesDrawFlag(param_1->unk_3A4, 1);
    TextOBJ_SetPaletteNum(param_1->unk_3A4, 4);
    return;
}
