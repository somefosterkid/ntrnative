// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/ac/dwc_ac.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// DWC's auto-connect, its core (TWL-DWC 5.3, ac). The file name is descriptive: the file creates and destroys the
// control block and the other blocks of memory that auto-connect keeps, and steps the state machine (DWC_AC_Process)
// whose states the other ac files run: 1 starts the wireless manager, 2 to 6 the search, 7 and 8 the connection to the
// candidate, 9 its end, 10 to 15 the sockets coming up and the connection kept, 16 the connection up, 17 the teardown
// and 18 the failure. The next function, at 0205ac24, is the first of dwc_connectinet.c.

#include "dwc/ac.h"
#include "dwc/bm.h"
#include "dwc/ext.h"
#include "nitro/mi.h"
#include "nitro/os.h"

// The pointers to the five blocks of memory, which func_0205c214 gives out by their flags
// Black 2's, not checked against Black
typedef struct DWCAcBlocks {
    void *wcm;               // DWC_AC_WORK_WCM
    void *socConfig;         // DWC_AC_WORK_SOC_CONFIG
    DWCAcMemFuncs *memFuncs; // DWC_AC_WORK_MEMFUNCS
    DWCAcMain *main;         // DWC_AC_WORK_MAIN
    DWCAcSearchWork *search; // DWC_AC_WORK_SEARCH
} DWCAcBlocks;

// pokebw2's sBlocks
static DWCAcBlocks data_02146ac0;

static void DWC_AC_SetSpecifyAp(const u8 *ssid, const u8 *key, u32 wepMode);
static void *func_0205c038(u32 flag, u32 size);
static void func_0205c080(u32 flag, void *block, u32 size);
static void DWCi_AC_FreeAll(void);
static void func_0205c25c(u8 state);
static void func_0205c528(void);
static u8 CheckDuplicate(void);
static BOOL func_0205c600(void);
static BOOL func_0205c670(void);

// Whether a DSi setting's kind (DWCBMApInfo.setType) says it uses WPA: any kind from 0x10 on that is not
// DWC_SETTYPE_NONE, which a setting not in use has
#define SETTING_USES_WPA(setting) ((setting).ntr.ap.setType >= 0x10 && (setting).ntr.ap.setType != DWC_SETTYPE_NONE)

void func_020584e0(void *param_1);
BOOL func_020584f4(void *param_1);

