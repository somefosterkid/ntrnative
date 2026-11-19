#include "global.h"

#include "save_trainer_house.h"

s32 ov112_021F35A4(TrainerHouseSet *arg0);

s32 ov112_021F35A4(TrainerHouseSet *arg0) {
    TrainerHouseSet *var_r5;
    s32 var_r4;
    s32 var_r6;

    var_r4 = 0;
    var_r5 = arg0;
    var_r6 = 0;
loop_1:
    if (TrainerHouseSet_CheckHasData(var_r5) != 0) {
        var_r4 += 1;
        var_r5 = (void *)((u8 *)var_r5 + (0x180));
        var_r6 += 1;
        if (var_r4 < 0xA) {
            goto loop_1;
        }
    }
    return var_r6;
}

s32 ov112_021F35C8(TrainerHouseSet *arg0, TrainerHouseTrainer *arg1, s32 *arg2);

s32 ov112_021F35C8(TrainerHouseSet *arg0, TrainerHouseTrainer *arg1, s32 *arg2) {
    s32 *sp0;
    TrainerHouseSet *var_r5;
    s32 var_r4;

    var_r5 = arg0;
    sp0 = arg2;
    var_r4 = 0;
loop_1:
    if ((TrainerHouseSet_CheckHasData(var_r5) != 0) && (TrainerHouseTrainer_Compare(&var_r5->trainer, arg1) != 0)) {
        *sp0 = 1;
        return var_r4;
    }
    var_r4 += 1;
    var_r5 = (void *)((u8 *)var_r5 + (0x180));
    if (var_r4 >= 0xA) {
        *sp0 = 0;
        return 0;
    }
    goto loop_1;
}
