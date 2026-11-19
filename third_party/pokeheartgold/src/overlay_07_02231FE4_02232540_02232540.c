#include "global.h"

typedef struct {
    int a[11];
} ov07_02237228_Array;

extern const ov07_02237228_Array ov07_02237228;

BOOL ov07_02232540(int param0);

BOOL ov07_02232540(int param0) {
    int v0;
    BOOL v1;
    ov07_02237228_Array v2 = ov07_02237228;

    v1 = 0;

    for (v0 = 0; v0 < NELEMS(v2.a); v0++) {
        if (v2.a[v0] != param0) {
            continue;
        }

        v1 = 1;
        break;
    }

    return v1;
}

BOOL ov07_02232580(int param0);

typedef struct {
    int a[6];
} ov07_02237188_Array;

extern const ov07_02237188_Array ov07_02237188;

BOOL ov07_02232580(int param0) {
    int v0;
    BOOL v1;
    ov07_02237188_Array v2 = ov07_02237188;

    v1 = 0;

    for (v0 = 0; v0 < (sizeof(v2.a) / sizeof(*(v2.a))); v0++) {
        if (v2.a[v0] != param0) {
            continue;
        }

        v1 = 1;
        break;
    }

    return v1;
}

typedef struct {
    int a[3];
} ov07_0223717C_Array;

extern const ov07_0223717C_Array ov07_0223717C;

BOOL ov07_022325BC(int param0);

BOOL ov07_022325BC(int param0) {
    int v0;
    BOOL v1;
    ov07_0223717C_Array v2 = ov07_0223717C;

    v1 = 1;

    for (v0 = 0; v0 < NELEMS(v2.a); v0++) {
        if (v2.a[v0] != param0) {
            continue;
        }

        v1 = 0;
        break;
    }

    return v1;
}
