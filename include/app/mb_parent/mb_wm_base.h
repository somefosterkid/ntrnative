#ifndef MB_WM_BASE_H
#define MB_WM_BASE_H

#include "nitro/mb.h"

int MB_Init(void *work, const MBUserInfo *user, u32 ggid, u32 tgid, u32 dma);
BOOL MB_SetParentCommParam(u16 sendSize, u16 maxChildren);

#endif
