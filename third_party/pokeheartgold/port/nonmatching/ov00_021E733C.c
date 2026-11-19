#include "global.h"
struct NNSSndStrm;
struct VoiceChatManager;
struct _VCTSession;
enum HeapID {
    HEAP_ID_SYSTEM = 0,
    HEAP_ID_SAVE,
    HEAP_ID_DEBUG,
    HEAP_ID_APPLICATION,
    HEAP_ID_FIELD1,
    HEAP_ID_BATTLE,
    HEAP_ID_BAG,
    HEAP_ID_POKETCH_MAIN,
    HEAP_ID_POKETCH_APP,
    HEAP_ID_BOX_DATA,
    HEAP_ID_BOX_GRAPHICS,
    HEAP_ID_FIELD2,
    HEAP_ID_PARTY_MENU,
    HEAP_ID_13,
    HEAP_ID_14,
    HEAP_ID_COMMUNICATION,
    HEAP_ID_16,
    HEAP_ID_TOWN_MAP,
    HEAP_ID_NAMING_SCREEN_APP,
    HEAP_ID_POKEMON_SUMMARY_SCREEN,
    HEAP_ID_20,
    HEAP_ID_21,
    HEAP_ID_22,
    HEAP_ID_23,
    HEAP_ID_24,
    HEAP_ID_TRAINER_CASE,
    HEAP_ID_TRADE_ROOM,
    HEAP_ID_27,
    HEAP_ID_28,
    HEAP_ID_MINING,
    HEAP_ID_DISTORTION_WORLD_WARP,
    HEAP_ID_31,
    HEAP_ID_FIELD3,
    HEAP_ID_UNDERGROUND,
    HEAP_ID_34,
    HEAP_ID_35,
    HEAP_ID_BERRY_TAG,
    HEAP_ID_POKEDEX,
    HEAP_ID_OPTIONS_MENU,
    HEAP_ID_DRAWING,
    HEAP_ID_MAIL,
    HEAP_ID_MAIL_VIEWER,
    HEAP_ID_JOURNAL,
    HEAP_ID_43,
    HEAP_ID_POFFIN_CASE,
    HEAP_ID_POFFIN_MANAGER,
    HEAP_ID_POFFIN_CUTSCENE,
    HEAP_ID_CHOOSE_STARTER_APP,
    HEAP_ID_48,
    HEAP_ID_NINTENDO_WFC,
    HEAP_ID_50,
    HEAP_ID_51,
    HEAP_ID_52,
    HEAP_ID_53,
    HEAP_ID_54,
    HEAP_ID_POFFIN_BERRY_SELECTION,
    HEAP_ID_56,
    HEAP_ID_TRADE_SEQUENCE,
    HEAP_ID_TRADE_SEQUENCE_PHASE,
    HEAP_ID_59,
    HEAP_ID_60,
    HEAP_ID_PC_HALL_OF_FAME,
    HEAP_ID_62,
    HEAP_ID_HALL_OF_FAME,
    HEAP_ID_64,
    HEAP_ID_65,
    HEAP_ID_66,
    HEAP_ID_MOVE_REMINDER,
    HEAP_ID_68,
    HEAP_ID_69,
    HEAP_ID_70,
    HEAP_ID_EGG_HATCH,
    HEAP_ID_BOAT_CUTSCENE,
    HEAP_ID_EVOLUTION,
    HEAP_ID_BATTLE_TOWER_RECORDS_APP,
    HEAP_ID_END_CREDITS,
    HEAP_ID_76,
    HEAP_ID_GAME_START,
    HEAP_ID_MIGRATE_FROM_GBA,
    HEAP_ID_79,
    HEAP_ID_RANKINGS_MACHINE,
    HEAP_ID_MAIN_MENU,
    HEAP_ID_ROWAN_INTRO,
    HEAP_ID_ROWAN_INTRO_TV,
    HEAP_ID_DIPLOMA,
    HEAP_ID_RANGER_LINK,
    HEAP_ID_MYSTERY_GIFT_APP,
    HEAP_ID_WONDER_CARDS_APP,
    HEAP_ID_88,
    HEAP_ID_89,
    HEAP_ID_90,
    HEAP_ID_NETWORK_ICON,
    HEAP_ID_92,
    HEAP_ID_BATTLE_FACTORY_APP,
    HEAP_ID_94,
    HEAP_ID_95,
    HEAP_ID_96,
    HEAP_ID_97,
    HEAP_ID_BATTLE_HALL_APP,
    HEAP_ID_99,
    HEAP_ID_BATTLE_CASTLE_APP,
    HEAP_ID_101,
    HEAP_ID_102,
    HEAP_ID_BATTLE_ARCADE_APP,
    HEAP_ID_104,
    HEAP_ID_105,
    HEAP_ID_106,
    HEAP_ID_107,
    HEAP_ID_108,
    HEAP_ID_109,
    HEAP_ID_110,
    HEAP_ID_SPEAR_PILLAR_CUTSCENE,
    HEAP_ID_112,
    HEAP_ID_113,
    HEAP_ID_FRONTIER_RECORD_APP,
    HEAP_ID_SCRATCH_OFF_CARD_APP,
    HEAP_ID_116,
    HEAP_ID_117,
    HEAP_ID_118,
    HEAP_ID_119,
    HEAP_ID_LIBRARY_TV,
    HEAP_ID_PARTY_MENU1,
    HEAP_ID_PARTY_MENU2,
    HEAP_ID_MAX,
};
typedef void (*VoiceChatCleanupCallback)(void);
typedef struct _VCTSession VCTSession;
typedef enum {
	VCT_MODE_NULL,
	VCT_MODE_PHONE,
	VCT_MODE_TRANSCEIVER,
	VCT_MODE_CONFERENCE
} VCTMode;
typedef enum {
	VCT_STATE_INIT,
	VCT_STATE_OUTGOING,
	VCT_STATE_TALKING,
	VCT_STATE_CONNECTED,
	VCT_STATE_INCOMING,
	VCT_STATE_DISCONNECTING
} VCTState;
struct _VCTSession {
	VCTMode mode;
	u8 aid;
	u8 talking;
	u32 aidBitmap;
	VCTState state;
	void *userData;
	VCTSession *next;
};
void VCT_EnableVAD(BOOL flag);
void VCT_EnableEchoCancel(BOOL flag);
enum VoiceChatState {
    VOICE_CHAT_STATE_INIT,
    VOICE_CHAT_STATE_CONNECTING,
    VOICE_CHAT_STATE_CONNECTED,
};
typedef struct VoiceChatManager {
    u8 sendBuffer[((8000 * 68 * 2 * 2) / 1000) ];
    void *allocStartPtr;
    u8 *audioBuffer;
    u8 *audioBufferAllocStartPtr;
    u8 receiveBuffer[((8000 * 68 * 2 * 2) / 1000) ];
    u8 emptyVoiceBuffer[((8000 * 68 * 2 * 2) / 1000) ];
    VoiceChatCleanupCallback cleanupCallback;
    VCTSession otherConfMembersSessions[4 - 1];
    BOOL occupiedConfSeats[4 ];
    VCTMode mode;
    enum VoiceChatState state;
    int disabled;
    enum HeapID heapID;
    struct NNSSndStrm soundStream;
    VCTSession *session;
    u8 unused_1A58;
    u8 shouldStartSampling;
    u16 timeout;
    MICAutoParam microphone;
    int unused_1A78;
    int gain;
    BOOL shouldUpdateGain;
    int unused_1A84;
    int unused_1A88;
    u16 unused_1A8C[16];
    int unused_1AAC;
} VoiceChatManager;
/* invented */ typedef struct {
    int sTimeToProcess;
    VoiceChatManager *sVoiceChatManager;
} VoiceChatStatics;
extern VoiceChatStatics _0221A684;

void ov00_021E733C(void);
void ov00_021E733C(void)
{
    MIC_Init();
    PM_Init();

    PM_SetAmp(PM_AMP_ON);
    PM_SetAmpGain(PM_AMPGAIN_160);

    _0221A684.sVoiceChatManager->unused_1A78 = 0;
    _0221A684.sVoiceChatManager->unused_1A84 = 0;
    _0221A684.sVoiceChatManager->gain = 0;
    _0221A684.sVoiceChatManager->shouldUpdateGain = FALSE;
    _0221A684.sVoiceChatManager->unused_1A88 = 0;

    for (int i = 0; i < 16; i++) {
        _0221A684.sVoiceChatManager->unused_1A8C[i] = 0;
    }

    _0221A684.sVoiceChatManager->unused_1AAC = 0;

    VCT_EnableVAD(TRUE);

    NNS_SndInit();
    NNS_SndStrmInit(&_0221A684.sVoiceChatManager->soundStream);
    MI_CpuClearFast(_0221A684.sVoiceChatManager->emptyVoiceBuffer, sizeof(_0221A684.sVoiceChatManager->emptyVoiceBuffer));

    _0221A684.sTimeToProcess = 0;

    VCT_EnableEchoCancel(TRUE);
}
