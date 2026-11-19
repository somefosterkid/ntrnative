#include "global.h"

extern BOOL VCT_SendAudio(void *data, u32 size);

extern BOOL VCT_ReceiveAudio(void *data, u32 size, void *info);

extern void ov00_021E73E4(void);

typedef struct {
    s32 timeToProcess;
    u8 *manager;
} UnkStruct_ov00_021E73E8;

extern UnkStruct_ov00_021E73E8 _0221A684;

void ov00_021E73E8(int status, u32 numOtherClients, void **receiveBuffers, u32 bufferSize, int unused, void *_sendBuffer);

void ov00_021E73E8(int status, u32 numOtherClients, void **receiveBuffers, u32 bufferSize, int unused, void *_sendBuffer) {
    u8 *sendBuffer = (u8 *)_sendBuffer;
    u32 i;

    if (status == 0) {
        for (i = 0; i < numOtherClients; i++) {
            MI_CpuFill8(receiveBuffers[i], 0, bufferSize);
        }
        return;
    }

    if (*(u8 *)(_0221A684.manager + 0x1A59)) {
        MIC_StartAutoSamplingAsync((const struct MICAutoParam *)(_0221A684.manager + 0x1A5C), (void *)ov00_021E73E4, 0L);
        *(u8 *)(_0221A684.manager + 0x1A59) = 0;
    }

    if ((u32)((u8 *)MIC_GetLastSamplingAddress() - sendBuffer) < bufferSize) {
        sendBuffer += bufferSize;
    }

    if ((*(vu16 *)HW_BUTTON_XY_BUF & 0x8000) >> 15) {
        sendBuffer = _0221A684.manager + 0x110C;
    }

    if (*(u32 *)(_0221A684.manager + 0x19F0) == 0) {
        VCT_SendAudio(sendBuffer, bufferSize);
    }

    for (i = 0; i < numOtherClients; i++) {
        VCT_ReceiveAudio(receiveBuffers[i], bufferSize, 0L);
    }

    *(u8 *)(_0221A684.manager + 0x1A58) = 2;
}
