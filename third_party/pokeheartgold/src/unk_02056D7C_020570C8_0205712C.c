#include "global.h"

#include "comm_player_manager.h"

void sub_0205712C(struct FieldSystem *unused) {
    s32 i;

    i = 0;
    do {
        CommPlayerManager *p = _021D41C4;
        if (p->active[i] != 0) {
            CommPlayerPosition *e = p->hostPositions;
            if (p->positionChanged[i] != 0 || p->sendAllPositions != 0) {
                p->positionChanged[i] = 0;
                sub_0205704C(i, &e[i]);
            }
        }
        i += 1;
    } while (i < 8);
    _021D41C4->sendAllPositions = 0;
}
