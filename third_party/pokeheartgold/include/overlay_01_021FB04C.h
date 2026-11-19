#ifndef POKEHEARTGOLD_OVERLAY_01_021FB04C_H
#define POKEHEARTGOLD_OVERLAY_01_021FB04C_H

#include "global.h"
#include "filesystem.h"
#include "overlay_01_loaded_map.h"
#include "sys_task.h"

void ov01_021FB174(SysTask *sysTask, void *sysTaskParam);

Bdhc *ov01_021FB254(void);
void ov01_021FB270(NARC *narc, const int bdhcSize, Bdhc *bdhc, u8 *buffer);
void ov01_021FB2E8(Bdhc *bdhc);
void ov01_021FB2F4(Bdhc *bdhc);
SysTask *ov01_021FB308(NARC *landDataNARC, const int bdhcSize, Bdhc *bdhc, BOOL *loadTaskRunning, u8 **buffer, BOOL *mapModelLoadTaskRunning);
void ov01_021FB354(SysTask *sysTask);

void ov01_021FB360(Bdhc *bdhc);

#endif // POKEHEARTGOLD_OVERLAY_01_021FB04C_H
