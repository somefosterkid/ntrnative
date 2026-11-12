// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/musical/musical_shot_sys.h: the lines in
// use so far.
#ifndef BWPORT_APP_MUSICAL_MUSICAL_SHOT_SYS_H
#define BWPORT_APP_MUSICAL_MUSICAL_SHOT_SYS_H

#include "types.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
struct MusicalShotParam {
    // Whether the touch screen asks to keep the photo in the save, after a musical, rather than showing a return
    // button
    BOOL askSave;
    // Whether to load overlay 132 (the musical's communication overlay) when the proc starts and unloads it at the end
    BOOL loadComm;
    MusicalShot *shot;
    MusicalSave *save;
    // The communication work
    void *comm;
};

extern GameProcFunctions data_ov112_021f8360;

#endif // BWPORT_APP_MUSICAL_MUSICAL_SHOT_SYS_H
