#ifndef POKEHEARTGOLD_UNK_0202DB34_H
#define POKEHEARTGOLD_UNK_0202DB34_H

#include "save.h"
#include "sav_chatot.h"

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined (tools/hg/absorb.py)
SOUND_CHATOT *sub_0202DB34(SaveData *saveData);
#else
int sub_0202DB34(SaveData *saveData);
#endif

#endif // POKEHEARTGOLD_UNK_0202DB34_H
