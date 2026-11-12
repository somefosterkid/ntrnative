#ifndef FLD_MMDL_BLACT_H
#define FLD_MMDL_BLACT_H

#include "struct_decls.h"
#include "gfl/blact.h"

struct FieldBlActWork;

// Called once the actor's material has loaded: the actor's BlAct index and the argument given to the loader
typedef void (*FldBlActLoadedFunc)(u32 actorIdx, void *arg);

u8 func_ov021_0219c648(FieldActor *param_1);
BlActSys *FieldBlAct_GetBlActSys(struct FieldBlActWork *work);
u32 func_ov021_0219c928(MMSys *param_1, const FieldActorConfig *param_2, u16 *param_3, VecFx32 *param_4, FldBlActLoadedFunc param_5, void *param_6);
void func_ov021_0219c9e0(FieldActor *param_1, u16 *param_2, u16 *param_3);
void func_ov021_0219c98c(MMSys *param_1, u16 *param_2);

#endif
