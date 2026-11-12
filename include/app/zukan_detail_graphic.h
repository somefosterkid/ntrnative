// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov298/zukan_detail_graphic.c: the detail
// screen's 3D system, which zukan_detail_graphic.c creates and ov208_021bd11c.c frees, begins and ends.
#ifndef BWPORT_APP_ZUKAN_DETAIL_GRAPHIC_H
#define BWPORT_APP_ZUKAN_DETAIL_GRAPHIC_H

#include "types.h"
#include "gfl/g3d.h"

typedef struct {
    G3DCamera *camera;
} ZukanDetailG3dSys;

// pokebw2's ZukanDetailG3dSys_Free
void func_ov208_021bd11c(ZukanDetailG3dSys *g3d);
// pokebw2's ZukanDetailG3dSys_Begin
void func_ov208_021bd12c(ZukanDetailG3dSys *g3d);
// pokebw2's ZukanDetailG3dSys_End
void func_ov208_021bd140(ZukanDetailG3dSys *g3d);

#endif
