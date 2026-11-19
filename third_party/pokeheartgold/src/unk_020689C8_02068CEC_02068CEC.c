#include "global.h"

#include "overworld_anim_manager.h"

u32 sub_02068CEC(const OverworldAnimManager *manager, u32 mask) {
    return manager->flags & mask;
}

void sub_02068CF4(OverworldAnimManager *manager, OverworldAnimManager_InitFunc initFunc) {
    manager->initFunc = initFunc;
}

BOOL sub_02068CFC(OverworldAnimManager *manager) {
    return manager->initFunc(manager, sub_02068D74(manager));
}

void sub_02068D10(OverworldAnimManager *manager, OverworldAnimManager_TickFunc tickFunc) {
    manager->tickFunc = tickFunc;
}

void sub_02068D18(OverworldAnimManager *manager) {
    manager->tickFunc(manager, sub_02068D74(manager));
}

void sub_02068D2C(OverworldAnimManager *manager, OverworldAnimManager_RenderFunc renderFunc) {
    manager->renderFunc = renderFunc;
}

void sub_02068D34(OverworldAnimManager *manager) {
    manager->renderFunc(manager, sub_02068D74(manager));
}

void sub_02068D48(OverworldAnimManager *manager, OverworldAnimManager_ExitFunc exitFunc) {
    manager->exitFunc = exitFunc;
}

void sub_02068D50(OverworldAnimManager *manager) {
    manager->exitFunc(manager, sub_02068D74(manager));
}

void sub_02068D64(OverworldAnimManager *manager, OverworldAnimManager_Unused1Func unused1Func) {
    manager->unused1Func = unused1Func;
}

void sub_02068D6C(OverworldAnimManager *manager, OverworldAnimManager_Unused2Func unused2Func) {
    manager->unused2Func = unused2Func;
}
