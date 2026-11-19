#include "global.h"
struct EvolutionData;

#include "pokepic.h"
#include "sys_task_api.h"
#include "unk_02026E30.h"
#include "evolution_data.h"

s32 sub_02075E14(void *);

s32 sub_020774E0(void);

void sub_02075D08(SysTask *task, void *param);

void sub_02075D08(SysTask *task, void *param) {
    EvolutionData *arg1 = param;
    sub_02075E14(arg1);
    if (*((s32 *)&arg1->summaryScreenAppMan) == 0) {
        PokepicManager_DrawAll(*((PokepicManager * *)&arg1->monSpriteMan));
        sub_020774E0();
        RequestSwap3DBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
    }
    if (arg1->done != 0) {
        SysTask_Destroy(task);
    }
}
