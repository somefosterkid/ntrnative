#include "global.h"

#include "gf_gfx_planes.h"

struct BoxDialGraphics;
typedef struct BoxDialGraphics BoxDialGraphics;

void ov83_022459A0(BoxDialGraphics *unused);

void ov83_022459A0(BoxDialGraphics *unused) {
    GfGfx_EngineATogglePlanes(GF_BG_LYR_SUB_0, 0);
}
