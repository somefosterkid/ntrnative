#include "global.h"

#include "sys_task.h"

void Heap_Free(void *ptr);

void ov07_02232E10(void *ptr);

s32 ov07_02233EB8(void *, s32);
s32 ov07_02233EBC(void *, s32);
s32 ov07_02232E18(void *arg0);
s32 ov07_02232E40(void *arg0);
s32 ov07_02232E68(void *arg0);
s32 ov07_02232E90(void *arg0);
s32 ov07_02232EB8(void *arg0);
s32 ov07_02232EE0(void *arg0);
s32 ov07_02232F08(void *arg0);
s32 ov07_02232F30(void *arg0);

void ov07_02232E10(void *ptr) {
    Heap_Free(ptr);
}

s32 ov07_02232E18(void *arg0) {
    s32 v0;

    v0 = *(s32 *)((u8 *)arg0 + 4);
    if (v0 == 0) {
        *(s32 *)((u8 *)arg0 + 4) = (s32)(v0 + 1);
        ov07_02233EB8(arg0, 1);
    }
    if (ov07_02233EBC(arg0, 2) == 1) {
        return 0;
    }
    return 1;
}

s32 ov07_02232E40(void *arg0) {
    s32 v0;

    v0 = *(s32 *)((u8 *)arg0 + 4);
    if (v0 == 0) {
        *(s32 *)((u8 *)arg0 + 4) = (s32)(v0 + 1);
        ov07_02233EB8(arg0, 3);
    }
    if (ov07_02233EBC(arg0, 4) == 1) {
        return 0;
    }
    return 1;
}

s32 ov07_02232E68(void *arg0) {
    s32 v0;

    v0 = *(s32 *)((u8 *)arg0 + 4);
    if (v0 == 0) {
        *(s32 *)((u8 *)arg0 + 4) = (s32)(v0 + 1);
        ov07_02233EB8(arg0, 5);
    }
    if (ov07_02233EBC(arg0, 6) == 1) {
        return 0;
    }
    return 1;
}

s32 ov07_02232E90(void *arg0) {
    s32 v0;

    v0 = *(s32 *)((u8 *)arg0 + 4);
    if (v0 == 0) {
        *(s32 *)((u8 *)arg0 + 4) = (s32)(v0 + 1);
        ov07_02233EB8(arg0, 7);
    }
    if (ov07_02233EBC(arg0, 0xE) == 1) {
        return 0;
    }
    return 1;
}

s32 ov07_02232EB8(void *arg0) {
    s32 v0;

    v0 = *(s32 *)((u8 *)arg0 + 4);
    if (v0 == 0) {
        *(s32 *)((u8 *)arg0 + 4) = (s32)(v0 + 1);
        ov07_02233EB8(arg0, 0xF);
    }
    if (ov07_02233EBC(arg0, 0x12) == 1) {
        return 0;
    }
    return 1;
}

s32 ov07_02232EE0(void *arg0) {
    s32 v0;

    v0 = *(s32 *)((u8 *)arg0 + 4);
    if (v0 == 0) {
        *(s32 *)((u8 *)arg0 + 4) = (s32)(v0 + 1);
        ov07_02233EB8(arg0, 0x13);
    }
    if (ov07_02233EBC(arg0, 0x1B) == 1) {
        return 0;
    }
    return 1;
}

s32 ov07_02232F08(void *arg0) {
    s32 v0;

    v0 = *(s32 *)((u8 *)arg0 + 4);
    if (v0 == 0) {
        *(s32 *)((u8 *)arg0 + 4) = (s32)(v0 + 1);
        ov07_02233EB8(arg0, 0x15);
    }
    if (ov07_02233EBC(arg0, 0x16) == 1) {
        return 0;
    }
    return 1;
}

s32 ov07_02232F30(void *arg0) {
    s32 v0;

    v0 = *(s32 *)((u8 *)arg0 + 4);
    if (v0 == 0) {
        *(s32 *)((u8 *)arg0 + 4) = (s32)(v0 + 1);
        ov07_02233EB8(arg0, 0x17);
    }
    if (ov07_02233EBC(arg0, 0x1B) == 1) {
        return 0;
    }
    return 1;
}
