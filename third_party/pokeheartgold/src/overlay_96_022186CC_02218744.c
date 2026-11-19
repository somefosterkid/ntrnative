#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "math_util.h"
#include "sprite_system.h"
#include "overlay_96_ov96_rd_obj.h"

BOOL ov96_02218744(Ov96RdObj *obj);

BOOL ov96_02218744(Ov96RdObj *obj) {
    BOOL a;
    BOOL b;
    BOOL c;
    s32 state;

    GF_ASSERT(obj != NULL);
    GF_ASSERT(obj->state != 0);
    state = obj->state;
    a = FALSE;
    b = FALSE;
    c = FALSE;
    if (state != 10 && !obj->f26) {
        c = TRUE;
    }
    if (c && state != 4) {
        b = TRUE;
    }
    if (b && state != 3) {
        a = TRUE;
    }
    return a;
}
