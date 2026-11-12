// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91, libraries/wm/src/wm_ks.c: ported as it is, with credit
// (the owner's decision, docs/plan.md, 2026-10-13), with only the functions Black has and its preprocessor's
// choices for Black's final ROM made (tools/port_lib.py).
#include <nitro/wm.h>

#include "wm_arm9_private.h"

WMErrCode WM_StartKeySharing (WMKeySetBuf * buf, u16 port)
{
    return WM_StartDataSharing(buf, port, 0xffff, WM_KEYDATA_SIZE, TRUE);
}

WMErrCode WM_EndKeySharing(WMKeySetBuf *buf) {
    return WM_EndDataSharing(buf);
}

