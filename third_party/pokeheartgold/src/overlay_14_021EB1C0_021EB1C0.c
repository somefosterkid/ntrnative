#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021EB1C0(UnkStruct_ov14_boxapp *app) {
    if (app->sys->unk_4 == NULL) {
        return ov14_021F7D9C[app->unk30](app);
    }
    return 5;
}
