// libdwcutil's setting.c (Black's ov016 0x021cf3b0-0x021cfebc): the file of the DWC utility whose object in NitroDWC's
// prebuilt libdwcutil.a has that name (its debug line table) and much of the same code, in the same order.
#include "dwcutil.h"
#include "system/dwc_backup.h"
#include "dwc/util_wifiidtool.h"

const u8 data_ov016_021da910[4] = { 0, 0, 0, 0 };

typedef struct SettingFlags {
    u8 mode : 2;
    u8 unk_2 : 6;
} SettingFlags;

typedef struct SettingInfo {
    u8 unk_00[0x40];
    u8 unk_40[0x20];
    u8 unk_60[0x20];
    u8 unk_80[4][0x10];
    u8 unk_c0[4];
    u8 unk_c4[4];
    u8 unk_c8[4];
    u8 unk_cc[4];
    u8 unk_d0;
    u8 unk_d1[5];
    u8 unk_d6[5];
    u8 unk_db[5];
    u8 unk_e0[6];
    SettingFlags flags;
    u8 unk_e7;
    u8 unk_e8[7];
    u8 unk_ef;
    u8 unk_f0[4];
    u8 unk_f4;
    u8 unk_f5;
    u8 unk_f6;
} SettingInfo;

typedef struct SettingEntry {
    SettingInfo info;
    u8 unk_f8[6];
    u16 crc;
} SettingEntry;

typedef struct SettingWork {
    SettingEntry entry[4];
    SettingInfo cur;
    MATHCRC16Table crcTable;
} SettingWork;

typedef struct SettingCopy {
    u16 unk_0[0x78];
} SettingCopy;

static void *data_ov016_021de644;
void write_setting(u32 param_1);
s32 ascii2hex(u32 param_1);
u8 DWCi_BACKUPlConvMaskCidr(const u8 *mask);
void DWCi_SETTIGNlInit(void);
void DWCi_SETTINGlSetWep(void *param_1);
void DWCi_SETTINGlErasePage(u32 param_1);

void DWCi_SETTIGNlInit(void) {
    data_ov016_021de644 = DWCi_HEAPlAllocEx(0x6f8, 0x20);
    MATHi_CRC16InitTableRev(&((SettingWork *)data_ov016_021de644)->crcTable, 0xa001);
    func_02058770(data_ov016_021de644);
}

void DWCi_SETTIGNlEnd(void) {
    DWCi_HEAPlFree_((u32 *)&data_ov016_021de644);
}

s32 DWCi_SETTIGNlGet(void) {
    return (s32)(data_ov016_021de644 + 0x400);
}

void DWCi_SETTINGlSetIpAuto(BOOL yes) {
    *(u8 *)(data_ov016_021de644 + 0x4f5) = yes;
}

void DWCi_SETTINGlSetDnsAuto(BOOL yes) {
    *(u8 *)(data_ov016_021de644 + 0x4f6) = yes;
}

void DWCi_SETTINGlSetSsid(void *ssid) {
    MI_CpuCopy8(ssid, (void *)(data_ov016_021de644 + 0x440), 0x20);
    *(u8 *)(data_ov016_021de644 + 0x4e7) = 0;
}

void DWCi_SETTINGlSetWep(void *wep) {
    SettingWork *work = data_ov016_021de644;
    u8 *adr;
    s32 i;
    s32 len;

    MI_CpuFill8(work->cur.unk_80, 0, 0x10);
    len = DWCi_STRlNLen((s32)wep, 0x20);
    switch (len) {
    case 0:
    case 10:
    case 26:
    case 32:
        ((SettingWork *)data_ov016_021de644)->cur.flags.unk_2 = 0;
        adr = ((SettingWork *)data_ov016_021de644)->cur.unk_80[0];
        for (i = 0; i < len; i += 2) {
            *adr++ = ascii2hex(((u8 *)wep)[i]) * 16 + ascii2hex(((u8 *)wep)[i + 1]);
        }
        break;
    default:
        ((SettingWork *)data_ov016_021de644)->cur.flags.unk_2 = 1;
        MI_CpuCopy8(wep, ((SettingWork *)data_ov016_021de644)->cur.unk_80, 0x10);
        break;
    }
    switch (len) {
    case 0:
        ((SettingWork *)data_ov016_021de644)->cur.flags.mode = 0;
        break;
    case 5:
    case 10:
        ((SettingWork *)data_ov016_021de644)->cur.flags.mode = 1;
        break;
    case 13:
    case 26:
        ((SettingWork *)data_ov016_021de644)->cur.flags.mode = 2;
        break;
    default:
        ((SettingWork *)data_ov016_021de644)->cur.flags.mode = 3;
        break;
    }
}

