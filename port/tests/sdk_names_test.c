// Host test of port/src/sdk_names.c: the decomp's names for libntr's and libntrsystem's functions link, and the ones
// that can run without the game do what the SDK's would. A NitroMain on libntr's simulator (a window: WSLg or Xvfb),
// built by port/tests/run_sdk_names_test.sh. Exit 0 when every check passes.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdk_black.h"

static int sFails, sChecks;

#define CHECK(cond, ...)                                                                                               \
    do {                                                                                                               \
        sChecks++;                                                                                                     \
        if (!(cond)) {                                                                                                 \
            sFails++;                                                                                                  \
            printf("FAIL %s:%d: ", __FILE__, __LINE__);                                                                \
            printf(__VA_ARGS__);                                                                                       \
            printf("\n");                                                                                              \
        }                                                                                                              \
    } while (0)

static void Step(const char *what)
{
    printf("sdk_names_test: %s\n", what);
    fflush(stdout);
}

void NitroMain(void)
{
    static const char data[] = "12345678";

    Step("os_init");
    os_init();
    OS_InitTick();
    GX_Init();
    FX_Init();

    Step("OS_IsRunOnTwl");
    CHECK(OS_IsRunOnTwl() == FALSE, "OS_IsRunOnTwl (the game's hw_isDSi, a port_compat.h macro)");

    Step("GetCRC16");
    CHECK(GetCRC16(0xffff, (void *)data, 8) == SVC_GetCRC16(0xffff, data, 8), "GetCRC16 forwards");
    CHECK(GetCRC16(0xffff, (void *)data, 8) != 0, "SVC_GetCRC16 is patched (0 is libntr's stub)");

    Step("FS_Init (FSi_InitOverlay, from the patch)");
    {
        static u8 mem[100];
        FSFile file;
        FS_Init(FS_DMA_NOT_USE);
        FS_InitFile(&file);
        CHECK(FS_CreateFileFromMemory(&file, mem, sizeof(mem)), "FS_CreateFileFromMemory");
        CHECK(FS_GetFileLength(&file) == sizeof(mem), "FS_GetFileLength %u (the game's GetFileSize)",
              FS_GetFileLength(&file));
        FS_CloseFile(&file);
    }

    Step("PM, answered by the patch (the game's func_0208c030 is PM_GetAmp)");
    {
        PMAmpSwitch amp = (PMAmpSwitch)0x55;
        PMBattery battery = PM_BATTERY_LOW;
        PMBackLightSwitch top = PM_BACKLIGHT_OFF, bottom = PM_BACKLIGHT_OFF;
        CHECK(PM_GetAmp(&amp) == PM_SUCCESS && amp == 0, "amp off at start: %u", amp);
        CHECK(PM_SetAmp(PM_AMP_ON) == PM_SUCCESS, "PM_SetAmp");
        CHECK(PM_GetAmp(&amp) == PM_SUCCESS && amp == 1, "amp on: %u", amp);
        CHECK(PM_SetAmp(PM_AMP_OFF) == PM_SUCCESS && PM_GetAmp(&amp) == PM_SUCCESS && amp == 0, "amp off");
        CHECK(PM_GetBattery(&battery) == PM_SUCCESS && battery == PM_BATTERY_HIGH, "battery");
        CHECK(PM_GetBackLight(&top, &bottom) == PM_SUCCESS && top == PM_BACKLIGHT_ON && bottom == PM_BACKLIGHT_ON,
              "backlights");
        CHECK(PM_SetBackLight(PM_LCD_ALL, PM_BACKLIGHT_ON) == PM_SUCCESS, "PM_SetBackLight");
    }

    Step("func_027315c0/func_027315d0 (PM_SetAutoExit/PM_GetAutoExit)");
    func_027315c0(FALSE);
    CHECK(func_027315d0() == FALSE, "auto exit off");
    func_027315c0(TRUE);
    CHECK(func_027315d0() == TRUE, "auto exit on");

    Step("func_020911a0 (CTRDG_DummyInit)");
    func_020911a0();

    Step("OS_GetConsoleType");
    // src/system/machine_use.c's SystemInit calls func_0208c988 (SCFG_SetCpuSpeed, whose libntr version reads a TWL
    // register at its hardware address) and writes reg_SCFG_EXT at 0x04004008 when this is nonzero
    printf("  OS_GetConsoleType() %08x: development bits (0x3000000) %s\n", OS_GetConsoleType(),
           (OS_GetConsoleType() & 0x3000000) ? "SET" : "clear");

    Step("TWL-only internals (sdk_twl.c)");
    {
        u32 count;
        CHECK(RTCi_GetCounter(&count) == RTC_RESULT_INVALID_COMMAND, "RTCi_GetCounter on a DS");
        CHECK(OS_GetISOCountryCodeA2(49) == NULL, "OS_GetISOCountryCodeA2 on a DS");
    }

    Step("a frame");
    G3X_Init();
    OS_WaitVBlankIntr();

    // linked, not run: they need a renderer and cell data
    CHECK((void *)func_02065ce4 != NULL && (void *)func_02065d4c != NULL && (void *)func_02066748 != NULL, "NNS G2d");
    CHECK((void *)func_027314f0 != NULL && (void *)func_0273154c != NULL, "PM exit");

    printf("sdk_names_test: %d checks, %d failed\n", sChecks, sFails);
    fflush(stdout);
    exit(sFails != 0);
}
