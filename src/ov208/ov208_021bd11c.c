// Black's code at ov208 0x021bd11c-0x021bd148. No name is known: the module and address.

#include "types.h"
#include "app/zukan_detail_graphic.h"
#include "gfl/g3d.h"

struct G3DCamera {
    G3DCameraProjection projection;
    FxLookAt lookAt;
};

void func_ov208_021bd11c(ZukanDetailG3dSys *g3d) {
    GFL_G3DCameraFree(g3d->camera);
    GFL_G3DSysFree();
}

void func_ov208_021bd12c(ZukanDetailG3dSys *g3d) {
    GFL_G3DSysReset();
    GFL_G3DCameraFlush(g3d->camera);
    GFL_G3DSysMtxViewFlush();
}

void func_ov208_021bd140(ZukanDetailG3dSys *g3d) {
    GFL_G3DSysReqSwapBuffers();
}
