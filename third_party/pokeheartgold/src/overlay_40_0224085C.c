#include "global.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

typedef struct UnkStruct_ov40_02240910_v0 {
    u8 unk_0[0x80];
    void *unk_80;
    u8 unk_84[0x8C];
    void *unk_110;
} UnkStruct_ov40_02240910_v0;
struct UnkStruct_0208C06C;

void ov39_022271C0(void *, void *);

void *sub_02030920(u32);

void *sub_020314A4(u32);

void ov40_0224085C(struct UnkStruct_0208C06C *param_1);

void *sub_02030938(void *);
void *sub_020314BC(void *);
void ov40_02240910(struct UnkStruct_0208C06C *arg0);

void ov40_0224085C(UnkStruct_0208C06C *param_1) {
    void *uVar1;
    u8 *iVar2;

    iVar2 = *((u8 **)&param_1->unk_860);
    uVar1 = sub_020314A4(0x6d);
    *(void **)(iVar2 + 0x80) = uVar1;
    ov39_022271C0(*(void **)(iVar2 + 0x4bc), *(void **)(iVar2 + 0x80));
    *(u32 *)(iVar2 + 0x10c) = *(u32 *)(iVar2 + 0x80);
    uVar1 = sub_02030920(0x6d);
    *(void **)(iVar2 + 0x110) = uVar1;
    MI_CpuCopy8((void *)(*(u8 **)(iVar2 + 0x4bc) + 0x80), *(void **)(iVar2 + 0x110), 100);
    return;
}

void ov40_022408AC(u8 *param_1);

void ov40_022408AC(u8 *param_1) {
    void *uVar1;
    u8 *iVar2;

    iVar2 = *(u8 **)(param_1 + 0x860);
    uVar1 = sub_020314A4(0x6d);
    *(void **)(iVar2 + 0x80) = uVar1;
    ov39_022271C0(*(void **)(param_1 + *(s32 *)(param_1 + 0x4d4) * 4 + 0x2608),
        *(void **)(iVar2 + 0x80));
    *(u32 *)(iVar2 + 0x10c) = *(u32 *)(iVar2 + 0x80);
    uVar1 = sub_02030920(0x6d);
    *(void **)(iVar2 + 0x110) = uVar1;
    MI_CpuCopy8((void *)(*(u8 **)(param_1 + *(s32 *)(param_1 + 0x4d4) * 4 + 0x2608) + 0x80),
        *(void **)(iVar2 + 0x110),
        100);
    return;
}

void ov40_02240910(UnkStruct_0208C06C *arg0) {
    UnkStruct_ov40_02240910_v0 *v0;

    v0 = arg0->unk_860;
    sub_02030938(v0->unk_110);
    sub_020314BC(v0->unk_80);
}
