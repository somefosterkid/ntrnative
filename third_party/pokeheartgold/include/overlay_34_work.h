#ifndef POKEHEARTGOLD_OVERLAY_34_WORK_H
#define POKEHEARTGOLD_OVERLAY_34_WORK_H

#include "global.h"

#include "bg_window.h"
#include "field_types_def.h"
#include "g2d_renderer.h"
#include "mail_message.h"
#include "message_format.h"
#include "msgdata.h"
#include "player_data.h"
#include "save_palpad.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

struct UnionRoomManager;

typedef struct Ov34ChatEntry {
    String *name;
    String *sentence;
    String *friendName;
    u32 id;
    u32 gender;
    MailMessage mail;
} Ov34ChatEntry;

typedef struct Ov34ChatLog {
    Ov34ChatEntry entries[30];
    int count;
    int start;
} Ov34ChatLog;

typedef struct Ov34LinkEntry {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4[0x14];
} Ov34LinkEntry;

// Partial view of the UnkStruct_0205AC88 object (the union-room link manager): only the fields the
// Pokegear chat-log screen reads. unk4D4 is the FieldSystem pointer.
struct UnkStruct_0205AC88 {
    u8 unk0[0xC];
    Ov34LinkEntry entries[51];
    u8 unk4D4[4];
    SavePalPad *palPad;
    Ov34ChatLog *chatLog;
};

typedef struct Ov34Row {
    Window window0;
    Window window1;
    Window window2;
    int unk30;
    int unk34;
} Ov34Row;

// The Pokegear chat-log screen's SysTask environment (HEAP_ID_FIELD1, 0x2A8 bytes).
typedef struct Ov34Work {
    int state;
    struct UnkStruct_0205AC88 *unk4;
    struct UnionRoomManager *unk8;
    FieldSystem *fieldSystem;
    PlayerProfile *profile;
    BgConfig *bgConfig;
    MessageFormat *msgFormat;
    MsgData *msgData;
    void *scrnBuf;
    NNSG2dScreenData *scrnData;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMans[4];
    SpriteResource *resObjs[4];
    SpriteResourcesHeader header;
    Sprite *sprites[8];
    int unk1B8[3];
    int selectedRow;
    Ov34Row rows[3];
    Ov34ChatLog *chatLog;
    Window titleWindow;
    u16 count;
    u16 prevCount;
    u16 scroll;
    u16 scrollbarVisible;
    int prevScroll;
    int unk290;
    BOOL needsRedraw;
    u8 touchTrigger;
    s8 repeatCounter;
    u8 repeatStart;
    u8 repeatInterval;
    BOOL scrollbarHeld;
    u16 animFlags[2];
    SysTask *task;
} Ov34Work;

// sizeof(Ov34Work) == 0x2a8
// sizeof(Ov34ChatLog) == 0x350

#endif // POKEHEARTGOLD_OVERLAY_34_WORK_H
