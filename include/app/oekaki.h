// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/oekaki.h: the lines in use so far.
#ifndef BWPORT_APP_OEKAKI_H
#define BWPORT_APP_OEKAKI_H

#include "types.h"
#include "field/union_app.h"
#include "gfl/clact.h"
#include "nitro/spi/ARM9/tp.h"
#include "system/app_taskmenu.h"
#include "system/printsys.h"
#include "struct_decls.h"

#define HEAPID_OEKAKI 0x28
#define OEKAKI_MEMBER_MAX UNION_APP_MEMBER_MAX

// The base of the net commands, and the commands
#define OEKAKI_NET_CMD_BASE 0x2900

enum {
    // A chunk of the picture, which the parent sends a newcomer. It goes to every member, and the parent's own copy
    // makes it send the next
    OEKAKI_NET_CMD_PICTURE = OEKAKI_NET_CMD_BASE,
    // A child's OekakiTouchInfo, to the parent
    OEKAKI_NET_CMD_TOUCH,
    // Every member's OekakiTouchInfo, from the parent
    OEKAKI_NET_CMD_ALL_TOUCH,
    // A member joined: its net ID
    OEKAKI_NET_CMD_NEWCOMER,
    // The whole picture has been sent, and drawing goes on
    OEKAKI_NET_CMD_PICTURE_END,
    // An OekakiSignal, to the parent or from it
    OEKAKI_NET_CMD_SIGNAL,
    // The parent ends the session
    OEKAKI_NET_CMD_PARENT_END,
    // A child that was allowed to leave is leaving
    OEKAKI_NET_CMD_LEAVING,
    OEKAKI_NET_CMD_END,
};

// Black 2's; twinned code confirms the offsets of app and gameData in Black
// The proc's param, which is union_main.c's UnionProcParam
struct OekakiParam {
    UnionApp *app;
    GameData *gameData;
    u8 unk08[0x10];
};

// The proc's sequences
enum {
    OEKAKI_PROC_SEQ_FADE_IN,
    OEKAKI_PROC_SEQ_MAIN,
    // Only the sequences that sSeqTable flags run
    OEKAKI_PROC_SEQ_END,
    OEKAKI_PROC_SEQ_FADE_OUT,
    OEKAKI_PROC_SEQ_START_WAIT,
};

// Black 2's; twinned code confirms the values of OEKAKI_SEQ_NONE, OEKAKI_SEQ_NEWCOMER, OEKAKI_SEQ_NEWCOMER_WAIT,
// OEKAKI_SEQ_NEWCOMER_END, OEKAKI_SEQ_MAIN, OEKAKI_SEQ_END_SELECT_MSG_WAIT, OEKAKI_SEQ_END_SELECT,
// OEKAKI_SEQ_END_ANSWER_WAIT, OEKAKI_SEQ_END_REQUEST, OEKAKI_SEQ_END_OK, OEKAKI_SEQ_END_LEAVE, OEKAKI_SEQ_END_REFUSED,
// OEKAKI_SEQ_END_MSG, OEKAKI_SEQ_END_MSG_WAIT, OEKAKI_SEQ_END_WAIT, OEKAKI_SEQ_PARENT_END_SELECT_MSG_WAIT,
// OEKAKI_SEQ_PARENT_END_SELECT, OEKAKI_SEQ_PARENT_END_SEND, OEKAKI_SEQ_PARENT_END_MSG, OEKAKI_SEQ_PARENT_END_MSG_WAIT,
// OEKAKI_SEQ_PARENT_END_SYNC_WAIT, OEKAKI_SEQ_NO_MEMBERS_MSG, OEKAKI_SEQ_NO_MEMBERS_MSG_WAIT, OEKAKI_SEQ_MEMBER_LEFT,
// OEKAKI_SEQ_MEMBER_LEFT_MSG_WAIT and OEKAKI_SEQ_MEMBER_LEFT_WAIT in Black
// The screen's sequences, the rows of sSeqTable. The net commands set some of them with Oekaki_SetStatus
enum {
    OEKAKI_SEQ_NONE,
    OEKAKI_SEQ_NEWCOMER,
    OEKAKI_SEQ_NEWCOMER_WAIT,
    OEKAKI_SEQ_NEWCOMER_END,
    OEKAKI_SEQ_MAIN,
    OEKAKI_SEQ_END_SELECT_MSG_WAIT,
    OEKAKI_SEQ_END_SELECT,
    OEKAKI_SEQ_END_ANSWER_WAIT,
    OEKAKI_SEQ_END_REQUEST,
    OEKAKI_SEQ_END_OK,
    OEKAKI_SEQ_END_LEAVE,
    OEKAKI_SEQ_END_REFUSED,
    OEKAKI_SEQ_END_MSG,
    OEKAKI_SEQ_END_MSG_WAIT,
    OEKAKI_SEQ_END_WAIT,
    OEKAKI_SEQ_PARENT_END_SELECT_MSG_WAIT,
    OEKAKI_SEQ_PARENT_END_SELECT,
    OEKAKI_SEQ_PARENT_END_SEND,
    OEKAKI_SEQ_PARENT_END_MSG,
    OEKAKI_SEQ_PARENT_END_MSG_WAIT,
    OEKAKI_SEQ_PARENT_END_SYNC_WAIT,
    OEKAKI_SEQ_21,
    OEKAKI_SEQ_NO_MEMBERS_MSG,
    OEKAKI_SEQ_NO_MEMBERS_MSG_WAIT,
    OEKAKI_SEQ_MEMBER_LEFT,
    OEKAKI_SEQ_MEMBER_LEFT_MSG_WAIT,
    OEKAKI_SEQ_MEMBER_LEFT_WAIT,
    OEKAKI_SEQ_COUNT,
};

