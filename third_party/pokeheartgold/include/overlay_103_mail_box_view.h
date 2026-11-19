#ifndef POKEHEARTGOLD_OVERLAY_103_MAIL_BOX_VIEW_H
#define POKEHEARTGOLD_OVERLAY_103_MAIL_BOX_VIEW_H

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "touchscreen_list_menu.h"
#include "unk_0201956C_types.h"
#include "unk_02019BA4.h"
#include "yes_no_prompt.h"

#define MAILBOX_VIEW_NUM_WINDOWS 30
#define MAILBOX_VIEW_NUM_SPRITES 7
#define MAILBOX_VIEW_NUM_MAILS   20

typedef struct MailBoxPaletteFx {
    u8 onScreen : 1;
    u8 target : 7;
    u8 firstPalette : 4;
    u8 secondPalette : 4;
    u8 phase;
    u8 timer;
    s8 x;
    s8 y;
    u8 width;
    u8 height;
} MailBoxPaletteFx;

// MailBoxView: the mailbox screen's graphics/text/sprite state (owned by MailBoxWork.view), allocated in heap 157.
typedef struct MailBoxView {
    BgConfig *bgConfig;                                 // 0x000
    UnkStruct_0201956C *frames;                         // 0x004
    u8 tileA[0x20];                                     // 0x008
    u8 tileB[0x20];                                     // 0x028
    Window windows[MAILBOX_VIEW_NUM_WINDOWS];           // 0x048
    MsgData *msgData;                                   // 0x228
    MessageFormat *msgFmt;                              // 0x22C
    String *string;                                     // 0x230
    u8 textPrinterId;                                   // 0x234
    u8 waitForInput;                                    // 0x235
    u8 pad_236[2];                                      // 0x236
    YesNoPrompt *yesNoPrompt;                           // 0x238
    u32 yesNoIndex;                                     // 0x23C
    TouchscreenListMenuSpawner *menuSpawner;            // 0x240
    TouchscreenListMenu *menu;                          // 0x244
    ListMenuItem *menuItems;                            // 0x248
    u8 pad_24C[4];                                      // 0x24C
    SpriteSystem *spriteSystem;                         // 0x250
    SpriteManager *spriteManager;                       // 0x254
    ManagedSprite *sprites[MAILBOX_VIEW_NUM_SPRITES];   // 0x258
    NARC *iconNarc;                                     // 0x274
    GridInputHandler *gridInput;                        // 0x278
    Mail *mails[MAILBOX_VIEW_NUM_MAILS];                // 0x27C
    u8 pageMails[MAILBOX_VIEW_NUM_MAILS];               // 0x2CC
    u16 mailCount;                                      // 0x2E0
    u16 maxPage;                                        // 0x2E2
    MailBoxPaletteFx paletteFx;                         // 0x2E4
    u32 wipeNextState;                                  // 0x2EC
} MailBoxView; // size = 0x2F0

// Functions taking the view
void ov103_021ECE18(MailBoxView *arg0);
void ov103_021ECE54(MailBoxView *arg0);
void ov103_021EDB60(MailBoxView *view, s32 mode);
void ov103_021EDF68(MailBoxView *view);
void ov103_021EDF88(MailBoxView *view);
void ov103_021EE078(MailBoxView *view, int slot);
void ov103_021EE0AC(MailBoxView *view, s32 idx, s32 anim);
void ov103_021EE0CC(MailBoxView *view, s32 idx, s32 show);
void ov103_021EE0F8(MailBoxView *view, s32 idx, s16 x, s16 y);
void ov103_021EE110(MailBoxView *view, s32 idx, s32 xlu);
void ov103_021EE13C(MailBoxView *view);
void ov103_021EE150(MailBoxView *view);
void ov103_021EE160(MailBoxView *arg0);
void ov103_021EE2E0(MailBoxView *arg0);
void ov103_021EE374(MailBoxView *arg0);
void ov103_021EE430(MailBoxView *view, MsgData *msgData, s32 windowIdx, s32 strno, u32 x, u32 y, enum Font font, u32 color, s32 align);
void ov103_021EE60C(MailBoxView *param_1);
void ov103_021EE628(MailBoxView *param_1);

#endif // POKEHEARTGOLD_OVERLAY_103_MAIL_BOX_VIEW_H
