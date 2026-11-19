#include "global.h"
#include "player_data.h"

void ov73_021E7468(void);

struct PlayerProfile;
PlayerProfile *sub_02034818(int netId);
int ov73_021E746C(void);

void ov73_021E7468(void) {
}

int ov73_021E746C(void) {
    int v0, v1;

    for (v1 = 0, v0 = 0; v0 < 5; v0++) {
        if (sub_02034818(v0) != 0L) {
            v1++;
        }
    }

    return v1;
}
