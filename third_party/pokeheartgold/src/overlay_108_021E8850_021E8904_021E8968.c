#include "global.h"

#include "gf_rtc.h"
#include "menu_input_state.h"
#include "options.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"
#include "player_data.h"
#include "safari_decoration_args.h"
#include "safari_zone.h"
#include "sav_system_info.h"

void ov108_021E8968(SafariDecorationApp *app, SafariDecorationArgs *args) {
    u32 sp4;
    Options *temp_r0_2;
    PlayerProfile *temp_r6;
    SafariZone *temp_r0;
    u32 temp_r0_3;
    u8 temp_r0_4;
    u8 temp_r7;
    u8 gender;

    temp_r0 = Save_SafariZone_Get(args->saveData);
    app->safariZone = temp_r0;
    SafariZone_CopyAreaSet(temp_r0, 0, &app->areaSet);
    temp_r0_2 = Save_PlayerData_GetOptionsAddr(args->saveData);
    app->textSpeed = (s8)Options_GetTextSpeed(temp_r0_2);
    app->frame = (s8)Options_GetFrame(temp_r0_2);
    app->menuInputState = MenuInputStateMgr_GetState(args->menuInputStatePtr);
    app->args = args;
    app->cursor = (u8)((s32)*args->unk20 % 6);
    app->page = (u8)((s32)*app->args->unk20 / 6);
    temp_r6 = Save_PlayerData_GetProfile(args->saveData);
    temp_r7 = SafariZone_GetObjectUnlockLevel(app->safariZone);
    temp_r0_3 = PlayerProfile_GetTrainerID(temp_r6);
    sp4 = temp_r0_3;
    gender = (u8)PlayerProfile_GetTrainerGender(temp_r6);
    app->objects = ov108_021EA63C(temp_r0_3, temp_r7, gender, &app->numObjects, app->heapID);
    temp_r0_4 = args->areaIndex;
    app->areaIndex = temp_r0_4;
    if ((u32)temp_r0_4 > 5U) {
        app->areaIndex = 0U;
    }
    app->freeObjectSlots = (u8)(SAFARI_ZONE_MAX_OBJECTS - app->areaSet.areas[app->areaIndex].active_object_count);
    app->numPages = (u8)((s32)app->numObjects / 6);
    if ((s32)((s32)app->numObjects % 6) > 0) {
        app->numPages = (u8)(app->numPages + 1);
    }
    if ((u32)app->page >= (u32)app->numPages) {
        app->page = 0U;
    }
    app->args->unk1C = 0xFF;
    app->timeOfDay = (u8)GF_RTC_GetTimeOfDayByHour(Save_SysInfo_RTC_Get(args->saveData)->time.hour);
    if (app->timeOfDay == 4) {
        app->timeOfDay = 3U;
    }
}
