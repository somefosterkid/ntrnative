#include "global.h"

#include "sys_task.h"

BOOL DWC_SetOwnStatusData(void *data, u32 size);

BOOL ov00_021E6D60(void *data, u32 size);

BOOL ov00_021E6D60(void *data, u32 size) {
    return DWC_SetOwnStatusData(data, size);
}
