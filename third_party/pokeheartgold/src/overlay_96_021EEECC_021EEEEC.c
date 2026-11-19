#include "global.h"

#include "bg_window.h"
#include "pm_string.h"
#include "save.h"
#include "player_data.h"
#include "msgdata.h"

struct JournalEntry;

struct MsgData;

struct MessageFormat;

struct PlayerProfile;

typedef struct JournalEntry JournalEntry;

typedef struct JournalManager {
    u8 pad_0[4];
    BgConfig *bgConfig;
    Window window[4];
    JournalEntry *journalEntry;
    PlayerProfile *trainerInfo;
    SaveData *saveData;
    MsgData *loader;
    MessageFormat *template;
    String *string;
    u16 tilemapBuffer_5C[1024];
    u16 tilemapBuffer_85C[1024];
    s8 page;
    u8 padding_105D;
    s8 state;
    s8 unk_105F;
    s8 unk_1060;
    u8 bgLayer1;
    u8 bgLayer2;
    u8 bgLayer3;
    u8 bgLayer4;
    u8 unused[3];
} JournalManager;

void ov96_021EEEEC(void *data);

void ov96_021EEEEC(void *data) {
    JournalManager *journalManager = data;

    DoScheduledBgGpuUpdates(journalManager->bgConfig);
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
