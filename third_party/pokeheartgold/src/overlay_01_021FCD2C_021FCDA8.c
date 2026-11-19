#include "global.h"

#include "overlay_01_021FCD2C.h"
#include "overlay_01_unk_struct_ov5_021f0fb8.h"
#include "sys_task.h"

typedef void (*Ov01_021FCDA8Func)(UnkStruct_ov5_021F0FB8 *);

extern const Ov01_021FCDA8Func ov01_02208E0C[4];

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_021FCDC4(struct UnkStruct_ov5_021F0FB8 *);
void ov01_021FCDFC(struct UnkStruct_ov5_021F0FB8 *);

const Ov01_021FCDA8Func ov01_02208E0C[4] = { ov01_021FCDBC, ov01_021FCDC4, ov01_021FCDFC, NULL };
#endif

void ov01_021FCDA8(SysTask *param0, void *param1) {
    UnkStruct_ov5_021F0FB8 *env = param1;

    ov01_02208E0C[env->unk_0C](env);
    return;
}

void ov01_021FCDBC(UnkStruct_ov5_021F0FB8 *arg0) {
    arg0->unk_08 = 1;
}
