#include "global.h"

#include "overlay_01_encounter_effect.h"
#include "overlay_116.h"
#include "sys_task.h"

void ov116_0225F1BC(SysTask *task, EncounterEffect *effect, const LugiaHoOhEncounterParam *param);

extern const LugiaHoOhEncounterParam _0225F384;

void ov116_0225F364(SysTask *arg0, void *arg1) {
    ov116_0225F1BC(arg0, arg1, &_0225F384);
}

extern const LugiaHoOhEncounterParam ov116_0225F398;

void ov116_0225F374(SysTask *arg0, void *arg1) {
    ov116_0225F1BC(arg0, arg1, &ov116_0225F398);
}
