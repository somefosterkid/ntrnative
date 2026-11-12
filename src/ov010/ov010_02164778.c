// Black's code at ov010 0x02164778-0x021647a0. No name is known: the module and address. One script command between
// hidden_item.c and calender.c, which pokebw2 has next to each other (each file as long as Black's): it loads overlay
// 47 and clears its gimmick's state

#include "types.h"
#include "field/field_script.h"
#include "gfl/overlay.h"
#include "ov047_021f36c0.h"
#include "system/vm.h"

extern u32 OVERLAY_47_ID[];


BOOL func_ov010_02164778(VM *vm, FieldScriptEnv *env) {
    GameSystem *gsys;

    gsys = FieldScriptEnv_GetGameSystem(env);
    GFL_OvlLoad(OVERLAY_ID(47));
    func_ov047_021f3800(gsys);
    GFL_OvlUnload(OVERLAY_ID(47));
    return FALSE;
}

