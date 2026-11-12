// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwcutil/src/dwc_aoss.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "dwc_aoss.h"
#include "dwc/ext.h"
#include "dwc_aoss_ap.h"
#include "dwc_wcm.h"
#include "gfl/std.h"
#include "nitro/math.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/rtc.h"
#include "nitro/wm.h"
#include "system/rtc.h"

// The AOSS client of DWC's settings (see dwc_aoss.h): Buffalo's one-touch setup. It looks for the access point named
// ESSID-AOSS, joins it with a fixed WEP key, takes an address in its subnet and talks to it in UDP packets on port
// 22416 in three stages, each answered by the access point: the hello (0x1000), the security settings that it offers
// (0x2000) and the end (0x3000). The packets are encrypted with RC4 under the id of the access point. It ends with the
// keys of the network. The file also has the small network helpers that it works with; the Wi-Fi scan and connect are
// in dwc_aoss_ap.c. The ROM names none of it; the names are ours.
//
// MWCC 2.0 lays the statics out in the reverse of their definition order

#define AOSS_SSID "ESSID-AOSS"
#define AOSS_KEY "MELCO"
#define AOSS_PORT 22416
#define AOSS_CLIENT_ADDR 0xc0a80b65 // 192.168.11.101
#define AOSS_ROUTER_ADDR 0xc0a80b01 // 192.168.11.1
#define AOSS_PACKET_SIZE 0x5dc
#define AOSS_BUFFER_SIZE 0x5f8 // of a packet, with the three words that the receiver keeps before the message
#define AOSS_PROTOCOL 0x11

// A record of a message: 4 bytes of header (the length is the payload's), and the payload
typedef struct AossItem {
    u8 type;
    u8 reserved;
    u16 length; // big endian
    u16 next;   // big endian, from the first item
    u8 data[1];
} AossItem;

typedef struct AossRecord {
    u8 type;
    u8 reserved;
    u16 length; // big endian
    union {
        s32 status; // type 7: big endian
        AossItem items[1];
        struct {
            u16 flags;
            AossItem items[1];
        } secure;
        u8 bytes[1];
    } payload;
} AossRecord;

// The header of a message
typedef struct AossHeader {
    u16 version; // 0x00: 1
    u16 unk_02;
    u16 unk_04;
    u16 type; // 0x06: 0x1010 + 0x1000 for each stage
    u16 unk_08;
    u16 length;  // 0x0a: of the body
    u16 flags;   // 0x0c: bit 4 says it is encrypted
    s8 checksum; // 0x0e
    u8 protocol; // 0x0f
    u8 id[8];    // 0x10: of a client, 6 bytes of address and 2 of a number
} AossHeader;

// A record with a status
typedef struct AossStatusRecord {
    u8 type;
    u8 reserved;
    u16 length; // big endian
    s32 status; // big endian
} AossStatusRecord;

// The body of an encrypted message
typedef struct AossEncrypted {
    u16 length; // big endian, of the data
    u8 iv[2];
    u8 data[1];
} AossEncrypted;

typedef struct AossPacket {
    int sock;
    int length;
    int unk_08;
    AossHeader header;
    u8 body[AOSS_PACKET_SIZE - sizeof(AossHeader)];
} AossPacket;

// What the hello has: two names and a number
typedef struct AossHello {
    u8 name[0x80];
    u8 info[0x80];
    u8 extra[4];
} AossHello;

// The hello's fields (see AossHello) take the first three buffers, and the keys of the stages the next ones
typedef struct AossSession {
    u8 buffer[5][0x80];
} AossSession;

typedef struct AossWepInfo {
    u32 unk_00;
    u32 keyLength;
    char ssid[0x28];
    u8 key[4][0x40];
} AossWepInfo;

typedef struct AossPskInfo {
    u32 unk_00;
    u32 keyLength;
    char ssid[0x28];
    u8 key[0x40];
} AossPskInfo;

typedef struct AossApInfo {
    u32 unk_00;
    u32 unk_04;
    AossWepInfo wep40;
    AossWepInfo wep104;
    AossPskInfo tkip;
    AossPskInfo aes;
    u32 unk_348[2];
} AossApInfo;

// The request that is run
typedef struct AossRequest {
    const u8 *data; // 0x24
    int length;     // 0x28
    u32 wanted;     // 0x2c: the types of security that were asked for
    u32 found;      // 0x30: the types that the access point has
    u32 addr;       // 0x34: of the access point
    u32 netmask;    // 0x38
    s8 reconnected; // 0x3c
    s8 version;     // 0x3d
} AossRequest;

typedef struct AossWork {
    int encrypt;         // 0x00
    WifiScanList *scanList; // 0x04
    int randReady;       // 0x08
    WifiConnectResult *apInfo; // 0x0c
    int error;           // 0x10
    AossPacket *packet;  // 0x14
    u32 randState;       // 0x18
    u32 randMul;         // 0x1c
    u32 randAdd;         // 0x20
    AossRequest request; // 0x24
} AossWork;

typedef struct AossRetry {
    s16 count;
    s16 delay;
} AossRetry;

typedef struct AossEndpoints {
    u32 router;
    u32 unk_04[3];
    u32 client;
} AossEndpoints;

typedef struct RC4Context {
    u32 i;
    u32 j;
    u8 *s;
    u32 n;
} RC4Context;

typedef struct SocTimeval {
    int sec;
    int usec;
} SocTimeval;

void *AOSS_MALLOC(u32 size);
void AOSS_FREE(void *ptr);
static int AOSS_Init_old(AossParam *param);
static u32 aoss_IP_sta(u32 addr, u32 netmask);
static void aoss_release(void);
static void aoss_data_init(AossParam *param);
static void aoss_set_error(int error);
static int aoss_get_error(void);
static int AOSS_seq_data_copy(AossParam *param);
static int aoss_check_str(const u8 *text, int length);
static int checkAP(const WifiScanList *list);
static int AOSS_set_APInfo(WifiConnectParam *target);
static void MakeSendSeqID(int count, u8 *ids, const u8 *mac);
int AOSS_StatusExclusion(int mode);
static int RecvMessage(int state, AossPacket *packet, int *errors, const u8 *ids, int sock);
static int AstsSetInitRes(int state, AossPacket *packet, int *errors, const u8 *ids);
static int AstsSetSecureRes(int state, AossPacket *packet, int *errors, const u8 *ids);
static int AstsRestartRes(int state, AossPacket *packet, int *errors, const u8 *ids);
static int DecodePacket(AossHeader *header);
static int CheckSeqID(int type, const u8 *id);
static int CheckRecvSeqID(const u8 *expected, u8 *received);
static int GetProductInfo(const AossItem *items, AossHello *hello);
static u32 aoss_get_bin(const u8 *data, int length);
static int GetWEPData(const AossRecord *record, AossWepInfo *info);
static int GetWPAData(const AossRecord *record, AossPskInfo *info);
static int GetBssidData(const AossRecord *record, u8 *key);
static int GetSecurityData(int slot, const AossRecord *record, int length, AossApInfo *slots, AossSession *session);
static int GetSecurityType(int flags);
static int SendMessage(int state, const AossEndpoints *endpoints, const u8 *ids, int sock);
static int AstsSetInitReq(const AossEndpoints *endpoints, const u8 *ids, int sock);
static int AstsSetSecureReq(const AossEndpoints *endpoints, const u8 *ids, int sock);
static int AstsRestartReq(const AossEndpoints *endpoints, const u8 *ids, int sock);
static s16 SetStaData(u8 *buffer);
static void PacketDataSet(int encrypt, u8 *dest, const u8 *src, s16 *length, s16 *flags, s8 *checksum);
static void SetPacketHeader(AossHeader *header, int type, int length, int flags, s8 checksum, s8 protocol,
                             const u8 *id);
