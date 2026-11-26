#include "global.h"
#include "frontier/frontier_script_context.h"
#include "frontier/frontier_script_manager.h"

u16 *ov80_0222BE24(void *, u16);

s32 ov80_0222C33C(void *arg0);

s32 ov80_0222C33C(void *arg0) {
    u16 *v0;
    void *v1;

    v1 = *(void **)arg0;
    v0 = ov80_0222BE24(arg0, *((u16 *)&((FrontierScriptContext *)arg0)->data[0]));
#if defined(SDK_PORT) || defined(NONMATCHING)
    if (((FrontierScriptManager *)v1)->menuMan != NULL) {
#else
    if (*(s32 *)((u8 *)v1 + 0x60) != 0) {
#endif
        return 0;
    }
    if (*v0 != 0xEEEE) {
        return 1;
    }
    return 0;
}
