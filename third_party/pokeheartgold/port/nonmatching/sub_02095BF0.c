#include "global.h"

typedef struct GridCursor_02095BF0 {
    s8 x;
    s8 y;
    u8 idx;
} GridCursor_02095BF0;

typedef struct GridDims_02095BF0 {
    u8 cols;
    u8 rows;
    u8 count;
} GridDims_02095BF0;

/* move a grid cursor by (dx, dy); returns 0 = moved, 1/2 = blocked left/right, 3/4 = blocked up/down
   (HGSS asm loads the 1 through a pool word the disassembler named SDK_OVERLAY_field_ID, = 1) */
s32 sub_02095BF0(s32 unused, GridCursor_02095BF0 *cursor, const GridDims_02095BF0 *dims, s32 dx, s8 dy) {
    u8 cols = dims->cols;
    u8 rows = dims->rows;
    s32 ret = 0;
    s8 nx, ny, cur;

    if (dx != 0) {
        nx = cursor->x + dx;
        cur = cursor->y;
        if (nx < 0) {
            ret = 1;
        } else if (nx >= cols) {
            ret = 2;
        } else if ((u8)(nx + cur * cols) >= dims->count) {
            ret = 2;
        }
        if (ret != 0) {
            return ret;
        }
        cursor->x = nx;
        cursor->idx = nx + cur * cols;
    }
    if (dy != 0) {
        ny = cursor->y + dy;
        cur = cursor->x;
        if (ny < 0) {
            ret = 3;
        } else if (ny >= rows) {
            ret = 4;
        } else if ((u8)(cur + ny * cols) >= dims->count) {
            ret = 4;
        }
        if (ret != 0) {
            return ret;
        }
        cursor->y = ny;
        cursor->idx = cur + ny * cols;
    }
    return 0;
}
