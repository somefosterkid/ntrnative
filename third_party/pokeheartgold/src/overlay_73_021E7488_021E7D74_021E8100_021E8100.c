#include "global.h"

#include "bg_window.h"
#include "unk_0200B150.h"
#include "vram_transfer_manager.h"
#include "pm_string.h"
#include "options.h"
#include "sprite.h"
#include "sprite_system.h"
#include "mail_message_types.h"
#include "message_format.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "save_frontier.h"
#include "msgdata.h"

struct FrontierSave;

struct BattleTowerRecordsApp;

struct BattleTowerRecordsAppStringManager;

struct BattleTowerRecordsWifiTrainer;

struct BgConfig;

struct MailMessage;

struct MsgData;

struct Options;

struct Sprite;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct WifiBattleTowerDownloadData;

struct WifiBattleTowerRecord;

struct Window;

typedef struct BattleTowerRecordsAppStringManager {
    MsgData *msgLoader;
    MessageFormat *strTemplate;
    String *string;
    String *cancelStr;
    String *headerStr;
    String *unused;
    String *countryAndCityStr;
    String *countryStr;
    String *noCountryStr;
    String *resultStrs[6];
    String *defaultNames[2];
} BattleTowerRecordsAppStringManager;

typedef struct BattleTowerRecordsWifiTrainer {
    String *name;
    u8 anonNameIdx;
    u8 country;
    u8 region;
    u8 useDefaultName;
    MailMessage message;
} BattleTowerRecordsWifiTrainer;

typedef struct BattleTowerRecordsApp {
    enum HeapID heapID;
    BgConfig *bgConfig;
    u32 unused;
    Options *options;
    FrontierSave *frontier;
    WifiBattleTowerRecord *towerRecord;
    WifiBattleTowerDownloadData *downloadData;
    BattleTowerRecordsAppStringManager strMan;
    Window windows[(3 >= 5) ? (3) : (5)];
    BattleTowerRecordsWifiTrainer wifiTrainers[30];
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    Sprite *sprites[4];
} BattleTowerRecordsApp;

void TransferSpriteSystemOam(void);

void ov73_021E8100(void *data);

void ov73_021E8100(void *data) {
    BattleTowerRecordsApp *app = data;

    GF_RunVramTransferTasks();
    DoScheduledBgGpuUpdates(app->bgConfig);
    OamManager_ApplyAndResetBuffers();
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
