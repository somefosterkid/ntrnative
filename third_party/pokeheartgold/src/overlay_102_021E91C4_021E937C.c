#include "global.h"

#include "bg_window.h"
#include "obj_char_transfer.h"
#include "unk_02013534.h"
#include "overlay_102_app_021e937c.h"

void ov102_021E937C(void *arg0);

void ov102_021E937C(void *arg0) {
    void *sp0;
    s32 var_r7;
    s32 var_r6;
    void *var_r4;
    UnkStruct_02021AC8 *var_r5;

    var_r6 = 0;
    sp0 = arg0;
    var_r4 = arg0;
    var_r5 = arg0 + 0x218;
    var_r7 = 0;
    do {
        TextOBJ_Destroy(*(TextOBJ **)((u8 *)var_r4 + offsetof(Ov102App_021E937C, unk_214[0].unk_0)));
        sub_02021B5C(var_r5);
        *(TextOBJ **)((u8 *)var_r4 + offsetof(Ov102App_021E937C, unk_214[0].unk_0)) = NULL;
        var_r6 += 1;
        var_r4 += sizeof(((Ov102App_021E937C *)0)->unk_214[0]);
        var_r5 = (void *)((u8 *)var_r5 + (0x10));
    } while (var_r6 < 2);
    sub_02013938(*(UnkStruct_02013910 **)((u8 *)sp0 + 0x200));
    RemoveWindow(sp0 + 0x204);
    sub_020135AC(*(UnkStruct_02013534 **)((u8 *)sp0 + 0x1FC));
}
