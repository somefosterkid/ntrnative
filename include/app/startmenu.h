#ifndef BWPORT_APP_STARTMENU_H
#define BWPORT_APP_STARTMENU_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/proc.h"

// The proc of the menu after the title screen (overlay 88)
extern const GameProcFunctions data_ov088_021bd66c;

// What the menu ends with for Continue and New Game: the procs that follow have no source file yet
void func_ov088_021bc0e4(void);
void func_ov088_021bc0fc(void);
void func_ov088_021bc114(void);

// The procs that the other items lead to, which have no header of their own yet: the Mystery Gift (overlay 121), the
// Game Sync settings (the main program) and the Unova Link (overlay 107)
extern const GameProcFunctions data_ov121_021df538;
extern const GameProcFunctions data_0209def0;
extern const GameProcFunctions MB_PARENT_PROC_FUNCTIONS;

#endif // BWPORT_APP_STARTMENU_H
