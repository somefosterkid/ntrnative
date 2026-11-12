// NitroSDK's nitro/spi/ARM9/pm.h, the power management, with TwlSDK's DSi part (which TwlSDK has in this header too).
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/pm.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_NITRO_SPI_ARM9_PM_H
#define BWPORT_NITRO_SPI_ARM9_PM_H

#include "types.h"

#define PM_RESULT_SUCCESS 0
#define PM_LCD_ALL 2
#define PM_BACKLIGHT_OFF 0
#define PM_BATTERY_HIGH 0

// What wakes the system: opening the lid, or the Game Pak being pulled
#define PM_TRIGGER_COVER_OPEN 0x04

#define PM_TRIGGER_CARTRIDGE 0x08

// Callbacks run before the system sleeps and after it wakes, in lists ordered by priority: the TwlSDK's
// PM_PrependPreSleepCallback, PM_AppendPostSleepCallback, PM_DeletePreSleepCallback and PM_DeletePostSleepCallback,
// by their code
typedef void (*PMGenCallback)(void *arg);

typedef struct PMGenCallbackInfo {
    PMGenCallback callback;
    void *arg;
    int priority;
    struct PMGenCallbackInfo *next;
} PMGenCallbackInfo;

typedef PMGenCallbackInfo PMSleepCallbackInfo;

void PM_PrependPreSleepCallback(PMSleepCallbackInfo *info);
void PM_AppendPostSleepCallback(PMSleepCallbackInfo *info);
static inline void PM_SetSleepCallbackInfo(PMSleepCallbackInfo *info, PMGenCallback callback, void *arg) {
    info->callback = callback;
    info->arg = arg;
}

void PM_InsertPreSleepCallback(PMSleepCallbackInfo *info, int priority);
void PM_DeletePostSleepCallback(PMSleepCallbackInfo *info);

// NitroSDK's enum (an int) and typedefs
typedef int PMBackLightSwitch;
typedef int PMLCDTarget;
typedef int PMBattery;
typedef u32 PMWakeUpTrigger;
typedef u32 PMLogic;

u32 PM_GetBackLight(PMBackLightSwitch *top, PMBackLightSwitch *bottom);
u32 PM_SetBackLight(PMLCDTarget target, PMBackLightSwitch sw);
u32 PM_GetBattery(PMBattery *batteryBuf);
u32 PM_GetBatteryLevel(u16 *level);
void PM_GoSleepMode(PMWakeUpTrigger trigger, PMLogic logic, u16 keyPattern);

// The ARM7 is still on a request
#define PM_RESULT_BUSY 1

#define PM_RESULT_ERROR 2

// A parameter has no request
#define PM_RESULT_ILLEGAL_PARAMETER 0xffff

// Called with the result when the ARM7 has answered
typedef void (*PMCallback)(u32 result, void *arg);

#define PM_LCD_TOP 0
#define PM_LCD_BOTTOM 1
#define PM_BACKLIGHT_ON 1
#define PM_BATTERY_LOW 1
#define PM_TRIGGER_CARD 0x08

// The priorities of the callbacks that go first and last, and where a new one goes among those of its own priority
#define PM_CALLBACK_PRIORITY_MIN (-255)

#define PM_CALLBACK_PRIORITY_MAX 255
#define PM_INSERT_APPEND 0
#define PM_INSERT_PREPEND 1

// PM_SetBackLight sends the request and waits for the answer, PM_SetBackLightAsync calls back
u32 PM_SetBackLightAsync(PMLCDTarget target, PMBackLightSwitch backlight, PMCallback callback, void *arg);
// Turns the screens' power on or off, and the upper engine off with it. TRUE when it is done, FALSE when the screens
// went off too recently to be turned on
BOOL PM_SetLCDPower(BOOL on);

// main's pm.c: a utility request to the ARM7 (TwlSDK's PM_SendUtilityCommand), and the callback lists, which TwlSDK's
// exit callbacks in the LTD autoload use too
u32 PMi_ReadRegister(u32 number, u16 parameter, u16 *retValue);
u32 PMi_InsertList(PMGenCallbackInfo **list, PMGenCallbackInfo *info, s32 priority, u32 mode);
void PMi_DeleteList(PMGenCallbackInfo **list, PMGenCallbackInfo *info);
void PMi_ExecuteList(PMGenCallbackInfo *list);

// The DSi's part. From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/include/twl/pm.h: its types
// and constants, and the prototypes of Black's named functions.
typedef PMGenCallback PMExitCallback;
typedef PMGenCallbackInfo PMExitCallbackInfo;

static inline void PM_SetExitCallbackInfo(PMExitCallbackInfo *info, PMExitCallback callback, void *arg) {
    info->callback = callback;
    info->arg = arg;
}

// The objects of the LTD autoload that main's pm.c reaches: the callbacks run when the system is forced to power off,
// and the one for a low battery
typedef struct {
    PMGenCallback callback;
    void *arg;
} PMBatteryLowCallbackInfo;

// Like PM_AppendPreExitCallback, but with the callback's priority: inserts it into the list of the callbacks that run
// before the application exits, and PM_DeletePreExitCallback takes it out
void PM_InsertPreExitCallback(PMExitCallbackInfo *info, int priority);
void PM_DeletePreExitCallback(PMExitCallbackInfo *info);

// TwlSDK's PM_GetExitFactor: why the system is exiting (pm.c's PMi_ExitFactor)
BOOL PMi_GetExitType(void);

// TwlSDK's: why the system is exiting
typedef enum {
    PM_EXIT_FACTOR_NONE = 0,
    PM_EXIT_FACTOR_PWSW = 1,
    PM_EXIT_FACTOR_BATTERY = 2,
    PM_EXIT_FACTOR_USER = 3
} PMExitFactor;

// The rest of TwlSDK's DSi part of pm.c (lib/nitro/src/spi/pm.c's twl/ltdmain_begin.h blocks), by Black's names
// (TwlSDK's in the comments)
extern PMGenCallbackInfo PMi_BatteryLowCallbackInfo;
extern PMGenCallbackInfo *PMi_PostExitCallbackList;
u32 PMi_GetBatteryLevelCore(u16 *level);
void PMi_StopNDma(u32 ndmaNo);
void PMi_ProcessExitRequest(PMExitFactor factor); // PMi_ProceedToExit
void func_027314f0(void);                         // PM_ReadyToExit
void func_0273154c(PMExitCallbackInfo *info);     // PM_AppendPreExitCallback
void PMi_ExecutePostExitCallbackList(void);
void func_027315c0(BOOL sw);                      // PM_SetAutoExit
BOOL func_027315d0(void);                         // PM_GetAutoExit

void PM_DeletePreSleepCallback(PMSleepCallbackInfo *info);

#endif // BWPORT_NITRO_SPI_ARM9_PM_H
