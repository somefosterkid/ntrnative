#include "global.h"
#include "vct.h"

struct _VCTSession;

BOOL VCT_StartStreaming(VCTSession *session);

void ov00_021E7638(VCTSession *session);

void ov00_021E7544(u8 unused0, VCTEvent event, VCTSession *session, void *unused3);

void ov00_021E7544(u8 unused0, VCTEvent event, VCTSession *session, void *unused3) {
    switch (event) {
    case VCT_EVENT_DISCONNECTED:
        ov00_021E7638(session);
        break;
    case VCT_EVENT_CONNECTED:
        VCT_StartStreaming(session);
        break;
    case VCT_EVENT_ABORT:
        ov00_021E7638(session);
        break;
    default:
        break;
    }
}