// What a member draws in a frame: a line from the first point to the second
struct OekakiTouchInfo {
    u8 x[2];
    u8 y[2];
    u8 color : 3;
    // How many of the points are set: 2 while the pen is down
    u8 size : 3;
    u8 brush : 2;
    // In the parent's: OEKAKI_BAN_*
    u8 banFlag;
};

// Whether the children may open the menu that leaves: not while the parent has its own open or a member is joining
enum {
    OEKAKI_BAN_NONE = 1,
    OEKAKI_BAN_ON,
};

// Where a member's line of the frame before ended
struct OekakiOldTouch {
    u8 x;
    u8 y;
    u16 size;
};

// The touch screen's points of a frame
struct OekakiTouchData {
    u16 size;
    TPData points[8];
};

// The frames of touches that wait to be sent: the parent's hold every member's, a child's its own in the first
#define OEKAKI_QUEUE_SIZE 100

struct OekakiQueue {
    OekakiTouchInfo fifo[OEKAKI_QUEUE_SIZE][OEKAKI_MEMBER_MAX];
    int start;
    int end;
};

// The picture is the drawing window's pixels, 30 by 15 tiles, sent in chunks
#define OEKAKI_PICTURE_SIZE (30 * 15 * 0x20)

#define OEKAKI_CHUNK_SIZE 1000

// What OEKAKI_NET_CMD_PICTURE carries
struct OekakiPictureChunk {
    u32 data[OEKAKI_CHUNK_SIZE / 4];
    // The data's words, exclusive-ored
    u32 checksum;
    u8 index;
};

// What OEKAKI_NET_CMD_SIGNAL carries
struct OekakiSignal {
    u8 netId;
    // The parent's answer to a request to leave: how many members there are
    u8 count;
    // OEKAKI_SIGNAL_*
    u8 type;
    // The parent's answer to a request to leave: whether the member may
    u8 ok;
};

enum {
    // A member asks the parent to leave, and the parent answers
    OEKAKI_SIGNAL_LEAVE_REQUEST,
    // The member leaves, which the parent tells everyone
    OEKAKI_SIGNAL_LEAVE,
};

// Black 2's; twinned code confirms the values of OEKAKI_RES_MAIN, OEKAKI_RES_COMMON and OEKAKI_RES_SUB in Black
// The actors' resources: the screen's own on each screen, and the menus' common ones
enum {
    OEKAKI_RES_MAIN,
    OEKAKI_RES_COMMON,
    OEKAKI_RES_SUB,
    OEKAKI_RES_COUNT,
};

struct OekakiResource {
    u32 palette;
    u32 chars;
    u32 cellAnims;
};

// The buttons: the eight colors, the button that ends, and the three brushes
enum {
    OEKAKI_BUTTON_COLOR,
    OEKAKI_BUTTON_END = 8,
    OEKAKI_BUTTON_BRUSH,
    OEKAKI_BUTTON_COUNT = 12,
};

struct OekakiMember {
    UnionAppMember *member;
    // The member of the update before
    UnionAppMember *old;
};

