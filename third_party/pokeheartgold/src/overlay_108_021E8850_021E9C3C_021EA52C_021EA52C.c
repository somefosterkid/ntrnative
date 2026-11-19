#include "global.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"

s32 ov108_021EA52C(SafariDecorationApp *arg0, u8 arg1) {
    if (arg0->freeObjectSlots == 0) {
        return 4;
    }
    if (arg0->args->side != arg0->objects[arg1].config.isAnimated) {
        if (arg0->args->side == 0) {
            return 2;
        }
        return 3;
    }
    if (arg0->args->targets[arg0->objects[arg1].slot].valid == 0) {
        return 1;
    }
    return 0;
}
