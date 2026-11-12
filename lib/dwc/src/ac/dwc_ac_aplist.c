// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/src/ac/dwc_ac_aplist.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// DWC's auto-connect, the lists of what a search looks for (TWL-DWC 5.3, ac). The file name is a guess: its functions
// make the targets (SSIDs and their ids) from the connection settings that the system keeps and from the SSIDs of the
// special access points, and the candidates still to visit. It ends firmly where dwc_wdsscan.c begins; where it starts
// is not known (dwc_ac_result.c is the file before, or the same one).

#include "dwc/ac.h"
#include "dwc/ext.h"
#include "nitro/mi.h"
#include "nitro/os.h"

// The SSIDs of the special access points: those of FREESPOT (a prefix, without a zero), of NINTENDOWFC, and the
// prefix of an NWCUSBAP's
// pokebw2's sFreeSpotSsid
static const char data_020a1620[8] = "FREESPOT";
// pokebw2's sNintendoWfcSsid
static const char data_020a1628[] = "NINTENDOWFC";
// pokebw2's sNwcUsbApSsid
static char data_020a80a4[] = "NWCUSBAP";

static u8 func_0205d470(DWCAcSearchWork *work);
static u8 MakeStealthList(DWCAcSearchWork *work);
static u8 func_0205d588(DWCAcSearchWork *work);
static u8 func_0205d848(DWCAcSearchWork *work);
static u8 CheckDifferChannelStart(DWCAcSearchWork *work);

// Makes the list that a step of the search works from. Step 0 is that of the first search: the targets of the stored
// settings and FREESPOT. Step 1 counts the candidates that the search found and still has to visit, and has the first
// of them to start with. Step 2 is the targets of the second search: the settings, NWCUSBAP and NINTENDOWFC. TRUE if
// there is something to look for
// pokebw2's DWCAc_MakeSearchList
BOOL func_0205d318(u32 step) {
    DWCAcSearchWork *work = func_0205c214(DWC_AC_WORK_SEARCH);

    switch (step) {
    case 0:
        MI_CpuClear32(work->targets, sizeof(work->targets));
        work->targetCount = func_0205d470(work);
        break;
    case 1:
        work->targetCount = func_0205d848(work);
        work->current = CheckDifferChannelStart(work);
        if (work->targetCount == 0) {
            return FALSE;
        }
        break;
    case 2:
        MI_CpuClear32(work->targets, sizeof(work->targets));
        work->current = 0;
        work->targetCount = MakeStealthList(work);
        if (work->targetCount == 0) {
            return FALSE;
        }
        break;
    }
    // A test of the console whose body is not in this build, most likely a debug print for the DSi
    OS_IsRunOnTwl();
    return TRUE;
}

// The id of FREESPOT if the SSID of the beacon starts with that and the search takes it, else 0
// pokebw2's DWCAc_GetFreeSpotId
int DWCi_AC_CheckFreespot(const WMBssDesc *bss) {
    DWCAcSearchWork *work = func_0205c214(DWC_AC_WORK_SEARCH);

    if (work->mode == 0 || work->mode == 6) {
        if (strncmp(bss->ssid, data_020a1620, sizeof(data_020a1620)) == 0) {
            return DWC_AC_ID_FREESPOT;
        }
    }
    return 0;
}

// How many targets there are: the first with an SSID of no length ends them
// pokebw2's DWCAc_CountTargets
u8 func_0205d434(void) {
    DWCAcSearchWork *work = func_0205c214(DWC_AC_WORK_SEARCH);
    u8 i;

    for (i = 0; i < DWC_AC_TARGET_MAX; i++) {
        if (work->targets[i].ssidLength == 0) {
            break;
        }
    }
    return i;
}

// The targets of the first search: those of the settings, and FREESPOT. Returns how many there are
// pokebw2's MakeFirstTargets
static u8 func_0205d470(DWCAcSearchWork *work) {
    u8 count;
    DWCAcTarget *target = work->targets;

    count = func_0205d588(work);
    target += count;

    if (work->mode == 0 || work->mode == 6) {
        MI_CpuCopy8(data_020a1620, target->ssid, sizeof(data_020a1620));
        target->ssidLength = sizeof(data_020a1620);
        target->id = DWC_AC_ID_FREESPOT;
        count++;
    }
    return count;
}

