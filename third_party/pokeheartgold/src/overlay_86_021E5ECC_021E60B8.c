#include "global.h"

#include "bg_window.h"
#include "options.h"
#include "overlay_86_frontier_records_app.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "message_format.h"
#include "save_frontier.h"
#include "msgdata.h"

struct OverlayManager;

struct FrontierSave;

struct MsgData;

struct MessageFormat;

void ov86_021E60B8(FrontierRecordsApp *app) {
    switch (app->facility) {
    case FACILITY_FACTORY:
    case FACILITY_FACTORY_OPEN:
        ov86_021E6114(app);
        ov86_021E62F4(app);
        break;
    case FACILITY_HALL:
        ov86_021E64E0(app);
        break;
    case FACILITY_CASTLE:
        ov86_021E66D0(app);
        break;
    case FACILITY_ARCADE:
        ov86_021E690C(app);
        break;
    case FACILITY_TOWER:
        if (app->challengeType == 0 || app->challengeType == 1) {
            ov86_021E6A88(app);
        } else {
            ov86_021E6BA8(app);
        }
        break;
    default:
        ov86_021E690C(app);
        break;
    }
}
