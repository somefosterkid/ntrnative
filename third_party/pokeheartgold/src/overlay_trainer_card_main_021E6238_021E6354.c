#include "global.h"

#include "constants/heap.h"
#include "constants/sndseq.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "menu_input_state.h"
#include "options.h"
#include "overlay_manager.h"
#include "overlay_trainer_card_main.h"
#include "player_data.h"
#include "render_text.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "system.h"
#include "unk_02005D10.h"
#include "unk_020210A0.h"
#include "unk_02035900.h"
#include "unk_0203A3B0.h"
#include "vram_transfer_manager.h"
#include "overlay_trainer_card.h"
#include "trainer_card_main.h"

void ov51_021E60F4(s32, s32, s32);

void ov51_021E6200(s32);

void ov51_021E6C6C(TrainerCardMain *);

void ov51_021E6CF0(void *, void *);

extern const u32 ov51_021E7F08[];

void ov51_021E6354(TrainerCardMain *data);

void ov51_021E6354(TrainerCardMain *data) {
    NNSG2dPaletteData *pltt1;
    NNSG2dPaletteData *pltt2;
    void *buf;
    TrainerCardMainCard *card;
    u8 idx;
    NNSG2dScreenData *scrn;

    buf = GfGfxLoader_GetPlttData(NARC_a_0_4_9, 0, &pltt1, HEAP_ID_25);
    DC_FlushRange(pltt1->pRawData, 0x200);
    GXS_LoadBGPltt(pltt1->pRawData, 0, 0x200);
    Heap_Free(buf);
    card = data->card;
    ov51_021E60F4(card->stars, card->pokedexObtained, card->version);
    buf = GfGfxLoader_GetPlttData(NARC_a_0_4_9, 0x1C, &pltt2, HEAP_ID_25);
    DC_FlushRange(pltt2->pRawData, 0x200);
    GX_LoadBGPltt(pltt2->pRawData, 0, 0x200);
    Heap_Free(buf);
    idx = data->card->caseVariant;
    if (idx == 0xFF) {
        data->frontCharFile = GfGfxLoader_LoadFromNarc(NARC_a_0_4_9, 0x2C, FALSE, HEAP_ID_25, FALSE);
        if (data->frontCharFile == NULL) {
            GF_AssertFail();
        }
        if (NNS_G2dGetUnpackedBGCharacterData(data->frontCharFile, &data->frontCharData) == FALSE) {
            GF_AssertFail();
        }
        if (data->card->gender == 0) {
            data->frontScreenFile = GfGfxLoader_GetScrnData(NARC_a_0_4_9, 0x36, FALSE, &data->frontScreen, HEAP_ID_25);
        } else {
            data->frontScreenFile = GfGfxLoader_GetScrnData(NARC_a_0_4_9, 0x37, FALSE, &data->frontScreen, HEAP_ID_25);
        }
    } else {
        data->frontCharFile = GfGfxLoader_LoadFromNarc(NARC_a_0_4_9, ov51_021E7F08[idx], FALSE, HEAP_ID_25, FALSE);
        if (data->frontCharFile == NULL) {
            GF_AssertFail();
        }
        if (NNS_G2dGetUnpackedBGCharacterData(data->frontCharFile, &data->frontCharData) == FALSE) {
            GF_AssertFail();
        }
        data->frontScreenFile = GfGfxLoader_GetScrnData(NARC_a_0_4_9, 0x3D, FALSE, &data->frontScreen, HEAP_ID_25);
        ov51_021E6200(data->card->caseVariant);
    }
    ov51_021E6C6C(data);
    GfGfxLoader_LoadCharData(NARC_a_0_4_9, 0x29, data->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, HEAP_ID_25);
    GfGfxLoader_LoadScrnData(NARC_a_0_4_9, 0x2F, data->bgConfig, GF_BG_LYR_SUB_2, 0, 0, FALSE, HEAP_ID_25);
    GfGfxLoader_LoadCharData(NARC_a_0_4_9, 0x2A, data->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_25);
    GfGfxLoader_LoadScrnData(NARC_a_0_4_9, 0x31, data->bgConfig, GF_BG_LYR_SUB_1, 0, 0, FALSE, HEAP_ID_25);
    GfGfxLoader_LoadCharData(NARC_a_0_4_9, 0x2B, data->bgConfig, GF_BG_LYR_SUB_1_F, 0, 0, FALSE, HEAP_ID_25);
    data->badgeFrameScreenFile = GfGfxLoader_GetScrnData(NARC_a_0_4_9, 0x35, FALSE, &data->badgeFrameScreen, HEAP_ID_25);
    if (data->showKantoBadges) {
        GfGfxLoader_LoadScrnData(NARC_a_0_4_9, 0x34, data->bgConfig, GF_BG_LYR_SUB_1_F, 0, 0, FALSE, HEAP_ID_25);
    } else {
        GfGfxLoader_LoadScrnData(NARC_a_0_4_9, 0x33, data->bgConfig, GF_BG_LYR_SUB_1_F, 0, 0, FALSE, HEAP_ID_25);
    }
    GfGfxLoader_LoadScrnData(NARC_a_0_4_9, 0x32, data->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_25);
    if ((data->showKantoBadges) == 0 && data->card->badges >= 0xFF) {
        scrn = data->badgeFrameScreen;
        CopyToBgTilemapRect(data->bgConfig, GF_BG_LYR_MAIN_2, 0, 7, 7, 9, scrn->rawData, 0, 0, scrn->screenWidth / 8, scrn->screenHeight / 8);
    } else if (data->card->badges == 0xFFFF) {
        scrn = data->badgeFrameScreen;
        CopyToBgTilemapRect(data->bgConfig, GF_BG_LYR_MAIN_2, 0, 14, 7, 9, scrn->rawData, 7, 0, scrn->screenWidth / 8, scrn->screenHeight / 8);
    }
    BgCommitTilemapBufferToVram(data->bgConfig, GF_BG_LYR_MAIN_2);
    ov51_021E6CF0(data->card->signature, data->signaturePixels);
}
