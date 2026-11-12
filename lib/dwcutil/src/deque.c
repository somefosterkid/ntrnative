// libdwcutil's deque.c (Black's ov016 0x021d53dc-0x021d551c): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"
#include "nitro/fx/fx_cp.h"

typedef struct Ov016Stack {
    u16 capacity;
    u8 unk_2;
    u8 top;
    u32 items[1];
} Ov016Stack;

typedef struct DequeWork {
    s16 unk_00;
    s8 unk_02;
    s8 unk_03;
} DequeWork;

u32 DWCi_QUElForm(s32 size) {
    DequeWork *ptr;

    ptr = DWCi_HEAPlAllocEx(((size + 1) * 4) + 8, 4);
    ptr->unk_00 = (s16) (size + 1);
    ptr->unk_02 = 0;
    ptr->unk_03 = 0;
    return (u32)ptr;
}

u32 DWCi_QUElFormEx(s32 size, s32 data, s32 dataSize) {
    Ov016Stack *qcb;
    s32 i;

    qcb = (Ov016Stack *)DWCi_QUElForm(size);
    for (i = 0; i < size; i++) {
        qcb->items[i] = data;
        data += dataSize;
    }
    qcb->top = size;
    return (u32)qcb;
}

void DWCi_QUElDelete(u32 queue) {
    DWCi_HEAPlFree_(&queue);
}

void DWCi_QUElPushBack(u16 *queue, u32 data) {
    u32 mask;
    u32 remainder;

    mask = OS_DisableIrqMask(1);
    remainder = FX_ModS32(*(u8 *)((s32)queue + 3) + 1, (u32)*queue);
    if (remainder == *(u8 *)(queue + 1)) {
        OS_Terminate();
    }
    *(u32 *)(queue + (u32)*(u8 *)((s32)queue + 3) * 2 + 2) = data;
    *(s8 *)((s32)queue + 3) = (s8)remainder;
    OS_EnableIrqMask(mask);
}

u32 DWCi_QUElPopBack(u16 *queue) {
    Ov016Stack *stack = (Ov016Stack *)queue;
    u32 item = 0;
    OSIntrMode irq = OS_DisableIrqMask(1);

    if (stack->unk_2 != stack->top) {
        stack->top = FX_ModS32(stack->top + stack->capacity - 1, stack->capacity);
        item = stack->items[stack->top];
    }
    OS_EnableIrqMask(irq);
    return item;
}
