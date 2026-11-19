#include "global.h"

BOOL ov07_022207D0(void *cmd, const void *data, int size);

BOOL ov07_02220800(void *cmd);

BOOL ov07_02220800(void *cmd) {
    return ov07_022207D0(cmd, 0L, 0);
}
