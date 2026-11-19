#include "global.h"

#include "overlay_90.h"

struct UnkStruct_ov114_0225D678_t;

typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;

void ov90_02258B24(UnkStruct_ov114_0225D678 *param0, int param1);

void ov90_0225C19C(int param0, int param1, void *param2, void *param3) {
    ov90_02258B24(param3, *((u32 *)param2));
}

struct UnkStruct_ov114_0225E854_t;

typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;

void ov90_02258B2C(UnkStruct_ov114_0225E854 *param0, u32 param1, BOOL param2);

void ov90_0225C1A8(int param0, int param1, void *param2, void *param3) {
    ov90_02258B2C(param3, param0, 1);
}

void ov90_0225C1B8(int param0, int param1, void *param2, void *param3) {
    ov90_02258B2C(param3, param0, 0);
}

void ov90_02258B44(UnkStruct_ov114_0225E854 *param0, BOOL param1);

void ov90_0225C1C8(int param0, int param1, void *param2, void *param3) {
    ov90_02258B44(param3, 1);
}

void ov90_0225C1D4(int param0, int param1, void *param2, void *param3) {
    ov90_02258B44(param3, 0);
}
