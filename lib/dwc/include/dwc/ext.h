// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/ext.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_EXT_H
#define BWPORT_DWC_EXT_H

#include <mem.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "types.h"
#include "dwc/soap.h"
#include "dwc/gsAvailable.h"
#include "dwc/gsCore.h"
#include "dwc/gsDebug.h"
#include "dwc/gsMemory.h"
#include "dwc/gsPlatformSocket.h"
#include "dwc/gsPlatformThread.h"
#include "dwc/gsPlatformUtil.h"
#include "nitro/os.h"
#include "nitrowifi/soc.h"

typedef struct GsHttpRequest {
    u8 unk_00[0x24];
    void *handle;
} GsHttpRequest;

typedef void (*GsHttpResponseFunc)(int status, int a1, SoapReader *reader, void *userData);

// A post: the form (or message) that a request sends
typedef struct GHIPost GHIPost;

u32 func_ov009_021803bc(void *param_1, s8 *param_2, s32 param_3);

#endif // BWPORT_DWC_EXT_H
