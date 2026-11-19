#include "global.h"

#include "gf_3d_vramman.h"
#include "overlay_89.h"

void ov89_02259D8C(void);

void ov89_02259D70(enum HeapID heapID) {
    GF_3DVramMan_Create(heapID, 0, 2, 0, 2, ov89_02259D8C);
}
