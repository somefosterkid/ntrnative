// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/demo3d/demo3d_exp.h: the lines in use so
// far.
#ifndef BWPORT_APP_DEMO3D_DEMO3D_EXP_H
#define BWPORT_APP_DEMO3D_DEMO3D_EXP_H

#include "types.h"
#include "app/demo3d/demo3d_graphic.h"
#include "gfl/heap.h"
#include "struct_decls.h"

typedef struct Demo3DExp Demo3DExp;

// Black 2's; matching code confirms the offsets of main and exit in Black
typedef struct {
    void (*init)(Demo3DExp *exp);
    void (*main)(Demo3DExp *exp);
    void (*exit)(Demo3DExp *exp);
} Demo3DExpFuncs;

// Black 2's; twinned code confirms the offsets of engine, funcs, work and skippable in Black
struct Demo3DExp {
    HeapID heapId;
    Demo3DGraphic *graphic;
    Demo3DEngine *engine;
    const Demo3DExpFuncs *funcs;
    void *work;
    // Whether a key ends the demo
    BOOL skippable;
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's gDemo3DExpCCruiserFuncs
extern const Demo3DExpFuncs data_ov203_021c3bc4;

#endif // BWPORT_APP_DEMO3D_DEMO3D_EXP_H
