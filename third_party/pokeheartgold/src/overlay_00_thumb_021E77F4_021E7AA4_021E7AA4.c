#include "global.h"
#include "vct.h"

struct VoiceChatManager;

struct _VCTVADInfo;

void VCT_GetVADInfo(VCTVADInfo *outInfo);

typedef struct VoiceChatManager VoiceChatManager;

extern VoiceChatManager *_0221A684[2];

BOOL ov00_021E7AA4(void);

BOOL ov00_021E7AA4(void) {
    if (_0221A684[1]) {
        VCTVADInfo vadInfo;

        VCT_GetVADInfo(&vadInfo);

        if (vadInfo.scale > 2) {
            return vadInfo.activity;
        }
    }

    return FALSE;
}
