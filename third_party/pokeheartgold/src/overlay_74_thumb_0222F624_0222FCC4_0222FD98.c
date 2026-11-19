#include "global.h"

#include "mystery_gift.h"
#include "save.h"
#include "save_arrays.h"

typedef struct MysteryGiftEventHeader MysteryGiftEventHeader;

typedef struct MysteryGiftEventData MysteryGiftEventData;

struct MysteryGiftEventHeader {
    u16 title[36];
    u32 validGames;
    u16 id;
    u8 unique : 1;
    u8 unk_4E_1 : 1;
    u8 hasWonderCard : 1;
    u8 savePgt : 1;
    u8 shareable : 1;
    u8 fromSharing : 1;
    u8 : 2;
    u8 padding_4F;
};

struct MysteryGiftEventData {
    MysteryGiftEventHeader header;
    WonderCard wonderCard;
};

s32 ov74_0222FD98(SaveData *saveData, MysteryGiftEventData *param1);

s32 ov74_0222FD98(SaveData *saveData, MysteryGiftEventData *param1) {
    MysteryGiftSave *mg;
    BOOL BVar1;
    u32 index;
    u32 uVar2;

    mg = Save_MysteryGift_Get(saveData);
    uVar2 = (param1->header).validGames;
    if ((uVar2 == 0xffffffff) && ((param1->header).id == 0xffff)) {
        Save_MysteryGift_Init(mg);
        Save_NowWriteFile_AfterMGInit(saveData, 0);
        OS_ResetSystem(0);
    }
    index = (u32)(param1->header).id;
    if ((index >= 0x64) && (index <= 0x98)) {
        uVar2 = uVar2 | 0x1180;
    }
    if (uVar2 == 0) {
        uVar2 = 0xffffffff;
    }
    if ((uVar2 & (1 << GAME_VERSION)) == 0) {
        return 1;
    }
    if (((param1->header).unique == 1) && (BVar1 = SaveMysteryGift_ReceivedFlagTest(mg, index), BVar1 == 1)) {
        return 2;
    }
    if (((param1->header).hasWonderCard == 1) && (BVar1 = SaveMysteryGift_CardFindAvailable(mg), BVar1 == 0)) {
        return 4;
    }
    BVar1 = SaveMysteryGift_FindAvailable(mg);
    if (BVar1 == 0) {
        return 3;
    }
    if ((param1->header).fromSharing == 1) {
        return 5;
    }
    return 0;
}
