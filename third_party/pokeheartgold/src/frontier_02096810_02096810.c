#include "global.h"

#include "frontier/frontier.h"

void *Frontier_GetData(void *a0) {
    return (void *)**(void ***)a0;
}

void Frontier_SetData(void *a0, u32 a1) {
    **(u32 **)a0 = a1;
    return;
}
