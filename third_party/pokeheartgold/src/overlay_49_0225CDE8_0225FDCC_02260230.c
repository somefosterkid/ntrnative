#include "global.h"

#include "overlay_49.h"

struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;
struct UnkStruct_ov70_0225DEE8_t;

struct UnkStruct_ov70_0225CC54_t *ov49_02258DAC(struct UnkStruct_ov70_0225CA20_t *param0);

u32 ov49_02258E60(const struct UnkStruct_ov70_0225CC54_t *param0, int param1);

struct UnkStruct_ov70_0225CA20_t *ov49_02259FF0(struct UnkStruct_ov70_0225DEE8_t *param0);

void ov49_0225A53C(struct UnkStruct_ov70_0225DEE8_t *param0, BOOL param1);

u32 ov49_02260230(void *param_1, void *param_2) {
    if (ov49_02258E60(ov49_02258DAC(ov49_02259FF0(param_2)), 5) != 0) {
        ov49_0225A53C(param_2, 1);
    }
    return 0;
}
