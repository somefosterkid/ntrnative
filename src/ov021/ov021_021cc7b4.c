// Black's code at ov021 0x021cc7b4-0x021cd1c8. No name is known: the module and address. The work is pokebw2's
// PartySelectList (src/ov036/party_select_list.c), same layout; its struct and field names.

#include "types.h"
#include "field/field.h"
#include "gfl/arc.h"
#include "gfl/clact.h"
#include "gfl/heap.h"
#include "pml/poke_party.h"
#include "struct_decls.h"
#include "system/app_menu_common.h"
#include "system/bmp_menulist.h"
#include "system/wordset.h"
#include "field/union_msg.h"
#include "field/field_msgbg.h"
#include "system/gf_font.h"
#include "pml/item.h"
#include "gfl/str.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "constants/pokemon.h"

// The party or Battle Box choice before a link battle, with the six icons and held items of each (0xc4 bytes)
struct PartySelectList {
    Field *field;
    PokeParty *temoti;
    PokeParty *bbox;
    HeapID heapId;
    u32 partyOk;
    u32 bboxOk;
    ClActUnit *unit;
    ClActor *partyIcons[6];
    ClActor *bboxIcons[6];
    ClActor *partyItems[6];
    ClActor *bboxItems[6];
    u16 iconPalette;
    u16 iconCells;
    u16 partyChars[6];
    u16 bboxChars[6];
    u16 itemPalette;
    u16 itemCells;
    u16 itemChars;
    WordSet *wordSet;
    FieldMsgBG *msgBGSys;
    MsgData *msgData;
    ListMenuUI *list;
    MsgBGTalkWin *infoWin;
    MsgBGSysWin *moneyWin;
    Font *font;
    s32 cursor;
    u8 state;
    u8 finish;
};

void func_ov021_021cceb8(PartySelectList *psl);
void func_ov021_021cc8dc(PartySelectList *psl);
void func_ov021_021cca00(PartySelectList *psl);
void func_ov021_021ccc50(PartySelectList *psl);
void func_ov021_021ccce4(PartySelectList *psl);
u32 func_ov021_021cc8b0(PartySelectList *psl, u32 *out);
void func_ov021_021ccdf8(PartySelectList *psl);
void func_ov021_021cccc4(PartySelectList *psl);
void func_ov021_021ccb98(PartySelectList *psl);
void func_ov021_021cc9a4(PartySelectList *psl);
void func_ov021_021ccee4(PartySelectList *psl);
void func_ov021_021ccefc(PartySelectList *psl);
void func_ov021_021ccf48(PartySelectList *psl);
u32 func_ov021_021ccf54(PartySelectList *psl);
void func_ov021_021ccf60(BmpMenuList *list, intptr_t value, u8 init);
void func_ov021_021ccf7c(BmpMenuList *list, intptr_t value, u8 y);
void func_ov021_021ccfc0(PartySelectList *psl);
void func_ov021_021ccfd8(PartySelectList *psl);
void func_ov021_021ccfe4(PartySelectList *psl, u32 messageId);
s32 func_ov021_021ccff8(PartySelectList *psl);
void func_ov021_021cd050(PartySelectList *psl, PokeParty *party);
void func_ov021_021cd184(PartySelectList *psl, s32 mode);
void func_ov021_021cd1ac(PartySelectList *psl);

// The menu's rows (Party, Battle Box, Cancel)
static const ListMenuRow data_ov021_021db414[3] = { { 0, { 0 } }, { 1, { 1 } }, { 2, { 2 } } };

static const ListMenuRequest data_ov021_021db42c = {
    .count = 3,
    .unk02 = 3,
    .unk05 = 0x0d,
    .unk07_4 = 1,
    .unk08_0 = 0xf,
    .unk08_4 = 2,
    .unk0A_3 = 1,
    .unk10 = 0x0c,
    .rowHeight = 0x10,
    .x = 0x12,
    .y = 0x0b,
    .width = 0x0d,
    .height = 0x06,
};

