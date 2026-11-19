#ifndef POKEHEARTGOLD_UNK_02018000_H
#define POKEHEARTGOLD_UNK_02018000_H

#include "global.h"
#include "field/overlay_01_021FB878.h"

void sub_02018068(Field3dModel *model);

void sub_020180E8(Field3DModelAnimation *anim, const Field3dModel *model, void *data, NNSFndAllocator *allocator);

void sub_020180F8(Field3DModelAnimation *anim, NNSFndAllocator *allocator);
void sub_02018124(Field3DModelAnimation *anim, fx32 frameDelta);
BOOL sub_0201815C(Field3DModelAnimation *anim, fx32 frameDelta);
void sub_02018198(Field3DModelAnimation *anim, fx32 frame);

s32 sub_020181A4(void *arg0);

void sub_020181D4(NNSG3dRenderObj *arg0, void *arg1);

void sub_020181E0(NNSG3dRenderObj *arg0, void *arg1);

void sub_020182A0(void *arg0, s32 arg1);
s32 sub_020182A4(void *arg0);
void sub_020182A8(void *arg0, s32 arg1, s32 arg2, s32 arg3);
void sub_020182B0(void *arg0, s32 *arg1, s32 *arg2, s32 *arg3);
void sub_020182C4(void *arg0, s32 arg1, s32 arg2, s32 arg3);
void sub_020182CC(void *arg0, s32 *arg1, s32 *arg2, s32 *arg3);
u16 sub_020182EC(const Field3dObject *arg0, s32 arg1);

#endif // POKEHEARTGOLD_UNK_02018000_H
