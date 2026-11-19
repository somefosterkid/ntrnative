#include "global.h"

#include "constants/heap.h"

#include "bag_types_def.h"
#include "bg_window.h"
#include "evolution_data.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "game_stats.h"
#include "gf_3d_vramman.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_summary_args.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "render_text.h"
#include "render_window.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_020163E0.h"
#include "unk_02016EDC.h"
#include "unk_02026E84.h"
#include "unk_0203A3B0.h"
#include "unk_020755E8.h"
#include "wild_encounter.h"

void sub_02075630(EvolutionData *);

void sub_02075D08(SysTask *task, void *data);

s32 sub_02076E64(EvolutionData *, BgConfig *);

s32 sub_020771E8(EvolutionData *);

s32 sub_020773AC(void);

s32 sub_020773D4(void);

GF3DVramMan *sub_02077400(enum HeapID);

s32 sub_020774A0(void);

EvolutionTaskData *sub_02075A7C(Party *party, Pokemon *mon, int species, Options *options, u32 a4, Pokedex *dex, Bag *bag, GameStats *stats, u32 evolutionCondition, u32 a9, enum HeapID heapID) {
    s32 sp20;
    s32 sp1C;
    Options *sp18;
    s32 sp14;
    EvolutionData *evo;
    PaletteData *temp_r0_2;
    UnkStruct_020163E0 *temp_r0_3;

    sp14 = species;
    sp18 = options;
    gSystem.screensFlipped = 0;
    GfGfx_SwapDisplay();
    FontID_Alloc(FONT_UNK_4, heapID);
    evo = Heap_Alloc(heapID, sizeof(EvolutionData));
    MIi_CpuClearFast(0U, evo, sizeof(EvolutionData));
    sp1C = sub_02026E8C();
    MIi_CpuClear16(0U, GetMainBgPlttAddr(), (u32)sp1C);
    sp20 = sub_02026E9C();
    MIi_CpuClear16(0U, GetSubBgPlttAddr(), (u32)sp20);
    evo->party = party;
    evo->mon = mon;
    evo->currentSpecies = (u16)GetMonData(mon, 5, NULL);
    evo->form = (s8)GetMonData(mon, 0x70, NULL);
    evo->targetSpecies = (u16)sp14;
    evo->heapID = heapID;
    evo->summaryScreenAppMan = NULL;
    evo->narc = NARC_New(NARC_a_1_8_0, heapID);
    sub_020729A4(evo->narc, &evo->pad_88[0], evo->currentSpecies, 1U);
    sub_020729A4(evo->narc, &evo->pad_88[1], evo->targetSpecies, 1U);
    temp_r0_2 = PaletteData_Init(heapID);
    evo->paletteData = temp_r0_2;
    PaletteData_SetAutoTransparent(temp_r0_2, 1);
    PaletteData_AllocBuffers(evo->paletteData, PLTTBUF_MAIN_BG, 0x200U, heapID);
    PaletteData_AllocBuffers(evo->paletteData, PLTTBUF_SUB_BG, 0x200U, heapID);
    PaletteData_AllocBuffers(evo->paletteData, PLTTBUF_MAIN_OBJ, 0x1C0U, heapID);
    evo->bgConfig = BgConfig_Alloc(heapID);
    evo->window = AllocWindows(heapID, 1);
    evo->options = sp18;
    evo->pipelineBuffers = sub_02077400(heapID);
    sub_020773AC();
    sub_020773D4();
    sub_020774A0();
    sub_02076E64(evo, evo->bgConfig);
    sub_02075630(evo);
    AddWindowParameterized(evo->bgConfig, evo->window, 1U, 2U, 0x13U, 0x1BU, 4U, 0xBU, 0x1FU);
    FillWindowPixelBuffer(evo->window, 0xFFU);
    DrawFrameAndWindow2(evo->window, 0, 1U, 0xAU);
    AddWindowParameterized(evo->bgConfig, &evo->windows[0], 6U, 3U, 8U, 0x1AU, 2U, 0xFU, 0x50U);
    AddWindowParameterized(evo->bgConfig, &evo->windows[1], 6U, 3U, 0xEU, 0x1AU, 2U, 0xFU, 0x84U);
    evo->monSpriteMan = PokepicManager_Create(heapID);
    evo->monAnimMan = sub_02016EDC(heapID, 1, 0);
    evo->done = 0;
    evo->delay = 2;
    evo->msgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xC5, heapID);
    evo->strTemplate = MessageFormat_New(heapID);
    evo->string = String_New(0x140U, heapID);
    evo->monSummary = Heap_Alloc(heapID, sizeof(PokemonSummaryArgs));
    MIi_CpuClearFast(0U, evo->monSummary, sizeof(PokemonSummaryArgs));
    evo->monSummary->unk2C = a4;
    evo->pokedex = dex;
    evo->bag = bag;
    evo->records = stats;
    evo->method = evolutionCondition;
    evo->flags = a9;
    sub_020771E8(evo);
    PaletteData_BeginPaletteFade(evo->paletteData, 0xFU, 0xFFFFU, 1, 0x10U, 0U, 0U);
    Pokepic_StartPaletteFadeAll(evo->monSpriteMan, 0x10, 0, 0, 0);
    temp_r0_3 = sub_020163E0(evo->paletteData, PM_LCD_TOP, 0xBU, heapID);
    evo->unk_58 = temp_r0_3;
    sub_0201649C(temp_r0_3, 1);
    SysTask_CreateOnMainQueue(sub_02075D08, evo, 0U);
    TextFlags_SetCanABSpeedUpPrint(1);
    TextFlags_SetCanTouchSpeedUpPrint(1);
    sub_0203A880();
    return evo;
}
