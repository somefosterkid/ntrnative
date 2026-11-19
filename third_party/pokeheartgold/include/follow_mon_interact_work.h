#ifndef POKEHEARTGOLD_FOLLOW_MON_INTERACT_WORK_H
#define POKEHEARTGOLD_FOLLOW_MON_INTERACT_WORK_H

#include "global.h"

// Stack-local context filled by ov02_0224F058 for the follower-Pokemon interaction table (ov02_0224EF94)
typedef struct FollowMonInteractWork {
    u8 hasItem;         // 0x00
    u8 itemClass;       // 0x01
    u8 hpClass;         // 0x02
    u8 statusClass;     // 0x03
    u8 levelClass;      // 0x04
    u8 type1Class;      // 0x05
    u8 type2Class;      // 0x06
    u8 unk_07;          // 0x07
    u8 natureClass;     // 0x08
    u8 unk_09;          // 0x09
    u8 unk_0A;          // 0x0A
    u8 unk_0B;          // 0x0B
    u8 nearbyObjCount;  // 0x0C
    u8 hiddenItemCount; // 0x0D
    u8 nearSprite55;    // 0x0E
    u8 nearSprite54;    // 0x0F
    u8 nearSprite56;    // 0x10
    u8 unk_11;          // 0x11
    u8 unk_12;          // 0x12
    u8 unk_13;          // 0x13
    u8 unk_14;          // 0x14
    s8 mood;            // 0x15
    u8 unk_16;          // 0x16
    u8 unk_17;          // 0x17
    u8 filler_18[2];
    u16 mapId; // 0x1A
} FollowMonInteractWork; // size: 0x1C

#endif // POKEHEARTGOLD_FOLLOW_MON_INTERACT_WORK_H
