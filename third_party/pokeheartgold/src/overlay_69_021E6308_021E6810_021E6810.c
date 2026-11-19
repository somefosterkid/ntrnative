#include "global.h"

#include "bg_window.h"
#include "geonet_globe.h"
#include "msgdata.h"
#include "options.h"
#include "pm_string.h"
#include "system.h"
#include "text.h"

BOOL ov69_021E6810(GeonetGlobe *globe, u32 entryId, BOOL autoConfirm);

BOOL ov69_021E6810(GeonetGlobe *globe, u32 entryId, BOOL autoConfirm) {
    BOOL done = 0;

    switch (globe->messageState) {
    case GEONET_GLOBE_MESSAGE_STATE_START:
        FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 15, 0, 0, 27 * 8, 4 * 8);
        globe->messageText = String_New(0x400, globe->heapID);
        ReadMsgDataIntoString(globe->messageLoader, entryId, globe->messageText);
        globe->messagePrinter = AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], FONT_MESSAGE, globe->messageText, 0, 0, Options_GetTextFrameDelay(globe->options), 0L);
        globe->messageState = GEONET_GLOBE_MESSAGE_STATE_PRINTING;
        break;
    case GEONET_GLOBE_MESSAGE_STATE_PRINTING:
        if (!(TextPrinterCheckActive(globe->messagePrinter))) {
            String_Delete(globe->messageText);
            globe->messageState = GEONET_GLOBE_MESSAGE_STATE_CONFIRM;
        }
        break;
    case GEONET_GLOBE_MESSAGE_STATE_CONFIRM:
        if ((autoConfirm != 0) || (gSystem.newKeys & (0x0001))) {
            globe->messageState = GEONET_GLOBE_MESSAGE_STATE_START;
            done = 1;
        }
    }

    return done;
}
