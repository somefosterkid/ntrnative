#include "global.h"
#include "constants/heap.h"
#include "heap.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "transform_data.h"
#include "pokemon_anim.h"

BOOL sub_02017068(struct PokepicAnimManager *monAnimMan, u8 index);
void sub_02017088(struct PokepicAnimManager *monAnimMan, u8 index);
void sub_020170C4(SysTask *task, void *monAnim);

BOOL sub_02017068(struct PokepicAnimManager *monAnimMan, u8 index) {
    ((index < monAnimMan->animCount) ? (void)0 : GF_AssertFail());
    return monAnimMan->anims[index].completed;
}

void sub_02017088(struct PokepicAnimManager *monAnimMan, u8 index) {
    if (monAnimMan->anims[index].task != 0L) {
        SysTask_Destroy(monAnimMan->anims[index].task);

        monAnimMan->anims[index].task = 0L;
        monAnimMan->anims[index].completed = 1;
        monAnimMan->anims[index].active = 0;

        Heap_Free(monAnimMan->anims[index].scriptData);
    }
}

void sub_020170C4(SysTask *task, void *monAnim) {
    PokemonAnim *anim = (PokemonAnim *)(monAnim);

    if (anim->startDelay == 0) {
        sub_020170FC(anim);
    } else {
        anim->startDelay--;
    }

    if (anim->endAnim) {
        anim->completed = 1;
        anim->active = 0;

        SysTask_Destroy(task);
        anim->task = 0L;
        Heap_Free(anim->scriptData);
    }
}
