// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov197/mystery_check.c, which is the same code in
// Black 2/White 2's overlay 197, less its fourth kind of gift. The names are Black's dsd names; pokebw2's guesses are
// in the comments.
#include "app/mystery/mystery_check.h"
#include "app/mystery/mystery_gift_data.h"
#include "gfl/heap.h"
#include "gfl/str.h"

// Mystery Gift's check of a received gift before it is offered: its texts are terminated, its ID and kind are in
// range and what it gives is valid. The name is pokebw2's (mystery_check.c); the ROM gives none for this file

static u32 func_ov121_021defac(MysteryGift *gift, GameData *gameData, HeapID heapId);

// pokebw2's MysteryCheck_CountErrors
u32 func_ov121_021def6c(const MysteryGiftRecvData *recv, GameData *gameData, HeapID heapId) {
    u32 errors = 0;
    BOOL unterminated = TRUE;
    int i;

    for (i = 0; i < 253; i++) {
        if (recv->text[i] == GFL_StrBufGetTerminator()) {
            unterminated = FALSE;
        }
    }
    if (unterminated) {
        errors++;
    }
    return errors + func_ov121_021defac(&recv->gift, gameData, heapId);
}

// pokebw2's MysteryCheck_CountGiftErrors
static u32 func_ov121_021defac(MysteryGift *gift, GameData *gameData, HeapID heapId) {
    u32 errors = 0;
    BOOL unterminated = TRUE;
    int i;
    PartyPkm *pkm;

    for (i = 0; i < 37; i++) {
        if (gift->title[i] == GFL_StrBufGetTerminator()) {
            unterminated = FALSE;
        }
    }
    if (unterminated) {
        errors++;
    }
    if (gift->id >= 2048) {
        errors++;
    }
    if (gift->kind == 0 || gift->kind >= 4) {
        errors++;
    }
    switch (gift->kind) {
    case 1:
        pkm = Mystery_CreateGiftPokemon((MysteryGift *)gift, heapId, gameData);
        if (pkm == NULL) {
            errors++;
        } else {
            GFL_HeapFree(pkm);
        }
        break;
    case 2:
        if (Mystery_GetGiftItem((MysteryGift *)gift) == MYSTERY_GIFT_ITEM_NONE) {
            errors++;
        }
        break;
    case 3:
        if (Mystery_GetGiftPassPower((MysteryGift *)gift) == MYSTERY_GIFT_PASS_POWER_NONE) {
            errors++;
        }
        break;
    }
    return errors;
}
