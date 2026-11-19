#include "global.h"
#include "sys_task.h"
#include "linear_interpolation_task.h"

struct Window;
typedef struct Window Window;
void *Heap_Alloc(u32 heapID, u32 size);
typedef struct UnkStruct_ov5_021DE79C {
    LinearInterpolationTaskS32 unk_00;
    LinearInterpolationTaskS32 unk_14;
    Window *unk_28;
    u8 unk_2C;
    u8 unk_2D;
    u8 unk_2E;
    u8 unk_2F;
} UnkStruct_ov5_021DE79C;

BOOL ov80_0223ADC8(LinearInterpolationTaskS32 *task);
UnkStruct_ov5_021DE79C *ov80_0223ADF4(u32 heapID);

void Heap_Free(void *ptr);
void ov80_0223AE0C(void *ptr);

BOOL ov80_0223ADC8(LinearInterpolationTaskS32 *task) {
    int interpolated = (task->delta * task->currentStep) / task->numSteps;

    task->currentValue = interpolated + task->startValue;

    if (task->currentStep + 1 <= task->numSteps) {
        task->currentStep++;
        return 0;
    }

    task->currentStep = task->numSteps;
    return 1;
}

UnkStruct_ov5_021DE79C *ov80_0223ADF4(u32 heapID) {
    UnkStruct_ov5_021DE79C *v0 = Heap_Alloc(heapID, sizeof(UnkStruct_ov5_021DE79C));

    memset(v0, 0, sizeof(UnkStruct_ov5_021DE79C));
    return v0;
}

void ov80_0223AE0C(void *ptr) {
    Heap_Free(ptr);
}
