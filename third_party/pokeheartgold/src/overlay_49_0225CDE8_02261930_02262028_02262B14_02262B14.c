#include "global.h"

#include "overlay_49_getters.h"
#include "overlay_49_unk_struct_ov70_02266af0.h"

#include "error_handling.h"

struct UnkStruct_ov70_0225CA20_t;
struct UnkStruct_ov70_0225CC54_t;

struct UnkStruct_ov70_0225CC54_t *ov49_02258DAC(struct UnkStruct_ov70_0225CA20_t *param0);

void ov49_02258EEC(struct UnkStruct_ov70_0225CA20_t *param0, struct UnkStruct_ov70_0225CC54_t *param1, int param2);


s32 ov49_0225A040(void *);

void ov49_0225A0EC(void *);

void ov49_0225A0FC(void *, u32);

u32 ov49_0225A30C(void *, u32, u32);

void ov49_0225A37C(void *, u32, u32);

void *ov49_0225EF40(void *, u32);

void ov49_0225EF68(void *);

void *ov49_0225EF84(void *);

s32 ov49_0225EF88(void *);

void ov49_0225EF90(void *);

u32 ov49_02262B14(void *param_1, void *param_2);

u32 ov49_02262B14(void *param_1, void *param_2) {
    UnkStruct_ov70_02266C68 *countdown;
    s32 iVar2;
    u32 uVar3;
    u32 uVar4;
    int new_var;
    countdown = ov49_0225EF84(param_1);
    iVar2 = ov49_0225EF88(param_1);
    switch (iVar2) {
    case 0:
        countdown = ov49_0225EF40(param_1, sizeof(UnkStruct_ov70_02266C68));
        iVar2 = ov49_0225A040(param_2);
        switch (iVar2) {
        case 39:
            uVar4 = 0x2a;
            uVar3 = 0;
            break;

        case 40:
            uVar4 = 0x29;
            uVar3 = 1;
            break;

        case 41:
            uVar4 = 0x2b;
            uVar3 = 2;
            break;

        default:
            GF_AssertFail();
            return 1;
        }

        ov49_0225A37C(param_2, uVar3, 0);
        uVar4 = ov49_0225A30C(param_2, 1, uVar4);
        ov49_0225A0FC(param_2, uVar4);
        ov49_02262D70(countdown, param_2, uVar3, 0, 0);
        ov49_02262E04(countdown, param_2, 1);
        ov49_0225EF90(param_1);
        break;

    case 1:
        new_var = gSystem.newKeys & 0xf3;
        if (new_var != 0) {
            ov49_02262DB8(countdown, param_2);
            ov49_0225A0EC(param_2);
            struct UnkStruct_ov70_0225CA20_t *v5 = ov49_02259FF0(param_2);
            ov49_02258EEC(v5, ov49_02258DAC(v5), 1);
            ov49_0225EF68(param_1);
            return 1;
        }
        ov49_02262E04(countdown, param_2, 0);
        break;
    }

    return 0;
}