PartySelectList *func_ov021_021cc720(Field *field, PokeParty *party, PokeParty *battleBoxParty, u8 partyOk,
                                     int battleBoxOk, HeapID heapId) {
    PartySelectList *psl;

    GFL_ASSERT(party != NULL && battleBoxParty != NULL);
    psl = GFL_HeapAllocate(heapId, sizeof(PartySelectList), TRUE, "party_select_list.c", 0xd3);
    psl->field = field;
    psl->temoti = party;
    psl->bbox = battleBoxParty;
    psl->heapId = heapId;
    psl->partyOk = partyOk;
    psl->bboxOk = battleBoxOk;
    psl->unit = func_0204b100(0x20, 10, psl->heapId);
    psl->font = GFL_FontCreate(0x17, 3, 0, 0, psl->heapId);
    func_ov021_021cceb8(psl);
    func_ov021_021cc8dc(psl);
    func_ov021_021cca00(psl);
    func_ov021_021ccc50(psl);
    func_ov021_021ccce4(psl);
    return psl;
}

u32 func_ov021_021cc7b4(PartySelectList *psl, u32 *out) {
    u32 result;

    result = func_ov021_021cc8b0(psl, out);
    GFL_ASSERT(psl->finish == 1);
    func_ov021_021ccdf8(psl);
    func_ov021_021cccc4(psl);
    func_ov021_021ccb98(psl);
    func_ov021_021cc9a4(psl);
    func_ov021_021ccee4(psl);
    GFL_FontFree(psl->font);
    func_0204b1cc(psl->unit);
    GFL_HeapFree(psl);
    return result;
}


BOOL func_ov021_021cc810(PartySelectList *psl) {
    u32 result;

    switch (psl->state) {
    case 0:
        func_ov021_021ccfc0(psl);
        func_ov021_021ccfe4(psl, 3);
        psl->state++;
        break;
    case 1:
        if (func_ov021_021ccff8(psl) == 1) {
            func_ov021_021ccefc(psl);
            psl->state++;
        }
        break;
    case 2:
        if (func_ov021_0219049c(psl->msgBGSys) == 1) {
            result = func_ov021_021ccf54(psl);
            if (result != -1) {
                func_ov021_021ccf48(psl);
                func_ov021_021cd1ac(psl);
                func_ov021_021ccfd8(psl);
                switch (result) {
                case -1:
                    break;
                case 2:
                case -2:
                    psl->cursor = 2;
                    psl->state++;
                    break;
                default:
                    psl->cursor = result;
                    psl->state++;
                    break;
                }
            }
        }
        break;
    default:
        psl->finish = 1;
        return TRUE;
    }
    return FALSE;
}

u32 func_ov021_021cc8b0(PartySelectList *psl, u32 *out) {
    *out = 0;
    if (psl->cursor == 0 && psl->partyOk == 1) {
        *out = 1;
    } else if (psl->cursor == 1 && psl->bboxOk == 1) {
        *out = 1;
    }
    return psl->cursor;
}

void func_ov021_021cc8dc(PartySelectList *psl) {
    ArcTool *arc;
    s32 count;
    s32 i;

    arc = GFL_ArcSysCreateFileHandle(7, psl->heapId);
    psl->iconPalette = func_0204acec(arc, func_0201bc3c(), 0, 0x60, psl->heapId);
    psl->iconCells = func_0204af28(arc, func_0201bc40(), getOBJTileMapping_MainEng(), psl->heapId);
    count = PokeParty_GetPkmCount(psl->temoti);
    for (i = 0; i < count; i++) {
        psl->partyChars[i] = func_0204a6c8(arc, func_0201ba68(func_020185f0(PokeParty_GetPkm(psl->temoti, i))), 0, 0, psl->heapId);
    }
    if (psl->bbox != NULL) {
        count = PokeParty_GetPkmCount(psl->bbox);
        for (i = 0; i < count; i++) {
            psl->bboxChars[i] = func_0204a6c8(arc, func_0201ba68(func_020185f0(PokeParty_GetPkm(psl->bbox, i))), 0, 0, psl->heapId);
        }
    }
    GFL_ArcToolFree(arc);
}


void func_ov021_021cc9a4(PartySelectList *psl) {
    s32 count;
    s32 i;

    if (psl->bbox != NULL) {
        count = PokeParty_GetPkmCount(psl->bbox);
        for (i = 0; i < count; i++) {
            func_0204a8d4(psl->bboxChars[i]);
        }
    }
    count = PokeParty_GetPkmCount(psl->temoti);
    for (i = 0; i < count; i++) {
        func_0204a8d4(psl->partyChars[i]);
    }
    func_0204afd8(psl->iconCells);
    func_0204ada4(psl->iconPalette);
}

