// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov298/zukan_detail_procsys.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/zukan_detail.h"
#include "gfl/heap.h"

// Runs one of the Pokédex detail screens' pages (its info, map, cry and forms) as a small process: its init, main and
// exit functions in turn, plus a command and a draw function that the detail screen calls every frame

enum {
    PROCSYS_INIT,
    PROCSYS_MAIN,
    PROCSYS_EXIT,
};

// pokebw2's ZukanDetailProcSys_Create
ZukanDetailProcSys *func_ov208_021bc240(const ZukanDetailProcFuncs *funcs, void *param, HeapID heapId) {
    ZukanDetailProcSys *sys = GFL_HeapAllocate(heapId, sizeof(ZukanDetailProcSys), FALSE, "zukan_detail_procsys.c", 79);
    sys->funcs = funcs;
    sys->seq = PROCSYS_INIT;
    sys->subSeq = 0;
    sys->param = param;
    sys->work = NULL;
    return sys;
}

// pokebw2's ZukanDetailProcSys_Free
void func_ov208_021bc26c(ZukanDetailProcSys *sys) {
    GFL_HeapFree(sys);
}

// pokebw2's ZukanDetailProcSys_Main
BOOL func_ov208_021bc274(ZukanDetailProcSys *sys, ZukanDetailCommon *common) {
    BOOL done = TRUE;

    switch (sys->seq) {
    case PROCSYS_INIT:
        if (sys->funcs->init != NULL) {
            done = sys->funcs->init(sys, &sys->subSeq, sys->param, sys->work, common);
        }
        if (done == TRUE) {
            sys->seq = PROCSYS_MAIN;
            sys->subSeq = 0;
        }
        break;
    case PROCSYS_MAIN:
        if (sys->funcs->main != NULL) {
            done = sys->funcs->main(sys, &sys->subSeq, sys->param, sys->work, common);
        }
        if (done == TRUE) {
            sys->seq = PROCSYS_EXIT;
            sys->subSeq = 0;
        }
        break;
    case PROCSYS_EXIT:
        if (sys->funcs->exit != NULL) {
            done = sys->funcs->exit(sys, &sys->subSeq, sys->param, sys->work, common);
        }
        if (done == TRUE) {
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// pokebw2's ZukanDetailProcSys_Command
void func_ov208_021bc2f0(ZukanDetailProcSys *sys, ZukanDetailCommon *common, int command) {
    if (sys->funcs->command != NULL) {
        sys->funcs->command(sys, &sys->subSeq, sys->param, sys->work, common, command);
    }
}

// pokebw2's ZukanDetailProcSys_Draw
void func_ov208_021bc310(ZukanDetailProcSys *sys, ZukanDetailCommon *common) {
    if (sys->funcs->draw != NULL) {
        sys->funcs->draw(sys, &sys->subSeq, sys->param, sys->work, common);
    }
}

// pokebw2's ZukanDetailProcSys_AllocWork
void *func_ov208_021bc32c(ZukanDetailProcSys *sys, u32 size, HeapID heapId) {
    sys->work = GFL_HeapAllocate(heapId, size, FALSE, "zukan_detail_procsys.c", 200);
    return sys->work;
}

// pokebw2's ZukanDetailProcSys_FreeWork
void func_ov208_021bc34c(ZukanDetailProcSys *sys) {
    GFL_HeapFree(sys->work);
    sys->work = NULL;
}