static int SendPacketUDP(void *buffer, int length, int toAll, int sock);
static int EncodeRC4(const u8 *in, u8 *out, u32 length, u8 *checksum, u8 *iv, const u8 *key, u32 keyLength);
static int DecodeRC4(const u8 *in, u8 *out, u32 length, u8 checksum, const u8 *iv, const u8 *key, u32 keyLength);
static void RC4init(RC4Context *context, const u8 *key, u32 keyLength, u32 n);
static void RC4encrypt(RC4Context *context, u8 *out, const u8 *in, u32 length);
static u32 arcfour_byte(RC4Context *context);
static u8 crc_aoss(const u8 *data, int length);
static u32 update_crc(u32 crc, const u8 *data, int length, int ready, u32 *table);
static void make_crc_table(int unused, u32 *table);
static int CryptSeqNo(u8 *block, int length, const char *key, int keyLength);
static void MakeKey(int round, char *out, int length, const char *key, int keyLength);
static void AddKey(const char *mask, u8 *block, int length);
static void Rotate(u8 *block, int length, u8 *work);
void *Alloc_aoss(u32 name, s32 size);
void Free_aoss(u32 name, void *ptr, s32 size);
int AOSS_SetIPAddr(u32 addr, u32 netmask, u32 gateway);
int AOSS_Disconnect(void);
static int AOSS_Memcmp(const u8 *a, const u8 *b, int size);
static void AOSS_Memcpy(void *dest, const void *src, u32 size);
static void AOSS_Memset(void *dest, int value, u32 size);
static int AOSS_Recvfrom(int sock, void *buffer, int size, int flags, SOCSockAddrIn *from, int *fromLength);
static int AOSS_Select(int count, SOCPollFD *readFds, void *writeFds, void *exceptFds, const SocTimeval *timeout);
static int AOSS_Sendto(int sock, const void *buffer, int size, int flags, SOCSockAddrIn *to, int toLength);
static int AOSS_Setsockopt(int sock, int level, int name, const void *value, int length);
static int AOSS_Socket(int domain, int type, int protocol);
static int AOSS_Bind(int sock, SOCSockAddrIn *address, int length);
static int AOSS_Close(int sock);
static u32 AOSS_Htonl(u32 value);
static u32 AOSS_Htons(u16 value);
static u32 AOSS_Ntohl(u32 value);
static u32 AOSS_Ntohs(u16 value);
static u32 AOSS_Strlen(const char *text);
u16 AOSS_Rand(void);
static void AOSS_FD_ZERO(SOCPollFD *fd);
static void AOSS_FD_SET(int sock, SOCPollFD *fd);

// The names of the file's data are the prebuilt library's (its DWARF)
// pokebw2's soconfig
static SOCConfig data_ov016_021daabc = {
    SOC_VENDOR_NINTENDO, SOC_VERSION, Alloc_aoss, Free_aoss, 0, { 0 }, { 0 }, { 0 }, { 0 }, { 0 }, 0x1000, 0x1000,
};
// pokebw2's s_sFd
static int data_ov016_021daab4 = -1;

// pokebw2's TANE
static u8 data_ov016_021dcda8[0x78];
// pokebw2's gSeqID
static u8 data_ov016_021dcda0[8];
// pokebw2's SecurityInfo
static AossApInfo data_ov016_021dc700[2];
// pokebw2's crc_table
static u32 data_ov016_021dc300[256];
// pokebw2's ProductInfo
static AossSession data_ov016_021dc080;
// pokebw2's gData
static AossRequest data_ov016_021dc064;
static int initflg;
// pokebw2's randContext
static MATHRandContext16 data_ov016_021dc058;
// pokebw2's SecurityType
static int data_ov016_021dc040;
static int gError;
static AossPacket * s_buf;
// pokebw2's bssList
static WifiScanList * data_ov016_021dc044;
static WifiConnectResult * linkInfo;
// The first address after addr in its subnet, or the first one if that is the last
// pokebw2's Aoss_NextAddress

void *AOSS_MALLOC(u32 size) {
    return data_ov016_021dce20(size);
}

void AOSS_FREE(void *ptr) {
    data_ov016_021dce2c(ptr);
}

int AOSSi_Init(AossParam *param) {
    int result;

    if (param->scanCount == 0 || param->scanCount < -1 || param->scanDelay < -1 || param->sendCount == 0 ||
        param->sendCount < -1 || param->sendDelay < -1 || param->timeout < -1 || param->client.length == 0 ||
        param->client.length > 0x100 || param->client.data[param->client.length - 1] != 0) {
        result = -1;
    } else {
        result = 0;
    }
    if (data_ov016_021dce20 == NULL || data_ov016_021dce2c == NULL) {
        result = -1;
    }
    if (result == -1) {
        param->error = 0xf;
        aoss_release();
        return -1;
    }
    s_buf = AOSS_MALLOC(AOSS_BUFFER_SIZE);
    if (s_buf == NULL) {
        param->error = 0xf;
        aoss_release();
        return -1;
    }
    AOSS_StatusExclusion(-1);
    result = AOSS_Init_old(param);
    AOSS_FREE(s_buf);
    aoss_release();
    if (data_ov016_021daab4 != -1) {
        AOSS_Close(data_ov016_021daab4);
    }
    return result;
}

