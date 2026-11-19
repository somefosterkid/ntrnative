#include "global.h"

#include "math_util.h"
#include "save.h"
#include "save_trainer_house.h"
#include "overlay_112_app_021f328c.h"

s32 ov112_021F35A4(TrainerHouse *);

s32 ov112_021F35C8(TrainerHouse *, TrainerHouseSet *, s32 *);

s32 ov112_021F3608(SaveData *, u8 *);

void ov112_021F3630(SaveData *, u8 *);

void ov112_021F328C(SaveData *saveData, TrainerHouseSet *set);

void ov112_021F328C(SaveData *saveData, TrainerHouseSet *set) {
    u8 order[12];
    u32 found;
    u8 slot;
    s32 count;
    s32 i;
    BOOL full;
    TrainerHouse *house;

    slot = 0;
    found = 0;
    full = FALSE;
    house = Save_TrainerHouse_Get(saveData);
    if (TrainerHouseSet_CheckHasData(set) == 0) {
        ov112_021F35A4(house);
        return;
    }
    if (((Ov112App_021F328C *)set)->unk_1F2 == GF_CalcCRC16(set, 0x1F2)) {
        count = ov112_021F35A4(house);
        ov112_021F3608(saveData, order);
        i = ov112_021F35C8(house, set, &found);
        if (count >= 10 || found != 0) {
            full = TRUE;
            count--;
            slot = order[i];
        }
        if (full && i < count) {
            for (; i < count; i++) {
                house->sets[i] = house->sets[i + 1];
                order[i] = order[i + 1];
            }
        }
        MI_CpuCopy8(set, &house->sets[count], 0x180);
        order[count] = slot;
        ov112_021F3630(saveData, order);
    }
}
