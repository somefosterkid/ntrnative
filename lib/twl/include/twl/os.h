#ifndef TWL_OS_H
#define TWL_OS_H

// TwlSDK's DSi halves of the ARM9's OS functions, in the LTD autoload's .ltdmain; main's halves call them on a DSi
#include "types.h"
#include "nitro/os.h"

// The callbacks of the DMA channels, the DSi's new DMA channels and the timers (main's os_irqTable.c)
typedef struct OSIrqCallbackInfo {
    void (*func)(void *arg);
    u32 enable;
    void *arg;
} OSIrqCallbackInfo;

extern OSIrqCallbackInfo OSi_IrqCallbackInfo[OSi_IRQCALLBACK_NUM];

// The MPU region size of OS_SetProtectionRegion's sizeStr 64MB (HW_C6_PR_64MB), and the cartridge ROM's area
#define HW_C6_PR_64MB 0x32
#define HW_CTRDG_ROM 0x08000000

void OS_SetProtectionRegion2(int regionNo, u32 address, u32 sizeDef);

// The DSi's system settings, which the launcher leaves for the program, through a pointer at HW_PRELOAD_PARAMETER_ADDR
// (pokebw2's lib/twl/include/twl/os.h): the flags word that starts them
#define HW_PRELOAD_PARAMETER_ADDR 0x02fffdfc

typedef struct OSTWLSettingsFlags {
    u32 isFinishedInitialSetting : 1;
    u32 isFinishedInitialSetting_Launcher : 1;
    u32 isSetLanguage : 1;
    u32 isAvailableWireless : 1; // the wireless is enabled in the settings
    u32 rsv : 20;
    u32 isAgreeEULAFlagList : 8;
} OSTWLSettingsFlags;

// nitro/os.h's OSTWLSettingsData has them as one word; their bits, read through the same pointer
#define TWL_SETTINGS_FLAGS (*(OSTWLSettingsFlags **)HW_PRELOAD_PARAMETER_ADDR)

// The DSi's secure hardware settings that the system leaves in the shared memory (libntr's OSTWLHWSecureInfo, as
// pokebw2 has it): the flag is the low bit of the byte after the first word
typedef struct OSTWLHWSecureInfo {
    u32 validLanguageBitmap;
    struct {
        u8 forceDisableWireless : 1;
        u8 : 7;
    } flags;
    u8 pad[3];
    u8 region;
    u8 serialNo[15];
} OSTWLHWSecureInfo;

#define HW_HW_SECURE_INFO 0x02fffd68

BOOL func_027312d0_unk(void);
BOOL OSi_IsForceDisableWirelessTwl(void);

// The titles that the system leaves in the shared memory: their count, and their IDs (pokebw2's
// os_application_jump_twl.c)
#define OS_TITLE_ID_LIST_MAX 0x76

typedef struct OSTitleIdList {
    u8 count;
    u8 unk_01[0x4f];
    u64 titleIds[OS_TITLE_ID_LIST_MAX];
} OSTitleIdList;

#define HW_OS_TITLE_ID_LIST 0x02ffd800
// The same IDs, which the search reads by their own address
#define OS_TITLE_IDS ((u64 *)0x02ffd850)
// A title the list marks as not to be jumped to
#define OS_TITLE_ID_HIDDEN 0x0000000800000000

u64 OSi_FindSystemTitleId(u64 titleId, u64 mask);

#endif // TWL_OS_H
