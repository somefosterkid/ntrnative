#include "global.h"

#include "list_menu_2d.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "frontier/frontier_script_context.h"

s16 *ov80_0222BE24(void *, u16);

s32 ov80_0222C4D0(void *arg0);

s32 ov80_0222C4D0(void *arg0) {

    void *temp_r4;
    s32 var_r6;
    s16 *temp_r5;
    temp_r4 = *((void **)arg0);
    temp_r5 = ov80_0222BE24(arg0, *((u16 *)&((FrontierScriptContext *)arg0)->data[0]));
    var_r6 = Handle2dMenuInput_DeleteOnFinish(*((struct ListMenu2D **)(((u8 *)temp_r4) + 0x74)), *((enum HeapID *)(((u8 *)temp_r4) + 0x34)));
    if (var_r6 == (-1)) {
        if (System_GetTouchNew() == 1) {
            PlaySE(0x5DCU);
            var_r6 = (s32)Get2dMenuSelection(*((struct ListMenu2D **)(((u8 *)temp_r4) + 0x74)));
            Clear2dMenuWindowAndDelete(*((struct ListMenu2D **)(((u8 *)temp_r4) + 0x74)), *((enum HeapID *)(((u8 *)temp_r4) + 0x34)));
        } else {
            return 0;
        }
    }
    if (var_r6 == 0) {
        *temp_r5 = 0;
    } else {
        *temp_r5 = 1;
    }
    return 1;
}
