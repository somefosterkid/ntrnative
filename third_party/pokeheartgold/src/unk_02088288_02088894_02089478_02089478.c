#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "render_window.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "system.h"
#include "wild_encounter.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

enum SummaryPoffinFeedMsg {
    SUMMARY_MSG_COOLNESS_ENHANCED = 0,
    SUMMARY_MSG_BEAUTY_ENHANCED,
    SUMMARY_MSG_CUTENESS_ENHANCED,
    SUMMARY_MSG_SMARTNESS_ENHANCED,
    SUMMARY_MSG_TOUGHNESS_ENHANCED,

    SUMMARY_MSG_NOTHING_CHANGED = 0xFE,
    SUMMARY_MSG_MON_WONT_EAT_MORE = 0xFF,
};

void *sub_0208A520(PokemonSummary *summaryScreen);

void sub_0208BCD4(PokemonSummary *arg0);

void sub_0208BDC8(PokemonSummary *summaryScreen);

void sub_0208DDA0(PokemonSummary *summaryScreen, enum SummaryPoffinFeedMsg msg);

void sub_0208E174(PokemonSummary *summaryScreen);

s32 sub_02089478(PokemonSummary *summaryScreen);

s32 sub_02089478(PokemonSummary *summaryScreen) {
    BoxPokemon *temp_r6;
    Pokemon *var_r4;
    u8 temp_r6_2;
    u8 temp_r6_3;
    u8 temp_r6_4;
    u8 temp_r6_5;
    u8 temp_r6_6;

    if (3 & gSystem.newKeys) {
        if (summaryScreen->args->unk11 == 2) {
            temp_r6 = sub_0208A520(summaryScreen);
            var_r4 = AllocMonZeroed(HEAP_ID_19);
            CopyBoxPokemonToPokemon(temp_r6, var_r4);
        } else {
            var_r4 = sub_0208A520(summaryScreen);
        }
        summaryScreen->unk7BE = 0U;
        temp_r6_2 = summaryScreen->mon.contestStats[0];
        summaryScreen->mon.contestStats[0] = GetMonData(var_r4, 0x13, NULL);
        if (temp_r6_2 != summaryScreen->mon.contestStats[0]) {
            summaryScreen->unk7BE = (1 | summaryScreen->unk7BE);
        }
        temp_r6_3 = summaryScreen->mon.contestStats[1];
        summaryScreen->mon.contestStats[1] = GetMonData(var_r4, 0x14, NULL);
        if (temp_r6_3 != summaryScreen->mon.contestStats[1]) {
            summaryScreen->unk7BE = (2 | summaryScreen->unk7BE);
        }
        temp_r6_4 = summaryScreen->mon.contestStats[2];
        summaryScreen->mon.contestStats[2] = GetMonData(var_r4, 0x15, NULL);
        if (temp_r6_4 != summaryScreen->mon.contestStats[2]) {
            summaryScreen->unk7BE = (4 | summaryScreen->unk7BE);
        }
        temp_r6_5 = summaryScreen->mon.contestStats[3];
        summaryScreen->mon.contestStats[3] = GetMonData(var_r4, 0x16, NULL);
        if (temp_r6_5 != summaryScreen->mon.contestStats[3]) {
            summaryScreen->unk7BE = (8 | summaryScreen->unk7BE);
        }
        temp_r6_6 = summaryScreen->mon.contestStats[4];
        summaryScreen->mon.contestStats[4] = GetMonData(var_r4, 0x17, NULL);
        if (temp_r6_6 != summaryScreen->mon.contestStats[4]) {
            summaryScreen->unk7BE = (0x10 | summaryScreen->unk7BE);
        }
        summaryScreen->mon.contestStats[5] = GetMonData(var_r4, 0x18, NULL);
        if (summaryScreen->args->unk11 == 2) {
            Heap_Free(var_r4);
        }
        LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_19);
        LoadUserFrameGfx2(summaryScreen->bgConfig, GF_BG_LYR_SUB_0_F, 0x3E2U, 0xDU, (s32)Options_GetFrame(summaryScreen->args->options), HEAP_ID_19);
        if (summaryScreen->unk7BE == 0) {
            sub_0208DDA0(summaryScreen, SUMMARY_MSG_NOTHING_CHANGED);
            return 0x12;
        }
        sub_0208E174(summaryScreen);
        sub_0208BCD4(summaryScreen);
        sub_0208BDC8(summaryScreen);
        return 0x11;
    }
    return 0x10;
}
