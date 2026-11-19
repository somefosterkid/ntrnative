#include "global.h"

#include "frontier/frontier.h"
#include "frontier/frontier_script_context.h"

s32 ov80_02235774(void *, void *, SaveData *, u16, u16);

s32 ov80_0223573C(void *arg0);

s32 ov80_0223573C(void *arg0) {
    FrontierLaunchArgs *frontierLaunchArgs;

    frontierLaunchArgs = Frontier_GetLaunchArgs(**(void ***)arg0);
    if (ov80_02235774(arg0, Frontier_GetData(**(void ***)arg0), frontierLaunchArgs->saveData, *((u16 *)&((FrontierScriptContext *)arg0)->data[0]), (s32) *((u16 *)&((FrontierScriptContext *)arg0)->data[1])) == 1) {
        return 1;
    }
    return 0;
}
