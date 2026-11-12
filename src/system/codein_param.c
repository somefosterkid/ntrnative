// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/codein_param.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "system/codein_param.h"
#include "types.h"
#include "gfl/heap.h"
#include "gfl/str.h"

// The parameters of the code input screen. Our names

// pokebw2's CodeInParam_Create
CodeInParam *func_0202e4ec(u32 heapId, u32 mode, u32 a2, u32 length, const int *blocks) {
    int i;
    CodeInParam *param = GFL_HeapAllocate(heapId, sizeof(CodeInParam), FALSE, "codein_param.c", 55);

    param->mode = mode;
    param->length = length;
    param->str = GFL_StrBufCreate(length + 1, heapId);
    param->unk1C = a2;
    for (i = 0; i < CODEIN_BLOCK_COUNT; i++) {
        param->blocks[i] = blocks[i];
    }
    param->blocks[i] = blocks[i - 1];
    return param;
}

// pokebw2's CodeInParam_Free
void func_0202e54c(CodeInParam *param) {
    GFL_StrBufFree(param->str);
    GFL_HeapFree(param);
}