static int AOSS_Init_old(AossParam *param) {
    u8 ids[3][8];
    WifiConnectParam target;
    AossEndpoints endpoints;
    AossRetry scan = { -1, -1 };
    AossRetry send = { 0, 0 };
    int option = 1;
    int sends = 0;
    SOCSockAddrIn from;
    int fromLength;
    int received;
    SOCPollFD pollFd;
    SocTimeval timeval;
    SOCSockAddrIn addr;
    int scanResult;
    int state = 0;
    int newState;
    u32 ip = 0;
    AossPacket *packet;
    s16 timeout;
    int ret;
    s16 i;

    AOSS_Memset(ids, state, sizeof(ids));
    scan.count = param->scanCount;
    if (scan.count == -1) {
        scan.count = 10;
    }
    send.count = param->sendCount;
    if (send.count == -1) {
        send.count = 10;
    }
    scan.delay = param->scanDelay;
    if (scan.delay == -1) {
        scan.delay = 100;
    }
    send.delay = param->sendDelay;
    if (send.delay == -1) {
        send.delay = 100;
    }
    timeout = param->timeout;
    if (timeout == -1) {
        timeout = 2000;
    }
    aoss_data_init(param);
    if ((data_ov016_021dc064.wanted & AOSS_TYPE_WEP40) != 1) {
        aoss_set_error(0x13);
        param->error = 0xf;
        aoss_release();
        return -1;
    }

    i = 0;
    AOSS_StatusExclusion(0);
    while (1) {
        if (data_ov016_021dc044 != NULL) {
            AOSS_FREE(data_ov016_021dc044);
            data_ov016_021dc044 = NULL;
        }
        if (AOSSi_WLANGetBSSList(&data_ov016_021dc044) == -1) {
            param->error = 0xf;
            aoss_release();
            return -1;
        }
        ret = checkAP(data_ov016_021dc044);
        if (ret == 4) {
            param->error = 2;
            aoss_release();
            return -1;
        }
        if (ret == 0) {
            break;
        }
        if (i >= scan.count) {
            param->error = 1;
            aoss_release();
            return -1;
        }
        AOSSi_Sleep(scan.delay);
        i++;
    }

    AOSS_StatusExclusion(1);
    AOSS_Memset(&target, 0, sizeof(target));
    if (AOSS_set_APInfo(&target) != 0) {
        param->error = 0xf;
        aoss_release();
        return -1;
    }
    linkInfo = AOSS_MALLOC(sizeof(WifiConnectResult));
    if (linkInfo == NULL) {
        param->error = 0xf;
        aoss_release();
        return -1;
    }
    AOSS_Memset(linkInfo, 0, sizeof(WifiConnectResult));
    for (i = 0; i < scan.count; i++) {
        ret = AOSSi_WLANConnect(&target, linkInfo);
        if (ret == -1) {
            param->error = 0xf;
            aoss_release();
            return -1;
        }
        if (ret != 0 || (ret == 0 && linkInfo->success != 1)) {
            AOSSi_Sleep(scan.delay);
        } else {
            break;
        }
    }
    if (i == scan.count) {
        param->error = 0xf;
        aoss_release();
        return -1;
    }
    if (AOSS_SetIPAddr(AOSS_CLIENT_ADDR, ~0xff, AOSS_CLIENT_ADDR) != 0) {
        aoss_set_error(0xc);
        param->error = 0xf;
        aoss_release();
        return -1;
    }
    aoss_release();
    MakeSendSeqID(3, ids[0], param->mac);
    data_ov016_021daab4 = AOSS_Socket(2, 2, 0);
    if (data_ov016_021daab4 < 0) {
        param->error = 0xf;
        aoss_release();
        return -1;
    }
    if (AOSS_Setsockopt(data_ov016_021daab4, 0xffff, 1, &option, 4) < 0) {
        aoss_set_error(0xb);
        param->error = 0xf;
        aoss_release();
        return -1;
    }
    AOSS_Memset(&addr, 0, sizeof(addr));
    addr.family = 2;
    addr.addr.addr = AOSS_Htonl(AOSS_CLIENT_ADDR);
    addr.port = AOSS_Htons(AOSS_PORT);
    if (AOSS_Bind(data_ov016_021daab4, &addr, 8) < 0) {
        param->error = 0xf;
        aoss_release();
        return -1;
    }

    while (1) {
        packet = s_buf;
        AOSS_Memset(&endpoints, 0, sizeof(endpoints));
        endpoints.client = AOSS_CLIENT_ADDR;
        endpoints.router = AOSS_ROUTER_ADDR;
        while (1) {
            if (state == 1 && data_ov016_021dc064.reconnected != 1) {
                if (data_ov016_021daab4 != -1) {
                    AOSS_Close(data_ov016_021daab4);
                }
                data_ov016_021daab4 = -1;
                if (AOSS_Disconnect() != 0) {
                    param->error = 0xf;
                    aoss_release();
                    return -1;
                }
                data_ov016_021dc044 = AOSS_MALLOC(sizeof(WifiConnectResult));
                if (data_ov016_021dc044 == NULL) {
                    param->error = 0xf;
                    aoss_release();
                    return -1;
                }
                while (1) {
                    if (data_ov016_021dc044 != NULL) {
                        AOSS_FREE(data_ov016_021dc044);
                        data_ov016_021dc044 = NULL;
                    }
                    scanResult = AOSSi_WLANGetBSSList(&data_ov016_021dc044);
                    if (scanResult == -1) {
                        param->error = 0xf;
                        aoss_release();
                        return -1;
                    }
                    ret = checkAP(data_ov016_021dc044);
                    if (ret == 4) {
                        param->error = 2;
                        aoss_release();
                        return -1;
                    }
                    if (ret == 0) {
                        break;
                    }
                    if (i >= scan.count) {
                        param->error = 1;
                        aoss_release();
                        return -1;
                    }
                    AOSSi_Sleep(scan.delay);
                    i++;
                }
                if (scanResult == -1) {
                    param->error = 0xf;
                    aoss_release();
                    return -1;
                }
                linkInfo = AOSS_MALLOC(sizeof(WifiConnectResult));
                if (linkInfo == NULL) {
                    param->error = 0xf;
                    aoss_release();
                    return -1;
                }
                AOSS_Memset(linkInfo, 0, sizeof(WifiConnectResult));
                for (i = 0; i < scan.count; i++) {
                    ret = AOSSi_WLANConnect(&target, linkInfo);
                    if (ret == -1) {
                        param->error = 0xf;
                        aoss_release();
                        return -1;
                    }
                    if (ret != 0 || (ret == 0 && linkInfo->success != 1)) {
                        AOSSi_Sleep(scan.delay);
                    } else {
                        break;
                    }
                }
                if (i == scan.count) {
                    param->error = 0xf;
                    aoss_release();
                    return -1;
                }
                ip = aoss_IP_sta(data_ov016_021dc064.addr, data_ov016_021dc064.netmask);
                if (AOSS_SetIPAddr(ip, data_ov016_021dc064.netmask, ip) != 0) {
                    aoss_set_error(0xc);
                    param->error = 0xf;
                    aoss_release();
                    return -1;
                }
                data_ov016_021dc064.reconnected = 1;
                aoss_release();
                data_ov016_021daab4 = AOSS_Socket(2, 2, 0);
                if (data_ov016_021daab4 < 0) {
                    param->error = 0xf;
                    aoss_release();
                    return -1;
                }
                if (AOSS_Setsockopt(data_ov016_021daab4, 0xffff, 1, &option, 4) < 0) {
                    aoss_set_error(0xb);
                    param->error = 0xf;
                    aoss_release();
                    return -1;
                }
                AOSS_Memset(&addr, 0, sizeof(addr));
                addr.family = 2;
                addr.addr.addr = AOSS_Htonl(ip);
                addr.port = AOSS_Htons(AOSS_PORT);
                if (AOSS_Bind(data_ov016_021daab4, &addr, 8) < 0) {
                    param->error = 0xf;
                    aoss_release();
                    return -1;
                }
            }

            if (SendMessage(state, &endpoints, ids[0], data_ov016_021daab4) == -1) {
                aoss_set_error(0x1000 + state);
                param->error = 0xf;
                aoss_release();
                return -1;
            }
            AOSS_Memset(packet, 0, AOSS_BUFFER_SIZE);
            AOSS_FD_ZERO(&pollFd);
            AOSS_FD_SET(data_ov016_021daab4, &pollFd);
            timeval.sec = timeout / 1000;
            timeval.usec = timeout % 1000 * 1000;
            if (AOSS_Select(data_ov016_021daab4 + 1, &pollFd, NULL, NULL, &timeval) <= 0) {
                sends++;
                if (sends > send.count) {
                    if (state == 0) {
                        aoss_set_error(0xf);
                    } else if (state == 1) {
                        aoss_set_error(0x10);
                    } else {
                        aoss_set_error(0x11);
                    }
                    ret = -1;
                    goto done;
                }
                AOSSi_Sleep(send.delay);
                continue;
            }
            fromLength = 8;
            received = AOSS_Recvfrom(data_ov016_021daab4, &packet->header, AOSS_PACKET_SIZE, 0, &from, &fromLength);
            packet->sock = data_ov016_021daab4;
            packet->length = AOSS_Ntohs(received);
            newState = RecvMessage(state, packet, &sends, ids[0], data_ov016_021daab4);
            if (newState == 100) {
                ret = 0;
                goto done;
            }
            if (newState == -1) {
                ret = -1;
                goto done;
            }
            if (state != newState) {
                if (newState == 2) {
                    if (data_ov016_021daab4 != -1) {
                        AOSS_Close(data_ov016_021daab4);
                    }
                    data_ov016_021daab4 = -1;
                    if (AOSS_Disconnect() != 0) {
                        param->error = 0xf;
                        aoss_release();
                        return -1;
                    }
                    i = 0;
                    AOSS_StatusExclusion(4);
                    while (1) {
                        if (data_ov016_021dc044 != NULL) {
                            AOSS_FREE(data_ov016_021dc044);
                            data_ov016_021dc044 = NULL;
                        }
                        scanResult = AOSSi_WLANGetBSSList(&data_ov016_021dc044);
                        if (scanResult == -1) {
                            param->error = 0xf;
                            aoss_release();
                            return -1;
                        }
                        ret = checkAP(data_ov016_021dc044);
                        if (ret == 4) {
                            param->error = 2;
                            aoss_release();
                            return -1;
                        }
                        if (ret == 0) {
                            break;
                        }
                        if (i >= scan.count) {
                            param->error = 1;
                            aoss_release();
                            return -1;
                        }
                        AOSSi_Sleep(scan.delay);
                        i++;
                    }
                    linkInfo = AOSS_MALLOC(sizeof(WifiConnectResult));
                    if (linkInfo == NULL) {
                        param->error = 0xf;
                        aoss_release();
                        return -1;
                    }
                    AOSS_Memset(linkInfo, 0, sizeof(WifiConnectResult));
                    for (i = 0; i < scan.count; i++) {
                        ret = AOSSi_WLANConnect(&target, linkInfo);
                        if (ret == -1) {
                            param->error = 0xf;
                            aoss_release();
                            return -1;
                        }
                        if (ret != 0 || (ret == 0 && linkInfo->success != 1)) {
                            AOSSi_Sleep(scan.delay);
                        } else {
                            break;
                        }
                    }
                    if (i == scan.count) {
                        param->error = 0xf;
                        aoss_release();
                        return -1;
                    }
                    if (AOSS_SetIPAddr(ip, data_ov016_021dc064.netmask, ip) != 0) {
                        aoss_set_error(0xc);
                        param->error = 0xf;
                        aoss_release();
                        return -1;
                    }
                    aoss_release();
                    data_ov016_021daab4 = AOSS_Socket(2, 2, 0);
                    if (data_ov016_021daab4 < 0) {
                        param->error = 0xf;
                        aoss_release();
                        return -1;
                    }
                    if (AOSS_Setsockopt(data_ov016_021daab4, 0xffff, 1, &option, 4) < 0) {
                        aoss_set_error(0xb);
                        param->error = 0xf;
                        aoss_release();
                        return -1;
                    }
                    AOSS_Memset(&addr, 0, sizeof(addr));
                    addr.family = 2;
                    addr.addr.addr = AOSS_Htonl(ip);
                    addr.port = AOSS_Htons(AOSS_PORT);
                    if (AOSS_Bind(data_ov016_021daab4, &addr, 8) < 0) {
                        param->error = 0xf;
                        aoss_release();
                        return -1;
                    }
                }
                state = newState;
                break;
            }
            state = newState;
            if (sends > send.count) {
                if (newState == 0) {
                    aoss_set_error(0xf);
                } else if (newState == 1) {
                    aoss_set_error(0x10);
                } else {
                    aoss_set_error(0x11);
                }
                ret = -1;
                goto done;
            }
            AOSSi_Sleep(send.delay);
            break;
        }
    }

done:
    if (data_ov016_021daab4 != -1) {
        AOSS_Close(data_ov016_021daab4);
    }
    data_ov016_021daab4 = -1;
    if (AOSS_Disconnect() != 0) {
        param->error = 0xf;
        aoss_release();
        return -1;
    }
    if (ret != 0) {
        u8 error;

        switch (aoss_get_error()) {
        case 0xf:
            error = 3;
            break;
        case 0x10:
            error = 4;
            break;
        case 0x11:
            error = 5;
            break;
        case 0x14:
            error = 7;
            break;
        case 0x15:
            error = 8;
            break;
        default:
            error = 0xf;
            break;
        }
        param->error = error;
        aoss_release();
        return -1;
    }
    if (AOSS_seq_data_copy(param) != 0) {
        param->error = 6;
        aoss_release();
        return -1;
    }
    return 0;
}



