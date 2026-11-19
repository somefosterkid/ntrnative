#include "global.h"

#include "overlay_38_thumb.h"

BOOL DWC_InitGHTTP(const char *gamename);

extern struct {
    int readyState;
    int requestHandle;
    int errorCode;
    int responseLength;
    int dwcProfileID;
    void *data;
    int dataSize;
    void *response;
    int maxResponseLength;
    char *url;
    char *hashStr;
    char *b64DataStr;
    int encodedDataSize;
} _02225040;

void ov38_0221BE84(void) {
    _02225040.readyState = 2;
    _02225040.requestHandle = -1;

    if (!DWC_InitGHTTP(NULL)) {
        _02225040.readyState = 1;
    }

    return;
}
