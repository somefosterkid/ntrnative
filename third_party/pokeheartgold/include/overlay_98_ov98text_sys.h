#ifndef POKEHEARTGOLD_OVERLAY_98_OV98TEXT_SYS_H
#define POKEHEARTGOLD_OVERLAY_98_OV98TEXT_SYS_H

#include "global.h"
#include "constants/heap.h"

struct BgConfig;
struct MessageFormat;
struct MsgData;
struct PlayerProfile;
struct String;
struct Window;
struct WindowTemplate;

typedef struct Ov98TextSys {
    enum HeapID heapId;
    struct Window *windows;
    struct MsgData *msgData;
    struct MsgData *commonMsgData;
    struct MessageFormat *msgFmt;
    struct String *string;
    int windowCount;
} Ov98TextSys;

// sizeof(Ov98TextSys) == 0x1c

Ov98TextSys *ov98_0221EABC(enum HeapID heapId, struct BgConfig *bgConfig, s32 windowCount, struct WindowTemplate *templates, s32 msgBank);
void ov98_0221EB84(Ov98TextSys *textSys, s32 windowCount);
void ov98_0221EBD8(Ov98TextSys *textSys, u32 windowIdx, u32 msgId, BOOL center);
void ov98_0221EBEC(Ov98TextSys *textSys, u32 windowIdx, u32 msgId, BOOL center, u8 y, u32 fontId);
void ov98_0221EC08(Ov98TextSys *textSys, u32 windowIdx, u32 msgId, u8 x, u8 y);
void ov98_0221EC24(Ov98TextSys *textSys, s32 windowIdx, s32 msgId, u8 fill);
void ov98_0221EC3C(Ov98TextSys *textSys, u32 windowIdx, u32 msgId, BOOL center, u8 x, u8 y, u32 fontId, u8 fill);
void ov98_0221ECD0(Ov98TextSys *textSys, s32 windowIdx, s32 msgId, s32 num, u32 numDigits, u32 idx);
void ov98_0221ED3C(Ov98TextSys *textSys, s32 windowIdx, s32 msgId);
void ov98_0221ED48(Ov98TextSys *textSys, s32 windowIdx, s32 msgId, u32 x, u8 y);
void ov98_0221EDA4(Ov98TextSys *textSys, s32 num, u32 numDigits, u32 idx);
void ov98_0221EDC4(Ov98TextSys *textSys, s32 windowIdx, s32 msgId, u32 idx, struct PlayerProfile *profile);
void ov98_0221EE28(Ov98TextSys *textSys, s32 windowIdx, u32 msgId);
void ov98_0221EE84(Ov98TextSys *textSys, s32 windowIdx);
void ov98_0221EE9C(Ov98TextSys *textSys, s32 windowIdx, struct String *string, u32 x, u8 y);
void ov98_0221EEDC(Ov98TextSys *textSys, s32 windowIdx);
void ov98_0221EEEC(Ov98TextSys *textSys, s32 windowIdx, u8 y);
struct MessageFormat *ov98_0221EEFC(Ov98TextSys *textSys);
void ov98_0221EF14(Ov98TextSys *textSys, s32 windowIdx);

#endif // POKEHEARTGOLD_OVERLAY_98_OV98TEXT_SYS_H