static u32 aoss_IP_sta(u32 addr, u32 netmask) {
    u32 network = addr & netmask;
    u32 next = ((addr & ~netmask) + 1) | network;

    if (next >= (~netmask | network)) {
        return network | 1;
    }
    return next;
}

// pokebw2's Aoss_ReleaseBuffers
static void aoss_release(void) {
    if (linkInfo != NULL) {
        AOSS_FREE(linkInfo);
        linkInfo = NULL;
    }
    if (data_ov016_021dc044 != NULL) {
        AOSS_FREE(data_ov016_021dc044);
        data_ov016_021dc044 = NULL;
    }
}

// pokebw2's Aoss_Begin
static void aoss_data_init(AossParam *param) {
    AOSS_Memset(data_ov016_021dcda0, 0, 8);
    gError = 1;
    AOSS_Memset(&data_ov016_021dc064, 0, sizeof(AossRequest));
    data_ov016_021dc064.data = param->client.data;
    data_ov016_021dc064.length = param->client.length;
    data_ov016_021dc064.wanted = param->types & 0xf;
    data_ov016_021dc064.version = param->client.version;
    data_ov016_021dc064.found = 0;
    data_ov016_021dc064.addr = AOSS_ROUTER_ADDR;
    data_ov016_021dc064.reconnected = 0;
}

// pokebw2's Aoss_SetError
static void aoss_set_error(int error) {
    gError = error;
}

// pokebw2's Aoss_GetError
static int aoss_get_error(void) {
    return gError;
}

// Copies the keys that the access point sent to the answer, if they are fit
// pokebw2's Aoss_StoreResult
static int AOSS_seq_data_copy(AossParam *param) {
    AossResult *result = &param->result;
    AossWepInfo *wep40 = &data_ov016_021dc700[0].wep40;
    AossWepInfo *wep104 = &data_ov016_021dc700[0].wep104;
    AossPskInfo *tkip = &data_ov016_021dc700[0].tkip;
    AossPskInfo *aes = &data_ov016_021dc700[0].aes;
    int ret;

    if (result == NULL) {
        return -1;
    }
    param->types = data_ov016_021dc064.wanted & data_ov016_021dc064.found;
    AOSS_Memset(result, 0, sizeof(AossResult));
    if (param->types & AOSS_TYPE_WEP40) {
        AOSS_Memcpy(result->wep40Key[0], wep40->key[0], wep40->keyLength);
        AOSS_Memcpy(result->wep40Key[1], wep40->key[1], wep40->keyLength);
        AOSS_Memcpy(result->wep40Key[2], wep40->key[2], wep40->keyLength);
        AOSS_Memcpy(result->wep40Key[3], wep40->key[3], wep40->keyLength);
        ret = aoss_check_str((u8 *)wep40->ssid, AOSS_Strlen(wep40->ssid));
        if (ret != 0) {
            goto fail;
        }
        AOSS_Memcpy(result->wep40Ssid, wep40->ssid, AOSS_Strlen(wep40->ssid));
    }
    if (param->types & AOSS_TYPE_WEP104) {
        AOSS_Memcpy(result->wep104Key[0], wep104->key[0], wep104->keyLength);
        AOSS_Memcpy(result->wep104Key[1], wep104->key[1], wep104->keyLength);
        AOSS_Memcpy(result->wep104Key[2], wep104->key[2], wep104->keyLength);
        AOSS_Memcpy(result->wep104Key[3], wep104->key[3], wep104->keyLength);
        ret = aoss_check_str((u8 *)wep104->ssid, AOSS_Strlen(wep104->ssid));
        if (ret != 0) {
            goto fail;
        }
        AOSS_Memcpy(result->wep104Ssid, wep104->ssid, AOSS_Strlen(wep104->ssid));
    }
    if (param->types & AOSS_TYPE_TKIP) {
        ret = aoss_check_str(tkip->key, tkip->keyLength - 1);
        if (ret != 0) {
            goto fail;
        }
        AOSS_Memcpy(result->tkipKey, tkip->key, tkip->keyLength);
        ret = aoss_check_str((u8 *)tkip->ssid, AOSS_Strlen(tkip->ssid));
        if (ret != 0) {
            goto fail;
        }
        AOSS_Memcpy(result->tkipSsid, tkip->ssid, AOSS_Strlen(tkip->ssid));
    }
    if (param->types & AOSS_TYPE_AES) {
        ret = aoss_check_str(aes->key, aes->keyLength - 1);
        if (ret != 0) {
            goto fail;
        }
        AOSS_Memcpy(result->aesKey, aes->key, aes->keyLength);
        ret = aoss_check_str((u8 *)aes->ssid, AOSS_Strlen(aes->ssid));
        if (ret != 0) {
            goto fail;
        }
        AOSS_Memcpy(result->aesSsid, aes->ssid, AOSS_Strlen(aes->ssid));
    }
    param->error = 0;
    return 0;

fail:
    AOSS_Memset(result, 0, sizeof(AossResult));
    return -1;
}

// pokebw2's Aoss_IsPrintable
static int aoss_check_str(const u8 *text, int length) {
    int i;

    for (i = 0; i < length; i++) {
        u8 c = *text++;

        if (c < 0x20 || c > 0x7f) {
            return -1;
        }
    }
    return 0;
}

// Whether the scan has exactly one access point named ESSID-AOSS: 0 if so, 4 if there are several, 5 if none
// pokebw2's Aoss_CheckScan
static int checkAP(const WifiScanList *list) {
    int result = 0;
    int found = 0;
    int count = list->count;
    int i;

    if (count == 0) {
        return 5;
    }
    if ((u32)count > 0x40) {
        count = 0x40;
    }
    for (i = 0; i < count; i++) {
        if ((list->ap[i].type & 1) && list->ap[i].ssidLength == AOSS_Strlen(AOSS_SSID) &&
            AOSS_Memcmp(list->ap[i].ssid, (u8 *)AOSS_SSID, AOSS_Strlen(AOSS_SSID)) == 0) {
            found++;
        }
    }
    if (found > 1) {
        result = 4;
    }
    if (found == 0) {
        result = 5;
    }
    return result;
}

