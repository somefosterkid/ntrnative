#include "global.h"

#include "player_data.h"
#include "save.h"
#include "save_trainer_card.h"
#include "field_system.h"

void sub_0206940C(PlayerProfile *param_1, FieldSystem *param_2, u8 *param_3);

void sub_0206940C(PlayerProfile *param_1, FieldSystem *param_2, u8 *param_3) {
    struct SaveTrainerCard *data;
    s32 iVar1;
    BOOL BVar2;
    u8 *iVar3;
    u32 uVar4;
    u16 uVar5;

    data = Save_TrainerCard_Get(param_2->saveData);
    TrainerCard_GetBadgeShininessArr(data);
    uVar4 = 0;
    iVar3 = param_3 + 0x48;
    do {
        iVar1 = uVar4 * 4;
        *(u32 *)(iVar3 + iVar1) = *(u32 *)(iVar3 + iVar1) & 0xfffffffe;
        *(u32 *)(iVar3 + iVar1) = *(u32 *)(iVar3 + iVar1) & 1;
        uVar4 = uVar4 + 1 & 0xff;
    } while (uVar4 < 8);
    uVar5 = 1;
    uVar4 = 0;
    do {
        BVar2 = PlayerProfile_TestBadgeFlag(param_1, uVar4);
        if (BVar2 != 0) {
            *(u16 *)(param_3 + 6) = *(u16 *)(param_3 + 6) | uVar5;
        }
        uVar5 = uVar5 << 1;
        uVar4 = uVar4 + 1 & 0xff;
    } while (uVar4 < 0x10);
    return;
}
