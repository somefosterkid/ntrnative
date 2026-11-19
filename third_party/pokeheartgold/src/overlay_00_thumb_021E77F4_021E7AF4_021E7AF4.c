#include "global.h"
#include "constants/heap.h"
#include "vct.h"

struct NNSSndStrm;

struct VoiceChatManager;

typedef void (*VoiceChatCleanupCallback)(void);

int VCT_AddConferenceClient(u8 aid);

enum VoiceChatState {
    VOICE_CHAT_STATE_INIT,
    VOICE_CHAT_STATE_CONNECTING,
    VOICE_CHAT_STATE_CONNECTED,
};

typedef struct VoiceChatManager {
    u8 sendBuffer[((8000 * 68 * 2 * 2) / 1000)];
    void *allocStartPtr;
    u8 *audioBuffer;
    u8 *audioBufferAllocStartPtr;
    u8 receiveBuffer[((8000 * 68 * 2 * 2) / 1000)];
    u8 emptyVoiceBuffer[((8000 * 68 * 2 * 2) / 1000)];
    VoiceChatCleanupCallback cleanupCallback;
    VCTSession otherConfMembersSessions[4 - 1];
    BOOL occupiedConfSeats[4];
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

extern VoiceChatManager *_0221A684[2];

BOOL ov00_021E7AF4(int newClientsBitmap, int currentClientID);

BOOL ov00_021E7AF4(int newClientsBitmap, int currentClientID) {
    int i, clientAddError;

    if (!(_0221A684[1]) || (_0221A684[1]->mode != VCT_MODE_CONFERENCE)) {
        return 0;
    }

    for (i = 0; i < 4; i++) {
        if (i == currentClientID) {
            continue;
        }

        if (newClientsBitmap & (1 << i)) {
            if (_0221A684[1]->occupiedConfSeats[i] == 1) {
                continue;
            }

            clientAddError = VCT_AddConferenceClient(i);

            if (clientAddError != 0) {
                return 0;
            } else {
                _0221A684[1]->occupiedConfSeats[i] = 1;
            }
        }
    }

    return 1;
}
