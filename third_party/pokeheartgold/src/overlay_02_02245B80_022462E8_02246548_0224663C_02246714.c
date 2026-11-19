#include "global.h"

#include "field_types_def.h"
#include "overlay_02_02245B80.h"
#include "task.h"

LegendCameraShakeTask *ov02_02246744(FieldSystem *fieldSystem, s32 param_2, s32 param_3, u16 param_4, u16 param_5);

s32 ov02_02246798(TaskManager *taskManager);

// overlay_02.h (the caller, ScrCmd_ScreenShake) declares this with four full-width params and the DS caller does not narrow them,
// so the narrow types below must not be visible to it: do not include overlay_02.h here and do not narrow the header prototype.
#if defined(SDK_PORT) || defined(NONMATCHING)
void ov02_02246714(TaskManager *man, u32 a1, u32 a2, u32 a3, u32 a4);
#else
void ov02_02246714(TaskManager *man, u8 a1, u8 a2, u16 a3, s16 a4);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov02_02246714(TaskManager *man, u32 a1, u32 a2, u32 a3, u32 a4) {
#else
void ov02_02246714(TaskManager *man, u8 a1, u8 a2, u16 a3, s16 a4) {
#endif
    FieldSystem *pFVar1;
    LegendCameraShakeTask *env;

    pFVar1 = TaskManager_GetFieldSystem(man);
    env = ov02_02246744(pFVar1, (s16)a1, (s16)a2, a3, (s16)a4);
    TaskManager_Call(man, (TaskFunc)(ov02_02246798), env);
    return;
}