// The access point to connect to: ESSID-AOSS with the WEP key MELCO
// pokebw2's Aoss_InitTarget
static int AOSS_set_APInfo(WifiConnectParam *target) {
    target->ssidLength = AOSS_Strlen(AOSS_SSID);
    AOSS_Memcpy(target->ssid, AOSS_SSID, target->ssidLength);
    target->authMode = 1;
    target->keyLength = AOSS_Strlen(AOSS_KEY);
    if (target->keyLength > 13) {
        return -1;
    }
    AOSS_Memcpy(target->key, AOSS_KEY, target->keyLength);
    return 0;
}

// pokebw2's Aoss_MakeIds
static void MakeSendSeqID(int count, u8 *ids, const u8 *mac) {
    typedef struct {
        u8 mac[6];
        u16 number;
    } Id;
    Id *id = (Id *)ids;
    int i;

    for (i = 0; i < count; i++) {
        AOSS_Memcpy(id->mac, mac, 6);
        id->number = AOSS_Rand();
        id->number = AOSS_Htons(id->number);
        id++;
    }
}

int AOSS_StatusExclusion(int mode) {
    static int before = -1;

    if (mode == -1) {
        before = mode;
        return 0;
    }
    if (before != mode) {
        before = mode;
        return AOSSi_Status(mode);
    }
    return 0;
}


// pokebw2's Aoss_ProcessPacket
static int RecvMessage(int state, AossPacket *packet, int *errors, const u8 *ids, int sock) {
    AossHeader *header = &packet->header;

    if (AOSS_Ntohs(header->version) < 1) {
        (*errors)++;
        return state;
    }
    if (header->protocol != AOSS_PROTOCOL) {
        (*errors)++;
        return state;
    }
    if (DecodePacket(&packet->header) > 0) {
        (*errors)++;
        return state;
    }
    switch (AOSS_Ntohs(header->type)) {
    case 0x1010:
        state = AstsSetInitRes(state, packet, errors, ids);
        break;
    case 0x2010:
        state = AstsSetSecureRes(state, packet, errors, ids);
        break;
    case 0x3010:
        state = AstsRestartRes(state, packet, errors, ids);
        break;
    }
    return state;
}

// pokebw2's Aoss_OnStage0
static int AstsSetInitRes(int state, AossPacket *packet, int *errors, const u8 *ids) {
    AossHeader *header;
    AossRecord *record;
    int result;

    if (state != 0) {
        (*errors)++;
        return state;
    }
    header = &packet->header;
    record = (AossRecord *)packet->body;
    if (CheckRecvSeqID(ids, header->id) < 0) {
        (*errors)++;
        return state;
    }
    if (AOSS_Ntohs(record->length) == 0) {
        (*errors)++;
        return state;
    }
    if (record->type == 7) {
        if (AOSS_Ntohl(record->payload.status) == -2) {
            aoss_set_error(0x14);
        } else if (AOSS_Ntohl(record->payload.status) == -3) {
            aoss_set_error(0x15);
        } else {
            aoss_set_error(0x18);
        }
        return -1;
    }
    if (record->type != 1) {
        (*errors)++;
        return state;
    }
    result = GetProductInfo(record->payload.items, (AossHello *)data_ov016_021dc080.buffer[0]);
    if (result < 0) {
        if (result == -2) {
            aoss_set_error(0x16);
            return -1;
        }
        (*errors)++;
        return state;
    }
    data_ov016_021dc040 = GetSecurityType(AOSS_Ntohs(header->flags));
    *errors = 0;
    return 1;
}

// pokebw2's Aoss_OnStage1
static int AstsSetSecureRes(int state, AossPacket *packet, int *errors, const u8 *ids) {
    AossHeader *header;
    AossRecord *record;

    if (state != 1) {
        (*errors)++;
        return state;
    }
    header = &packet->header;
    record = (AossRecord *)packet->body;
    if (CheckRecvSeqID(ids + 8, header->id) < 0) {
        (*errors)++;
        return state;
    }
    if (AOSS_Ntohs(record->length) == 0) {
        (*errors)++;
        return state;
    }
    if (record->type == 7) {
        if (AOSS_Ntohl(record->payload.status) == -2) {
            aoss_set_error(0x14);
        } else if (AOSS_Ntohl(record->payload.status) == -3) {
            aoss_set_error(0x15);
        } else {
            aoss_set_error(0x18);
        }
        return -1;
    }
    AOSS_Memset(data_ov016_021dc700, 0, sizeof(data_ov016_021dc700));
    if (GetSecurityData(0, record, AOSS_Ntohs(header->length), data_ov016_021dc700, &data_ov016_021dc080) < 0) {
        (*errors)++;
        return state;
    }
    if ((data_ov016_021dc064.found & data_ov016_021dc064.wanted) == 0) {
        return state;
    }
    *errors = 0;
    return 2;
}

// pokebw2's Aoss_OnStage2
static int AstsRestartRes(int state, AossPacket *packet, int *errors, const u8 *ids) {
    AossHeader *header;
    AossRecord *record;

    if (state != 2) {
        (*errors)++;
        return state;
    }
    record = (AossRecord *)packet->body;
    header = (AossHeader *)(packet->body - sizeof(AossHeader));
    if (CheckRecvSeqID(ids + 16, header->id) < 0) {
        (*errors)++;
        return state;
    }
    if (record->type != 7) {
        (*errors)++;
        return state;
    }
    if (AOSS_Ntohs(record->length) == 0) {
        (*errors)++;
        return state;
    }
    if (AOSS_Ntohl(record->payload.status) == 0) {
        return 100;
    }
    if (AOSS_Ntohl(record->payload.status) == -2) {
        aoss_set_error(0x14);
        return -1;
    }
    if (AOSS_Ntohl(record->payload.status) == -3) {
        aoss_set_error(0x15);
        return -1;
    }
    aoss_set_error(0x18);
    return -1;
}

// Checks the id of a message and decrypts its body; 0 if it is fit
// pokebw2's Aoss_DecodePacket
static int DecodePacket(AossHeader *header) {
    AossEncrypted *body = (AossEncrypted *)(header + 1);
    u8 id[8];
    u32 length;
    u8 *buffer;
    int result;

    AOSS_Memcpy(id, header->id, 8);
    if (CryptSeqNo(id, 8, AOSS_KEY, AOSS_Strlen(AOSS_KEY)) == -1) {
        aoss_set_error(2);
        return -100;
    }
    result = CheckSeqID(AOSS_Ntohs(header->type), id);
    if (result != 0) {
        return result;
    }
    if (AOSS_Ntohs(header->type) == 0x1000) {
        AOSS_Memcpy(data_ov016_021dcda0, id, 8);
    }
    if ((AOSS_Ntohs(header->flags) & 0xf) == 0) {
        return 0;
    }
    length = AOSS_Ntohs(body->length);
    buffer = AOSS_MALLOC(length);
    if (buffer == NULL) {
        aoss_set_error(2);
        return 100;
    }
    if (DecodeRC4(body->data, buffer, length, header->checksum, body->iv, data_ov016_021dcda0, 8) < 0) {
        AOSS_FREE(buffer);
        if (aoss_get_error() == 2) {
            return 100;
        }
        return 200;
    }
    AOSS_Memcpy(body, buffer, length);
    header->length = AOSS_Htons(length);
    AOSS_FREE(buffer);
    return 0;
}

// Whether a message may come from the access point we talk to: 0 if so, 1 if it is another, 2 if there is none yet
// pokebw2's Aoss_CheckPeer
static int CheckSeqID(int type, const u8 *id) {
    int result = 0;
    int i;
    int known = 0;

    for (i = 0; i < 6; i++) {
        if (data_ov016_021dcda0[i] != 0) {
            known = 1;
            break;
        }
    }
    if (known) {
        if (AOSS_Memcmp(data_ov016_021dcda0, id, 6) != 0) {
            result = 1;
        }
    } else if (type != 0x1000) {
        result = 2;
    }
    return result;
}

// Whether the id that came back is the one that was sent, with its number one higher: 0 if so, -1 or -2
// pokebw2's Aoss_CheckIds
static int CheckRecvSeqID(const u8 *expected, u8 *received) {
    int result = 0;

    CryptSeqNo(received, 8, AOSS_KEY, AOSS_Strlen(AOSS_KEY));
    if (AOSS_Memcmp(expected, received, 6) != 0) {
        result--;
    } else if (AOSS_Ntohs(*(u16 *)(expected + 6)) + 1 != AOSS_Ntohs(*(u16 *)(received + 6))) {
        result -= 2;
    }
    return result;
}

