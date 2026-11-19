#include "global.h"

#include "battle/battle_system.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "render_window.h"
#include "overlay_08_battle_party.h"
#include "battle/battle_party_context.h"

s32 ov08_022217F0(void *, u32 *);

s32 ov08_02221B1C(void *, u32 *);

s32 sub_0200E63C(u32 messageBoxFrame);

void ov08_0221CF38(BattleParty *battleParty);

void ov08_0221CF38(BattleParty *battleParty) {
    NNSG2dScreenData *sp10;
    NARC *temp_r0;
    u16 *temp_r0_4;
    u16 *temp_r1;
    u16 *temp_r1_2;
    u16 *temp_r6;
    u32 temp_r4;
    void *temp_r0_2;
    void *temp_r0_3;
    void *temp_r2;

    temp_r0 = NARC_New(NARC_a_0_7_1, battleParty->context->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(temp_r0, 0x16, battleParty->background, GF_BG_LYR_SUB_3, 0U, 0U, 0, battleParty->context->heapID);
    temp_r0_2 = NARC_AllocAndReadWholeMember(temp_r0, 0x14U, battleParty->context->heapID);
    NNS_G2dGetUnpackedScreenData(temp_r0_2, &sp10);
    ov08_022217F0(battleParty, sp10->rawData);
    Heap_Free(temp_r0_2);
    temp_r0_3 = NARC_AllocAndReadWholeMember(temp_r0, 0x15U, battleParty->context->heapID);
    NNS_G2dGetUnpackedScreenData(temp_r0_3, &sp10);
    ov08_02221B1C(battleParty, sp10->rawData);
    Heap_Free(temp_r0_3);
    PaletteData_LoadNarc(battleParty->palette, NARC_a_0_7_1, 0x17, battleParty->context->heapID, PLTTBUF_SUB_BG, 0x200U, 0U);
    NARC_Delete(temp_r0);
    memcpy(battleParty->screenPaletteData, (u8 *)PaletteData_GetUnfadedBuf(battleParty->palette, PLTTBUF_SUB_BG) + 0x180, 0x40U);
    PaletteData_LoadNarc(battleParty->palette, NARC_graphic_font, 7, battleParty->context->heapID, PLTTBUF_SUB_BG, 0x20U, 0xD0U);
    PaletteData_LoadNarc(battleParty->palette, NARC_graphic_font, 8, battleParty->context->heapID, PLTTBUF_SUB_BG, 0x20U, 0xF0U);
    temp_r4 = BattleSystem_GetFrame(battleParty->context->battleSys);
    GfGfxLoader_LoadCharData(NARC_a_0_3_8, sub_0200E63C(temp_r4), battleParty->background, GF_BG_LYR_SUB_2_F, 1U, 0U, 0, battleParty->context->heapID);
    PaletteData_LoadNarc(battleParty->palette, NARC_a_0_3_8, (s32)sub_0200E640((s32)temp_r4), battleParty->context->heapID, PLTTBUF_SUB_BG, 0x20U, 0xE0U);
    temp_r6 = PaletteData_GetUnfadedBuf(battleParty->palette, PLTTBUF_SUB_BG);
    temp_r0_4 = Heap_Alloc(battleParty->context->heapID, 0x20U);
    memcpy(temp_r0_4, temp_r6 + 208, 0x20U);
    temp_r2 = temp_r0_4 + 7;
    temp_r1 = temp_r6 + 154;
    *(u8 *)((u8 *)temp_r0_4 + 14) = (u8) * (u8 *)((u8 *)temp_r6 + 0x134);
    *(u8 *)((u8 *)temp_r2 + 1) = (u8) * (u8 *)((u8 *)temp_r1 + 1);
    *(u8 *)((u8 *)temp_r2 + 2) = (u8) * (u8 *)((u8 *)temp_r1 + 2);
    *(u8 *)((u8 *)temp_r2 + 3) = (u8) * (u8 *)((u8 *)temp_r1 + 3);
    temp_r1_2 = temp_r6 + 156;
    *(u8 *)((u8 *)temp_r0_4 + 6) = (u8) * (u8 *)((u8 *)temp_r6 + 0x138);
    *(u8 *)((u8 *)temp_r0_4 + 7) = (u8) * (u8 *)((u8 *)temp_r1_2 + 1);
    *(u8 *)((u8 *)temp_r0_4 + 8) = (u8) * (u8 *)((u8 *)temp_r1_2 + 2);
    *(u8 *)((u8 *)temp_r0_4 + 9) = (u8) * (u8 *)((u8 *)temp_r1_2 + 3);
    PaletteData_LoadPalette(battleParty->palette, temp_r0_4, PLTTBUF_SUB_BG, 0xD0U, 0x20U);
    Heap_Free(temp_r0_4);
}
