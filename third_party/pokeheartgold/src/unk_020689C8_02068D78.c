#include "global.h"

#include "sys_task.h"
#include "overworld_anim_manager.h"

void sub_02068D78(OverworldAnimManager *manager, int ctxSize) {
    memset(sub_02068D74(manager), 0, ctxSize);
}

void sub_02068D8C(OverworldAnimManager *manager, int userInt) {
    manager->userInt = userInt;
}

int sub_02068D90(const OverworldAnimManager *manager) {
    return manager->userInt;
}

void sub_02068D94(OverworldAnimManager *manager, const void *userData) {
    manager->userData = userData;
}

const void *sub_02068D98(const OverworldAnimManager *manager) {
    return manager->userData;
}

void sub_02068D9C(OverworldAnimManager *manager, SysTask *sysTask) {
    manager->tickSysTask = sysTask;
}

SysTask *sub_02068DA0(const OverworldAnimManager *manager) {
    return manager->tickSysTask;
}
