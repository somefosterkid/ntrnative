#include "global.h"
#include "constants/heap.h"
#include "palette.h"
#include "sys_task.h"

struct PaletteAnimator;

struct PaletteData;

struct SelectedPaletteData;

struct SysTask;

struct SysTaskQueue;

u16 *PaletteData_GetUnfadedBuf(PaletteData *palette, enum PaletteBufferId bufferID);

void *SysTask_GetData(SysTask *task);

typedef struct PaletteAnimator PaletteAnimator;

typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);

struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};

u16 *GetMainBgPlttAddr(void);

u16 *GetSubBgPlttAddr(void);

SysTask *CreateSysTaskAndEnvironment(SysTaskFunc callback, int paramSize, u32 priority, enum HeapID heapID);

void sub_020164D0(SysTask *param0, void *param1);

void sub_020165A4(PaletteAnimator *param0, u16 *param1);

void sub_020165BC(PaletteAnimator *param0, u16 *param1);

void sub_020165D4(PaletteAnimator *param0, u16 *param1);

void sub_020165F0(PaletteAnimator *param0, u16 *param1);

PaletteAnimator *sub_020163E0(PaletteData *param0, u16 param1, u16 param2, enum HeapID heapID);

PaletteAnimator *sub_020163E0(PaletteData *param0, u16 param1, u16 param2, enum HeapID heapID) {
    PaletteAnimator *v0;
    SysTask *v1;
    u16 *v2;

    v1 = CreateSysTaskAndEnvironment(sub_020164D0, sizeof(PaletteAnimator), 0, heapID);
    v0 = (PaletteAnimator *)SysTask_GetData(v1);

    if (param0 != 0L) {
        if (param1 == 0) {
            v2 = PaletteData_GetUnfadedBuf(param0, (enum PaletteBufferId)(0));
            v0->unk_04 = sub_020165D4;
        } else {
            v2 = PaletteData_GetUnfadedBuf(param0, (enum PaletteBufferId)(1));
            v0->unk_04 = sub_020165F0;
        }
    } else {
        if (param1 == 0) {
            v2 = (u16 *)GetMainBgPlttAddr();
            v0->unk_04 = sub_020165A4;
        } else {
            v2 = (u16 *)GetSubBgPlttAddr();
            v0->unk_04 = sub_020165BC;
        }
    }

    MI_CpuCopy16(&v2[param2 * 16], v0->unk_0C, 0x20);
    MI_CpuCopy16(&v2[param2 * 16], v0->unk_2C, 0x20);

    v0->unk_00 = v1;
    v0->unk_08 = param0;
    v0->unk_4C = param1;
    v0->unk_4D = param2;
    v0->unk_4E = 1;
    v0->unk_4F = 0;

    return v0;
}
