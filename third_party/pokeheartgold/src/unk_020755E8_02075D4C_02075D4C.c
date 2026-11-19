#include "global.h"

#include "bg_window.h"
#include "evolution_data.h"
#include "filesystem.h"
#include "font.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "palette.h"
#include "pokepic.h"
#include "render_text.h"
#include "screen_fade.h"
#include "system.h"
#include "unk_020163E0.h"
#include "unk_02016EDC.h"
#include "unk_020755E8.h"

void sub_02075770(EvolutionData *);

s32 sub_020771A0(void *);

void sub_02075D4C(EvolutionTaskData *evo) {
    sub_0200FBF4(PM_LCD_TOP, 0U);
    sub_0200FBF4(PM_LCD_BOTTOM, 0U);
    Main_SetVBlankIntrCB(NULL, NULL);
    FontID_Release(FONT_UNK_4);
    sub_02075770(evo);
    WindowArray_Delete(evo->window, 1);
    RemoveWindow(&evo->windows[0]);
    RemoveWindow(&evo->windows[1]);
    PaletteData_FreeBuffers(evo->paletteData, PLTTBUF_MAIN_BG);
    PaletteData_FreeBuffers(evo->paletteData, PLTTBUF_SUB_BG);
    PaletteData_FreeBuffers(evo->paletteData, PLTTBUF_MAIN_OBJ);
    PaletteData_Free(evo->paletteData);
    PokepicManager_Delete(evo->monSpriteMan);
    sub_02016F2C(evo->monAnimMan);
    GF_3DVramMan_Delete(evo->pipelineBuffers);
    sub_020771A0(evo->bgConfig);
    DestroyMsgData(evo->msgLoader);
    MessageFormat_Delete(evo->strTemplate);
    Heap_Free(evo->string);
    Heap_Free(evo->monSummary);
    sub_020164C4(evo->unk_58);
    Heap_Free(evo->bgConfig);
    NARC_Delete(evo->narc);
    Heap_Free(evo);
    TextFlags_SetCanABSpeedUpPrint(0);
    TextFlags_SetCanTouchSpeedUpPrint(0);
    gSystem.screensFlipped = 1;
    GfGfx_SwapDisplay();
}
