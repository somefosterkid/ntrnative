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
extern u8 DWC_GetMyAID(void);
typedef struct _VCTSession VCTSession;
typedef enum {
	VCT_CODEC_8BIT_RAW,
	VCT_CODEC_G711_ULAW,
	VCT_CODEC_2BIT_ADPCM,
	VCT_CODEC_3BIT_ADPCM,
	VCT_CODEC_4BIT_ADPCM,
	VCT_CODEC_END
} VCTCodec;
typedef enum {
	VCT_MODE_NULL,
	VCT_MODE_PHONE,
	VCT_MODE_TRANSCEIVER,
	VCT_MODE_CONFERENCE
} VCTMode;
typedef enum {
	VCT_EVENT_NONE,
	VCT_EVENT_INCOMING,
	VCT_EVENT_REJECT,
	VCT_EVENT_BUSY,
	VCT_EVENT_CANCEL,
	VCT_EVENT_NOTIFY_FREE,
	VCT_EVENT_NOTIFY_BUSY,
	VCT_EVENT_CONNECTED,
	VCT_EVENT_RESPONDBYE,
	VCT_EVENT_DISCONNECTED,
	VCT_EVENT_CONTACT,
	VCT_EVENT_TIMEOUT,
	VCT_EVENT_ABORT
} VCTEvent;
typedef void (*VCTEventCallback)(u8 aid, VCTEvent result, VCTSession *session, void *param);
typedef struct {
	VCTSession *session;
	u32 numSession;
	VCTMode mode;
	u8 aid;
	void *audioBuffer;
	u32 audioBufferSize;
	VCTEventCallback callback;
	void *userData;
} VCTConfig;
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
BOOL VCT_Init(VCTConfig *config);
BOOL VCT_SetCodec(VCTCodec codec);
void VCT_EnableEchoCancel(BOOL flag);
int sub_0203993C(void);
BOOL sub_02034084(int param0);
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
void ov00_021E79B4(void);
extern struct {
    int unk_00;
    VoiceChatManager *sVoiceChatManager;
} _0221A684; /* invented */
void ov00_021E733C(void);
void ov00_021E73E8(NNSSndStrmCallbackStatus status, int numOtherClients, void *receiveBuffers[], u32 bufferSize, NNSSndStrmFormat unused, void *_sendBuffer);
void ov00_021E7544(u8 unused0, VCTEvent event, VCTSession *session, void *unused3);
void ov00_021E756C(u8 unused0, VCTEvent event, VCTSession *session, void *unused3);
void ov00_021E77CC(void **allocStartPtr, void **alignedPtr, int size, enum HeapID heapID);

void ov00_021E77F4(enum HeapID heapID, int codec, int numConferenceSessions);

void ov00_021E77F4(enum HeapID heapID, int codec, int numConferenceSessions)
{
    u8 voiceSoundChannels[3] = { 0, 0, 0 };

    if (_0221A684.sVoiceChatManager == 0L ) {
        void *voiceChatManAllocPtr = 0L ;

        ov00_021E77CC(&voiceChatManAllocPtr, (void **)&_0221A684.sVoiceChatManager, sizeof(VoiceChatManager), heapID);
        _0221A684.sVoiceChatManager->allocStartPtr = voiceChatManAllocPtr;
        ov00_021E77CC((void **)&_0221A684.sVoiceChatManager->audioBufferAllocStartPtr, (void **)&_0221A684.sVoiceChatManager->audioBuffer, 1144 * numConferenceSessions * 8 + 32, heapID);

        _0221A684.sVoiceChatManager->heapID = heapID;
        _0221A684.sVoiceChatManager->cleanupCallback = 0L ;

        ov00_021E733C();
    }

    _0221A684.sVoiceChatManager->microphone.type = MIC_SAMPLING_TYPE_SIGNED_12BIT;
    _0221A684.sVoiceChatManager->microphone.buffer = _0221A684.sVoiceChatManager->sendBuffer;
    _0221A684.sVoiceChatManager->microphone.size = ((8000 * 68 * 2 * 2) / 1000) ;
    _0221A684.sVoiceChatManager->microphone.rate = (((33513982 / 2) / 32) / 8000) * 64;
    _0221A684.sVoiceChatManager->microphone.loop_enable = 1 ;
    _0221A684.sVoiceChatManager->microphone.full_callback = 0L ;
    _0221A684.sVoiceChatManager->microphone.full_arg = 0L ;
    _0221A684.sVoiceChatManager->shouldStartSampling = 1 ;

    NNS_SndStrmAllocChannel(&_0221A684.sVoiceChatManager->soundStream, 1, voiceSoundChannels);
    NNS_SndStrmSetVolume(&_0221A684.sVoiceChatManager->soundStream, 0);

    NNS_SndStrmSetup(&_0221A684.sVoiceChatManager->soundStream, NNS_SND_STRM_FORMAT_PCM16, _0221A684.sVoiceChatManager->receiveBuffer, ((8000 * 68 * 2 * 2) / 1000) , ((33513982 / 2) / 32) / 8000, 2, ov00_021E73E8, _0221A684.sVoiceChatManager->sendBuffer);

    _0221A684.sVoiceChatManager->state = VOICE_CHAT_STATE_INIT;
    _0221A684.sVoiceChatManager->session = 0L ;

    VCTConfig voiceChatConfig;

    if (!sub_02034084(sub_0203993C())) {
        voiceChatConfig.mode = VCT_MODE_PHONE;
    } else {
        voiceChatConfig.mode = VCT_MODE_CONFERENCE;
    }

    _0221A684.sVoiceChatManager->mode = voiceChatConfig.mode;

    voiceChatConfig.session = _0221A684.sVoiceChatManager->otherConfMembersSessions;
    voiceChatConfig.numSession = numConferenceSessions;
    voiceChatConfig.aid = DWC_GetMyAID();

    ((voiceChatConfig.aid != -1) ? (void)0 : GF_AssertFail()) ;

    if (_0221A684.sVoiceChatManager->mode == VCT_MODE_CONFERENCE) {
        voiceChatConfig.callback = ov00_021E7544;
    } else {
        voiceChatConfig.callback = ov00_021E756C;
    }

    voiceChatConfig.userData = 0L ;
    voiceChatConfig.audioBuffer = _0221A684.sVoiceChatManager->audioBuffer;
    voiceChatConfig.audioBufferSize = 1144 * numConferenceSessions * 8 + 32;

    VCT_Init(&voiceChatConfig);

    _0221A684.sVoiceChatManager->disabled = 0 ;

    VCT_SetCodec((VCTCodec)(codec));
    ov00_021E79B4();
    VCT_EnableEchoCancel(1 );
}