void func_ov021_021cca00(PartySelectList *psl) {
    ClActorSetup setup = { 0, 32, 0, 21, 0 };
    int count = PokeParty_GetPkmCount(psl->temoti);
    int bboxCount;
    u32 species;
    u32 form;
    u32 species2;
    u32 form2;
    int i;

    for (i = 0; i < count; i++) {
        PartyPkm *pkm;
        BOOL egg;
        u32 sex;
        u32 palette;

        setup.x = i * 40 + 24;
        psl->partyIcons[i] = func_0204b294(psl->unit, psl->partyChars[i], psl->iconPalette, psl->iconCells, &setup, 0, psl->heapId);
        pkm = PokeParty_GetPkm(psl->temoti, i);
        species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
        form = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
        if (PokeParty_GetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL) == SPECIES_EGG) {
            egg = TRUE;
        } else {
            egg = FALSE;
        }
        sex = PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL);
        palette = func_0201bb5c(species, form, sex, egg);
        func_0204b6f4(psl->partyIcons[i], (u8)palette, 1);
        func_0204b3dc(psl->partyIcons[i], FALSE);
    }
    if (psl->bbox != NULL) {
        bboxCount = PokeParty_GetPkmCount(psl->bbox);
        for (i = 0; i < bboxCount; i++) {
            PartyPkm *pkm;
            BOOL egg;
            u32 sex;
            u32 palette;

            setup.x = i * 40 + 24;
            psl->bboxIcons[i] = func_0204b294(psl->unit, psl->bboxChars[i], psl->iconPalette, psl->iconCells, &setup, 0, psl->heapId);
            pkm = PokeParty_GetPkm(psl->bbox, i);
            species2 = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
            form2 = PokeParty_GetParam(pkm, PKM_PARAM_FORM, NULL);
            if (PokeParty_GetParam(pkm, PKM_PARAM_LEGAL_SPECIES, NULL) == SPECIES_EGG) {
                egg = TRUE;
            } else {
                egg = FALSE;
            }
            sex = PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL);
            palette = func_0201bb5c(species2, form2, sex, egg);
            func_0204b6f4(psl->bboxIcons[i], (u8)palette, 1);
            func_0204b3dc(psl->bboxIcons[i], FALSE);
        }
    }
}


void func_ov021_021ccb98(PartySelectList *psl) {
    s32 count;
    s32 i;

    count = PokeParty_GetPkmCount(psl->temoti);
    for (i = 0; i < count; i++) {
        func_0204b3b4(psl->partyIcons[i]);
    }
    if (psl->bbox != NULL) {
        count = PokeParty_GetPkmCount(psl->bbox);
        for (i = 0; i < count; i++) {
            func_0204b3b4(psl->bboxIcons[i]);
        }
    }
}

void func_ov021_021ccbe0(PartySelectList *psl, s32 mode) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (psl->partyIcons[i] != NULL) {
            func_0204b3dc(psl->partyIcons[i], 0);
        }
        if (psl->bboxIcons[i] != NULL) {
            func_0204b3dc(psl->bboxIcons[i], 0);
        }
    }
    if (mode == 0) {
        for (i = 0; i < 6; i++) {
            if (psl->partyIcons[i] != NULL) {
                func_0204b3dc(psl->partyIcons[i], 1);
            }
        }
        return;
    }
    if (mode == 1) {
        for (i = 0; i < 6; i++) {
            if (psl->bboxIcons[i] != NULL) {
                func_0204b3dc(psl->bboxIcons[i], 1);
            }
        }
    }
}

void func_ov021_021ccc50(PartySelectList *psl) {
    ArcTool *arc;
    u32 fileId;
    u32 animFileId;

    arc = GFL_ArcSysCreateFileHandle(0x53, psl->heapId);
    fileId = func_02027694();
    fileId = func_0204ac18(arc, fileId, 0, 0xc0, 0, 1, psl->heapId);
    psl->itemPalette = fileId;
    fileId = func_0202769c(1);
    animFileId = func_020276a0(1);
    fileId = func_0204af28(arc, fileId, animFileId, psl->heapId);
    psl->itemCells = fileId;
    fileId = func_02027698();
    fileId = func_0204a6c8(arc, fileId, 0, 0, psl->heapId);
    psl->itemChars = fileId;
    GFL_ArcToolFree(arc);
}

void func_ov021_021cccc4(PartySelectList *psl) {
    func_0204a8d4(psl->itemChars);
    func_0204afd8(psl->itemCells);
    func_0204ada4(psl->itemPalette);
}

