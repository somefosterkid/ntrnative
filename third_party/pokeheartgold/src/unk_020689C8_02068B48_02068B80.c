#include "global.h"

#include "overworld_anim_manager.h"

void sub_02068B80(OverworldAnimManagerList *managerList) {
    int count = sub_02068CC4(managerList);
    OverworldAnimManager *iter = sub_02068CC8(managerList);

    do {
        if (sub_02068CCC(iter) == TRUE) {
            sub_02068B48(iter);
        }

        iter++;
        count--;
    } while (count != 0);
}