void DWCi_SETTINGlSetIp(const u8 *ip) {
    DWCi_SETTINGlConvAddress(ip, ((SettingWork *)data_ov016_021de644)->cur.unk_c0);
}

void DWCi_SETTINGlSetNetMask(const u8 *netmask) {
    DWCi_SETTINGlConvAddress(netmask, ((SettingWork *)data_ov016_021de644)->cur.unk_f0);
}

void DWCi_SETTINGlSetGateway(const u8 *gateway) {
    DWCi_SETTINGlConvAddress(gateway, ((SettingWork *)data_ov016_021de644)->cur.unk_c4);
}

void DWCi_SETTINGlSetDnsP(const u8 *dns) {
    DWCi_SETTINGlConvAddress(dns, ((SettingWork *)data_ov016_021de644)->cur.unk_c8);
}

void DWCi_SETTINGlSetDnsS(const u8 *dns) {
    DWCi_SETTINGlConvAddress(dns, ((SettingWork *)data_ov016_021de644)->cur.unk_cc);
}

void DWCi_SETTINGlGetSsid(void *buf) {
    MI_CpuCopy8((void *)(data_ov016_021de644 + 0x440), buf, 0x20);
}

void DWCi_SETTINGlGetIp(u8 *param_1) {
    OS_SPrintf((char *)param_1, "%3d%3d%3d%3d", *(u8 *)((u8 *)(u8 *)data_ov016_021de644 + 0x4C0), *(u8 *)((u8 *)(u8 *)data_ov016_021de644 + 0x4C1), (s32) *(u8 *)((u8 *)(u8 *)data_ov016_021de644 + 0x4C2), (s32) *(u8 *)((u8 *)(u8 *)data_ov016_021de644 + 0x4C3));
}

void DWCi_SETTINGlGetNetMask(u8 *param_1) {
    OS_SPrintf((char *)param_1, "%3d%3d%3d%3d", *(u8 *)((u8 *)(u8 *)data_ov016_021de644 + 0x4F0), *(u8 *)((u8 *)(u8 *)data_ov016_021de644 + 0x4F1), (s32) *(u8 *)((u8 *)(u8 *)data_ov016_021de644 + 0x4F2), (s32) *(u8 *)((u8 *)(u8 *)data_ov016_021de644 + 0x4F3));
}

void DWCi_SETTINGlGetGateway(u8 *buf) {
    u8 *p = ((SettingWork *)data_ov016_021de644)->cur.unk_c4;

    OS_SPrintf((char *)buf, "%3d%3d%3d%3d", p[0], p[1], p[2], p[3]);
}

void DWCi_SETTINGlGetDnsP(u8 *buf) {
    u8 *p = ((SettingWork *)data_ov016_021de644)->cur.unk_c8;

    OS_SPrintf((char *)buf, "%3d%3d%3d%3d", p[0], p[1], p[2], p[3]);
}

void DWCi_SETTINGlGetDnsS(u8 *buf) {
    u8 *p = ((SettingWork *)data_ov016_021de644)->cur.unk_cc;

    OS_SPrintf((char *)buf, "%3d%3d%3d%3d", p[0], p[1], p[2], p[3]);
}

u8 DWCi_SETTINGlGetSetType(s32 idx) {
    return *(u8 *)(data_ov016_021de644 + idx * 0x100 + 0xe7);
}

