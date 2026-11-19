#include "global.h"

#include "overlay_27_ov27task.h"

// The four neighbours (up, down, left, right: 3 candidates each, in order of preference) of each of the 7 start-menu icons.
extern const u8 ov27_0225D0B4[7][4][3];

s32 ov27_0225B360(s32 arg0, s32 arg1, Ov27TaskIcon *arg2);

// Returns the icon the cursor moves to from icon arg0 in direction arg1, or -1 if none of the candidates is enabled.
s32 ov27_0225B360(s32 arg0, s32 arg1, Ov27TaskIcon *arg2) {
    s32 i;
    const u8 *candidate;
    u8 icon;
    s32 result;

    result = -1;
    i = 0;
    candidate = ov27_0225D0B4[arg0][arg1];
    do {
        icon = *candidate;
        if (arg2[icon].unk_0 != 0) {
            result = icon;
        } else {
            i++;
            candidate++;
            if (i >= 3) {
                break;
            }
            continue;
        }
        break;
    } while (1);
    return result;
}
