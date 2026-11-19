#include "global.h"
struct PokemonSummary;

#include "gf_gfx_loader.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "pm_string.h"
#include "unk_0201956C.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"

void sub_0208887C(void);

void sub_02088894(struct PokemonSummary *arg0);
void sub_020889D0(PokemonSummary *arg0, NARC *arg1);

void sub_020887C4(PokemonSummary *arg0, NARC *arg1, NARC *narc) {
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x200U, HEAP_ID_19);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0x4400U, 0, HEAP_ID_19);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0xD, arg0->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 0, HEAP_ID_19);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 0, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x200U, HEAP_ID_19);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 1, arg0->bgConfig, GF_BG_LYR_SUB_2, 0U, 0x4400U, 0, HEAP_ID_19);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0x14, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_19);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x15, arg0->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 0, HEAP_ID_19);
}

void sub_0208887C(void) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, 23, 8);
}

void sub_02088894(PokemonSummary *arg0) {
    const u16 *src;

    arg0->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x12E, HEAP_ID_19);
    arg0->ribbonMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x1A8, HEAP_ID_19);
    arg0->numberPrinter = MessagePrinter_New(1U, 2U, 0U, HEAP_ID_19);
    arg0->msgFormat = MessageFormat_New(HEAP_ID_19);
    arg0->mon.speciesName = String_New(0xCU, HEAP_ID_19);
    arg0->mon.nickname = String_New(0xCU, HEAP_ID_19);
    arg0->mon.otName = String_New(8U, HEAP_ID_19);
    arg0->buffer = String_New(0x80U, HEAP_ID_19);
    arg0->moveNames = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0x2EE, HEAP_ID_19);
    arg0->playerName = String_New(8U, HEAP_ID_19);
    src = arg0->args->OTName;
    if (src != NULL) {
        CopyU16ArrayToString(arg0->playerName, src);
    }
}

void sub_0208895C(PokemonSummary *arg0) {
    DestroyMsgData(arg0->moveNames);
    DestroyMsgData(arg0->ribbonMsgData);
    DestroyMsgData(arg0->msgData);
    MessagePrinter_Delete(arg0->numberPrinter);
    MessageFormat_Delete(arg0->msgFormat);
    String_Delete(arg0->mon.speciesName);
    String_Delete(arg0->mon.nickname);
    String_Delete(arg0->mon.otName);
    String_Delete(arg0->buffer);
    String_Delete(arg0->playerName);
}

void sub_020889D0(PokemonSummary *arg0, NARC *narc) {
    arg0->cellManager = sub_0201956C(arg0->bgConfig, 2, 7, HEAP_ID_19);
    sub_020195F4(arg0->cellManager, 0, 6, 0x11, 6);
    sub_020196B8(arg0->cellManager, 0, narc, 0x45, 0);
    sub_020195F4(arg0->cellManager, 1, 6, 0x11, 6);
    sub_020196B8(arg0->cellManager, 1, narc, 0x46, 0);
    sub_020195F4(arg0->cellManager, 2, 6, 0xA, 4);
    sub_020196B8(arg0->cellManager, 2, narc, 0x47, 0);
    sub_020195F4(arg0->cellManager, 3, 6, 0xA, 4);
    sub_020196B8(arg0->cellManager, 3, narc, 0x48, 0);
    sub_020195F4(arg0->cellManager, 4, 6, 0xB, 0xE);
    sub_020196B8(arg0->cellManager, 4, narc, 0x49, 0);
    sub_020195F4(arg0->cellManager, 5, 6, 6, 3);
    sub_020196B8(arg0->cellManager, 5, narc, 0x4A, 0);
    sub_020195F4(arg0->cellManager, 6, 6, 6, 3);
    sub_020196B8(arg0->cellManager, 6, narc, 0x4B, 0);
}
