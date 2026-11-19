#ifndef POKEHEARTGOLD_NETWORK_ICON_H
#define POKEHEARTGOLD_NETWORK_ICON_H

#include "global.h"
#include "sys_task.h"

// sizeof(NetworkIcon) == 0x24 on the DS
typedef struct NetworkIcon {
    int unused;
    int strength;
    int unused3;
    u16 x;
    u16 y;
    u8 isWifi;
    u8 screenId;
    u8 createdOnSubScreen;
    void *unused2;
    SysTask *postVBlankTask;
    GXOamAttr *oam;
    u8 palette;
    u8 vramType;
    u8 oamMode;
} NetworkIcon;

extern NetworkIcon *_021D4154;

#endif // POKEHEARTGOLD_NETWORK_ICON_H
