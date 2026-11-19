#ifndef POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225F270_H
#define POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225F270_H

// UnkStruct_ov114_0225F270: the yes/no prompt state; one of the sub-objects of UnkStruct_ov114_0225E854_t (the ov90 Wi-Fi Plaza minigame app, 0x664)

#include "global.h"
#include "yes_no_prompt.h"

typedef struct UnkStruct_ov114_0225F270 {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
    YesNoPrompt *unk_04;
    YesNoPromptTemplate unk_08;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E;
    u8 unk_1F;
} UnkStruct_ov114_0225F270;

// sizeof(UnkStruct_ov114_0225F270) == 0x20

#endif // POKEHEARTGOLD_OVERLAY_90_UNK_STRUCT_OV114_0225F270_H
