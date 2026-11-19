#include "global.h"

#include "battle/battle.h"
#include "battle/battle_controller.h"

#include "bg_window.h"
#include "overlay_12_battle_controller_opponent.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"

typedef struct HealthBox {
    u8 state;
    ManagedSprite *mainSprite;
    ManagedSprite *arrowSprite;
    BattleSystem *battleSys;
    SysTask *task_10;
    Window unused_14;
    u8 battler;
    u8 type;
    u8 selectedPartySlot;
    u8 numSafariBalls;
    s32 curHP;
    s32 maxHP;
    s32 damage;
    s32 hpTemp;
    s32 curExp;
    s32 maxExp;
    s32 expReward;
    s32 expTemp;
    u8 level;
    u8 gender;
    u8 status;
    u8 caughtSpecies;
    u8 command;
    u8 delay;
    u8 expSoundTimer;
    u8 scrollDirection : 1;
    u8 doneScrolling : 1;
    u8 unused_4F_2 : 1;
    u8 numberMode : 1;
    u8 padding : 4;
    SysTask *bobbingAnimTask;
    u16 degrees;
} HealthBox;

void ov12_02264E34(HealthBox *healthbox, int expReward);

s32 ov12_02264E68(HealthBox *healthbox);

void ov12_02260030(SysTask *task, void *data) {
    HealthBox *healthbox = data;
    int result;

    switch (healthbox->state) {
    case 0:
        healthbox->expSoundTimer = 0;
        PlaySE(1803);
        ov12_02264E34(healthbox, healthbox->expReward);
        healthbox->state++;
    case 1:
        if (healthbox->expSoundTimer < 8) {
            healthbox->expSoundTimer++;
        }

        result = ov12_02264E68(healthbox);

        if (result == -1) {
            if (healthbox->expSoundTimer >= 8) {
                StopSE(1803, 0);
                healthbox->state = 100;
            } else {
                healthbox->state++;
            }
        }
        break;
    case 2:
        healthbox->expSoundTimer++;

        if (healthbox->expSoundTimer >= 8) {
            StopSE(1803, 0);
            healthbox->state = 100;
        }
        break;
    default:
        BattleController_EmitClearCommand(healthbox->battleSys, healthbox->battler, healthbox->command);
        healthbox->task_10 = 0L;
        SysTask_Destroy(task);
        break;
    }
}