// pokebw2's Aoss_ParseHello
static int GetProductInfo(const AossItem *items, AossHello *hello) {
    const AossItem *item;

    AOSS_Memset(hello, 0, 0x104);
    item = items;
    while (1) {
        int length = AOSS_Ntohs(item->length);

        if (length <= 0) {
            return -1;
        }
        switch (item->type) {
        case 0:
            AOSS_Memcpy(hello->name, item->data, length);
            break;
        case 1:
            AOSS_Memcpy(hello->info, item->data, length);
            break;
        case 2:
            AOSS_Memcpy(hello->extra, item->data, length);
            break;
        case 3:
        case 4: {
            int value = AOSS_Ntohs(item->data[0]);

            if (value <= 0) {
                return -2;
            }
        } break;
        case 5:
            data_ov016_021dc064.addr = AOSS_Ntohl(aoss_get_bin(item->data, length));
            break;
        case 6:
            data_ov016_021dc064.netmask = AOSS_Ntohl(aoss_get_bin(item->data, length));
            break;
        default:
            return -1;
        }
        if (item->next == 0) {
            break;
        }
        item = (const AossItem *)((const u8 *)items + AOSS_Ntohs(item->next));
    }
    return 0;
}

// pokebw2's Aoss_ReadLe
static u32 aoss_get_bin(const u8 *data, int length) {
    u32 value = 0;
    int i;
    const u8 *p = data + (length - 1);

    for (i = 0; i < length; i++) {
        value = (value << 8) + *p--;
    }
    return value;
}

// pokebw2's Aoss_ParseWep
static int GetWEPData(const AossRecord *record, AossWepInfo *info) {
    const AossItem *item = record->payload.secure.items;
    const AossItem *items;
    char *ssid = info->ssid;
    u8 *key3 = info->key[3];
    u8 *key2 = info->key[2];
    u8 *key1 = info->key[1];
    u8 *key0 = info->key[0];

    items = item;
    while (1) {
        u32 length = AOSS_Ntohs(item->length);

        switch (item->type) {
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
            if (length > 5) {
                return -1;
            }
            break;
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x23:
            if (length > 13) {
                return -1;
            }
            break;
        case 0x15:
        case 0x25:
            if (length > 33) {
                return -1;
            }
            break;
        }
        switch (item->type) {
        case 0x10:
        case 0x20:
            AOSS_Memcpy(key0, item->data, length);
            info->keyLength = length;
            break;
        case 0x11:
        case 0x21:
            AOSS_Memcpy(key1, item->data, length);
            info->keyLength = length;
            break;
        case 0x12:
        case 0x22:
            AOSS_Memcpy(key2, item->data, length);
            info->keyLength = length;
            break;
        case 0x13:
        case 0x23:
            AOSS_Memcpy(key3, item->data, length);
            info->keyLength = length;
            break;
        case 0x15:
        case 0x25:
            if (length != 0 && item->data[length - 1] != 0) {
                return -1;
            }
            AOSS_Memcpy(ssid, item->data, length);
            break;
        default:
            return -1;
        }
        if (item->next == 0) {
            break;
        }
        item = (const AossItem *)((const u8 *)items + AOSS_Ntohs(item->next));
    }
    return 0;
}

// pokebw2's Aoss_ParsePsk
static int GetWPAData(const AossRecord *record, AossPskInfo *info) {
    const AossItem *item = record->payload.secure.items;
    const AossItem *items;
    char *ssid = info->ssid;
    u8 *key = info->key;

    items = item;
    while (1) {
        u32 length = AOSS_Ntohs(item->length);

        switch (item->type) {
        case 0x30:
        case 0x40:
            if (length > 0x40) {
                return -1;
            }
            break;
        case 0x35:
        case 0x45:
            if (length > 0x21) {
                return -1;
            }
            break;
        }
        switch (item->type) {
        case 0x30:
        case 0x40:
            AOSS_Memcpy(key, item->data, length);
            info->keyLength = length;
            break;
        case 0x35:
        case 0x45:
            if (length != 0 && item->data[length - 1] != 0) {
                return -1;
            }
            AOSS_Memcpy(ssid, item->data, length);
            break;
        default:
            return -1;
        }
        if (item->next != 0) {
            item = (const AossItem *)((const u8 *)items + AOSS_Ntohs(item->next));
        } else {
            return 0;
        }
    }
}

// pokebw2's Aoss_ParseKey
static int GetBssidData(const AossRecord *record, u8 *key) {
    const AossItem *item = record->payload.secure.items;
    int length = AOSS_Ntohs(item->length);

    if (length <= 0) {
        return -1;
    }
    if (item->type != 0x70) {
        return -1;
    }
    AOSS_Memcpy(key, item->data, length);
    return 0;
}

// Reads the security settings that the reply of a stage has into slot of slots; 0 if they are fit
// pokebw2's Aoss_ParseSecurity
static int GetSecurityData(int slot, const AossRecord *record, int length, AossApInfo *slots, AossSession *session) {
    static u8 AttrNo[2] = { 9, 8 };
    u32 found = 0;
    int result;
    AossApInfo *ap;
    const AossRecord *entry;
    const u8 *ptr;
    int step;
    int size;

    if (length <= 0) {
        return -2;
    }
    ptr = (const u8 *)record;
    while (1) {
        entry = (const AossRecord *)ptr;
        if (entry->type == AttrNo[slot]) {
            break;
        }
        step = AOSS_Ntohs(entry->length) + 4;
        length -= step;
        ptr += step;
        if (length <= 0) {
            return -4;
        }
    }
    ptr += 4;
    size = AOSS_Ntohs(entry->length);
    ap = &slots[slot];
    do {
        entry = (const AossRecord *)ptr;
        switch (entry->type) {
        case 3:
            result = GetWEPData(entry, &ap->wep40);
            found |= AOSS_TYPE_WEP40;
            break;
        case 4:
            result = GetWEPData(entry, &ap->wep104);
            found |= AOSS_TYPE_WEP104;
            break;
        case 5:
            result = GetWPAData(entry, &ap->tkip);
            found |= AOSS_TYPE_TKIP;
            break;
        case 6:
            result = GetWPAData(entry, &ap->aes);
            found |= AOSS_TYPE_AES;
            break;
        case 10:
            result = GetBssidData(entry, session->buffer[slot + 3]);
            break;
        default:
            result = -3;
            break;
        }
        if (result != 0) {
            return result;
        }
        step = AOSS_Ntohs(entry->length) + 4;
        size -= step;
        ptr += step;
    } while (size > 0);
    data_ov016_021dc064.found |= found;
    return 0;
}

// pokebw2's Aoss_IsEncrypted
static int GetSecurityType(int flags) {
    int result = 0;

    if (flags & 0x10) {
        result = 1;
    }
    return result;
}

// pokebw2's Aoss_Send
static int SendMessage(int state, const AossEndpoints *endpoints, const u8 *ids, int sock) {
    switch (state) {
    case 0:
        AOSS_StatusExclusion(2);
        return AstsSetInitReq(endpoints, ids, sock);
    case 1:
        AOSS_StatusExclusion(3);
        return AstsSetSecureReq(endpoints, ids, sock);
    case 2:
        AOSS_StatusExclusion(5);
        return AstsRestartReq(endpoints, ids, sock);
    }
    return -1;
}

// pokebw2's Aoss_SendStage0
static int AstsSetInitReq(const AossEndpoints *endpoints, const u8 *ids, int sock) {
    s8 checksum = 0;
    s16 length = 0;
    s16 flags = 0;
    u8 id[8];
    AossHeader *header = (AossHeader *)s_buf;
    u8 *body;
    u8 *payload;

    AOSS_Memset(header, 0, AOSS_PACKET_SIZE);
    body = AOSS_MALLOC(0x210);
    if (body == NULL) {
        aoss_set_error(2);
        return -1;
    }
    AOSS_Memset(body, 0, 0x210);
    payload = (u8 *)(header + 1);
    AOSS_Memcpy(data_ov016_021dcda0, ids, 8);
    AOSS_Memcpy(id, data_ov016_021dcda0, 8);
    length = SetStaData(body + 4);
    if (length < 0) {
        aoss_set_error(3);
        if (body != NULL) {
            AOSS_FREE(body);
        }
        return -1;
    }
    body[0] = 0;
    *(u16 *)(body + 2) = AOSS_Htons(length);
    length += 4;
    PacketDataSet(0, payload, body, &length, &flags, &checksum);
    flags |= 0x10;
    if (CryptSeqNo(id, 8, AOSS_KEY, sizeof(AOSS_KEY)) != 0) {
        aoss_set_error(2);
        if (body != NULL) {
            AOSS_FREE(body);
        }
        return -1;
    }
    SetPacketHeader(header, 0x1000, length, flags, checksum, AOSS_PROTOCOL, id);
    length += sizeof(AossHeader);
    SendPacketUDP(header, length, 0xff, sock);
    if (body != NULL) {
        AOSS_FREE(body);
    }
    return 0;
}

