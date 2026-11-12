// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/demo3d/demo3d_cmd.h: the lines in use so
// far.
#ifndef BWPORT_APP_DEMO3D_DEMO3D_CMD_H
#define BWPORT_APP_DEMO3D_DEMO3D_CMD_H

#include "types.h"
#include "app/demo3d/demo3d_data.h"
#include "app/demo3d/demo3d_engine.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
typedef void (*Demo3DCmdFunc)(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);

// The commands, in the order of DEMO3D_CMD_* from DEMO3D_CMD_SE_PLAY on
void func_ov203_021bebc8(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bec24(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bec3c(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bec60(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bec84(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bee04(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bee14(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bee1c(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bee40(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bef84(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bf128(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bf248(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bf26c(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bf290(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bf4c4(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bf78c(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);
void func_ov203_021bf79c(Demo3DCmdSys *sys, Demo3DEngine *engine, s32 *params);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's gDemo3DCmdFuncs
extern Demo3DCmdFunc data_ov203_021c3d00[DEMO3D_CMD_COUNT + 1];

#endif // BWPORT_APP_DEMO3D_DEMO3D_CMD_H
