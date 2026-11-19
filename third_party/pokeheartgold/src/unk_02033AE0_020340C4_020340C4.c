#include "global.h"

BOOL sub_020340C4(int cmd);

BOOL sub_020340C4(int cmd) {
    switch (cmd) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 8:
    case 37:
    case 38:
    case 39:
    case 40:
        return 1;
    }
    return 0;
}
