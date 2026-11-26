#include "global.h"

#include "overlay_70_gtsapplication_state.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "msgdata.h"
#include "party.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "sprite.h"

s32 ov70_0223E114(BoxPokemon *, void *);

s32 ov70_0223E170(BoxPokemon *, void *, void *, s16 *, u32, NARC *, void *, void *);

s32 ov70_0223E738(void *, void *, void *, void *);

s32 ov70_02245084(void *, String *, s32, s32, s32, s32);

extern s32 ov70_0223E120;

void ov70_0223E264(GTSApplicationState *arg0, s32 arg1);

void ov70_0223E264(GTSApplicationState *arg0, s32 arg1) {
    s16 sp2C[30];
    BoxPokemon *sp28;
    Pokemon *sp24;
    PCStorage *sp20;
    NARC *sp1C;
    PokemonIcon *sp18;
    int sp14;
    s32 sp10;
    BoxPokemon *temp_r0_4;
    Pokemon *temp_r0_3;
    s32 temp_r0_2;
    s32 temp_r1;
    s32 temp_r6;
    s32 temp_r6_2;
    s32 var_r4_3;
    u32 var_r4;
    u32 var_r4_2;
    PokemonIcon *temp_r0;
    sp10 = arg1;
    sp20 = *((PCStorage **)((u8 *)arg0->playerData + 0xC));
    temp_r0 = Heap_AllocAtEnd(HEAP_ID_3, 30 * sizeof(PokemonIcon));
    arg0->selectPokemonIconHeapPtr = temp_r0;
    sp18 = temp_r0;
    sp1C = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_61);
    if ((sp10 >= 0) && (sp10 < 0x12)) {
        var_r4 = 0;
        do {
            ov70_0223E114(PCStorage_GetMonByIndexPair(sp20, (u32)sp10, var_r4), (u8 *)arg0->boxCriteria + (var_r4 * 4));
            var_r4 = (u32)((u16)(var_r4 + 1));
        } while (var_r4 < 0x1EU);
        var_r4_2 = 0;
        do {
            temp_r6 = var_r4_2 * 4;
            *((u16 *)((u8 *)arg0->boxCriteria + temp_r6)) = 0;
                        ov70_0223E170(PCStorage_GetMonByIndexPair(sp20, (u32)sp10, var_r4_2), arg0->boxSlotIconSprites[var_r4_2], arg0->boxSlotItemSprites[var_r4_2], &(sp2C)[var_r4_2], var_r4_2, sp1C, (u8 *)arg0->boxCriteria + temp_r6, &sp18[var_r4_2]);
            if (var_r4_2 < 6U) {
                Sprite_SetDrawFlag(arg0->selectPokemonPartySprites[var_r4_2], 0);
            }
            var_r4_2 = (u32)((u16)(var_r4_2 + 1));
        } while (var_r4_2 < 0x1EU);
        PCStorage_GetBoxName(sp20, (u32)sp10, arg0->selectPokemonBoxName);
    } else {
        temp_r0_2 = Party_GetCount(*((Party **)((u8 *)arg0->playerData + 8)));
        var_r4_3 = 0;
        sp14 = temp_r0_2;
        if (temp_r0_2 > 0) {
            do {
                temp_r0_3 = Party_GetMonByIndex(*((Party **)((u8 *)arg0->playerData + 8)), var_r4_3);
                sp24 = temp_r0_3;
                temp_r0_4 = Mon_GetBoxMon(temp_r0_3);
                temp_r6_2 = var_r4_3 * 4;
                sp28 = temp_r0_4;
                ov70_0223E114(temp_r0_4, (u8 *)arg0->boxCriteria + temp_r6_2);
                ov70_0223E170(temp_r0_4, arg0->boxSlotIconSprites[var_r4_3], arg0->boxSlotItemSprites[var_r4_3], &(sp2C)[var_r4_3], (u32)var_r4_3, sp1C, (u8 *)arg0->boxCriteria + temp_r6_2, &sp18[var_r4_3]);
                if (GetMonData(temp_r0_3, 0xA2, 0L) != 0) {
                    Sprite_SetDrawFlag(arg0->selectPokemonPartySprites[var_r4_3], 1);
                } else {
                    Sprite_SetDrawFlag(arg0->selectPokemonPartySprites[var_r4_3], 0);
                }
                var_r4_3 = (s32)((u16)(var_r4_3 + 1));
            } while (var_r4_3 < sp14);
        }
        if (((u32)var_r4_3) < 0x1EU) {
            do {
                temp_r1 = var_r4_3 * 4;
                *((u16 *)((u8 *)arg0->boxCriteria + temp_r1)) = 0;
                Sprite_SetDrawFlag(arg0->boxSlotIconSprites[var_r4_3], 0);
                Sprite_SetDrawFlag(arg0->boxSlotItemSprites[var_r4_3], 0);
                sp18[var_r4_3].sprite = NULL;
                if (((u32)var_r4_3) < 6U) {
                    Sprite_SetDrawFlag(arg0->selectPokemonPartySprites[var_r4_3], 0);
                }
                var_r4_3 = (s32)((u16)(var_r4_3 + 1));
            } while (((u32)var_r4_3) < 0x1EU);
        }
        ReadMsgDataIntoString(arg0->gtsMessageLoader, 0x5C, arg0->selectPokemonBoxName);
    }
    NARC_Delete(sp1C);
    FillWindowPixelBuffer(&arg0->networkErrWindow, 0U);
    ov70_02245084(&arg0->networkErrWindow, arg0->selectPokemonBoxName, 0, 5, 1, 0x10200);
    if (arg0->screenArgument == 6) {
        ov70_0223E738(arg0->boxCriteria, arg0->boxSlotIconSprites, &arg0->searchResults[arg0->selectedSearchResult].requirements, sp18);
    }
    arg0->updateBoxPalettesFunc = &ov70_0223E120;
}
