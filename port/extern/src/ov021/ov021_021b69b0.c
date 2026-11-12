// The PC build compiles src/ov021/ov021_021b69b0.c through this file (tools/port_sources.py). The elevator script
// command (func_ov021_021b6ae0, field script command 0x1cb, src/ov010/scrcmd_vm.c) reads its floor table's offset in
// the expression that adds it to the script pointer, `param_1->pc + (s32)VM_Read32(param_1)`: Metrowerks reads pc after
// the read moves it on by 4, clang before, so on the PC the table would start 4 bytes early. The same order bug hung the
// trainer AI (port/extern/src/ov096/tr_ai.c, which explains it); here the command reads the offset first. Not yet run
// on the PC: no test save has an elevator. The DS build never sees this file.
#pragma push_macro("func_ov021_021b6ae0")
#define func_ov021_021b6ae0(vm, env) func_ov021_021b6ae0_game(vm, env)
#include "../../../../src/ov021/ov021_021b69b0.c"
#pragma pop_macro("func_ov021_021b6ae0")

u32 func_ov021_021b6ae0(VM *param_1, FieldScriptEnv *param_2) {
    s32 offset = VM_Read32(param_1);

    FieldScriptEnv_SetElevatorTable(param_2, (ElevatorFloor *)(param_1->pc + offset));
    return 0;
}
