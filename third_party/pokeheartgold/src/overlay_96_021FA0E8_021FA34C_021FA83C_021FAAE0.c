#include "global.h"

BOOL ov96_021FAAE0(int pos, int radius, s16 *rect);

BOOL ov96_021FAAE0(int pos, int radius, s16 *rect) {
    int max = pos + radius;
    int min = pos - radius;
    BOOL ret = FALSE;
    if (min <= rect[1] + rect[3] && rect[1] <= max) {
        ret = TRUE;
    }
    return ret;
}
