#include "global.h"

#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov49_0225B0E0(Window *, UnkStruct_ov70_0225E4EC *, u32, u8, u8, u8, u8);
#else
void ov49_0225B0E0(Window *, UnkStruct_ov70_0225E4EC *, u32, u32, u32, u32, u8);
#endif

void ov49_0225A204(UnkStruct_ov70_0225DEE8 *param_1, u32 param_2, u32 param_3, u32 param_4, u8 param_5);

void ov49_0225A204(UnkStruct_ov70_0225DEE8 *param_1, u32 param_2, u32 param_3, u32 param_4, u8 param_5) {
    ov49_0225B0E0(&param_1->unk_428.unk_00, &param_1->unk_3C, 0x77, param_2, param_3, param_4, param_5);
    return;
}
