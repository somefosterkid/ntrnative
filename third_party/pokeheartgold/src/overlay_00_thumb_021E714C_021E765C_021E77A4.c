#include "global.h"
#include "constants/heap.h"

struct VoiceChatManager;

BOOL VCT_HandleData(u8 aid, u8 *buffer, int size);

typedef struct VoiceChatManager VoiceChatManager;

extern VoiceChatManager *_0221A684[2];

void ov00_021E77CC(void **allocStartPtr, void **alignedPtr, int size, enum HeapID heapID);

BOOL ov00_021E77A4(int id, void *buffer, int size);

BOOL ov00_021E77A4(int id, void *buffer, int size) {
    if (_0221A684[1] == NULL) {
        return FALSE;
    }

    if (VCT_HandleData(id, buffer, size)) {
        return TRUE;
    }

    return FALSE;
}