void DWCi_SETTINGlSetSetting(s32 idx) {
    SettingEntry *info = &((SettingWork *)data_ov016_021de644)->entry[idx];

    *(SettingCopy *)&((SettingWork *)data_ov016_021de644)->cur = *(SettingCopy *)info;
    ((SettingWork *)data_ov016_021de644)->cur.unk_f4 = idx;
    if (memcmp(info->info.unk_c0, data_ov016_021da910, 4) != 0) {
        ((SettingWork *)data_ov016_021de644)->cur.unk_f5 = FALSE;
    } else {
        ((SettingWork *)data_ov016_021de644)->cur.unk_f5 = TRUE;
    }
    if (memcmp(info->info.unk_c8, data_ov016_021da910, 4) != 0 || memcmp(info->info.unk_cc, data_ov016_021da910, 4) != 0) {
        ((SettingWork *)data_ov016_021de644)->cur.unk_f6 = FALSE;
    } else {
        ((SettingWork *)data_ov016_021de644)->cur.unk_f6 = TRUE;
    }
    DWCi_BACKUPlConvMaskAddr(info->info.unk_d0, ((SettingWork *)data_ov016_021de644)->cur.unk_f0);
}


void DWCi_SETTINGlSet(void) {
    SettingWork *work = data_ov016_021de644;
    SettingInfo *temp = &work->cur;
    SettingEntry *info = &work->entry[temp->unk_f4];

    *(SettingCopy *)info = *(SettingCopy *)temp;
    if (temp->unk_f5 != 0) {
        MI_CpuFill8(info->info.unk_c0, 0, 4);
        MI_CpuFill8(info->info.unk_c4, 0, 4);
        info->info.unk_d0 = 0;
    } else {
        MI_CpuCopy8(temp->unk_c0, info->info.unk_c0, 4);
        MI_CpuCopy8(temp->unk_c4, info->info.unk_c4, 4);
        info->info.unk_d0 = DWCi_BACKUPlConvMaskCidr(temp->unk_f0);
    }
    if (temp->unk_f6 != 0) {
        MI_CpuFill8(info->info.unk_c8, 0, 8);
    } else {
        MI_CpuCopy8(temp->unk_c8, info->info.unk_c8, 8);
    }
    write_setting(temp->unk_f4);
}

void DWCi_SETTINGlSetRaku(void *param) {
    SettingWork *work = data_ov016_021de644;
    SettingInfo *info = &work->cur;
    u8 size = 0;
    s32 i;

    MI_CpuFill8(info, 0, 0xef);
    MI_CpuCopy8(param, info->unk_40, 0x20);
    switch (*(s32 *)((u8 *)param + 0x20)) {
    case 1:
        size = 5;
        info->flags.mode = 1;
        break;
    case 2:
        size = 0xd;
        info->flags.mode = 2;
        break;
    case 3:
        size = 0x10;
        info->flags.mode = 3;
        break;
    default:
        info->flags.mode = 0;
        break;
    }
    info->flags.unk_2 = 0;
    for (i = 0; i < 4; i++) {
        MI_CpuCopy8((u8 *)param + 0x28 + i * 0x20, info->unk_80[i], size);
    }
    info->unk_e7 = 2;
    MI_CpuFill8(info->unk_f0, 0, 4);
    info->unk_f5 = 1;
    info->unk_f6 = 1;
    DWCi_SETTINGlSet();
}

void DWCi_SETTINGlSetAoss(u8 *param) {
    SettingInfo *info = (SettingInfo *)((u8 *)data_ov016_021de644 + 0x400);

    MI_CpuFill8(info, 0, 0xef);
    MI_CpuCopy8(param, info->unk_d1, 5);
    MI_CpuCopy8(param + 6, info->unk_d6, 5);
    MI_CpuCopy8(param + 0xc, info->unk_db, 5);
    MI_CpuCopy8(param + 0x12, info->unk_e0, 5);
    MI_CpuCopy8(param + 0x18, info->unk_60, 0x20);
    MI_CpuCopy8(param + 0x39, info->unk_80[0], 0xd);
    MI_CpuCopy8(param + 0x47, info->unk_80[1], 0xd);
    MI_CpuCopy8(param + 0x55, info->unk_80[2], 0xd);
    MI_CpuCopy8(param + 0x63, info->unk_80[3], 0xd);
    MI_CpuCopy8(param + 0x71, info->unk_40, 0x20);
    info->flags.mode = 2;
    info->flags.unk_2 = 0;
    info->unk_e7 = 1;
    MI_CpuFill8(info->unk_f0, 0, 4);
    info->unk_f5 = 1;
    info->unk_f6 = 1;
    DWCi_SETTINGlSet();
}



