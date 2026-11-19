#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "frontier/frontier_script_context.h"

struct BattleFrontier;

struct FrontierMenuManager;

struct FrontierScriptContext;

struct FrontierScriptManager;

struct Menu;

struct MsgData;

struct MessageFormat;

typedef struct BattleFrontier BattleFrontier;

typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;

typedef struct Menu Menu;

typedef struct FrontierScriptManager FrontierScriptManager;

typedef struct FrontierMenuManager FrontierMenuManager;

struct FrontierScriptManager {
    BattleFrontier *frontier;
    FrontierScriptContext *ctx[8];
    UnkStruct_ov104_0222E8C8 unk_24;
    enum HeapID heapID;
    u8 unk_38;
    MsgData *msgLoader;
    u8 *script;
    MessageFormat *strTemplate;
    String *string;
    String *fmtString;
    u8 printerID;
    s8 numSetContexts;
    u16 scene;
    u8 unused[5];
    u8 movementCount;
    u8 isMsgBoxOpen;
    int unused2;
    FrontierMenuManager *menuMan;
    Window msgWindow;
    Menu *menu;
    u16 *unused3;
    void *savingIcon;
    int unused4[10];
    Window *castleHostInfoWin;
    Window *castlePlayer2InfoWin;
    u16 unk_B0;
    u16 unk_B2;
    u16 unk_B4;
    u16 unk_B6;
    u16 *unk_B8;
};

void ov80_0222E2B8(FrontierScriptManager *scriptMan);

void ov80_0222E344(FrontierScriptManager *scriptMan, enum Font font, int renderDelay, BOOL canSpeedUp, int autoScroll);

void ov80_0222E400(String *msgBuf, u16 sentenceType, u16 sentenceID, u16 word1, u16 word2);

void ov80_0222E3B8(FrontierScriptManager *scriptMan, u8 renderDelay, u16 sentenceType, u16 sentenceID, u16 word1, s16 word2, u8 canSpeedUp);

void MailMsg_Init(MailMessage *sentence);
String *MailMsg_GetExpandedString(const MailMessage *sentence, enum HeapID heapID);

void ov80_0222E3B8(FrontierScriptManager *scriptMan, u8 renderDelay, u16 sentenceType, u16 sentenceID, u16 word1, s16 word2, u8 canSpeedUp) {
    ov80_0222E2B8(scriptMan);

    ov80_0222E400(scriptMan->string, sentenceType, sentenceID, word1, word2);

    if (canSpeedUp != 0xFF) {
        ov80_0222E344(scriptMan, FONT_MESSAGE, renderDelay, canSpeedUp, 0);
    } else {
        ov80_0222E344(scriptMan, FONT_MESSAGE, 0, canSpeedUp, 0);
    }
}

void ov80_0222E400(String *msgBuf, u16 sentenceType, u16 sentenceID, u16 word1, u16 word2) {
    MailMessage sentence;
    String *string;

    MailMsg_Init(&sentence);
    MailMsg_SetMsgBankAndNum(&sentence, sentenceType, sentenceID);
    MailMsg_SetFieldI(&sentence, 0, word1);
    MailMsg_SetFieldI(&sentence, 1, word2);

    string = MailMsg_GetExpandedString(&sentence, HEAP_ID_FIELD3);
    String_Copy(msgBuf, string);
    String_Delete(string);
}