// pokebw2's Aoss_SendStage1
static int AstsSetSecureReq(const AossEndpoints *endpoints, const u8 *ids, int sock) {
    s8 checksum = 0;
    s16 length = 0;
    s16 flags = 0;
    u8 id[8];
    AossStatusRecord record;
    AossHeader *header = (AossHeader *)s_buf;

    AOSS_Memset(&record, 0, 8);
    AOSS_Memset(header, 0, AOSS_PACKET_SIZE);
    record.type = 2;
    record.reserved = 0;
    record.length = AOSS_Htons(4);
    record.status = data_ov016_021dc064.wanted;
    record.status = AOSS_Htonl(record.status);
    length = 8;
    PacketDataSet(data_ov016_021dc040, (u8 *)(header + 1), (u8 *)&record, &length, &flags, &checksum);
    AOSS_Memcpy(id, ids + 8, 8);
    if (CryptSeqNo(id, 8, AOSS_KEY, sizeof(AOSS_KEY)) != 0) {
        aoss_set_error(2);
        return -1;
    }
    SetPacketHeader(header, 0x2000, length, flags, checksum, AOSS_PROTOCOL, id);
    length += sizeof(AossHeader);
    SendPacketUDP(header, length, 0, sock);
    return 0;
}

// pokebw2's Aoss_SendStage2
static int AstsRestartReq(const AossEndpoints *endpoints, const u8 *ids, int sock) {
    u8 id[8];
    AossHeader *header = (AossHeader *)s_buf;

    AOSS_Memset(header, 0, AOSS_PACKET_SIZE);
    AOSS_Memcpy(id, ids + 16, 8);
    CryptSeqNo(id, 8, AOSS_KEY, AOSS_Strlen(AOSS_KEY));
    SetPacketHeader(header, 0x3000, 0, 0, 0, AOSS_PROTOCOL, id);
    SendPacketUDP(header, sizeof(AossHeader), 0, sock);
    return 0;
}

// The body of the hello: the request's data and the item 0x60 with 14; returns its length
// pokebw2's Aoss_BuildHello
static s16 SetStaData(u8 *buffer) {
    AossItem *first = (AossItem *)buffer;
    s16 length;
    s16 total = 0;
    AossItem *second;
    s16 offset;
    u32 value;

    first->type = data_ov016_021dc064.version;
    first->reserved = 1;
    length = data_ov016_021dc064.length;
    AOSS_Memcpy(first->data, data_ov016_021dc064.data, length);
    first->length = AOSS_Htons(length);
    offset = ((s16)(length + 6) + 1) / 2 * 2;
    first->next = AOSS_Htons(offset);
    total += offset;
    second = (AossItem *)(buffer + offset);
    second->type = 0x60;
    second->reserved = 0;
    second->next = AOSS_Htons(0);
    value = AOSS_Htonl(0xe);
    AOSS_Memcpy(second->data, &value, 4);
    second->length = AOSS_Htons(4);
    total += 10;
    return total;
}

// Puts the body in the packet, encrypted when the client wants it so
// pokebw2's Aoss_BuildPayload
static void PacketDataSet(int encrypt, u8 *dest, const u8 *src, s16 *length, s16 *flags, s8 *checksum) {
    if (encrypt == 1) {
        *flags = 1;
        EncodeRC4(src, dest + 4, *length, (u8 *)checksum, dest + 2, data_ov016_021dcda0, 8);
        *(u16 *)dest = AOSS_Htons(*length);
        *length += 4;
    } else {
        AOSS_Memcpy(dest, src, *length);
    }
}

static void SetPacketHeader(AossHeader *header, int type, int length, int flags, s8 checksum, s8 protocol,
                             const u8 *id) {
    header->version = AOSS_Htons(1);
    header->unk_02 = 0;
    header->unk_04 = 0;
    header->type = AOSS_Htons(type);
    header->unk_08 = 0;
    header->length = AOSS_Htons(length);
    header->flags = AOSS_Htons(flags);
    header->checksum = checksum;
    header->protocol = protocol;
    AOSS_Memcpy(header->id, id, 8);
}

// pokebw2's Aoss_SendPacket
static int SendPacketUDP(void *buffer, int length, int toAll, int sock) {
    SOCSockAddrIn to;

    AOSS_Memset(&to, 0, sizeof(to));
    to.family = 2;
    to.port = AOSS_Htons(AOSS_PORT);
    to.addr.addr = AOSS_Htonl(data_ov016_021dc064.addr);
    if (toAll == 0xff || data_ov016_021dc064.reconnected == 0) {
        to.addr.addr = -1;
    }
    return AOSS_Sendto(sock, buffer, length, 0, &to, 8);
}

// pokebw2's Aoss_Encrypt
static int EncodeRC4(const u8 *in, u8 *out, u32 length, u8 *checksum, u8 *iv, const u8 *key, u32 keyLength) {
    RC4Context context;
    u16 number;

    *checksum = crc_aoss(in, length);
    context.s = AOSS_MALLOC(length);
    if (context.s == NULL) {
        return -1;
    }
    number = AOSS_Rand();
    AOSS_Memcpy(iv, &number, 2);
    AOSS_Memcpy(data_ov016_021dcda8, iv, 2);
    AOSS_Memcpy(data_ov016_021dcda8 + 2, key, keyLength);
    RC4init(&context, data_ov016_021dcda8, keyLength + 2, length);
    RC4encrypt(&context, out, in, length);
    AOSS_FREE(context.s);
    return 0;
}

// pokebw2's Aoss_Decrypt
static int DecodeRC4(const u8 *in, u8 *out, u32 length, u8 checksum, const u8 *iv, const u8 *key, u32 keyLength) {
    RC4Context context;

    context.s = AOSS_MALLOC(length);
    if (context.s == NULL) {
        aoss_set_error(2);
        return -1;
    }
    AOSS_Memcpy(data_ov016_021dcda8, iv, 2);
    AOSS_Memcpy(data_ov016_021dcda8 + 2, key, keyLength);
    RC4init(&context, data_ov016_021dcda8, keyLength + 2, length);
    RC4encrypt(&context, out, in, length);
    u8 actual = crc_aoss(out, length);
    if (actual != checksum) {
        aoss_set_error(0x12);
        AOSS_FREE(context.s);
        return -1;
    }
    AOSS_FREE(context.s);
    return 0;
}

// RC4 with a state of n bytes instead of 256
// pokebw2's Rc4_Init
void RC4init(RC4Context *context, const u8 *key, u32 keyLength, u32 n) {
    u8 *s = context->s;
    u32 i;
    u32 j;
    u32 k;
    u8 t;
    u8 u;

    context->j = 0;
    context->i = 0;
    context->n = n;
    for (i = 0; i < n; i++) {
        s[i] = i;
    }
    j = 0;
    k = 0;
    for (i = 0; i < n; i++) {
        t = s[i];
        j = (j + key[k] + t) % context->n;
        u = s[j];
        s[j] = t;
        s[i] = u;
        k++;
        if (k >= keyLength) {
            k = 0;
        }
    }
}

// pokebw2's Rc4_Crypt
static void RC4encrypt(RC4Context *context, u8 *out, const u8 *in, u32 length) {
    u32 i;

    for (i = 0; i < length; i++) {
        u8 key = arcfour_byte(context);

        out[i] = in[i] ^ key;
    }
}

