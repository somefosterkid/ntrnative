#include "global.h"

#include "bg_window.h"
#include "overlay_96_unk_struct_ov96_0220e3f0.h"

extern WindowTemplate ov96_0221CEEC;

void ov96_0221022C(UnkStruct_ov96_0220E3F0 *arg0);

void ov96_0221022C(UnkStruct_ov96_0220E3F0 *arg0) {
    AddWindow(arg0->bgConfig, &arg0->window, &ov96_0221CEEC);
}
