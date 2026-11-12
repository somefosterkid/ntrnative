// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/digitalcardcheck.h: the lines in use so
// far.
#ifndef BWPORT_APP_DIGITALCARDCHECK_H
#define BWPORT_APP_DIGITALCARDCHECK_H

#include "types.h"
#include "app/wifibattlematch_graphic.h"
#include "app/wifibattlematch_util.h"
#include "app/wifibattlematch_view.h"
#include "gfl/heap.h"
#include "system/printsys.h"
#include "struct_decls.h"

typedef struct DigitalCardCheck DigitalCardCheck;

// Black 2's; twinned code confirms the values of DIGITAL_CARD_CHECK_WIFI in Black
// Which competition's card
enum {
    DIGITAL_CARD_CHECK_WIFI,
    DIGITAL_CARD_CHECK_LIVE,
};

typedef struct {
    // A DIGITAL_CARD_CHECK_*
    u32 mode;
    WifiBattleMatchMsgBox *msgBox;
    Font *font;
    // The Entree Forest's messages
    MsgData *msgData;
    PrintQueue *printQueue;
    WifiBattleMatchRes *seq;
    WifiBattleMatchGraphic *net;
    GameData *gameData;
} DigitalCardCheckParam;

struct DigitalCardCheck {
    // The messages of the digital card (event_fishing.h's)
    MsgData *msgData;
    WordSet *wordSet;
    WifiBattleMatchCard *card;
    WifiBattleMatchMenu *menu;
    WifiBattleMatchTask *task;
    DigitalCardCheckParam param;
    u16 heapId;
};


void func_ov176_0217d458(WifiBattleMatchTask *task, u32 *seq, void *work);

// pokebw2's DigitalCardCheck_Update
// Called every frame
void func_ov176_0217d434(DigitalCardCheck *check);
// pokebw2's DigitalCardCheck_Delete
void func_ov176_0217d410(DigitalCardCheck *check);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's DigitalCardCheck_Create
DigitalCardCheck *func_ov176_0217d398(const DigitalCardCheckParam *param, HeapID heapId);
// pokebw2's DigitalCardCheck_IsFinished
BOOL func_ov176_0217d44c(DigitalCardCheck *check);

#endif // BWPORT_APP_DIGITALCARDCHECK_H
