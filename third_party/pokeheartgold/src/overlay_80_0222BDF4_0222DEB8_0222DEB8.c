#include "global.h"

#include "frontier/frontier_script_context.h"
#include "frontier/overlay_80_0222BDF4.h"

#include "error_handling.h"
#include "sys_task.h"
#include "sys_task_api.h"

void ov80_0222F53C(SysTask *task, void *data);

void *sub_0209680C(void *);

s32 FrtCmd_078(FrontierScriptContext *ctx);

s32 FrtCmd_078(FrontierScriptContext *ctx) {
    FrontierScriptContext *new_var2;
    void *v0;
    u8 *new_var3;
    void (*new_var)(SysTask *task, void *data);
    void *v1;
    v0 = sub_0209680C(ctx->frontierSystem->unk0);
    new_var2 = ctx;
    new_var3 = ((u8 *)v0) + 0xB0;
    v1 = v0 + 0xB0;
    if ((*((SysTask **)new_var3)) != 0L) {
        GF_AssertFail();
        SysTask_Destroy(*((SysTask **)new_var3));
    }
    MI_CpuFill8(v1, 0U, 0xCU);
    *((s8 *)(((u8 *)v1) + 4)) = (s8)FrontierScript_ReadVar(new_var2);
    *((s8 *)(((u8 *)v1) + 5)) = (s8)FrontierScript_ReadVar(new_var2);
    *((s8 *)(((u8 *)v1) + 6)) = (s8)FrontierScript_ReadVar(ctx);
    *((s8 *)(((u8 *)v1) + 7)) = (s8)FrontierScript_ReadVar(new_var2);
    *((s16 *)(((u8 *)v1) + 8)) = (s16)FrontierScript_ReadVar(ctx);
    *((s8 *)(((u8 *)v1) + 0xA)) = (s8)FrontierScript_ReadVar(new_var2);
    new_var = ov80_0222F53C;
    *((SysTask **)new_var3) = SysTask_CreateOnMainQueue(new_var, v1, 0x12CU);
    return 0;
}