// Black 2's; twinned code confirms the offsets of param, vblankTcb, font, wordSet, msgData, nameBufs, endBuf, msgBuf,
// actorUnit, resources, cursors, nameActors, buttons, endButton, nameWindows, drawWindow, msgWindow, namePrint,
// msgPrint, nameQueue, printQueue, stream, tcbManager, menuRes, menu, menuItems, procSeq, seq, nextSeq, wait,
// memberCount, shareNum, shareBit, newcomerId, members, paletteAngle, color, brush, myTouch, allTouch, oldTouch,
// touchData, buffer, sendChunk, picture, sendChunkData, recvChunks, sendCommand, sendArg, signal, leftCount,
// leavingBits, signalCount, signalWait, leaving, parentEnded, queue, sendTouch, banFlag, menuOpen, unk9E74, waitAlone,
// joining and joiningBits in Black
struct OekakiWork {
    u32 unk0;
    OekakiParam *param;
    TCB *vblankTcb;
    Font *font;
    WordSet *wordSet;
    MsgData *msgData;
    StrBuf *nameBufs[OEKAKI_MEMBER_MAX];
    StrBuf *endBuf;
    StrBuf *msgBuf;
    u32 unk34;
    ClActUnit *actorUnit;
    OekakiResource resources[OEKAKI_RES_COUNT];
    // Each member's pen
    ClActor *cursors[OEKAKI_MEMBER_MAX];
    u8 unk74[0x24];
    // The marks beside the members' names on the top screen
    ClActor *nameActors[OEKAKI_MEMBER_MAX];
    u8 unkAC[0x24];
    ClActor *buttons[OEKAKI_BUTTON_COUNT];
    ClActor *endButton;
    BmpWin *nameWindows[OEKAKI_MEMBER_MAX];
    BmpWin *drawWindow;
    BmpWin *msgWindow;
    u8 unk120[0xc];
    PrintWindow namePrint[OEKAKI_MEMBER_MAX];
    u8 unk154[8];
    PrintWindow msgPrint;
    PrintQueue *nameQueue;
    PrintQueue *printQueue;
    PrintStream *stream;
    TCBExManager *tcbManager;
    AppTaskMenuRes *menuRes;
    AppTaskMenu *menu;
    AppTaskMenuItem menuItems[2];
    // The proc's sequence, an OEKAKI_PROC_SEQ_*
    int procSeq;
    // OEKAKI_SEQ_*: the sequence, and the one that the next frame changes to
    int seq;
    int nextSeq;
    u32 unk1A0;
    int wait;
    // How many members there were the frame before
    int memberCount;
    // The members that share the picture: how many, and their net ID bits
    int shareNum;
    u32 shareBit;
    int newcomerId;
    u8 unk1B8[0x10];
    OekakiMember members[OEKAKI_MEMBER_MAX];
    u8 unk1F0[0x4018];
    // The angle of the sine that pulses a color of the actors' palette
    u16 paletteAngle;
    u8 color;
    u8 brush;
    // What a child sends, and what the parent sent of every member
    OekakiTouchInfo myTouch;
    OekakiTouchInfo allTouch[OEKAKI_MEMBER_MAX];
    OekakiOldTouch oldTouch[OEKAKI_MEMBER_MAX];
    OekakiTouchData touchData;
    // Allocated and never used
    void *buffer;
    // The parent's: the chunk of the picture being sent
    int sendChunk;
    u8 picture[OEKAKI_PICTURE_SIZE];
    OekakiPictureChunk sendChunkData;
    OekakiPictureChunk recvChunks[OEKAKI_MEMBER_MAX];
    // A command that waits to be sent, or 0, with what it carries
    int sendCommand;
    u8 sendArg;
    u8 unk9275[3];
    OekakiSignal signal;
    // The parent's: how many members there were when one left, until those left have settled
    int leftCount;
    // The parent's: the children that are leaving (net ID bits)
    u32 leavingBits;
    // The count of the parent's answer to this machine's request to leave
    u16 signalCount;
    s16 signalWait;
    // Whether this machine was allowed to leave
    u16 leaving;
    // Whether the parent has ended the session
    u16 parentEnded;
    OekakiQueue queue;
    // What the parent sends with OEKAKI_NET_CMD_ALL_TOUCH
    OekakiTouchInfo sendTouch[OEKAKI_MEMBER_MAX];
    // The parent's OEKAKI_BAN_*
    int banFlag;
    BOOL menuOpen;
    u32 unk9E74;
    // Whether the parent, left alone, still has to wait for the others to be gone before it ends
    BOOL waitAlone;
    // The parent's: whether a member is joining, and who (net ID bits)
    BOOL joining;
    u32 joiningBits;
};

// pokebw2's OekakiComm_AddCommands
// oekaki_comm.c
// Adds the net commands
void func_ov023_021841e0(OekakiWork *wk);
// pokebw2's Oekaki_SetStatus
// oekaki.c
// Called by the net commands: sets the screen's sequence
void func_ov023_02185ab0(OekakiWork *wk, int seq, int netId);
// pokebw2's Oekaki_RequestSend
// Has the frame's update send a command, unless one is waiting already
void func_ov023_02186860(OekakiWork *wk, int command, int arg);
// pokebw2's Oekaki_QueuePush
// Puts a member's touch in the frame that is being filled
void func_ov023_02186988(OekakiTouchInfo *touch, OekakiQueue *queue, int netId);

#define OEKAKI_NET_CMD_COUNT (OEKAKI_NET_CMD_END - OEKAKI_NET_CMD_BASE)

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's OekakiComm_LimitEntry
// Lets up to `limit` more players enter
void func_ov023_02184560(OekakiWork *wk, u8 limit);

// The proc, which the Union Room runs for the drawing activity
extern const GameProcFunctions data_ov023_02186b30;

#endif // BWPORT_APP_OEKAKI_H