u32 DWCi_SETTINGlGetMemMap(void) {
    return (u32)data_ov016_021de644;
}

void DWCi_SETTINGlErasePage(u32 page) {
    void *dest;

    dest = (void *)(data_ov016_021de644 + page * 0x100);
    MI_CpuFill8(dest, 0, 0xef);
    *(u8 *)((s32)(u8 *)dest + 0xe7) = 0xff;
    write_setting(page);
}

void DWCi_SETTINGlEraseAll(void) {
    const u8 *src;
    u32 i;
    s32 j;
    u8 auStack_28 [20];

    j = 0;
    MIi_CpuClear16(0, data_ov016_021de644, 0x400);
    do {
                *(u8 *)((s32)((u8 *)data_ov016_021de644) + (j * 0x100) + 0xe7) = 0xff;
        j = j + 1;
    } while (j < 3);
    DWCi_AUTH_GetNewWiFiInfo((DWCWiFiInfo *)auStack_28);
    src = DWCi_BACKUPlConvWifiInfo((DWCWiFiInfo *)auStack_28);
    j = 0;
    do {
        MI_CpuCopy8(src, (void *)((s32)(u8 *)data_ov016_021de644 + j * 0x100 + 0xf0), 0xe);
        j = j + 1;
    } while (j < 2);
    i = 0;
    do {
        write_setting(i);
        i = i + 1;
    } while ((s32)i < 4);
}

void write_setting(u32 page) {
    SettingWork *work = data_ov016_021de644;
    u8 state = work->entry[page].info.unk_e7;
    u32 bit = 1 << page;
    s32 flags[4];
    void *buf;
    s32 i;

    MI_CpuFill8(flags, 0, 0x10);
    flags[page] = 1;
    BOOL isSet = FALSE;
    if ((s32)page <= 2) {
        if (((SettingWork *)data_ov016_021de644)->entry[0].info.unk_ef & bit) {
            isSet = TRUE;
        }
        if (state == 0xff && isSet) {
            ((SettingWork *)data_ov016_021de644)->entry[0].info.unk_ef &= ~bit;
            ((SettingWork *)data_ov016_021de644)->entry[1].info.unk_ef &= ~bit;
            flags[1] = 1;
            flags[0] = 1;
        } else if (state != 0xff && !isSet) {
            ((SettingWork *)data_ov016_021de644)->entry[0].info.unk_ef |= bit;
            ((SettingWork *)data_ov016_021de644)->entry[1].info.unk_ef |= bit;
            flags[1] = 1;
            flags[0] = 1;
        }
    }
    for (i = 0; i < 4; i++) {
        if (flags[i] != 0) {
            ((SettingWork *)data_ov016_021de644)->entry[i].crc =
                MATH_CalcCRC16(&((SettingWork *)data_ov016_021de644)->crcTable, &((SettingWork *)data_ov016_021de644)->entry[i], 0xfe);
        }
    }
    buf = DWCi_HEAPlAllocEx(0x100, 0x20);
    func_02058784(data_ov016_021de644, flags, buf);
    DWCi_HEAPlFree_((u32 *)&buf);
}


void DWCi_SETTINGlConvAddress(const u8 *adr, u8 *param_2) {
    u8 buf[4];
    s32 i;
    s32 j;

    MI_CpuFill8(buf, 0, 4);
    for (i = 0; i < 4; i++) {
        MI_CpuCopy8(adr + i * 3, buf, 3);
        for (j = 0; j < 3; j++) {
            if (buf[j] != 0) {
                break;
            }
            buf[j] = ' ';
        }
        param_2[i] = atol((char *)buf);
    }
}


s32 ascii2hex(u32 ascii) {
    s32 val;
    if (0x39 >= ascii) {
        return ascii - 0x30;
    }
    if (ascii <= 0x46) {
        val = ascii - 0x37;
    } else {
        val = ascii - 0x57;
    }
    return val;
}
