// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/save_control_intr.h: the lines in use
// so far.
#ifndef BWPORT_SAVE_SAVE_CONTROL_INTR_H
#define BWPORT_SAVE_SAVE_CONTROL_INTR_H

#include "types.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// pokebw2's SaveControlIntr_Free
void func_ov088_021bd29c(SaveControlIntr *intr);
// pokebw2's SaveControlIntr_Pause
void func_ov088_021bd368(SaveControlIntr *intr);
// pokebw2's SaveControlIntr_Resume
void func_ov088_021bd374(SaveControlIntr *intr);
// pokebw2's SaveControlIntr_IsPausedOrDone
BOOL func_ov088_021bd380(SaveControlIntr *intr);
// pokebw2's SaveControlIntr_IsDone
BOOL func_ov088_021bd3a0(SaveControlIntr *intr);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's SaveControlIntr_Create
SaveControlIntr *func_ov088_021bd25c(HeapID heapId, SaveControl *save);
// pokebw2's SaveControlIntr_Start
void func_ov088_021bd2b0(SaveControlIntr *intr);
// pokebw2's SaveControlIntr_Update
// Returns 0 until saving ends, then the save control's result
u32 func_ov088_021bd2f4(SaveControlIntr *intr);
// pokebw2's SaveControlIntr_Nop
void func_ov088_021bd39c(SaveControlIntr *intr);

#endif // BWPORT_SAVE_SAVE_CONTROL_INTR_H