// Creates auto-connect: the control block and the four more blocks of memory, the settings that it works from (the DS's
// and, on a DSi, the DSi's), and the wireless manager. FALSE if the wireless manager does not start
// pokebw2's DWCAc_Create
BOOL func_0205b98c(DWCAcParam *param) {
    DWCAcMain *main;
    DWCAcMemFuncs *funcs;
    DWCAcSearchWork *work;
    s32 result;

    data_02146ac0.main = param->alloc(1, sizeof(DWCAcMain));
    MI_CpuClear32(data_02146ac0.main, sizeof(DWCAcMain));
    main = data_02146ac0.main;
    main->alloc = param->alloc;
    main->free = param->free;
    main->state = 1;
    main->maxState = 1;
    main->allocated = DWC_AC_WORK_MAIN;
    data_02146ac0.search = func_0205c038(DWC_AC_WORK_SEARCH, sizeof(DWCAcSearchWork));
    data_02146ac0.wcm = func_0205c038(DWC_AC_WORK_WCM, 0x4800);
    data_02146ac0.socConfig = func_0205c038(DWC_AC_WORK_SOC_CONFIG, 0x58);
    data_02146ac0.memFuncs = func_0205c038(DWC_AC_WORK_MEMFUNCS, sizeof(DWCAcMemFuncs));
    MI_CpuClear32(data_02146ac0.search, sizeof(DWCAcSearchWork));
    MI_CpuClear32(data_02146ac0.wcm, 0x4800);
    MI_CpuClear32(data_02146ac0.socConfig, 0x58);
    MI_CpuClear32(data_02146ac0.memFuncs, sizeof(DWCAcMemFuncs));
    work = data_02146ac0.search;
    work->dmaNo = param->dmaNo;
    work->radioOn = param->radioOn;
    work->rwin = param->rwin;
    work->ignoreEula = param->ignoreEula;
    funcs = data_02146ac0.memFuncs;
    funcs->alloc = param->alloc;
    funcs->free = param->free;
    funcs->unk_08 = NULL;
    work->mode = param->mode;
    work->skipConnectionTest = param->skipConnectionTest;
    work->rssiPacked = param->rssiPacked;
    func_020584e0(work);
    if (OS_IsRunOnTwl()) {
        work->preferWpa = 0;
        func_020584f4(work->dsiSettings);
        if (work->mode == 0) {
            if (SETTING_USES_WPA(work->dsiSettings[0]) || SETTING_USES_WPA(work->dsiSettings[1]) ||
                SETTING_USES_WPA(work->dsiSettings[2])) {
                work->preferWpa = 1;
            }
        } else if (work->mode == 0xb) {
            if (SETTING_USES_WPA(work->dsiSettings[0])) {
                work->preferWpa = 1;
            }
        } else if (work->mode == 0xc) {
            if (SETTING_USES_WPA(work->dsiSettings[1])) {
                work->preferWpa = 1;
            }
        } else if (work->mode == 0xd) {
            // BUG: the third setting's byte is tested for 0x10 and the second's for 0xff
#ifdef BUGFIX
            if (SETTING_USES_WPA(work->dsiSettings[2])) {
#else
            if (work->dsiSettings[2].ntr.ap.setType >= 0x10 &&
                work->dsiSettings[1].ntr.ap.setType != DWC_SETTYPE_NONE) {
#endif
                work->preferWpa = 1;
            }
        }
    }
    if (OS_IsRunOnTwl()) {
        if (work->rssiPacked == 1) {
            result = func_ov009_02164d64(data_02146ac0.wcm, 0x4800);
        } else {
            result = func_ov009_02164d74(data_02146ac0.wcm, 0x4800, 2);
        }
    } else {
        result = func_ov009_02164d64(data_02146ac0.wcm, 0x4800);
    }
    if (result == 1 || result > 4) {
        DWCi_AC_FreeAll();
        return FALSE;
    }
    return TRUE;
}

// Runs the state at hand once and moves to the one that follows. Returns 1 once the connection is up (state 16, or 2 if
// CheckDuplicate finds another candidate like the one connected to), -1 when it has failed for good (state
// 18), else 0. Once it is up or has failed, the search's blocks of memory are let go
// pokebw2's DWCAc_Process
s32 DWC_AC_Process(void) {
    u8 state = DWCi_AC_GetPhase();
    s32 result;

    if (state == 1) {
        if (func_0205c600()) {
            if (func_0205c670()) {
                state = DWCi_AC_Start();
            } else {
                DWCi_AC_SetError(8);
                state = DWCi_AC_Error();
            }
        } else {
            DWCi_AC_SetError(7);
            state = DWCi_AC_Error();
        }
    } else if (state < 7) {
        u32 irq = OS_DisableInterrupts();

        state = DWCi_AC_SearchAP();
        func_0205c25c(state);
        OS_RestoreInterrupts(irq);
    } else if (state < 9) {
        state = func_0205b33c();
    } else if (state < 10) {
        state = DWCi_AC_ConnectRetryAP();
    } else if (state < 16) {
        state = DWCi_AC_ConnectTest();
    } else if (state == 17) {
        state = DWCi_AC_Error();
    }
    func_0205c25c(state);
    if (state == 16) {
        result = CheckDuplicate();
        func_0205c528();
        return result;
    }
    if (state != 18) {
        return 0;
    }
    func_0205c528();
    return -1;
}

// What the state is, in a few values for the caller: 0 before the search, 1 searching (states 2 to 6), 2 connecting (7
// and 8), 3 the sockets coming up (10, 12 to 15), 4 for 9, 11 and 17, which go on to another candidate or to the
// teardown, 5 connected (16); for a failure (18), the error code of dwc_ac_result.c's DWCi_AC_GetResult
// pokebw2's DWCAc_GetStatus
s32 DWC_AC_GetStatus(void) {
    u8 state = DWCi_AC_GetPhase();

    if (state <= 1) {
        return 0;
    }
    if (state < 7) {
        return 1;
    }
    if (state == 9) {
        return 4;
    }
    if (state < 10) {
        return 2;
    }
    if (state == 11) {
        return 4;
    }
    if (state < 16) {
        return 3;
    }
    if (state == 16) {
        return 5;
    }
    if (state == 17) {
        return 4;
    }
    return DWCi_AC_GetResult();
}

// The type of the access point that the connection is with, 0xff while there is none
// pokebw2's DWCAc_GetApType
u8 DWC_AC_GetApType(void) {
    u8 type = 0xff;
    u8 state = DWCi_AC_GetPhase();

    if (state >= 10 && state <= 16) {
        type = data_02146ac0.main->type;
    }
    return type;
}

// Copies the 10 bytes of spot information to spotInfo, if the connection is with an access point that has any (the type
// 4 or 8). TRUE if it did
// pokebw2's DWCAc_GetApSpotInfo
BOOL DWC_AC_GetApSpotInfo(u8 *spotInfo) {
    BOOL found = FALSE;
    u8 state = DWCi_AC_GetPhase();

    if (state >= 10 && state <= 16) {
        DWCAcMain *main = data_02146ac0.main;

        if (main->type == 4 || main->type == 8) {
            MI_CpuCopy8(main->spotInfo, spotInfo, sizeof(main->spotInfo));
            found = TRUE;
        }
    }
    return found;
}

// The key mode of the setting that the connection is made with
// pokebw2's DWCAc_GetKeyMode
u8 func_0205be58(void) {
    DWCAcMain *main = func_0205c214(DWC_AC_WORK_MAIN);

    return main->keyMode;
}

// Takes auto-connect down. Once it has no state or is over (18), lets its memory go and returns TRUE; else steps the
// teardown that the state at hand needs
// pokebw2's DWCAc_Destroy
BOOL DWC_AC_Destroy(void) {
    u8 state = DWCi_AC_GetPhase();

    if (state == 0 || state == 18) {
        DWCi_AC_FreeAll();
        return TRUE;
    }
    func_0205b1e4(&state);
    func_0205c25c(state);
    return FALSE;
}

// Gives the connection to make by hand: the first setting of the DS is made from the SSID, the WEP key and its mode and
// the MTU, and the search is made to try that and nothing else. The spot information, if any, and the type that the
// access point is given are what the connection reports
// pokebw2's DWCAc_SetGivenAp
void DWC_AC_SetSpecifyApEx(const u8 *ssid, const u8 *key, u32 wepMode, u16 mtu, const u8 *spotInfo, u32 type) {
    DWCAcMain *main = func_0205c214(DWC_AC_WORK_MAIN);
    DWCAcSearchWork *work = func_0205c214(DWC_AC_WORK_SEARCH);

    if (spotInfo != NULL) {
        MI_CpuCopy8(spotInfo, main->spotInfo, sizeof(main->spotInfo));
    } else {
        MI_CpuFill8(main->spotInfo, 0, sizeof(main->spotInfo));
    }
    main->givenType = type;
    DWC_AC_SetSpecifyAp(ssid, key, wepMode);
    work->userInfo[0].ap.mtu = mtu;
}

// Clears the first setting of the DS and fills it with the SSID (a string of 32 bytes at most) and, unless the mode is
// 0, a WEP key of the length that the mode says; makes the search look for that setting alone (mode 1)
// pokebw2's DWCAc_FillDsSetting
static void DWC_AC_SetSpecifyAp(const u8 *ssid, const u8 *key, u32 wepMode) {
    DWCAcSearchWork *work = func_0205c214(DWC_AC_WORK_SEARCH);
    int i;

    work->mode = 1;
    MI_CpuFill8(&work->userInfo[0].ap, 0, sizeof(DWCBMApInfo));
    for (i = 0; i < 32; i++) {
        if (ssid[i] == 0) {
            break;
        }
        work->userInfo[0].ap.ssid[i] = ssid[i];
    }
    if (key == NULL || wepMode == 0) {
        work->userInfo[0].ap.wepMode = 0;
        return;
    }
    MI_CpuCopy8(key, work->userInfo[0].ap.wepKey[0], wepMode == 1 ? 5 : wepMode == 2 ? 13 : 16);
    work->userInfo[0].ap.wepMode = wepMode;
}

// Whether the SSID of length bytes is that of an NDWCSHAP
// pokebw2's DWCAc_IsShopSsid
BOOL DWC_AC_CheckWiFiStation(const u8 *ssid, u32 length) {
    if (length != 32) {
        return FALSE;
    }
    return DWCi_AC_CheckNintendoShopAP(ssid);
}

// Replaces the connection of the DS's setting `index` with the 0xf0 bytes of setting
// pokebw2's DWCAc_SetDsConnection
void DWCi_AC_InsertApInfo(u32 index, const void *setting) {
    DWCAcSearchWork *work = func_0205c214(DWC_AC_WORK_SEARCH);

    MIi_CpuCopy32(setting, &work->userInfo[index].ap, sizeof(DWCBMApInfo));
}

// Stops the search from preferring the access points with WPA
// pokebw2's DWCAc_ClearPreferWpa
void func_0205c018(void) {
    DWCAcSearchWork *work = func_0205c214(DWC_AC_WORK_SEARCH);

    work->preferWpa = 0;
}

// Takes a block of memory of the flag, if it is not taken yet; NULL if it is
// pokebw2's DWCAc_AllocBlock
static void *func_0205c038(u32 flag, u32 size) {
    DWCAcMain *main = func_0205c214(DWC_AC_WORK_MAIN);

    if (main->allocated & flag) {
        return NULL;
    }
    main->allocated |= flag;
    return main->alloc(1, size);
}

// Lets a block of memory go, if it is taken; the control block's pointer is cleared with its own block
// pokebw2's DWCAc_FreeBlock
static void func_0205c080(u32 flag, void *block, u32 size) {
    DWCAcMain *main = func_0205c214(DWC_AC_WORK_MAIN);

    if (main == NULL) {
        return;
    }
    if (!(main->allocated & flag)) {
        return;
    }
    main->allocated &= ~flag;
    main->free(1, block, size);
    if (flag == DWC_AC_WORK_MAIN) {
        data_02146ac0.main = NULL;
    }
}

// Lets go of every block that is taken, the control block last
// pokebw2's DWCAc_FreeAllBlocks
static void DWCi_AC_FreeAll(void) {
    DWCAcMain *main = func_0205c214(DWC_AC_WORK_MAIN);

    if (main == NULL) {
        return;
    }
    if (main->allocated & DWC_AC_WORK_SEARCH) {
        void *block = func_0205c214(DWC_AC_WORK_SEARCH);

        main->allocated &= ~DWC_AC_WORK_SEARCH;
        main->free(1, block, sizeof(DWCAcSearchWork));
    }
    if (main->allocated & DWC_AC_WORK_MEMFUNCS) {
        void *block = func_0205c214(DWC_AC_WORK_MEMFUNCS);

        main->allocated &= ~DWC_AC_WORK_MEMFUNCS;
        main->free(1, block, sizeof(DWCAcMemFuncs));
    }
    if (main->allocated & DWC_AC_WORK_SOC_CONFIG) {
        void *block = func_0205c214(DWC_AC_WORK_SOC_CONFIG);

        main->allocated &= ~DWC_AC_WORK_SOC_CONFIG;
        main->free(1, block, 0x58);
    }
    if (main->allocated & DWC_AC_WORK_WCM) {
        void *block = func_0205c214(DWC_AC_WORK_WCM);

        main->allocated &= ~DWC_AC_WORK_WCM;
        main->free(1, block, 0x4800);
    }
    if (main->allocated & DWC_AC_WORK_MAIN) {
        main->allocated &= ~DWC_AC_WORK_MAIN;
        main->free(1, main, sizeof(DWCAcMain));
        data_02146ac0.main = NULL;
    }
}

// The block of memory that a flag names: the first of its bits that is set decides. NULL while it is not taken
// pokebw2's DWCAc_GetBlock
void *func_0205c214(u32 flag) {
    if (flag & DWC_AC_WORK_MAIN) {
        return data_02146ac0.main;
    }
    if (flag & DWC_AC_WORK_WCM) {
        return data_02146ac0.wcm;
    }
    if (flag & DWC_AC_WORK_SOC_CONFIG) {
        return data_02146ac0.socConfig;
    }
    if (flag & DWC_AC_WORK_MEMFUNCS) {
        return data_02146ac0.memFuncs;
    }
    if (flag & DWC_AC_WORK_SEARCH) {
        return data_02146ac0.search;
    }
    return NULL;
}

// Sets the state, and keeps track of the highest one: when a state past 7 is reached for the first time, notes the type
// and the result of the candidate that the connection works on
// pokebw2's DWCAc_SetState
static void func_0205c25c(u8 state) {
    DWCAcMain *main = func_0205c214(DWC_AC_WORK_MAIN);
    DWCAcSearchWork *work = func_0205c214(DWC_AC_WORK_SEARCH);

    main->state = state;
    if (state >= 16) {
        return;
    }
    if (state > main->maxState) {
        main->maxState = state;
        if (state > 7) {
            main->candidateType = func_0205c4c8(work->kind);
            main->candidateResult = work->infos[work->selected].result;
        }
    }
}

// The state of the control block, 0 while there is none
// pokebw2's DWCAc_GetState
u8 DWCi_AC_GetPhase(void) {
    return data_02146ac0.main != NULL ? data_02146ac0.main->state : 0;
}

// Notes an error: the code, and the state it came in, from which the teardown goes on
// pokebw2's DWCAc_SetError
void DWCi_AC_SetError(s32 error) {
    DWCAcMain *main = func_0205c214(DWC_AC_WORK_MAIN);

    main->error = error;
    main->failedState = DWCi_AC_GetPhase();
}

// The error that was noted last
// pokebw2's DWCAc_GetError
s32 DWCi_AC_GetError(void) {
    return data_02146ac0.main->error;
}

// Notes the type of the access point that the connection is with: the one that DWC_AC_SetSpecifyApEx was given, else that of
// the target id; and its spot information, which an NDWCSHAP's SSID holds (zeroes for any other)
// pokebw2's DWCAc_NoteConnectedAp
void func_0205c31c(u8 id) {
    u8 type = data_02146ac0.main->givenType;
    const u8 *ssid;

    if (type == 0) {
        type = func_0205c4c8(id);
    }
    data_02146ac0.main->type = type;
    ssid = func_ov009_021663c8(NULL);
    if (ssid != NULL) {
        DC_InvalidateRange((void *)ssid, 32);
        if (func_0205ea70(ssid, data_02146ac0.main->spotInfo)) {
            return;
        }
        MI_CpuFill8(data_02146ac0.main->spotInfo, 0, sizeof(data_02146ac0.main->spotInfo));
    } else {
        MI_CpuFill8(data_02146ac0.main->spotInfo, 0, sizeof(data_02146ac0.main->spotInfo));
    }
}

// Notes the key mode of the setting that the connection to the target id uses: the WCM mode of its WPA if it is one of
// the DSi's three and uses WPA, else its WEP mode; the two kinds told by their SSIDs (NWCUSBAP, NDWCSHAP) have mode 2.
// For the DSi's three it also copies some of the setting's details to the control block
// pokebw2's DWCAc_NoteKeyMode
void func_0205c3b0(u8 id) {
    DWCAcMain *main = func_0205c214(DWC_AC_WORK_MAIN);
    DWCAcSearchWork *work = func_0205c214(DWC_AC_WORK_SEARCH);
    u8 type = func_0205c4c8(id);

    if (type >= 10 && type <= 12 && OS_IsRunOnTwl()) {
        type -= 10;
        main->unk_24 = work->dsiSettings[type].extra.unk_82;
        main->unk_25 = work->dsiSettings[type].extra.unk_83;
        main->unk_26 = work->dsiSettings[type].extra.unk_e8;
        MI_CpuCopy8(work->dsiSettings[type].extra.unk_84, main->unk_28, sizeof(main->unk_28));
        MI_CpuCopy8(work->dsiSettings[type].ntr.ap.unk0, main->unk_8c, sizeof(main->unk_8c));
        MI_CpuCopy8(work->dsiSettings[type].ntr.ap.unk20, main->unk_ac, sizeof(main->unk_ac));
        if (work->dsiSettings[type].ntr.ap.setType == 0x10 || work->dsiSettings[type].ntr.ap.setType == 0x11 ||
            work->dsiSettings[type].ntr.ap.setType == 0x12 || work->dsiSettings[type].ntr.ap.setType == 0x13) {
            main->keyMode = work->dsiSettings[type].extra.wcmMode;
        } else {
            main->keyMode = work->dsiSettings[type].ntr.ap.wepMode;
        }
    } else if (id == DWC_AC_ID_NWCUSBAP || id == DWC_AC_ID_NDWCSHAP) {
        main->keyMode = 2;
    } else {
        main->keyMode = work->userInfo[type].ap.wepMode;
    }
}

// The type of the access point of a target id: 0 to 2 for the DS's three settings, which ids 0 to 5 are, 3 to 5 for
// NWCUSBAP, NDWCSHAP and FREESPOT (ids 0x12 to 0x14), and 10 to 12 for the DSi's three, ids 6 to 17
// pokebw2's DWCAc_GetApTypeOfId
u8 func_0205c4c8(u8 id) {
    if (id >= 0x12) {
        return id - 0xf;
    }
    if (id >= 0xf) {
        return id - 5;
    }
    if (id >= 0xc) {
        return id - 2;
    }
    if (id >= 9) {
        return id + 1;
    }
    if (id >= 6) {
        return id + 4;
    }
    if (id >= 3) {
        return id - 3;
    }
    return id;
}

// Lets go of the two blocks that only the search and the connection used
// pokebw2's DWCAc_FreeSearchBlocks
static void func_0205c528(void) {
    func_0205c080(DWC_AC_WORK_MEMFUNCS, data_02146ac0.memFuncs, sizeof(DWCAcMemFuncs));
    func_0205c080(DWC_AC_WORK_SEARCH, data_02146ac0.search, sizeof(DWCAcSearchWork));
}

// 2 if another ordinary candidate (id below 0x12) looks like the one that was connected to, else 1; a special target
// (id 0x12 on) always gives 1. The candidates are compared as far as the other one's SSID is long, starting at the
// BSSID
//
// style-ok: bug no BUGFIX, since the callers don't show which comparison was meant
// BUG: strncmp compares the BSSIDs for the length of the SSID, and stops at a zero byte of a BSSID, so the access
// points whose OUIs start with 0 come out equal. Either the SSIDs were meant (another access point with the same name)
// or the six bytes of the BSSIDs (memcmp); the callers only test the result for 0, so the code doesn't say which
// pokebw2's DWCAc_FindSimilarCandidate
static u8 CheckDuplicate(void) {
    u8 i;
    int selected;
    DWCAcSearchWork *work = data_02146ac0.search;
    u8 count;
    u8 kind;
    const char *name;

    selected = work->selected;
    kind = work->kind;
    name = (const char *)work->candidates[selected].bssid;
    if (kind >= 0x12) {
        return 1;
    }
    count = work->candidateCount;
    for (i = 0; i < count; i++) {
        if (i != selected && work->infos[i].id < 0x12) {
            if (strncmp(name, (const char *)work->candidates[i].bssid, work->candidates[i].ssidLength) == 0) {
                return 2;
            }
        }
    }
    return 1;
}

// On a DSi, whether the license agreement has been agreed to, in the version that the ROM asks for; always TRUE on a
// DS, or when the work says to ignore it
// pokebw2's DWCAc_CheckEula
static BOOL func_0205c600(void) {
    if (OS_IsRunOnTwl()) {
        if (data_02146ac0.search->ignoreEula != 0 || (OS_IsAgreeEULA() && OS_GetAgreedEULAVersion() != 0 &&
                                                OS_GetAgreedEULAVersion() >= OS_GetROMHeaderEULAVersion())) {
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

// On a DSi, whether the system settings allow the wireless connection and the parental controls do not forbid it;
// always TRUE on a DS
// pokebw2's DWCAc_CheckWirelessAllowed
static BOOL func_0205c670(void) {
    if (OS_IsRunOnTwl()) {
        if (OS_IsAvailableWireless() == TRUE && OS_IsForceDisableWireless() == 0) {
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}
