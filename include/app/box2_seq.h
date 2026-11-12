// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/box2_seq.h, the lines in use so far.
#ifndef BWPORT_APP_BOX2_SEQ_H
#define BWPORT_APP_BOX2_SEQ_H

#include "types.h"
#include "app/box2_main.h"
#include "struct_decls.h"

// The PC box's sequences. Names are pokebw2's; the list stops at the box's first sequence

enum {
    BOX2SEQ_INIT = 0,
    BOX2SEQ_RELEASE,
    BOX2SEQ_WIPE,
    BOX2SEQ_PALETTE_FADE,
    BOX2SEQ_WAIT,
    BOX2SEQ_VFUNC,
    BOX2SEQ_TRGWAIT,
    BOX2SEQ_YESNO,
    BOX2SEQ_BUTTON_ANM,
    BOX2SEQ_SUBPROC_CALL,
    BOX2SEQ_SUBPROC_MAIN,
    BOX2SEQ_START,
};

// pokebw2's Box2Seq_Main: runs the current sequence; FALSE once the box is done
BOOL func_ov170_021e1e34(Box2SysWork *syswk, u32 *seq);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov170_021ea878(Box2SysWork *syswk, s16 item);
u32 func_ov170_021e8f50(Box2SysWork *syswk, u32 seq);

#endif // BWPORT_APP_BOX2_SEQ_H
