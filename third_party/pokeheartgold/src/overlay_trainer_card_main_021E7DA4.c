#include "global.h"

typedef struct BadgeChimeState {
    u8 badgeChimePlayers[6 - 3 + 1][2];
} BadgeChimeState;

void ov51_021E7DA4(BadgeChimeState *badgeChimeState);

void ov51_021E7DA4(BadgeChimeState *badgeChimeState) {
    for (int i = 0; i < 6 - 3 + 1; i++) {
        badgeChimeState->badgeChimePlayers[i][0] = i;
        badgeChimeState->badgeChimePlayers[i][1] = 0xFF;
    }
}
