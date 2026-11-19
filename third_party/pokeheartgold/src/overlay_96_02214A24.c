#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_96_app_02210C98.h"

Ov96App_02210C98_Cpu *ov96_02214A24(enum HeapID heapId, s8 numCpu);

Ov96App_02210C98_Cpu *ov96_02214A24(enum HeapID heapId, s8 numCpu) {
    Ov96App_02210C98_Cpu *cpu;

    cpu = Heap_Alloc(heapId, sizeof(Ov96App_02210C98_Cpu));
    MI_CpuFill8(cpu, 0, sizeof(Ov96App_02210C98_Cpu));
    cpu->numCpu = numCpu;
    cpu->numHuman = 4 - numCpu;
    cpu->firstCpu = cpu->numHuman;
    cpu->interval = 3;
    cpu->tick = cpu->interval;
    return cpu;
}
