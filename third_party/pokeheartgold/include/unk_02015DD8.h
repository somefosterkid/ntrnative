#ifndef POKEHEARTGOLD_UNK_02015DD8_H
#define POKEHEARTGOLD_UNK_02015DD8_H

#include "global.h"

void *sub_02015F8C(s32 *arg0);

s32 sub_02015FCC(void *arg0);

NNSGfdTexKey sub_02016170(NNSG2dCharacterData *charsData);
NNSGfdPlttKey sub_02016184(int paletteSlot);
void sub_02016198(NNSG2dCharacterData *charsData, NNSGfdTexKey vramKey, NNSG2dImageProxy *proxy);
void sub_020161A8(NNSG2dPaletteData *paletteData, NNSGfdPlttKey vramKey, NNSG2dImagePaletteProxy *proxy);

#endif // POKEHEARTGOLD_UNK_02015DD8_H
