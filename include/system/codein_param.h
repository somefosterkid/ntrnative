// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/system/codein_param.h: the lines in use so
// far.
#ifndef BWPORT_SYSTEM_CODEIN_PARAM_H
#define BWPORT_SYSTEM_CODEIN_PARAM_H

#include "types.h"
#include "struct_decls.h"

// The groups the digits are shown in, the last one repeated after them
#define CODEIN_BLOCK_COUNT 3

// Black 2's; twinned code confirms the offsets of mode, length, blocks, unk1C and str, and its size in Black
struct CodeInParam {
    u32 mode;
    // How many digits the code has
    u32 length;
    int blocks[CODEIN_BLOCK_COUNT + 1];
    u32 unk18;
    int unk1C;
    // The code typed, length characters
    StrBuf *str;
};

// pokebw2's CodeInParam_Create
// blocks holds the number of digits of each group
CodeInParam *func_0202e4ec(u32 heapId, u32 mode, u32 a2, u32 length, const int *blocks);

// Black 2's; twinned code confirms the values of CODEIN_MODE_TRAIN and CODEIN_MODE_RANK in Black
// What is asked for, which is also the message shown (text bank 28)
enum {
    CODEIN_MODE_FRIEND_CODE,
    CODEIN_MODE_TRAIN,
    CODEIN_MODE_RANK,
};

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's CodeInParam_Free
void func_0202e54c(CodeInParam *param);

#endif // BWPORT_SYSTEM_CODEIN_PARAM_H
