#ifndef BWPORT_OV037_021F36C0_H
#define BWPORT_OV037_021F36C0_H

// Overlay 37's gimmick code (src/ov037/ov037_021f36c0.c), which scrcmd_proc.c's commands load and call: they set
// the saved states of gimmick slots 2, 3, 4, 6 and 10 from script variables

#include "types.h"
#include "struct_decls.h"

void func_ov037_021f36c0(GameSystem *gsys, BOOL seen1, BOOL seen2);
void func_ov037_021f36e8(GameSystem *gsys, BOOL flag);
void func_ov037_021f3724(GameSystem *gsys, u32 flags);
// vars: eight script variables, each set to 1 where its slot is set
void func_ov037_021f3768(GameSystem *gsys, u16 **vars);
void func_ov037_021f3800(GameSystem *gsys);

#endif // BWPORT_OV037_021F36C0_H
