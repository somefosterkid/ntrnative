#include "global.h"

#include "bg_window.h"
#include "trainer_card_main.h"

void ov51_021E6C6C(TrainerCardMain *arg0);

void ov51_021E6C6C(TrainerCardMain *arg0) {
    NNSG2dScreenData *temp_r0;
    NNSG2dCharacterData *temp_r3;

    temp_r3 = arg0->frontCharData;
    BG_LoadCharTilesData(arg0->bgConfig, 7U, temp_r3->pRawData, temp_r3->szByte, 0U);
    temp_r0 = arg0->frontScreen;
    CopyToBgTilemapRect(arg0->bgConfig, 7U, 0x15U, 5U, 0xAU, 0xBU, temp_r0->rawData, 0x15U, 5U, (u8)((u32)temp_r0->screenWidth >> 3), (u8)((u32)temp_r0->screenHeight >> 3));
    BgCommitTilemapBufferToVram(arg0->bgConfig, 7U);
}
