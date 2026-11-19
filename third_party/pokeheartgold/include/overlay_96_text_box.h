#ifndef POKEHEARTGOLD_OVERLAY_96_TEXT_BOX_H
#define POKEHEARTGOLD_OVERLAY_96_TEXT_BOX_H

#include "constants/heap.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"

struct WaitingIcon;

// hk389: the Pokeathlon events' message box (ov96_021EE740: Heap_Alloc(heapId, 0x38)), kept in each event's
// work object (Ov96App_021ED1D0.textBox, UnkStruct_ov96_021F7934.unk_18, ...). ov96_021EE75C sets it up,
// ov96_021EE8CC / ov96_021EE908 print a message, ov96_021EE830 ticks it, ov96_021EE808 frees it.
typedef struct Ov96TextBox {
    BgConfig *bgConfig;              // 0x00
    MessageFormat *msgFormat;        // 0x04
    MsgData *msgData;                // 0x08 msg 0x135
    MsgData *msgData2;               // 0x0C msg 0x137
    String *string;                  // 0x10 the printed message (ov96_021EEA08)
    enum HeapID heapId;              // 0x14
    Window window;                   // 0x18
    s32 unk_28;                      // 0x28 0: wait for the counter (auto-advance)
    s32 printerId;                   // 0x2C -1: none
    struct WaitingIcon *waitingIcon; // 0x30
    u32 counter : 16;                // 0x34 auto-advance frames left
    u32 speed : 8;                   // text speed
    u32 bgLayer : 3;
    u32 unk_34_27 : 1;
    u32 busy : 1;                    // a message is printing or waiting (ov96_021EEA80)
    u32 unk_34_29 : 3;
} Ov96TextBox; // 0x38

#endif // POKEHEARTGOLD_OVERLAY_96_TEXT_BOX_H