// The targets of the second search: those of the settings, an NWCUSBAP's SSID and NINTENDOWFC. Returns how many there
// are
// pokebw2's MakeSecondTargets
static u8 MakeStealthList(DWCAcSearchWork *work) {
    u8 count;
    DWCAcTarget *target = work->targets;

    count = func_0205d588(work);
    target += count;

    if (work->mode == 0 || work->mode == 4) {
        MI_CpuCopy8(data_020a80a4, target->ssid, 8);
        target->ssidLength = 8;
        target->id = DWC_AC_ID_NWCUSBAP;
        count++;
        target++;
    }
    if (work->mode == 0 || work->mode == 8) {
        MI_CpuCopy8(data_020a1628, target->ssid, 11);
        target->ssidLength = 11;
        target->id = DWC_AC_ID_NINTENDOWFC;
        count++;
    }
    return count;
}

// Puts the SSID of a stored setting in the target at hand, with the id that the setting's key gives it. FALSE, with the
// target untouched, for a setting with no SSID
static inline BOOL AddTarget(DWCAcTarget *target, const u8 *ssid, u8 id) {
    u8 length;

    for (length = 0; length < 32; length++) {
        if (ssid[length] == 0) {
            break;
        }
        target->ssid[length] = ssid[length];
    }
    if (length != 0) {
        target->ssidLength = length;
        target->id = id;
        return TRUE;
    }
    return FALSE;
}

// The targets of the stored settings that the search takes (all of them, or the one that its mode names), one for each
// SSID of a setting that is in use. Returns how many there are. The ids are those that the connection reads the keys
// by (ids 0 to 2 the DS settings' first SSIDs, 3 to 5 their second, 6 to 8 the DSi settings' first SSIDs, 9 to 11 their
// second, 12 to 14 and 15 to 17 their SSIDs for WPA)
// pokebw2's MakeSettingTargets
static u8 func_0205d588(DWCAcSearchWork *work) {
    u8 count = 0;
    int i;
    const DWCMemPage *ds = work->userInfo;
    const DWCBmTwlPage *dsi = work->dsiSettings;
    DWCAcTarget *target = work->targets;

    for (i = 0; i < 3; i++, ds++) {
        if (work->mode == 0 || work->mode == i + 1) {
            if (ds->ap.setType != DWC_SETTYPE_NONE) {
                if (AddTarget(target, ds->ap.ssid, i)) {
                    count++;
                    target++;
                }
                if (ds->ap.setType == 1) {
                    if (AddTarget(target, ds->ap.aossSsid, i + 3)) {
                        count++;
                        target++;
                    }
                }
            }
        }
    }

    if (OS_IsRunOnTwl()) {
        for (i = 0; i < 3; i++, dsi++) {
            if (work->mode == 0 || work->mode == i + 0xb) {
                if (dsi->ntr.ap.setType != DWC_SETTYPE_NONE) {
                    if (AddTarget(target, dsi->ntr.ap.ssid, i + 6)) {
                        count++;
                        target++;
                    }
                    if (dsi->ntr.ap.setType == 1 || dsi->ntr.ap.setType == 0x11) {
                        if (AddTarget(target, dsi->ntr.ap.aossSsid, i + 9)) {
                            count++;
                            target++;
                        }
                        if (AddTarget(target, dsi->extra.ssid3, i + 12)) {
                            count++;
                            target++;
                        }
                        if (AddTarget(target, dsi->extra.ssid4, i + 15)) {
                            count++;
                            target++;
                        }
                    }
                }
            }
        }
    }
    return count;
}

// Counts the candidates that are still to be visited: those that no connection was tried on and whose tag is not the
// channel they are on, which means that the search heard them from another channel. The others are marked as seen
// pokebw2's CountCandidatesToVisit
static u8 func_0205d848(DWCAcSearchWork *work) {
    u8 count = 0;
    int i;

    for (i = 0; i < work->candidateCount; i++) {
        if (work->infos[i].result == 0 && work->candidates[i].channel - 1 != work->infos[i].tag) {
            work->infos[i].seen = 0;
            count++;
        } else {
            work->infos[i].seen = 1;
        }
    }
    return count;
}

// The first candidate that is not marked as seen, else the first of all
// pokebw2's FirstUnseenCandidate
static u8 CheckDifferChannelStart(DWCAcSearchWork *work) {
    u8 i;
    u8 first = 0;
    u8 count = work->candidateCount;

    for (i = 0; i < count; i++) {
        if (work->infos[i].seen == 0) {
            first = i;
            break;
        }
    }
    return first;
}
