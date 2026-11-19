#include "global.h"
#include "overlay_108_021E8850.h"

#include "safari_zone.h"
#include "overlay_108_safari_decoration_app.h"

void ov108_021E8C64(SafariDecorationApp *arg0) {
    SAFARIZONE_OBJECT sp0;
    SafariDecorationEntry *entry;

    MI_CpuFill8(&sp0, 0U, sizeof(SAFARIZONE_OBJECT));
    entry = &arg0->objects[arg0->selected];
    sp0.id = entry->id;
    sp0.x = (arg0->args->targets[entry->slot].x % 32);
    sp0.z = (arg0->args->targets[entry->slot].y % 32);
    sp0.y = arg0->args->unk1A;
    SafariZone_AddObjectToArea(&arg0->areaSet, arg0->args->areaIndex, &sp0);
    arg0->args->unk1C = (s16)sp0.id;
}
