#include "global.h"

#include "system.h"
#include "pm_string.h"
#include "overlay_43_unk_struct_ov64_0222_e060.h"
#include "overlay_43_palpad.h"

void ov43_0222A320(s32 *arg0);

void ov43_0222A338(struct PalPadShared *arg0, String *arg1, String *arg2);

void ov43_0222A320(s32 *arg0) {
    if (0xCF3 & gSystem.newKeys) {
        *arg0 = 0;
    }
}

void ov43_0222A338(PalPadShared *arg0, String *src, String *arg2) {
    if (src != NULL) {
        String_Copy(arg0->name, src);
    }
    if (arg2 != NULL) {
        String_Copy(arg0->code, arg2);
    }
}
