// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/wipe_sub.h: the lines in use so far.
#ifndef BWPORT_SYSTEM_WIPE_SUB_H
#define BWPORT_SYSTEM_WIPE_SUB_H

#include "types.h"
#include "struct_decls.h"

// The steps of a pattern, in its WipeScreen's seq: setting up, running, freeing its work (where GFL_WipeForceEnd sends
// it) and done
enum {
    WIPE_SEQ_INIT,
    WIPE_SEQ_MAIN,
    WIPE_SEQ_EXIT,
    WIPE_SEQ_DONE,
};

// pokebw2's WipeFunc_BrightnessOut
// Master brightness fades
BOOL func_020226e0(WipeScreen *screen);
// pokebw2's WipeFunc_BrightnessIn
BOOL func_020226fc(WipeScreen *screen);
// pokebw2's WipeFunc_ShrinkLeftOut
// Window rectangles
BOOL func_02022804(WipeScreen *screen);
// pokebw2's WipeFunc_GrowRightIn
BOOL func_02022830(WipeScreen *screen);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's WipeFunc_LinesDownOut
// Scanline wipes
BOOL func_0202271c(WipeScreen *screen);
// pokebw2's WipeFunc_LinesDownIn
BOOL func_02022758(WipeScreen *screen);
// pokebw2's WipeFunc_LinesUpOut
BOOL func_02022790(WipeScreen *screen);
// pokebw2's WipeFunc_LinesUpIn
BOOL func_020227cc(WipeScreen *screen);
// pokebw2's WipeFunc_CircleOut
// H-blank circles
BOOL func_0202285c(WipeScreen *screen);
// pokebw2's WipeFunc_CircleIn
BOOL func_02022888(WipeScreen *screen);
// pokebw2's WipeFunc_GrowRightOut
// Window rectangles
BOOL func_020228b4(WipeScreen *screen);
// pokebw2's WipeFunc_ShrinkLeftIn
BOOL func_020228e0(WipeScreen *screen);

#endif // BWPORT_SYSTEM_WIPE_SUB_H
