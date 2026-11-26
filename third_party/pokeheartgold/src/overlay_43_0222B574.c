#include "global.h"
#include "options.h"
#include "player_data.h"
#include "overlay_43_palpad.h"
#include "overlay_43_palpad_fns.h"

struct AppTaskMenu;

struct AppTaskMenuRes;

struct BmpWin;

struct GameData;

struct StrBuf;

struct WifiList;

struct WifiListFriend;

typedef struct AppTaskMenu AppTaskMenu;

typedef struct AppTaskMenuRes AppTaskMenuRes;

typedef struct BmpWin BmpWin;

typedef struct GameData GameData;

typedef struct StrBuf StrBuf;

typedef struct WifiList WifiList;

typedef struct WifiListFriend WifiListFriend;

typedef struct {
    BmpWin *window;
    u8 flushPending;
} PrintWindow;

typedef struct DWCLoginIdData {
    u32 data[3];
} DWCLoginIdData;

typedef struct DWCUserData {
    u32 unk_0;
    DWCLoginIdData unk_4;
    u32 unk_10[3];
    s32 unk_1c;
    u32 flag;
    u32 unk_24;
    u8 unk_28[0x14];
    u32 crc32;
} DWCUserData;

typedef struct DWCFriendData {
    u32 data[3];
} DWCFriendData;

BOOL sub_0202C2DC(WifiList *wifiList, u32 index);

struct WifiListFriend {
    u16 name[8];
    u8 unk10[0xc];
    s32 profileId;
    u32 id;
    u16 wins;
    u16 losses;
    u16 draws;

    u16 lastYear;
    u8 lastMonth;
    u8 lastDay;
    u8 trainerView;
    u8 gender : 2;
    u8 unk2F_2 : 6;
};

struct WifiList {
    DWCUserData myUserData;
    DWCFriendData friendData[32];
    WifiListFriend friends[32];
    u8 unk7C0[4];
};

typedef struct {
    StrBuf *str;
    u16 color;
    u32 type;
} AppTaskMenuItem;

WifiList *sub_0202C6F4(GameData *gameData);

enum {
    MODE_SELECT_FRIEND_LIST,
    MODE_SELECT_CODE_IN,
    MODE_SELECT_MY_CODE,
    MODE_SELECT_RETURN,
    MODE_SELECT_EXIT,
};

typedef struct {
    u32 count;

    u32 metCount;

    u8 index[32];

    u8 number[32];
} WifiNoteFriends;

typedef struct {

    BOOL touch;
    GameData *gameData;
    u8 seq;

    u8 nextMode;
    u8 nextSeq;

    s8 friendPos;
    u8 textSpeed;
    u8 frameType;

    u8 lrEnabled;
    WifiNoteFriends friends;

    StrBuf *name;
    StrBuf *code;

    const u16 *oldName;
    BOOL hasOldName;
} WifiNoteData;

typedef struct {
    u32 cursor;
    u32 unk4;

    u16 buttonSeq;
    u16 buttonCount;
    PrintWindow menuWindow;
    void *screenBuffer;
    NNSG2dScreenData *screen;
    PrintWindow msgWindow;
    u32 unk24[2];
    StrBuf *msgStr;
    AppTaskMenuItem yesNoItems[2];
    AppTaskMenuRes *menuRes;
    AppTaskMenu *menu;
} WifiNoteModeSelect;

BOOL ov43_0222B574(WifiNoteModeSelect *select, WifiNoteData *data);


BOOL ov43_0222B574(WifiNoteModeSelect *select, WifiNoteData *data) {
    if (select->cursor == MODE_SELECT_CODE_IN) {
        int i;
        WifiList *wifiList = sub_0202C6F4(data->gameData);

        for (i = 0; i < 32; i++) {
            if (!sub_0202C2DC(wifiList, i)) {
                return 1;
            }
        }
    } else {
        return 1;
    }
    return 0;
}

void ov43_0222B5A8(PalPadRoster *roster, PalPadShared *shared, PalPadGfx *gfx, enum HeapID heapID) {
    roster->textSpeed = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(shared->saveData));
    ov43_0222B944(roster, shared, gfx, heapID);
}
