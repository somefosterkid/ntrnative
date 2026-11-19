#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "overlay_01_021F1348.h"
#include "overlay_01_021FB878.h"
#include "overlay_02_hmcut_in.h"

void ov02_0224B90C(HMCutIn *arg0);

void ov02_0224B90C(HMCutIn *arg0) {
    HMCutInFieldModel *d = &arg0->fieldModel;

    ov01_021FBDFC(&d->model);
    ov01_021F1448(d->modelData);
    Field3dModelAnimation_Unload(&d->anims[0], &d->allocator);
    Field3dModelAnimation_Unload(&d->anims[1], &d->allocator);
}
