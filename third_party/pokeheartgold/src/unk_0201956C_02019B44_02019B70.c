#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "unk_0201956C_types.h"

void sub_02019B70(UnkStruct_0201956C *frames, int a1, int a2, int a3, s8 a4, s8 a5);

void sub_02019B70(UnkStruct_0201956C *frames, int a1, int a2, int a3, s8 a4, s8 a5) {
    frames->entries[a1].left = a2;
    frames->entries[a1].right = a3;
    frames->entries[a1].top = a4;
    frames->entries[a1].bottom = a5;
}
