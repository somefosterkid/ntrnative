#ifndef POKEHEARTGOLD_UNK_STRUCT_OV70_02242E58_H
#define POKEHEARTGOLD_UNK_STRUCT_OV70_02242E58_H

#include "global.h"
#include "bg_window.h"
#include "list_menu_items.h"
#include "message_printer.h"
#include "msgdata.h"
#include "save_layout.h"
#include "sprite.h"
#include "unk_0201956C.h"

// GTS search panel work: allocated by ov70_02242014 (copied from a GTSSearchPanelTemplate), then
// handed to every handler of the ov70_022466F8 state table. DS 0x80.
typedef struct UnkStruct_ov70_02242E58 {
    struct BgConfig *unk_0;
    Window *unk_4; // menuButtonWindows
    Window *unk_8;
    Sprite *unk_C;
    Sprite *unk_10;
    Sprite *unk_14;
    Sprite *unk_18;
    UnkStruct_0201956C *unk_1C;
    struct Pokedex *unk_20;
    MsgData *unk_24; // gts messages
    MsgData *unk_28; // species names
    MsgData *unk_2C; // country names
    u8 *unk_30; // dexSeenFlags
    ListMenuItem *unk_34;
    MessagePrinter *unk_38;
    s16 unk_3C;
    s16 unk_3E;
    s16 unk_40;
    s16 unk_42;
    s8 unk_44;
    s8 unk_45;
    u8 unk_46[0x2];
    int unk_48;
    int unk_4C;
    int unk_50;
    int unk_54; // ov70_02242014 arg1
    s16 unk_58;
    s16 unk_5A;
    s32 unk_5C;
    s32 unk_60; // ov70_02242014 arg2
    u8 unk_64[0x1A];
    u8 unk_7E;
    u8 unk_7F;
} UnkStruct_ov70_02242E58;

#ifndef SDK_PORT
LAYOUT_CHECK(UnkStruct_ov70_02242E58_size, sizeof(UnkStruct_ov70_02242E58) == 0x80);
#endif

#endif // POKEHEARTGOLD_UNK_STRUCT_OV70_02242E58_H