void func_ov021_021ccce4(PartySelectList *psl) {
    ClActorSetup setup = { 0, 0x28, 0, 0x14, 0 };
    PartyPkm *pkm;
    s32 count;
    s32 i;
    u32 item;

    count = PokeParty_GetPkmCount(psl->temoti);
    for (i = 0; i < count; i++) {
        pkm = PokeParty_GetPkm(psl->temoti, i);
        item = PokeParty_GetParam(pkm, 6, NULL);
        if (item != 0) {
            if (PML_ItemIsMail(item) == TRUE) {
                setup.sequence = 1;
            } else {
                setup.sequence = 0;
            }
            setup.x = i * 0x28 + 0x20;
            psl->partyItems[i] = func_0204b294(psl->unit, psl->itemChars, psl->itemPalette, psl->itemCells, &setup, 0, psl->heapId);
            func_0204b3dc(psl->partyItems[i], 0);
        }
    }
    if (psl->bbox != NULL) {
        count = PokeParty_GetPkmCount(psl->bbox);
        for (i = 0; i < count; i++) {
            pkm = PokeParty_GetPkm(psl->bbox, i);
            item = PokeParty_GetParam(pkm, 6, NULL);
            if (item != 0) {
                if (PML_ItemIsMail(item) == TRUE) {
                    setup.sequence = 1;
                } else {
                    setup.sequence = 0;
                }
                setup.x = i * 0x28 + 0x20;
                psl->bboxItems[i] = func_0204b294(psl->unit, psl->itemChars, psl->itemPalette, psl->itemCells, &setup, 0, psl->heapId);
                func_0204b3dc(psl->bboxItems[i], 0);
            }
        }
    }
}


void func_ov021_021ccdf8(PartySelectList *psl) {
    s32 count;
    s32 i;

    count = PokeParty_GetPkmCount(psl->temoti);
    for (i = 0; i < count; i++) {
        if (psl->partyItems[i] != NULL) {
            func_0204b3b4(psl->partyItems[i]);
        }
    }
    if (psl->bbox != NULL) {
        count = PokeParty_GetPkmCount(psl->bbox);
        for (i = 0; i < count; i++) {
            if (psl->bboxItems[i] != NULL) {
                func_0204b3b4(psl->bboxItems[i]);
            }
        }
    }
}

void func_ov021_021cce48(PartySelectList *psl, s32 mode) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (psl->partyItems[i] != NULL) {
            func_0204b3dc(psl->partyItems[i], 0);
        }
        if (psl->bboxItems[i] != NULL) {
            func_0204b3dc(psl->bboxItems[i], 0);
        }
    }
    if (mode == 0) {
        for (i = 0; i < 6; i++) {
            if (psl->partyItems[i] != NULL) {
                func_0204b3dc(psl->partyItems[i], 1);
            }
        }
        return;
    }
    if (mode == 1) {
        for (i = 0; i < 6; i++) {
            if (psl->bboxItems[i] != NULL) {
                func_0204b3dc(psl->bboxItems[i], 1);
            }
        }
    }
}

void func_ov021_021cceb8(PartySelectList *psl) {
    FieldMsgBG *msgBGSys;

    msgBGSys = Field_GetMsgBGSys(psl->field);
    psl->msgData = func_ov021_021904b0(msgBGSys, 0x54);
    psl->msgBGSys = msgBGSys;
    psl->wordSet = GFL_WordSetSystemCreateDefault(psl->heapId);
}

void func_ov021_021ccee4(PartySelectList *psl) {
    GFL_WordSetSystemFree(psl->wordSet);
    func_ov021_021904c8(psl->msgData);
}


void func_ov021_021ccefc(PartySelectList *psl) {
    ListMenuOption *options;

    options = func_ov021_02190a28(data_ov021_021db414, 3, psl->msgData, psl->heapId);
    psl->list = func_ov021_02190938(psl->msgBGSys, &data_ov021_021db42c, options, 0, 0,
                                    func_ov021_021ccf60, func_ov021_021ccf7c, psl);
}


void func_ov021_021ccf48(PartySelectList *psl) {
    func_ov021_021909bc(psl->list);
}

// The menu's value: 0 to 2 (the rows'), or BMPMENULIST_NULL or BMPMENULIST_CANCEL
u32 func_ov021_021ccf54(PartySelectList *psl) {
    return (u32)ListMenuUI_Update(psl->list);
}