// pokebw2's Rc4_Next
static u32 arcfour_byte(RC4Context *context) {
    u8 *s = context->s;
    u32 n = context->n;
    u8 i = (context->i + 1) % n;
    u8 si = s[i];
    u8 j = (si + context->j) % n;
    u8 sj;

    sj = s[j];
    context->i = i;
    context->j = j;
    s[j] = si;
    s[i] = sj;
    return s[(si + sj) % context->n];
}

// The low byte of the complement of the CRC-32
// pokebw2's Aoss_Checksum
static u8 crc_aoss(const u8 *data, int length) {
    return update_crc(-1, data, length, 0, data_ov016_021dc300) ^ 0xffffffff;
}

// pokebw2's Crc32_Update
static u32 update_crc(u32 crc, const u8 *data, int length, int ready, u32 *table) {
    int i;

    if (ready == 0) {
        make_crc_table(ready, table);
    }
    for (i = 0; i < length; i++) {
        crc = (crc >> 8) ^ table[(u8)(data[i] ^ crc)];
    }
    return crc;
}

// pokebw2's Crc32_MakeTable
static void make_crc_table(int unused, u32 *table) {
    u32 value;
    int i;
    int bit;
    for (i = 0; i < 256; i++) {
        value = i;
        for (bit = 0; bit < 8; bit++) {
            if (value & 1) {
                value = (value >> 1) ^ 0xedb88320;
            } else {
                value >>= 1;
            }
        }
        *table++ = value;
    }
}

// Scrambles a block with two rounds of a Feistel network keyed by the text; 0 if it had the memory
// pokebw2's Aoss_Scramble
static int CryptSeqNo(u8 *block, int length, const char *key, int keyLength) {
    char *mask;
    u8 *work;
    int round;

    mask = AOSS_MALLOC(length / 2);
    if (mask == NULL) {
        return -1;
    }
    work = AOSS_MALLOC(length);
    if (work == NULL) {
        AOSS_FREE(mask);
        return -1;
    }
    for (round = 0; round < 2; round++) {
        MakeKey(round, mask, length, key, keyLength);
        AddKey(mask, block, length);
        Rotate(block, length, work);
    }
    AOSS_FREE(mask);
    AOSS_FREE(work);
    return 0;
}

// pokebw2's Aoss_ScrambleKey
static void MakeKey(int round, char *out, int length, const char *key, int keyLength) {
    int half = length / 2;
    int k = round % keyLength;
    int i;

    for (i = 0; i < half; i++) {
        out[i] = i;
        out[i] ^= key[k];
        k++;
        if (k >= keyLength) {
            k = 0;
        }
    }
}

// pokebw2's Aoss_ScrambleXor
static void AddKey(const char *mask, u8 *block, int length) {
    int i;
    int half = length / 2;

    for (i = 0; i < half; i++) {
        block[half + i] ^= mask[i];
    }
}

// pokebw2's Aoss_ScrambleSwap
static void Rotate(u8 *block, int length, u8 *work) {
    int half = length / 2;

    AOSS_Memcpy(work, block + half, half);
    AOSS_Memcpy(work + half, block, half);
    AOSS_Memcpy(block, work, length);
}

void *Alloc_aoss(u32 name, s32 size) {
    if (size > 0) {
        return data_ov016_021dce20(size);
    }
    return NULL;
}


void Free_aoss(u32 name, void *ptr, s32 size) {
    data_ov016_021dce2c(ptr);
}

extern u32 data_ov009_021a82bc;
int AOSS_SetIPAddr(u32 addr, u32 netmask, u32 gateway) {
    data_ov016_021daabc.addr.addr = AOSS_Htonl(addr);
    data_ov016_021daabc.netmask.addr = AOSS_Htonl(netmask);
    data_ov016_021daabc.router.addr = AOSS_Htonl(gateway);
    if (func_ov009_0215f8fc(&data_ov016_021daabc) < 0) {
        return -1;
    }
    if (data_ov009_021a82bc == 0) {
        do {
            OS_Sleep(100);
        } while (data_ov009_021a82bc == 0);
    }
    return 0;
}


int func_ov009_0215fa90(void);

int AOSS_Disconnect(void) {
    if (func_ov009_0215fa90() < 0) {
        return -1;
    }
    return -(AOSSi_Disconnect() != 0);
}



// pokebw2's Mem_Compare
static int AOSS_Memcmp(const u8 *a, const u8 *b, int size) {
    int difference = 0;

    while (size-- > 0 && (difference = *a - *b) == 0) {
        a++;
        b++;
    }
    return difference;
}

// pokebw2's Mem_Copy
static void AOSS_Memcpy(void *dest, const void *src, u32 size) {
    MI_CpuCopy8(src, dest, size);
}

// pokebw2's Mem_Fill
static void AOSS_Memset(void *dest, int value, u32 size) {
    MI_CpuFill8(dest, (u8)value, size);
}

// pokebw2's Net_RecvFrom
static int AOSS_Recvfrom(int sock, void *buffer, int size, int flags, SOCSockAddrIn *from, int *fromLength) {
    from->len = *fromLength;
    return func_ov009_0215f500(sock, buffer, size, flags, from);
}

// Waits for the descriptor to be ready for at most the time
// pokebw2's Net_Poll
static int AOSS_Select(int count, SOCPollFD *readFds, void *writeFds, void *exceptFds, const SocTimeval *timeout) {
    SOCPollFD fd = *readFds;
    u64 ticks = 0;

    ticks += ((u64)(s64)timeout->sec * 33514000) >> 6;
    ticks += ((u64)(s64)timeout->usec * 33514000) >> 6;
    return func_ov009_0215fbcc(&fd, 1, ticks);
}

// pokebw2's Net_SendTo
static int AOSS_Sendto(int sock, const void *buffer, int size, int flags, SOCSockAddrIn *to, int toLength) {
    to->len = toLength;
    return func_ov009_0215f5b4(sock, buffer, size, flags, to);
}

// The socket library takes no options
// pokebw2's Net_SetSockOpt
static int AOSS_Setsockopt(int sock, int level, int name, const void *value, int length) {
}

// pokebw2's Net_Socket
static int AOSS_Socket(int domain, int type, int protocol) {
    return func_ov009_0215f428(domain, type, protocol);
}

// pokebw2's Net_Bind
static int AOSS_Bind(int sock, SOCSockAddrIn *address, int length) {
    address->len = length;
    return func_ov009_0215f454(sock, address);
}

// pokebw2's Net_Close
static int AOSS_Close(int sock) {
    return func_ov009_0215f644(sock);
}

// pokebw2's Net_Htonl
static u32 AOSS_Htonl(u32 value) {
    return ((value >> 24) & 0xff) | ((value >> 8) & 0xff00) | ((value << 8) & 0xff0000) | ((value << 24) & 0xff000000);
}

// pokebw2's Net_Htons
static u32 AOSS_Htons(u16 value) {
    return (u16)(((value >> 8) & 0xff) | ((value << 8) & 0xff00));
}

// pokebw2's Net_Ntohl
static u32 AOSS_Ntohl(u32 value) {
    return ((value >> 24) & 0xff) | ((value >> 8) & 0xff00) | ((value << 8) & 0xff0000) | ((value << 24) & 0xff000000);
}

// pokebw2's Net_Ntohs
static u32 AOSS_Ntohs(u16 value) {
    return (u16)(((value >> 8) & 0xff) | ((value << 8) & 0xff00));
}

// pokebw2's Str_Length
static u32 AOSS_Strlen(const char *text) {
    u32 length = 0;

    while (text[length] != '\0') {
        length++;
    }
    return length;
}

u16 AOSS_Rand(void) {
    if (initflg == 0) {
        RTCTime time;
        u32 seed = 0;

        AOSS_Memset(&time, 0, sizeof(time));
        if (RTC_GetTime(&time) == 0) {
            seed += time.hour << 10;
            seed += time.minute << 3;
            seed += time.second;
        }
        MATH_InitRand16(&data_ov016_021dc058, seed);
        initflg = 1;
    }
    return MATH_Rand16(&data_ov016_021dc058, 0x7fff);
}


// pokebw2's Net_PollInit
static void AOSS_FD_ZERO(SOCPollFD *fd) {
    fd->fd = 0;
    fd->events = 0;
    fd->revents = 0;
}

// pokebw2's Net_PollSet
static void AOSS_FD_SET(int sock, SOCPollFD *fd) {
    fd->fd = sock;
    fd->events = 1;
}
