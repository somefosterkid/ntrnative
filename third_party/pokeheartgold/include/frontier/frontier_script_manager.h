#ifndef POKEHEARTGOLD_FRONTIER_SCRIPT_MANAGER_H
#define POKEHEARTGOLD_FRONTIER_SCRIPT_MANAGER_H

#include "constants/heap.h"

#include "frontier/frontier_script_context.h"

#include "bg_window.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"

struct BattleFrontier;
struct FrontierMenuManager;
struct Menu;

// The object FrontierScriptContext::frontierSystem really points at (DS size 0xBC).
// "FrontierSystem" in frontier_system.h is a 0x48-byte stand-in for its head; never
// index it as an array (param[1].unk4 + N) and never use raw +0xNN offsets on it:
// both are wrong on 64-bit because of the 11 pointers before msgWindow.
typedef struct FrontierScriptManager {
    struct BattleFrontier *frontier; // 0x00
    FrontierScriptContext *ctx[8]; // 0x04
    u16 unk_24[8]; // 0x24
    enum HeapID heapID; // 0x34
    u8 unk_38; // 0x38
    u8 unk_39; // 0x39
    MsgData *msgLoader; // 0x3C
    u8 *script; // 0x40
    MessageFormat *strTemplate; // 0x44
    String *string; // 0x48
    String *fmtString; // 0x4C
    u8 printerID; // 0x50
    s8 numSetContexts; // 0x51
    u16 scene; // 0x52
    u8 unused[5]; // 0x54
    u8 movementCount; // 0x59
    u8 isMsgBoxOpen; // 0x5A
    int unused2; // 0x5C
    struct FrontierMenuManager *menuMan; // 0x60
    Window msgWindow; // 0x64
    struct Menu *menu; // 0x74
    u16 *unused3; // 0x78
    void *savingIcon; // 0x7C
    int unused4[10]; // 0x80
    Window *castleHostInfoWin; // 0xA8
    Window *castlePlayer2InfoWin; // 0xAC
    u16 unk_B0;
    u16 unk_B2;
    u16 unk_B4;
    u16 unk_B6;
    u16 *unk_B8; // 0xB8
} FrontierScriptManager;

#endif // POKEHEARTGOLD_FRONTIER_SCRIPT_MANAGER_H
