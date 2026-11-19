#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "list_menu_items.h"
#include "pm_string.h"
#include "overlay_27.h"
#include "text.h"

ListMenuItem *ov01_021EEF58(struct FieldMenuManager *menu);

u8 ov01_021EEF60(struct FieldMenuManager *menu);

// asm table of 7 pointers (the .word symbols), one WindowTemplate array per menu size 2..8
extern WindowTemplate *ov27_0225D4B8[];

void ov27_0225C618(Ov27TaskData *data);

void ov27_0225C618(Ov27TaskData *data) {
    ListMenuItem *puVar1;
    WindowTemplate **new_var2;
    s32 memberNo;
    NARC *narc;
    s32 iVar2;
    Window *pWVar3;
    s32 iStack_1c;
    puVar1 = ov01_021EEF58(data->menu);
    memberNo = ov01_021EEF60(data->menu);
    narc = NARC_New(NARC_a_2_3_7, HEAP_ID_8);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, data->bgConfig, GF_BG_LYR_MAIN_CNT, 0, 0, 0, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, memberNo, data->bgConfig, GF_BG_LYR_MAIN_CNT, 0, 0, 0, HEAP_ID_8);
    NARC_Delete(narc);
    iStack_1c = 0;
    if (0 < memberNo) {
        iVar2 = 0;
        pWVar3 = data->menuWindows;
        new_var2 = ov27_0225D4B8 + (memberNo - 2);
        do {
#if defined(SDK_PORT) || defined(NONMATCHING)
            AddWindow(data->bgConfig, pWVar3, *new_var2 + iStack_1c);
#else
            AddWindow(data->bgConfig, pWVar3, (WindowTemplate *)((u8 *)*new_var2 + iVar2));
            iVar2 = iVar2 + 8;
#endif
            pWVar3 = pWVar3 + 1;
            iStack_1c = iStack_1c + 1;
        } while (iStack_1c < memberNo);
    }
    iVar2 = 0;
    if (0 < memberNo) {
        pWVar3 = data->menuWindows;
        do {
            FillWindowPixelBuffer(pWVar3, 0);
            AddTextPrinterParameterizedWithColor(pWVar3, FONT_UNK_4, puVar1->text, 0, 0, 0xff, 0x20100, NULL);
            ScheduleWindowCopyToVram(pWVar3);
            iVar2 = iVar2 + 1;
            pWVar3 = pWVar3 + 1;
            puVar1++;
        } while (iVar2 < memberNo);
    }
    data->nMenuWindows = memberNo;
    return;
}
