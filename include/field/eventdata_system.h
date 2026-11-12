#ifndef BWPORT_FIELD_EVENTDATA_SYSTEM_H
#define BWPORT_FIELD_EVENTDATA_SYSTEM_H

#include "types.h"
#include "struct_decls.h"
#include "nitro/fx/fx.h"

typedef struct EventDataSystem EventDataSystem;

void func_ov010_0216245c(EventDataSystem *system, u16 index);
u16 func_ov010_02162958(EventData *param_1, EventWork *param_2, RailPosition *param_3);
u16 func_ov010_02162a54(EventData *eventData, EventWork *eventWork, void *position, u32 dir);
u16 func_ov010_02162a5c(EventData *eventData, EventWork *eventWork, void *position, u32 dir);
u16 func_ov010_02162940(EventData *eventData, EventWork *eventWork, const VecFx32 *position);

#endif // BWPORT_FIELD_EVENTDATA_SYSTEM_H