void func_ov021_021ccf60(BmpMenuList *list, intptr_t value, u8 init) {
    PartySelectList *psl;

    psl = func_02020d0c(list);
    // The party or the Battle Box, and not a negative result
    if ((u32)value < 2) {
        func_ov021_021cd184(psl, (s32)value);
        return;
    }
    func_ov021_021cd1ac(psl);
}

void func_ov021_021ccf7c(BmpMenuList *list, intptr_t value, u8 y) {
    PartySelectList *psl;

    psl = func_02020d0c(list);
    if (value == 0 && psl->partyOk == 1) {
        func_02020298(list, 1, 0, 2);
        return;
    }
    if (value == 1) {
        func_02020298(list, 1, 0, 2);
        return;
    }
    func_02020298(list, 1, 0, 2);
}

void func_ov021_021ccfc0(PartySelectList *psl) {
    psl->infoWin = func_ov021_02190f64(psl->msgBGSys, psl->msgData);
}

void func_ov021_021ccfd8(PartySelectList *psl) {
    func_ov021_02190e40(psl->infoWin);
}

void func_ov021_021ccfe4(PartySelectList *psl, u32 messageId) {
    func_ov021_02190e74(psl->infoWin, 0, 0, messageId);
}

s32 func_ov021_021ccff8(PartySelectList *psl) {
    return func_ov021_02190ef0(psl->infoWin);
}

void func_ov021_021cd004(PartySelectList *psl) {
    if (psl->moneyWin == NULL) {
        psl->moneyWin = FieldMsgBG_CreateMoneyWin(psl->msgBGSys, psl->msgData, 1, 2, 0x1e, 6);
    }
}

void func_ov021_021cd038(PartySelectList *psl) {
    if (psl->moneyWin != NULL) {
        func_ov021_02190728(psl->moneyWin);
        psl->moneyWin = NULL;
    }
}

void func_ov021_021cd050(PartySelectList *psl, PokeParty *party) {
    StrBuf *dest;
    StrBuf *strbuf;
    s32 count;
    s32 i;
    PartyPkm *pkm;
    u32 species;
    u32 number;
    u32 sex;
    u32 flag;
    int x;

    func_ov021_021cd004(psl);
    func_ov021_02190788(psl->moneyWin);
    dest = GFL_StrBufCreate(0x40, psl->heapId);
    strbuf = GFL_StrBufCreate(0x40, psl->heapId);
    count = PokeParty_GetPkmCount(party);
    for (i = 0; i < count; i++) {
        pkm = PokeParty_GetPkm(party, i);
        if (PokeParty_GetParam(pkm, 0x4c, NULL) == 0) {
            species = PokeParty_GetParam(pkm, 5, NULL);
            flag = PokeParty_GetParam(pkm, 0xad, NULL);
            number = PokeParty_GetParam(pkm, 0x9e, NULL);
            sex = PokeParty_GetParam(pkm, 0x6e, NULL);
            GFL_MsgDataLoadStrbuf(psl->msgData, 4, strbuf);
            WordSetNumber(psl->wordSet, 0, number, 3, 0, 1);
            GFL_WordSetFormatStrbuf(psl->wordSet, dest, strbuf);
            x = i * 0x28;
            func_ov021_02190764(psl->moneyWin, x, 0x20, dest, 0x440, psl->font);
            if (sex != 2 && ((species != 0x1d && species != 0x20) || flag != 0)) {
                if (sex == 0) {
                    func_ov021_02190740(psl->moneyWin, x + 0x1f, 0x10, 5);
                } else {
                    func_ov021_02190740(psl->moneyWin, x + 0x1f, 0x10, 6);
                }
            }
        }
    }
    GFL_StrBufFree(dest);
    GFL_StrBufFree(strbuf);
}


void func_ov021_021cd184(PartySelectList *psl, s32 mode) {
    PokeParty *party;

    if (mode == 0) {
        party = psl->temoti;
    } else {
        party = psl->bbox;
    }
    func_ov021_021cd050(psl, party);
    func_ov021_021ccbe0(psl, mode);
    func_ov021_021cce48(psl, mode);
}

void func_ov021_021cd1ac(PartySelectList *psl) {
    func_ov021_021cd038(psl);
    func_ov021_021ccbe0(psl, 2);
    func_ov021_021cce48(psl, 2);
}
