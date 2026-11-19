#include "global.h"

#include "pokemon_summary_app.h"

#include "message_format.h"
#include "move.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "ribbon.h"
#include "unk_02088288.h"

void sub_0208981C(PokemonSummary *summaryScreen, Pokemon *arg1, PokemonSummaryMonData *monData);

void sub_0208981C(PokemonSummary *summaryScreen, Pokemon *arg1, PokemonSummaryMonData *monData) {
    s32 sp4;
    BoxPokemon *temp_r6;
    s32 var_r6_2;
    u16 var_r4;
    u16 var_r6;
    u32 temp_r1;
    u32 var_r0;
    u16 var_r6_3;
    u8 maxPp;

    sp4 = AcquireMonLock(arg1);
    monData->species = GetMonData(arg1, 5, NULL);
    temp_r6 = Mon_GetBoxMon(arg1);
    ReadMsgDataIntoString(summaryScreen->msgData, 0xB, summaryScreen->buffer);
    BufferBoxMonSpeciesName(summaryScreen->msgFormat, 0U, temp_r6);
    StringExpandPlaceholders(summaryScreen->msgFormat, summaryScreen->mon.speciesName, summaryScreen->buffer);
    ReadMsgDataIntoString(summaryScreen->msgData, 0, summaryScreen->buffer);
    BufferBoxMonNickname(summaryScreen->msgFormat, 0U, temp_r6);
    StringExpandPlaceholders(summaryScreen->msgFormat, summaryScreen->mon.nickname, summaryScreen->buffer);
    ReadMsgDataIntoString(summaryScreen->msgData, 0xE, summaryScreen->buffer);
    BufferBoxMonOTName(summaryScreen->msgFormat, 0U, temp_r6);
    StringExpandPlaceholders(summaryScreen->msgFormat, summaryScreen->mon.otName, summaryScreen->buffer);
    monData->heldItem = GetMonData(arg1, 6, NULL);
    monData->level = GetMonData(arg1, 0xA1, NULL);
    monData->isEgg = GetMonData(arg1, 0x4C, NULL);
    if ((GetMonData(arg1, 0xB0, NULL) == 1) && (monData->isEgg == 0)) {
        monData->noPrintGender = 0;
    } else {
        monData->noPrintGender = 1;
    }
    monData->gender = GetMonGender(arg1);
    monData->pokeball = GetMonData(arg1, 0x9B, NULL);
    monData->type1 = GetMonData(arg1, 0xB1, NULL);
    monData->type2 = GetMonData(arg1, 0xB2, NULL);
    monData->otId = GetMonData(arg1, 7, NULL);
    monData->exp = GetMonData(arg1, 8, NULL);
    monData->otGender = GetMonData(arg1, 0x9D, NULL);
    monData->curLevelExp = GetMonExpBySpeciesAndLevel(monData->species, monData->level);
    temp_r1 = monData->level;
    if (temp_r1 == 0x64) {
        var_r0 = monData->curLevelExp;
    } else {
        var_r0 = GetMonExpBySpeciesAndLevel(monData->species, temp_r1 + 1);
    }
    monData->nextLevelExp = var_r0;
    monData->hp = GetMonData(arg1, 0xA3, NULL);
    monData->maxHp = GetMonData(arg1, 0xA4, NULL);
    monData->attack = GetMonData(arg1, 0xA5, NULL);
    monData->defense = GetMonData(arg1, 0xA6, NULL);
    monData->spAttack = GetMonData(arg1, 0xA8, NULL);
    monData->spDefense = GetMonData(arg1, 0xA9, NULL);
    monData->speed = GetMonData(arg1, 0xA7, NULL);
    monData->ability = GetMonData(arg1, 0xA, NULL);
    monData->nature = GetMonNature(arg1);
    var_r6 = 0;
    do {
        monData->moves[var_r6] = GetMonData(arg1, var_r6 + 0x36, NULL);
        monData->pp[var_r6] = GetMonData(arg1, var_r6 + 0x3A, NULL);
        maxPp = GetMonData(arg1, var_r6 + 0x3E, NULL);
        monData->maxPP[var_r6] = GetMoveMaxPP(monData->moves[var_r6], maxPp);
        var_r6 += 1;
    } while ((u32)var_r6 < 4U);
    monData->contestStats[0] = GetMonData(arg1, 0x13, NULL);
    monData->contestStats[1] = GetMonData(arg1, 0x14, NULL);
    monData->contestStats[2] = GetMonData(arg1, 0x15, NULL);
    monData->contestStats[3] = GetMonData(arg1, 0x16, NULL);
    monData->contestStats[4] = GetMonData(arg1, 0x17, NULL);
    monData->contestStats[5] = GetMonData(arg1, 0x18, NULL);
    monData->favoriteFlavor = 5;
    var_r6_2 = 0;
loop_10:
    if (MonGetFlavorPreference(arg1, var_r6_2) == 1) {
        monData->favoriteFlavor = var_r6_2;
    } else {
        var_r6_2 = (s32)(u16)(var_r6_2 + 1);
        if ((u32)var_r6_2 < 5U) {
            goto loop_10;
        }
    }
    monData->markings = GetMonData(arg1, 0xB, NULL);
    monData->form = GetMonData(arg1, 0x70, NULL);
    monData->status = Pokemon_GetStatusIconId(arg1);
    if (Pokemon_IsImmuneToPokerus(arg1) == 1) {
        monData->pokerus = 2;
    } else if (Pokemon_HasPokerus(arg1) == 1) {
        monData->pokerus = 1;
        if (monData->status == 7) {
            monData->status = 0;
        }
    } else {
        monData->pokerus = 0;
    }
    if (MonIsShiny(arg1) == 1) {
        monData->shiny = 1;
    } else {
        monData->shiny = 0;
    }
    var_r6_3 = 0;
    monData->ribbons[0] = 0;
    monData->ribbons[1] = 0;
    monData->ribbons[2] = 0;
    monData->ribbons[3] = 0;
    summaryScreen->ribbonCount = 0U;
    do {
        if (GetMonData(arg1, (s32)GetRibbonAttr((u8)var_r6_3, RIBBONDAT_MONDATNO), NULL) != 0) {
            monData->ribbons[var_r6_3 / 32] |= (1 << (var_r6_3 & 0x1F));
            summaryScreen->ribbonCount = (u8)(summaryScreen->ribbonCount + 1);
        }
        var_r6_3 += 1;
    } while ((u32)var_r6_3 < 0x50U);
    var_r4 = 0;
    do {
        summaryScreen->mon.shinyLeaves[var_r4] = (s8)GetMonData(arg1, var_r4 + 0xB5, NULL);
        var_r4 += 1;
    } while ((u32)var_r4 < 6U);
    ReleaseMonLock(arg1, sp4);
}
