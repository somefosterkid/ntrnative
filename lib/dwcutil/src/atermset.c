// libdwcutil's atermset.c (Black's ov016 0x021b9038-0x021bc820): NEC's Aterm setup for the DWC utility, Thumb code
// that Black has as NitroDWC's prebuilt libdwcutil.a has it (its object of that name, built with MWCC 2.0/sp1p2): the
// WCM wrapper of dwc_wcm.c (apc.c) again, the setup, and in the same object RFC 1321's MD5 (md5.c),
// rijndael-alg-fst.c's AES and keywrap.c's AES key wrap, which match as their reference C is written.
#include <string.h>
#include "dwcutil.h"

typedef struct {
    s32 state;
    s32 timeout;
    s32 result;
} ATERMApConfigState;

// The setup's settings and result (the prebuilt's atermset.o's DWARF)
typedef struct _WS_11G_ENCRYPT_KEY {
    u32 KeyType;
    u32 KeyLength;
    char KeyData[32];
} WS_11G_ENCRYPT_KEY;

typedef struct _WS_11G_ENCRYPT_DATA {
    u32 EncryptMode;
    u32 DefaultKey;
    WS_11G_ENCRYPT_KEY UniqueKey;
    WS_11G_ENCRYPT_KEY SharedKey[4];
    char Passphrase[72];
} WS_11G_ENCRYPT_DATA;

typedef struct _WS_11G_PROFILE_DATA {
    char ConfigName[256];
    char Ssid[32];
    u32 NetworkType;
    u32 Channel;
    u32 PowerManagement;
    WS_11G_ENCRYPT_DATA EncryptData;
    u32 AdHocBand;
    u32 AdHocChannel;
    u32 scanMode;
    u32 tpc;
} WS_11G_PROFILE_DATA;

typedef struct _ATERMApConfigParam {
    char ssid[32];
    int auth_mode;
    int wepkeyid;
    char wepkey[4][32];
    char wpakey[64];
} ATERMApConfigParam;

// The prebuilt's .bss statics (its DWARF's names and types). MWCC sorts them with the file's .data and .rodata in one
// list, these definitions without an initializer last, the last one first: this order gives the prebuilt's layout
// The steps of the automatic setup (the prebuilt's DWARF)
enum PHASE {
    P_INIT,
    P_SCANAP,
    P_CONNECTAP,
    P_INITSOCK,
    P_WAITSEARCH,
    P_SENDSEARCHRES,
    P_WAITENCINFO,
    P_SENDENCINFORES,
    P_WAITWLANSET,
    P_SENDWLANSETRES,
    P_SESSIONEND,
};

static u8 FrameData[2048];
static u8 ElementData[2048];
static u8 gAPSSID[34];
static u8 apci_essid_buf[32];
static u8 aKeySource[32];
static u8 aMacEncKey[16];
static u8 btAdapterMAC[6];
static u8 apci_bssid_buf[6];
static u8 gAPMac[6];
static WS_11G_PROFILE_DATA AutoProfile;
static ATERMApConfigParam gAtermApConfigParam;
static OSThread gAtermThread;
static OSMutex AllocLock;
static s32 event_buf[4];
// Where the wrapper is (the prebuilt's DWARF: an enum of no name)
static enum {
    APC_STATE_NONE,
    APC_STATE_INIT,
    APC_STATE_TO_INIT,
    APC_STATE_IDLE,
    APC_STATE_TO_IDLE,
    APC_STATE_SEARCH,
    APC_STATE_TO_SEARCH,
    APC_STATE_CONNECT,
    APC_STATE_TO_CONNECT,
    APC_STATE_END
} apci_state;
static WCMConfig *apci_IWConfig;
static s32 event_get_ptr;
static s32 gAtermApState;
static u8 *apci_IWBuffer;
static u8 *apc_work;
static u8 *apci_bssid_ptr;
static u8 *apci_essid_ptr;
static WMBssDesc *apci_wmbssdesc;
static s32 bAtermApCanceled;
static enum PHASE AutoPhase;
static s32 bSO_Startup;
static WMBssDesc *ap_list;
static s32 cbElementData;
static s32 bDualCfg;
static u8 *atermapc_work;
static u32 apci_connect_option;
static s32 gAtermThreadId;
static s32 event_set_ptr;
static s32 bApConnect;
static s32 gMaxApList;
static s32 bInitAtermApNetwork;
static s32 gAtermApResult;
static void (*gAtermNotify)(ATERMApConfigState *);
static DWCWcmCallback apci_user_callback;
static u32 apci_search_option;
static void (*gAtermFree)(void *);
static u64 *gThreadStack;
static void * (*gAtermAlloc)(u32);
static s32 gAPId;
static s32 cbFrameData;
static u8 *apci_Wep;
static u8 btAutoResult;
BOOL APC_Init_atermset(DWCWcmCallback callback, void *work, u32 size);
u32 AtermApc_GetEvent(void);
void SendBroadcast(s32 param_1, void *param_2, s32 param_3);
u16 *GetFrameData(u16 *param_1, u32 *param_2, u32 *param_3);
int keywrap_decrypt(u8 *out, const u8 *in, u32 length, const u8 *key, u32 keyLength);
u64 __aeabi_uldivmod(u32 param_1, u32 param_2, u32 param_3, u32 param_4);
void APCi_Notify_atermset(WCMNotice *notice);
int keywrap_encrypt(u8 *out, const u8 *in, u32 length, const u8 *key, u32 keyLength);
void MD5_memcpy(u8 *output, u8 *input, u32 len);
void MD5Transform(u32 *state, u8 *block);


// The ESSID of an access point that hides it
static char gszStealth[] = "******";
void Free_atermset(void *ptr);
int PutMac(char *out, const s8 *mac);

// The scan result that the WCM gives, 0xc0 bytes
typedef struct {
    u16 length;
    u16 rssi;
    u8 bssid[6];
    u16 essidLength;
    u8 essid[0x20];
    u16 capaInfo;
    u8 unk_2e[0x92];
} ScanBss;


// A list of the access points found: a count and the entries
typedef struct {
    u32 len;       // 0x00, the length of the ESSID
    u8 essid[0x20];
    u32 unk_24;
    u8 bssid[6];   // 0x28
    u16 unk_2e;
} ApEntry;

typedef struct {
    u32 count;
    ApEntry entries[1];
} ApList;

#define GETU32(pt) (((u32)(pt)[0] << 24) ^ ((u32)(pt)[1] << 16) ^ ((u32)(pt)[2] << 8) ^ ((u32)(pt)[3]))
#define PUTU32(ct, st) { (ct)[0] = (u8)((st) >> 24); (ct)[1] = (u8)((st) >> 16); (ct)[2] = (u8)((st) >> 8); (ct)[3] = (u8)(st); }

BOOL APCi_MoveIdleState_atermset(void);
CPSInAddr func_ov009_0215ef48(void);
s32 AutoConfigThreadEx(void);
void SendNotify(void);
int rijndaelKeySetupEnc(u32 *rk, const u8 *cipherKey, int keyBits);
BOOL APC_SearchStart_atermset(const u8 *bssid, const u8 *essid, int essidLength, u32 option);
BOOL APC_Disconnect_atermset(void);
BOOL APC_End_atermset(void);
BOOL APC_Connect_atermset(const void *bssDesc, const void *wepDesc, u32 wepMode);
int APC_GetState(void);
void *soAlloc(u32 name, s32 size);
void soFree(u32 name, void *ptr, s32 size);
// The prebuilt's .data statics (atermset.o's DWARF): the element buffer's write pointer past the frame header, the
// debug switch, the thread's stack size, the deadline of the setup, the most access points listed, the socket
// configuration (192.168.0.176/24, gateway 192.168.0.200, DNS 192.168.0.32, in network order). MWCC sorts them with
// the .bss statics above and the AES tables in one list: this order of the definitions gives the prebuilt's layout,
// lpElementData, bDebug, gThreadStackSize, gAtermApTimeout, gMaxBssIdList, then AtermSOConfig
static s32 bDebug = 1;
static u8 *lpElementData = ElementData + 8;
static u32 gAtermApTimeout = 0xffffffff;
static s32 gMaxBssIdList = 64;
static u32 gThreadStackSize = 0x800;
static SOCConfig AtermSOConfig = {
    SOC_VENDOR_NINTENDO, SOC_VERSION, soAlloc, soFree, 0, { 0xb000a8c0 }, { 0x00ffffff }, { 0xc800a8c0 }, { 0x2000a8c0 },
    { 0 }, 0x1000, 0x1000, 0, 0, 0, NULL, NULL, NULL, "NINTENDO-DS", 4, 0, 0,
};
void AtermApc_InitEventBuf(void);
void AtermApc_callback(int event, int param);
s32 InitAtermApcNetwork(s32 maxap);
s32 FreeAtermApcNetwork(void);
void DisconnectAP(void);
s32 SendFrameViaInterface(s32 param_1, SOCSockAddrIn *param_2, u32 param_3, void *param_4, s32 param_5);
void SendFrame(s32 param_1, SOCSockAddrIn *param_2, void *param_3, s32 param_4);
u16 *GetElementData(u16 **param_1, u16 *param_2, u32 *param_3, u32 *param_4);
u32 SetFrameData(u8 *param_1, u32 param_2, u8 *param_3, u32 param_4, void *param_5);
u8 *SetElementData(u8 *lpb, s32 eid, void *pele, s32 esize);
u32 GetCommandElement(u16 *param_1, u32 param_2, void *param_3, u8 *param_4);
u32 SetSearchRes(u8 *param_1);
u32 GetAdapterMac(u8 *param_1);
BOOL AsciiToHex(u8 *dst, const s8 *src, s32 len);
u32 AutoConfigThread(void);
void aeskw_xor64(u8 *param_1, u8 *param_2, u8 *param_3);
int rijndaelKeySetupDec(u32 *rk, const u8 *cipherKey, int keyBits);
void rijndaelEncrypt(const u32 *rk, int Nr, const u8 *pt, u8 *ct);
void rijndaelDecrypt(const u32 *rk, int Nr, const u8 *ct, u8 *pt);
// RFC 1321's MD5 (md5.c in the prebuilt's object)
typedef struct {
    u32 state[4];
    u32 count[2]; // bits, modulo 2^64 (low word first)
    u8 buffer[64];
} MD5_CTX;

void MY_MD5Init(MD5_CTX *context);
void MY_MD5Update(MD5_CTX *context, u8 *input, u32 inputLen);
void MY_MD5Final(u8 *digest, MD5_CTX *context);
void Encode(u8 *output, u32 *input, u32 len);
void Decode(u32 *output, u8 *input, u32 len);
void MD5_memset(u8 *output, int value, u32 len);
static u8 PADDING[64] = {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// MD5Transform's constants and steps
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define FF(a, b, c, d, x, s, ac) { (a) += F((b), (c), (d)) + (x) + (u32)(ac); (a) = ROTATE_LEFT((a), (s)); (a) += (b); }
#define GG(a, b, c, d, x, s, ac) { (a) += G((b), (c), (d)) + (x) + (u32)(ac); (a) = ROTATE_LEFT((a), (s)); (a) += (b); }
#define HH(a, b, c, d, x, s, ac) { (a) += H((b), (c), (d)) + (x) + (u32)(ac); (a) = ROTATE_LEFT((a), (s)); (a) += (b); }
#define II(a, b, c, d, x, s, ac) { (a) += I((b), (c), (d)) + (x) + (u32)(ac); (a) = ROTATE_LEFT((a), (s)); (a) += (b); }
u64 GetTickCount(void);
void *Calloc(s32 param_1, s32 param_2);
s32 PutHex(s8 *arg0, s32 arg1);

s32 ATERMi_ApConfigGetState(ATERMApConfigState *state);



void MD5Encode(u8 *digest, u8 *input, u32 len);

void APCi_Notify_atermset(WCMNotice *notice) {
    if (notice == NULL) {
        return;
    }
    switch (notice->notify) {
    case WCM_NOTIFY_STARTUP:
        if (notice->result == 0) {
            if (apci_state == APC_STATE_TO_IDLE) {
                apci_state = APC_STATE_IDLE;
                if (apci_user_callback != NULL) {
                    apci_user_callback(DWC_WCM_EVENT_STARTED, 0);
                }
            } else if (apci_state == APC_STATE_TO_SEARCH) {
                if (func_ov009_02165144(apci_bssid_ptr, apci_essid_ptr, apci_search_option) != WCM_RESULT_ACCEPT) {
                    apci_state = APC_STATE_IDLE;
                    if (apci_user_callback != NULL) {
                        apci_user_callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (apci_state == APC_STATE_TO_CONNECT) {
                if (func_ov009_021652a4(apci_wmbssdesc, apci_Wep, apci_connect_option) != WCM_RESULT_ACCEPT) {
                    apci_state = APC_STATE_IDLE;
                    if (apci_user_callback != NULL) {
                        apci_user_callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            }
        } else {
            apci_state = APC_STATE_INIT;
            if (apci_user_callback != NULL) {
                apci_user_callback(DWC_WCM_EVENT_FAILED, 0);
            }
        }
        break;
    case WCM_NOTIFY_BEGIN_SEARCH:
        if (notice->result == 0) {
            if (apci_state == APC_STATE_TO_SEARCH) {
                apci_state = APC_STATE_SEARCH;
                if (apci_user_callback != NULL) {
                    apci_user_callback(DWC_WCM_EVENT_SEARCHING, 0);
                }
            }
        } else {
            apci_state = APC_STATE_IDLE;
            if (apci_user_callback != NULL) {
                apci_user_callback(DWC_WCM_EVENT_SEARCH_FAILED, 0);
            }
        }
        break;
    case WCM_NOTIFY_CONNECT:
        if (notice->result == 0) {
            if (apci_state == APC_STATE_TO_CONNECT) {
                apci_state = APC_STATE_CONNECT;
                if (apci_user_callback != NULL) {
                    apci_user_callback(DWC_WCM_EVENT_CONNECTED, 0);
                }
            }
        } else {
            apci_state = APC_STATE_IDLE;
            if (apci_user_callback != NULL) {
                apci_user_callback(DWC_WCM_EVENT_CONNECT_FAILED, 0);
            }
        }
        break;
    case WCM_NOTIFY_END_SEARCH:
        if (notice->result == 0) {
            if (apci_state == APC_STATE_TO_IDLE) {
                apci_state = APC_STATE_IDLE;
                if (apci_user_callback != NULL) {
                    apci_user_callback(DWC_WCM_EVENT_SEARCH_ENDED, 0);
                }
            } else if (apci_state == APC_STATE_TO_SEARCH) {
                if (func_ov009_02165144(apci_bssid_ptr, apci_essid_ptr, apci_search_option) != WCM_RESULT_ACCEPT) {
                    apci_state = APC_STATE_IDLE;
                    if (apci_user_callback != NULL) {
                        apci_user_callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (apci_state == APC_STATE_TO_INIT) {
                if (func_ov009_02165088() != WCM_RESULT_ACCEPT) {
                    apci_state = APC_STATE_IDLE;
                    if (apci_user_callback != NULL) {
                        apci_user_callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (apci_state == APC_STATE_TO_CONNECT) {
                if (func_ov009_021652a4(apci_wmbssdesc, apci_Wep, apci_connect_option) != WCM_RESULT_ACCEPT) {
                    apci_state = APC_STATE_IDLE;
                    if (apci_user_callback != NULL) {
                        apci_user_callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            }
        } else {
            apci_state = APC_STATE_IDLE;
            if (apci_user_callback != NULL) {
                apci_user_callback(DWC_WCM_EVENT_END_SEARCH_FAILED, 0);
            }
        }
        break;
    case WCM_NOTIFY_DISCONNECT:
        if (notice->result == 0) {
            if (apci_state == APC_STATE_TO_IDLE) {
                apci_state = APC_STATE_IDLE;
                if (apci_user_callback != NULL) {
                    apci_user_callback(DWC_WCM_EVENT_DISCONNECTED, 0);
                }
            } else if (apci_state == APC_STATE_TO_SEARCH) {
                if (func_ov009_02165144(apci_bssid_ptr, apci_essid_ptr, apci_search_option) != WCM_RESULT_ACCEPT) {
                    apci_state = APC_STATE_IDLE;
                    if (apci_user_callback != NULL) {
                        apci_user_callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (apci_state == APC_STATE_TO_INIT) {
                if (func_ov009_02165088() != WCM_RESULT_ACCEPT) {
                    apci_state = APC_STATE_IDLE;
                    if (apci_user_callback != NULL) {
                        apci_user_callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (apci_state == APC_STATE_TO_CONNECT) {
                if (func_ov009_021652a4(apci_wmbssdesc, apci_Wep, apci_connect_option) != WCM_RESULT_ACCEPT) {
                    apci_state = APC_STATE_IDLE;
                    if (apci_user_callback != NULL) {
                        apci_user_callback(DWC_WCM_EVENT_FAILED, 0);
                    }
                }
            } else if (apci_state == APC_STATE_CONNECT) {
                apci_state = APC_STATE_IDLE;
            }
        } else {
            apci_state = APC_STATE_IDLE;
            if (apci_user_callback != NULL) {
                apci_user_callback(DWC_WCM_EVENT_DISCONNECT_FAILED, 0);
            }
        }
        break;
    case WCM_NOTIFY_CLEANUP:
        if (notice->result == 0) {
            if (apci_state == APC_STATE_TO_INIT) {
                func_ov009_02164ee0();
                apci_state = APC_STATE_NONE;
                if (apci_user_callback != NULL) {
                    apci_user_callback(DWC_WCM_EVENT_CLEANED_UP, 0);
                }
            }
        } else {
            apci_state = APC_STATE_IDLE;
            if (apci_user_callback != NULL) {
                apci_user_callback(DWC_WCM_EVENT_FAILED, 0);
            }
        }
        break;
    case WCM_NOTIFY_FOUND_AP:
        if (apci_state == APC_STATE_SEARCH) {
            if (apci_user_callback != NULL) {
                apci_user_callback(DWC_WCM_EVENT_FOUND_AP, 0);
            }
        }
        break;
    case 8:
        if (apci_user_callback != NULL) {
            apci_user_callback(4, 0);
        }
        break;
    case 9:
        apci_state = APC_STATE_NONE;
        if (apci_user_callback != NULL) {
            apci_user_callback(3, 0);
        }
        break;
    case WCM_NOTIFY_COMMON:
    default:
        if (apci_user_callback != NULL) {
            apci_user_callback(DWC_WCM_EVENT_UNKNOWN, 0);
        }
        break;
    }
}


BOOL APCi_MoveIdleState_atermset(void) {
    switch (apci_state) {
    case APC_STATE_SEARCH:
        if (func_ov009_02165144(NULL, NULL, 0) != WCM_RESULT_ACCEPT) {
            return FALSE;
        }
        break;
    case APC_STATE_CONNECT:
        if (func_ov009_02165478() != WCM_RESULT_ACCEPT) {
            return FALSE;
        }
        break;
    case APC_STATE_INIT:
        if (func_ov009_02164f98(apci_IWConfig, APCi_Notify_atermset) != WCM_RESULT_ACCEPT) {
            return FALSE;
        }
        break;
    case APC_STATE_NONE:
    case APC_STATE_TO_INIT:
    case APC_STATE_IDLE:
    case APC_STATE_TO_IDLE:
    case APC_STATE_TO_SEARCH:
    case APC_STATE_TO_CONNECT:
    default:
        return FALSE;
    }
    return TRUE;
}


int APC_GetAPList_atermset(void *dest, int max) {
    u8 *out = dest;
    int count;
    int i;

    func_ov009_02165c28(TRUE);
    count = func_ov009_02165bd0();
    if (count > 0) {
        for (i = 0; i < count; i++) {
            if (i >= max) {
                break;
            }
            MIi_CpuCopy32(func_ov009_02165ca0(i), out, 0xc0);
            out += 0xc0;
        }
    }
    func_ov009_02165c28(FALSE);
    return count;
}

BOOL APC_SearchStart_atermset(const u8 *bssid, const u8 *essid, int essidLength, u32 option) {
    u32 irq = OS_DisableInterrupts();
    int i;

    apci_search_option = option;
    apci_bssid_ptr = apci_bssid_buf;
    if (bssid != NULL) {
        for (i = 0; i < 6; i++) {
            apci_bssid_buf[i] = *bssid++;
        }
    } else {
        MI_CpuFill8(apci_bssid_buf, 0xff, 6);
        apci_bssid_ptr = WCM_Bssid_Any;
    }
    apci_essid_ptr = apci_essid_buf;
    if (essid != NULL && essidLength > 0 && essidLength < 0x20) {
        for (i = 0; i < essidLength; i++) {
            apci_essid_buf[i] = *essid++;
        }
        for (; i < 0x20; i++) {
            apci_essid_buf[i] = 0;
        }
    } else {
        MI_CpuFill8(apci_essid_buf, 0xff, 0x20);
        apci_essid_ptr = WCM_Essid_Any;
    }
    if (apci_state == APC_STATE_IDLE) {
        if (func_ov009_02165144(apci_bssid_buf, apci_essid_ptr, apci_search_option) == WCM_RESULT_ACCEPT) {
            apci_state = APC_STATE_TO_SEARCH;
            OS_RestoreInterrupts(irq);
            return TRUE;
        }
    } else if (APCi_MoveIdleState_atermset() == TRUE) {
        apci_state = APC_STATE_TO_SEARCH;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}

BOOL APC_Disconnect_atermset(void) {
    u32 irq = OS_DisableInterrupts();

    if (apci_state == APC_STATE_CONNECT && func_ov009_02165478() == WCM_RESULT_ACCEPT) {
        apci_state = APC_STATE_TO_IDLE;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}

static inline s32 Ov016WcmGetState(void) {
    return apci_state;
}

static inline void Ov016WcmSetState(s32 state) {
    apci_state = state;
}

BOOL APC_End_atermset(void) {
    u32 irq = OS_DisableInterrupts();

    if (apci_state == APC_STATE_IDLE) {
        if (func_ov009_02165088() != WCM_RESULT_ACCEPT) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        apci_state = APC_STATE_TO_INIT;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    if (APCi_MoveIdleState_atermset() == TRUE) {
        apci_state = APC_STATE_TO_INIT;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}

BOOL APC_Connect_atermset(const void *bssDesc, const void *wepDesc, u32 wepMode) {
    u32 irq = OS_DisableInterrupts();

    apci_connect_option = wepMode;
    if (wepDesc != NULL) {
        MI_CpuCopy8(wepDesc, apci_Wep, 0x60);
    } else {
        MI_CpuFill8(apci_Wep, 0, 0x60);
    }
    MIi_CpuCopy32(bssDesc, apci_wmbssdesc, 0xc0);
    if (APCi_MoveIdleState_atermset() == TRUE) {
        apci_state = APC_STATE_TO_CONNECT;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    if (apci_state == APC_STATE_IDLE &&
        func_ov009_021652a4(apci_wmbssdesc, apci_Wep, apci_connect_option) == WCM_RESULT_ACCEPT) {
        apci_state = APC_STATE_TO_CONNECT;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}

BOOL APC_Init_atermset(DWCWcmCallback callback, void *work, u32 size) {
    u32 irq = OS_DisableInterrupts();
    u8 *buf = work;

    apci_Wep = buf;
    apci_IWConfig = (WCMConfig *)(((u32)buf + 0x63) & ~3);
    apci_IWBuffer = (u8 *)(((u32)apci_IWConfig + sizeof(WCMConfig) + 0x1f) & ~0x1f);
    apci_wmbssdesc = (WMBssDesc *)(((u32)apci_IWBuffer + WCM_WORK_SIZE + 0x1f) & ~0x1f);
    apci_IWConfig->pbdbuffer = (void *)(((u32)apci_wmbssdesc + 0xc0 + 0x1f) & ~0x1f);
    apci_IWConfig->nbdbuffer = (s32)(buf + size) - (s32)apci_IWConfig->pbdbuffer;
    apci_IWConfig->nbdmode = 0;
    apci_IWConfig->dmano = 3;
    apci_user_callback = callback;
    if (apci_state == APC_STATE_NONE) {
        if (func_ov009_02164d64(apci_IWBuffer, WCM_WORK_SIZE) != 0) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        apci_state = APC_STATE_INIT;
    }
    if (apci_state == APC_STATE_INIT) {
        if (func_ov009_02164f98(apci_IWConfig, APCi_Notify_atermset) != WCM_RESULT_ACCEPT) {
            OS_RestoreInterrupts(irq);
            return FALSE;
        }
        apci_state = APC_STATE_TO_IDLE;
        OS_RestoreInterrupts(irq);
        return TRUE;
    }
    OS_RestoreInterrupts(irq);
    return FALSE;
}

int APC_GetState(void) {
    return apci_state;
}

void *soAlloc(u32 name, s32 size) {
    void *ptr;

    if (0 < size) {
        OS_LockMutex(&AllocLock);
        ptr = gAtermAlloc(size);
        OS_UnlockMutex(&AllocLock);
        return ptr;
    }
    return NULL;
}

void soFree(u32 name, void *ptr, s32 size) {
    if ((ptr != NULL) && (0 < size)) {
        OS_LockMutex(&AllocLock);
        gAtermFree(ptr);
        OS_UnlockMutex(&AllocLock);
    }
}

void AtermApc_InitEventBuf(void) {
    u32 mask;
    s32 i;
    u32 *ptr;

    mask = OS_DisableInterrupts();
    i = 0;
    event_set_ptr = 0;
    event_get_ptr = 0;
    ptr = (u32 *)event_buf;
    do {
        i = i + 1;
        *ptr = 0;
        ptr = ptr + 1;
    } while (i < 4);
    OS_RestoreInterrupts(mask);
}

u32 AtermApc_GetEvent(void) {
    OSIntrMode intr = OS_DisableInterrupts();
    u32 ret;
    s32 tail;

    tail = event_get_ptr;
    if (event_set_ptr == tail) {
        ret = 0;
    } else {
        ret = event_buf[tail];
        tail++;
        event_get_ptr = tail;
        if (tail >= 4) {
            event_get_ptr = 0;
        }
    }
    OS_RestoreInterrupts(intr);
    return ret;
}

void AtermApc_callback(int event, int param) {
    s32 val;

    val = event_set_ptr + 1;
    if ((val != event_get_ptr) && (event_set_ptr != event_get_ptr + 3)) {
        event_buf[event_set_ptr] = event;
        event_set_ptr = val;
        if (val >= 4) {
            event_set_ptr = 0;
        }
    }
}

void alarmCallback_atermset(s32 event) {
    AtermApc_callback(event, 0);
}

s32 InitAtermApcNetwork(s32 maxap)
{
    s32 loop_flag = 1;
    s32 retv;
    s32 wcmSize;
    s32 size;

    gMaxApList = maxap;
    AtermApc_InitEventBuf();
    wcmSize = maxap * 0xd0;
    atermapc_work = gAtermAlloc(wcmSize + 0x24d0 + maxap * 0xc0);
    if (atermapc_work == NULL) {
        return -1;
    }
    apc_work = (u8 *)(((u64)atermapc_work + 0x1f) & ~0x1f);
    size = wcmSize + 0x2490;
    ap_list = (WMBssDesc *)(((u64)apc_work + size + 0x1f) & ~0x1f);
    if (!APC_Init_atermset(AtermApc_callback, (void *)apc_work, size)) {
        return -2;
    }
    do {
        s32 event;

        OS_Sleep(10);
        while ((event = AtermApc_GetEvent()) != 0) {
            switch (event) {
            case 4:
            case 5:
                break;
            case 6:
                loop_flag = 0;
                retv = 1;
                break;
            default:
                loop_flag = 0;
                retv = -2;
                break;
            }
        }
    } while (loop_flag);
    return retv;
}

s32 FreeAtermApcNetwork(void) {
    s32 loop_flag = TRUE;
    u32 ev;

    if (APC_End_atermset()) {
        do {
            OS_Sleep(10);
            while ((ev = AtermApc_GetEvent()) != 0) {
                switch (ev) {
                case 4:
                case 5:
                    break;
                case 20:
                    loop_flag = FALSE;
                    break;
                default:
                    loop_flag = FALSE;
                    break;
                }
            }
        } while (loop_flag);
    }
    if (atermapc_work != NULL) {
        gAtermFree(atermapc_work);
        atermapc_work = NULL;
    }
    return 1;
}

s32 ConnectAP(void) {
    BOOL running = TRUE;
    s32 result = -2;
    u8 *bssDesc = (u8 *)&ap_list[gAPId];
    OSAlarm alarm;
    int event;

    if (bssDesc == NULL) {
        return 0;
    }
    if (!APC_Connect_atermset(bssDesc, NULL, 0x30000)) {
        return -2;
    }
    OS_CreateAlarm(&alarm);
    OS_SetAlarm(&alarm, 0x3fec42, (OSAlarmHandler)alarmCallback_atermset, (void *)0x12);
    do {
        if ((u32)GetTickCount() >= gAtermApTimeout) {
            result = -3;
            break;
        }
        if (bAtermApCanceled != 0) {
            result = -8;
            break;
        }
        OS_Sleep(10);
        while ((event = AtermApc_GetEvent()) != 0) {
            switch (event) {
            case 4:
            case 5:
            case 0x12:
            case 0x13:
                break;
            case 0xc:
                running = FALSE;
                result = 1;
                break;
            case 0xd:
                if (bAtermApCanceled != 0) {
                    running = FALSE;
                    result = -8;
                } else if (!APC_Connect_atermset(bssDesc, NULL, 0x30000)) {
                    return result;
                }
                break;
            default:
                running = FALSE;
                break;
            }
        }
    } while (running);
    OS_CancelAlarm(&alarm);
    while (AtermApc_GetEvent() != 0) {
    }
    if (result > 0) {
        bApConnect = 1;
        if (func_ov009_0215f8fc(&AtermSOConfig) < 0) {
            result = -2;
        } else {
            bSO_Startup = 1;
        }
    }
    return result;
}


void DisconnectAP(void) {
    s32 flag;
    u32 event;

    flag = 1;
    if (bApConnect != 0) {
        if (APC_Disconnect_atermset() != 0) {
            do {
                OS_Sleep(0xAU);
                event = AtermApc_GetEvent();
                if (event != 0) {
                    do {
                        switch (event) {
                        case 4:
                        case 5:
                            break;
                        case 14:
                            flag = 0;
                            break;
                        default:
                            flag = 0;
                            break;
                        }
                        event = AtermApc_GetEvent();
                    } while (event != 0);
                }
            } while (flag != 0);
        }
        bApConnect = 0;
    }
    if (bSO_Startup != 0) {
        bSO_Startup = 0;
        func_ov009_0215fa90();
    }
}

BOOL CheckAccessPoint(ApList *param_1, ApList *param_2, u32 *param_3) {
    BOOL found = FALSE;
    BOOL ret = FALSE;
    BOOL hasA;
    BOOL hasB;
    u32 n;
    u32 i;

    {
        ApEntry *pa = param_1->entries;
        ApEntry *pb = param_2->entries;
        u32 j;

        i = found;
        for (; i < param_1->count; pa++, pb = param_2->entries, i++) {
            char name1[0x22] = {0};

            memcpy(name1, pa->essid, 0x20);
            name1[pa->len] = 0;
            n = param_2->count;
            for (j = 0; j < n; j++, pb++) {
                if (pa->len == 0 || pa->len > 0x20 || (pa->len == 1 && (pa->essid[0] == 0 || pa->essid[0] == 0x20))) {
                    break;
                }
                if (memcmp(name1, pb->essid, strlen(name1)) == 0
                    && memcmp(pa->bssid, pb->bssid, 6) == 0
                    && pa->unk_2e != pb->unk_2e && pa->unk_2e == 0) {
                    found = TRUE;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
    }
    if (!found) {
        char name2[0x22] = {0};
        ApEntry *pa;
        ApEntry *pb;
        u32 j;

        hasB = hasA = FALSE;
        pa = param_1->entries;
        j = hasA;
        pb = param_2->entries;
        for (; j < param_2->count; j++, pb++) {
            memcpy(name2, pb->essid, 0x20);
            name2[pb->len] = 0;
            if (memcmp(name2, gszStealth, strlen(gszStealth)) == 0 && pb->unk_2e == 0) {
                hasB = TRUE;
                break;
            }
        }
        i = 0;
        for (; i < param_1->count; i++, pa++) {
            memcpy(name2, pa->essid, 0x20);
            name2[pa->len] = 0;
            if (strlen(name2) == strlen(gszStealth)) {
                if (memcmp(name2, gszStealth, strlen(gszStealth)) == 0 && pa->unk_2e == 0) {
                    hasA = TRUE;
                    break;
                }
            }
        }
        if (hasA && !hasB) {
            found = TRUE;
        }
    }
    if (found) {
        *param_3 = i;
        ret = TRUE;
    }
    return ret;
}

s32 ScanAP(void) {
    s32 retv = -1;
    s32 retry;
    s32 i, n;
    u32 ApNo = 0;
    u32 BssListSize;
    ApList *pBssList;
    ApList *pOldList = NULL;
    ApEntry *pBssid;
    s8 apBSSID[32];
    s32 loop_flag;
    OSAlarm alarm;
    s32 ap_count;
    s32 nfound;
    ScanBss *wmbssdesc;

    BssListSize = sizeof(ApEntry) * gMaxApList;
    BssListSize += sizeof(ApList);
    pBssList = Calloc(1, BssListSize);
    if (pBssList != NULL) {
        pOldList = Calloc(1, BssListSize);
        if (pOldList != NULL) {
            for (retry = 0; retry < 30 && bAtermApCanceled == 0; retry++) {
                if ((u32)GetTickCount() >= gAtermApTimeout) {
                    break;
                }
                if (!APC_SearchStart_atermset(NULL, NULL, 0, 0x30bffe)) {
                    retv = -2;
                    goto end;
                }
                OS_CreateAlarm(&alarm);
                OS_SetAlarm(&alarm, 0xffb10, (OSAlarmHandler)alarmCallback_atermset, (void *)0x13);
                loop_flag = 1;
                ap_count = 0;
                do {
                    OS_Sleep(10);
                    if ((u32)GetTickCount() >= gAtermApTimeout || bAtermApCanceled != 0) {
                        break;
                    }
                    while ((nfound = AtermApc_GetEvent()) != 0) {
                        switch (nfound) {
                        case 4:
                        case 8:
                        case 0x12:
                            break;
                        case 0x13:
                            loop_flag = 0;
                            break;
                        case 5:
                            n = APC_GetAPList_atermset(ap_list, gMaxApList);
                            if (n > ap_count) {
                                ap_count = n;
                                OS_CancelAlarm(&alarm);
                                OS_SetAlarm(&alarm, 0xffb10, (OSAlarmHandler)alarmCallback_atermset, (void *)0x13);
                            }
                            break;
                        case 10:
                            loop_flag = 0;
                            break;
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        case 6:
                        case 7:
                        case 9:
                        case 11:
                        case 12:
                        case 13:
                        case 14:
                        case 15:
                        case 16:
                        case 17:
                        default:
                            loop_flag = 0;
                            break;
                        }
                    }
                } while (loop_flag);
                OS_CancelAlarm(&alarm);
                while (AtermApc_GetEvent() != 0) {
                }
                if (bAtermApCanceled != 0) {
                    break;
                }
                n = 0;
                if (ap_count >= gMaxApList) {
                    retv = -6;
                    goto end;
                }
                wmbssdesc = (ScanBss *)ap_list;
                i = 0;
                if (ap_count > 0) {
                    for (; i < ap_count; i++) {
                        memcpy(pBssList->entries[n].essid, wmbssdesc->essid, 0x20);
                        pBssList->entries[n].len = wmbssdesc->essidLength;
                        pBssList->entries[n].essid[wmbssdesc->essidLength] = 0;
                        if (wmbssdesc->capaInfo & 0x10) {
                            pBssList->entries[n].unk_2e = 1;
                        } else {
                            pBssList->entries[n].unk_2e = 0;
                        }
                        memcpy(pBssList->entries[n].bssid, wmbssdesc->bssid, 6);
                        wmbssdesc++;
                        n++;
                    }
                }
                pBssList->count = ap_count;
                if (gAtermApState != 1 && CheckAccessPoint(pBssList, pOldList, &ApNo)) {
                    pBssid = &pBssList->entries[ApNo];
                    gAPId = ApNo;
                    strcpy((char *)gAPSSID, (char *)pBssid->essid);
                    memcpy(gAPMac, pBssid->bssid, 6);
                    PutMac((char *)apBSSID, (s8 *)gAPMac);
                    break;
                }
                memcpy(pOldList, pBssList, BssListSize);
                gAtermApState = 2;
                SendNotify();
            }
            if (retry >= 30 || (u32)GetTickCount() > gAtermApTimeout) {
                retv = -3;
            } else if (bAtermApCanceled != 0) {
                retv = -8;
            } else {
                retv = 1;
            }
        }
    }
end:
    if (pBssList != NULL) {
        Free_atermset(pBssList);
    }
    if (pOldList != NULL) {
        Free_atermset(pOldList);
    }
    return retv;
}



s32 SendFrameViaInterface(s32 param_1, SOCSockAddrIn *param_2, u32 param_3, void *param_4, s32 param_5) {
    s32 param_5_new;
    param_5_new = func_ov009_0215f5b4(param_1, param_4, param_5, 0, param_2);
    if (param_5_new < 0) {
        param_5_new = -4;
        return param_5_new;
    }
}

void SendBroadcast(s32 sock, void *lpb, s32 size) {
    SOCSockAddrIn sp8;
    u8 sp4;

    *(s8 *)((u8 *)(&sp4) + 4) = 8;
    *(s8 *)((u8 *)(&sp4) + 5) = 2;
    sp8.addr.addr = -1U;
    *(s16 *)((u8 *)(&sp4) + 6) = 0x1E6;
    func_ov009_0215fbac(func_ov009_0215ef48(), &sp4);
    SendFrameViaInterface(sock, &sp8, (u32)(&sp4), lpb, size);
}

void SendFrame(s32 param_1, SOCSockAddrIn *param_2, void *param_3, s32 param_4) {
    SendBroadcast(param_1, param_3, param_4);
}

u16 *GetElementData(u16 **lplpb, u16 *lplimit, u32 *pid, u32 *psize) {
    u8 *lpb;
    struct _ELEMENTINFO {
        u16 ElementID;
        u16 ElementLength;
    } *pele;

    lpb = (u8 *)*lplpb;
    if (lpb >= (u8 *)lplimit) {
        return NULL;
    }
    pele = (struct _ELEMENTINFO *)lpb;
    *pid = (u16)(((pele->ElementID >> 8) & 0xff) | ((pele->ElementID << 8) & 0xff00));
    *psize = (u16)(((pele->ElementLength >> 8) & 0xff) | ((pele->ElementLength << 8) & 0xff00));
    lpb += sizeof(struct _ELEMENTINFO);
    *lplpb = (u16 *)(lpb + (((*psize + 0xb) & ~7) - 4));
    return (u16 *)lpb;
}

u16 *GetFirstElement(u16 *lpelement, u32 *pid, u32 *psize) {
    u16 *cur;
    u16 v;

    cur = lpelement + 4;
    v = *lpelement;
    return GetElementData(&cur, (u16 *)((u8 *)cur + (u16)(((v >> 8) & 0xff) | ((v << 8) & 0xff00))), pid, psize);
}


u8 *SetElementData(u8 *lpb, s32 eid, void *pele, s32 esize) {
    u16 *pei = (u16 *)lpb;
    s32 size;
    u16 v;

    lpb[0] = 0;
    lpb[1] = 0;
    lpb[2] = 0;
    lpb[3] = 0;
    v = eid;
    pei[0] = (u16)(((v >> 8) & 0xff) | ((v << 8) & 0xff00));
    size = ((esize + 0xb) & ~7) - 4;
    v = (u16)esize;
    pei[1] = (u16)(((v >> 8) & 0xff) | ((v << 8) & 0xff00));
    lpb = (u8 *)(pei + 2);
    memset(lpb, 0, size);
    memcpy(lpb, pele, esize);
    return lpb + size;
}

int SetSingleElement(u8 *out, int type, const void *data, int length) {
    u8 *end;

    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    out[4] = 0;
    out[5] = 0;
    out[6] = 0;
    out[7] = 0;
    end = SetElementData(out + 8, type, data, length);
    *(u16 *)out = end - out - 8;
    return end - out;
}

#define SWAP16(value) ((u16)((((value) >> 8) & 0xff) | (((value) << 8) & 0xff00)))
int IsSearchCommand(const u8 *packet, int *secure) {
    u32 type;
    u32 length;
    u32 itemType;
    u32 itemLength;
    const u8 *end;
    const u8 *message = GetFrameData(packet, &type, &length);
    const u8 *cursor = message;
    const u8 *data;
    u32 version = 0;
    u32 mode = 0;
    int count = 0;

    if (message == NULL) {
        return 0;
    }
    if (type != 1) {
        return 0;
    }
    end = message + length;
    cursor = message + 8;
    while ((data = GetElementData(&cursor, end, &itemType, &itemLength)) != NULL) {
        switch (itemType) {
        case 1:
            version = SWAP16(*(u16 *)data);
            break;
        case 2:
            mode = SWAP16(*(u16 *)data);
            break;
        case 5:
            count = SWAP16(*(u16 *)data);
            break;
        }
    }
    if (version != 1 || mode != 1) {
        return 0;
    }
    if (count >= 1) {
        *secure = 1;
    } else {
        *secure = 0;
    }
    return 1;
}



u16 *GetFrameData(u16 *param_1, u32 *param_2, u32 *param_3)
{
    const u8 *packet = (const u8 *)param_1;
    const u8 *end;
    const u8 *p;
    u32 sum = 0;

    *param_2 = SWAP16(*(u16 *)packet);
    *param_3 = SWAP16(*(u16 *)(packet + 2));
    end = packet + 6 + *param_3;
    for (p = packet; p < end; p++) {
        sum += *p;
    }
    if ((sum & 0xffff) != SWAP16(*(u16 *)end)) {
        return NULL;
    } else {
        return (u16 *)((u8 *)param_1 + 6);
    }
}

u32 SetFrameData(u8 *param_1, u32 param_2, u8 *param_3, u32 param_4, void *param_5) {
    u8 *p = param_1;
    u32 sum = 0;
    u8 *q;

    param_3[0] = 0;
    param_3[1] = 0;
    param_3[2] = 0;
    param_3[3] = 0;
    param_3[4] = 0;
    param_3[5] = 0;
    param_3[6] = 0;
    param_3[7] = 0;
    *(u16 *)param_3 = SWAP16((u16)(param_4 - 8));
    if (param_5 != NULL) {
        keywrap_encrypt(param_1 + 6, param_3, param_4, param_5, 0x10);
        param_4 += 8;
    } else {
        memcpy(param_1 + 6, param_3, param_4);
    }
    param_1[0] = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    param_1[3] = 0;
    param_1[4] = 0;
    param_1[5] = 0;
    *(u16 *)param_1 = SWAP16((u16)param_2);
    *(u16 *)(param_1 + 2) = SWAP16((u16)param_4);
    p += 6;
    p += param_4;
    for (q = param_1; q < p; q++) {
        sum += *q;
    }
    *(u16 *)p = SWAP16((u16)sum);
    return p + 2 - param_1;
}

u32 GetCommandElement(u16 *lpframe, u32 cmdid, void *lpelement, u8 *lpkey) {
    u16 *src;
    u32 uStack_18;
    u32 uStack_1c;

    src = GetFrameData(lpframe, &uStack_18, &uStack_1c);
    if (src == NULL) {
        return 0;
    }
    if (uStack_18 != cmdid) {
        return 0;
    }
    if (lpkey != NULL) {
        keywrap_decrypt(lpelement, (u8 *)src, uStack_1c, lpkey, 0x10);
        uStack_1c = uStack_1c - 8;
    } else {
        memcpy(lpelement, src, uStack_1c);
    }
    return uStack_1c;
}

u32 SetSearchRes(u8 *lpframe) {
    u8 *lpb;
    u16 wd = 0x100;
    u8 SupportEnc[7] = { 6, 0, 1, 2, 3, 4, 5 };

    lpb = SetElementData(lpElementData, 1, &wd, 2);
    lpb = SetElementData(lpb, 2, &wd, 2);
    if (bDualCfg != 0) {
        lpb = SetElementData(lpb, 5, &wd, 2);
    }
    lpb = SetElementData(lpb, 3, SupportEnc, 7);
    if (bDualCfg != 0) {
        lpb = SetElementData(lpb, 4, btAdapterMAC, 6);
    }
    return SetFrameData(lpframe, 2, ElementData, lpb - lpElementData + 8, NULL);
}

u32 GetAdapterMac(u8 *lpmac) {
    OS_GetMacAddress(lpmac);
    return 1;
}

BOOL GetMacEncKey(u8 *out, SOCSockAddrIn *target) {
    u8 ap_mac[6];
    u8 sta_mac[6];
    s32 ret;

    memcpy(out + 12, "WARP", 4);
    memcpy(ap_mac, gAPMac, 6);
    ap_mac[0] &= 0xfd;
    GetAdapterMac(sta_mac);
    memcpy(btAdapterMAC, sta_mac, 6);
    ret = memcmp(ap_mac, sta_mac, 6);
    if (ret <= 0) {
        memcpy(out, sta_mac, 6);
        memcpy(out + 6, ap_mac, 6);
    } else {
        memcpy(out, ap_mac, 6);
        memcpy(out + 6, sta_mac, 6);
    }
    if (bDebug != 0) {
        s8 strmac[32];
        s8 strap[32];

        PutMac((char *)strmac, (s8 *)sta_mac);
        PutMac((char *)strap, (s8 *)ap_mac);
    }
    return TRUE;
}



BOOL GetWLanSetElement(u16 *lpelement) {
    u8 *lpb = (u8 *)lpelement + 8;
    u8 *lpe;
    u8 *lplimit;
    u32 eid;
    u32 esize;
    s32 i;
    BOOL retv = FALSE;
    u16 value;

    lplimit = lpb + SWAP16(*lpelement);
    while ((lpe = (u8 *)GetElementData((u16 **)&lpb, (u16 *)lplimit, &eid, &esize)) != NULL) {
        switch (eid) {
        case 0x201:
            memset(AutoProfile.Ssid, 0, 0x20);
            memcpy(AutoProfile.Ssid, lpe, esize);
            retv = TRUE;
            break;
        case 0x202:
            AutoProfile.EncryptData.EncryptMode = SWAP16(*(u16 *)lpe);
            break;
        case 0x203:
            value = SWAP16(*(u16 *)lpe);
            for (i = 0; i < 4; i++) {
                AutoProfile.EncryptData.SharedKey[i].KeyType = value;
            }
            break;
        case 0x204:
            value = SWAP16(*(u16 *)lpe);
            for (i = 0; i < 4; i++) {
                AutoProfile.EncryptData.SharedKey[i].KeyLength = value;
            }
            break;
        case 0x205:
            AutoProfile.EncryptData.DefaultKey = SWAP16(*(u16 *)lpe);
            break;
        case 0x206:
        case 0x207:
        case 0x208:
        case 0x209:
            memset(AutoProfile.EncryptData.SharedKey[eid - 0x206].KeyData, 0, 0x20);
            if (AutoProfile.EncryptData.SharedKey[0].KeyType == 1) {
                s8 *lpw = (s8 *)AutoProfile.EncryptData.SharedKey[eid - 0x206].KeyData;

                for (i = 0; i < (s32)esize; i++) {
                    lpw += PutHex(lpw, (s8)*lpe++);
                }
            } else {
                memcpy(AutoProfile.EncryptData.SharedKey[eid - 0x206].KeyData, lpe, esize);
            }
            break;
        case 0x20a:
            memset(AutoProfile.EncryptData.Passphrase, 0, 0x48);
            memcpy(AutoProfile.EncryptData.Passphrase, lpe, esize);
            break;
        }
    }
    return retv;
}



BOOL AsciiToHex(u8 *byDest, const s8 *szStr, s32 nLen) {
    s32 ch;
    s32 b = 0;
    s32 i;

    for (i = 0; i < nLen; i++) {
        ch = szStr[i];
        switch (ch) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            b += ch - '0';
            break;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            b += ch - 'a' + 10;
            break;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            b += ch - 'A' + 10;
            break;
        default:
            return FALSE;
        }
        if (i % 2 == 0) {
            b <<= 4;
        } else {
            byDest[i / 2] = (u8)b;
            b = 0;
        }
    }
    return TRUE;
}


s32 StoreNetParam(void) {
    s32 ret;
    s8 *lpwep;
    s32 defkey;
    s32 i;
    s8 szwep[36];

    ret = 1;
    strcpy(gAtermApConfigParam.ssid, AutoProfile.Ssid);
    switch (AutoProfile.EncryptData.EncryptMode) {
    case 0:
        gAtermApConfigParam.auth_mode = 0;
        break;
    case 1:
        defkey = AutoProfile.EncryptData.DefaultKey;
        if (defkey == 0) {
            ret = -7;
        } else {
            gAtermApConfigParam.wepkeyid = defkey;
            for (i = 0; i < 4; i++) {
                lpwep = szwep;
                memcpy(lpwep, AutoProfile.EncryptData.SharedKey[i].KeyData, 0x20);
                szwep[0x20] = 0;
                switch (strlen(lpwep)) {
                case 0:
                    break;
                case 5:
                    gAtermApConfigParam.auth_mode = 1;
                    memcpy(gAtermApConfigParam.wepkey[i], lpwep, 5);
                    break;
                case 10:
                    gAtermApConfigParam.auth_mode = 1;
                    AsciiToHex(gAtermApConfigParam.wepkey[i], lpwep, 10);
                    break;
                case 13:
                    gAtermApConfigParam.auth_mode = 2;
                    memcpy(gAtermApConfigParam.wepkey[i], lpwep, 13);
                    break;
                case 26:
                    gAtermApConfigParam.auth_mode = 2;
                    AsciiToHex(gAtermApConfigParam.wepkey[i], lpwep, 26);
                    break;
                case 16:
                    gAtermApConfigParam.auth_mode = 3;
                    memcpy(gAtermApConfigParam.wepkey[i], lpwep, 16);
                    break;
                case 32:
                    gAtermApConfigParam.auth_mode = 3;
                    AsciiToHex(gAtermApConfigParam.wepkey[i], lpwep, 32);
                    break;
                default:
                    ret = -7;
                    break;
                }
            }
        }
        break;
    case 2:
        gAtermApConfigParam.auth_mode = 4;
        memcpy(gAtermApConfigParam.wpakey, AutoProfile.EncryptData.Passphrase, 0x40);
        break;
    case 3:
        gAtermApConfigParam.auth_mode = 5;
        memcpy(gAtermApConfigParam.wpakey, AutoProfile.EncryptData.Passphrase, 0x40);
        break;
    default:
        ret = -7;
        break;
    }
    return ret;
}

typedef struct { u8 b[8]; } SessionKeyHalf;
static inline s32 UtilAc_TakeSessionKey(u16 *message, s32 retries) {
    u32 now;
    u32 length;
    u32 type;
    u8 *data = (u8 *)GetFirstElement(message, &type, &length);

    if (type == 0x101) {
        now = (u32)GetTickCount();
        *(SessionKeyHalf *)aKeySource = *(SessionKeyHalf *)data;
        MD5Encode(aKeySource + 8, (u8 *)&now, 4);
        retries = 0;
        AutoPhase = P_SENDENCINFORES;
        gAtermApState = 5;
        gAtermApTimeout = 0xffffffff;
        SendNotify();
    }
    return retries;
}
s32 AutoConfigThreadEx(void) {
    s32 sock;
    s32 ret;
    SOCSockAddrIn server;
    SOCSockAddrIn target;
    u32 n;
    u32 dwSendTick;
    s32 iretry;
    BOOL bfinish;

    sock = 0;
    ret = -5;
    dwSendTick = 0;
    iretry = 0;
    bfinish = FALSE;
    AutoPhase = P_SCANAP;
    while (!bfinish && bAtermApCanceled == 0) {
        OS_Sleep(500);
        switch (AutoPhase) {
        case P_SCANAP:
            ret = ScanAP();
            if (ret != 1) {
                bfinish = TRUE;
            } else {
                gAtermApState = 3;
                SendNotify();
                AutoPhase = P_CONNECTAP;
            }
            break;
        case P_CONNECTAP:
            ret = ConnectAP();
            if (ret != 1) {
                bfinish = TRUE;
            } else {
                AutoPhase = P_INITSOCK;
            }
            break;
        case P_INITSOCK:
            sock = func_ov009_0215f428(2, 2, 0);
            if (sock < 0) {
                ret = -2;
                bfinish = TRUE;
            } else {
                memset(&server, 0, sizeof(SOCSockAddrIn));
                server.len = 8;
                server.family = 2;
                server.port = 0x1e6;
                server.addr.addr = 0;
                ret = func_ov009_0215f454(sock, &server);
                if (ret < 0) {
                    ret = -2;
                    bfinish = TRUE;
                } else {
                    AutoPhase = P_WAITSEARCH;
                }
            }
            break;
        case P_WAITSEARCH:
            if ((u32)GetTickCount() >= gAtermApTimeout) {
                func_ov009_0215f644(sock);
                ret = -3;
                bfinish = TRUE;
            } else {
                target.len = 8;
                GetMacEncKey(aMacEncKey, &target);
                if (func_ov009_0215f500(sock, FrameData, 0x800, 4, &target) > 0 && IsSearchCommand(FrameData, &bDualCfg)) {
                    gAtermApTimeout = (u32)GetTickCount() + 30000;
                    AutoPhase = P_SENDSEARCHRES;
                    gAtermApState = 4;
                    SendNotify();
                }
            }
            break;
        case P_SENDSEARCHRES:
            cbFrameData = SetSearchRes(FrameData);
            SendFrame(sock, &target, FrameData, cbFrameData);
            dwSendTick = (u32)GetTickCount();
            AutoPhase = P_WAITENCINFO;
            break;
        case P_WAITENCINFO:
            if ((u32)GetTickCount() >= gAtermApTimeout) {
                func_ov009_0215f644(sock);
                ret = -4;
                bfinish = TRUE;
            } else if (func_ov009_0215f500(sock, FrameData, 0x800, 4, &target) > 0 &&
                       GetCommandElement((u16 *)FrameData, 3, ElementData, aMacEncKey)) {
                iretry = UtilAc_TakeSessionKey((u16 *)ElementData, iretry);
            } else {
                if ((u32)GetTickCount() >= dwSendTick + 1000) {
                    AutoPhase = P_SENDSEARCHRES;
                }
            }
            break;
        case P_SENDENCINFORES:
            cbElementData = SetSingleElement(ElementData, 0x102, aKeySource + 8, 8);
            cbFrameData = SetFrameData(FrameData, 4, ElementData, cbElementData, aMacEncKey);
            SendFrame(sock, &target, FrameData, cbFrameData);
            dwSendTick = (u32)GetTickCount();
            memset(&AutoProfile, 0, sizeof(WS_11G_PROFILE_DATA));
            AutoPhase = P_WAITWLANSET;
            break;
        case P_WAITWLANSET:
            if (func_ov009_0215f500(sock, FrameData, 0x800, 4, &target) > 0 &&
                (cbElementData = GetCommandElement((u16 *)FrameData, 5, ElementData, aKeySource)) != 0 &&
                GetWLanSetElement((u16 *)ElementData)) {
                if (AutoProfile.Ssid[0] != 0) {
                    btAutoResult = TRUE;
                } else {
                    btAutoResult = FALSE;
                }
                iretry = 0;
                AutoPhase = P_SENDWLANSETRES;
            } else if ((u32)GetTickCount() >= dwSendTick + 1000) {
                if (++iretry >= 10) {
                    func_ov009_0215f644(sock);
                    ret = -2;
                    bfinish = TRUE;
                } else {
                    AutoPhase = P_SENDENCINFORES;
                }
            }
            break;
        case P_SENDWLANSETRES:
            cbElementData = SetSingleElement(ElementData, 0x301, &btAutoResult, 1);
            cbFrameData = SetFrameData(FrameData, 6, ElementData, cbElementData, aKeySource);
            if (APC_GetState() != 7) {
                dwSendTick = (u32)GetTickCount() + 1000;
                iretry = 10;
                AutoPhase = P_SESSIONEND;
            } else {
                SendFrame(sock, &target, FrameData, cbFrameData);
                dwSendTick = (u32)GetTickCount();
                AutoPhase = P_SESSIONEND;
            }
            break;
        case P_SESSIONEND:
            if ((u32)GetTickCount() >= dwSendTick + 1000) {
                if (++iretry >= 10) {
                    bfinish = TRUE;
                    ret = StoreNetParam();
                } else {
                    AutoPhase = P_SENDWLANSETRES;
                }
            }
            break;
        }
    }
    if (sock != 0) {
        func_ov009_0215f644(sock);
    }
    if (bAtermApCanceled != 0) {
        ret = -8;
    }
    return ret;
}



u32 AutoConfigThread(void) {
    u32 result;

    result = AutoConfigThreadEx();
    gAtermApResult = result;
    DisconnectAP();
    if (result == 1) {
        gAtermApState = 6;
    }
    else {
        gAtermApState = 7;
    }
    gAtermApTimeout = 0xffffffff;
    SendNotify();
}

#define BSWAP64(value) ((((value) & 0x00000000000000ffULL) << 56) | (((value) & 0x000000000000ff00ULL) << 40) | (((value) & 0x0000000000ff0000ULL) << 24) | (((value) & 0x00000000ff000000ULL) << 8) | (((value) & 0x000000ff00000000ULL) >> 8) | (((value) & 0x0000ff0000000000ULL) >> 24) | (((value) & 0x00ff000000000000ULL) >> 40) | (((value) & 0xff00000000000000ULL) >> 56))
int keywrap_encrypt(u8 *out, const u8 *in, u32 length, const u8 *key, u32 keyLength) {
    u64 t;
    u32 iv[2];
    u8 block[16];
    u32 rk[80];
    int n;
    int j;
    int i;
    int Nr;

    iv[0] = 0xa6a6a6a6;
    iv[1] = 0xa6a6a6a6;
    if ((length & 7) != 0 || (keyLength & 7) != 0) {
        return 0;
    }
    n = length >> 3;
    if (n < 2) {
        return 0;
    }
    Nr = rijndaelKeySetupEnc(rk, key, keyLength * 8);
    memcpy(out + 8, in, length);
    memcpy(block, iv, 8);
    for (j = 0; j < 6; j++) {
        for (i = 1; i <= n; i++) {
            memcpy(block + 8, out + i * sizeof(u64), 8);
            rijndaelEncrypt(rk, Nr, block, block);
            t = BSWAP64((u64)n * j + i);
            aeskw_xor64(block, (u8 *)&t, block);
            memcpy(out + i * sizeof(u64), block + 8, 8);
        }
    }
    memcpy(out, block, 8);
    return 1;
}

int keywrap_decrypt(u8 *out, const u8 *in, u32 length, const u8 *key, u32 keyLength) {
    u64 t;
    u32 iv[2];
    u8 block[16];
    u32 rk[80];
    int n;
    int j;
    int i;
    int Nr;
    int result = 1;

    iv[0] = 0xa6a6a6a6;
    iv[1] = 0xa6a6a6a6;
    if ((length & 7) != 0 || (keyLength & 7) != 0) {
        return 0;
    }
    n = (length - 1) >> 3;
    if (n < 2) {
        return 0;
    }
    Nr = rijndaelKeySetupDec(rk, key, keyLength * 8);
    memcpy(block, in, 8);
    // BUG: the input has length - 8 bytes after its first block, and this copies 7 bytes more, from past its end
#ifdef BUGFIX
    memcpy(out, in + 8, length - 8);
#else
    memcpy(out, in + 8, length - 1);
#endif
    for (j = 5; j >= 0; j--) {
        for (i = n; i > 0; i--) {
            t = BSWAP64((u64)n * j + i);
            aeskw_xor64(block, (u8 *)&t, block);
            memcpy(block + 8, out + (i - 1) * sizeof(u64), 8);
            rijndaelDecrypt(rk, Nr, block, block);
            memcpy(out + (i - 1) * sizeof(u64), block + 8, 8);
        }
    }
    if (memcmp(iv, block, 8) != 0) {
        result = 0;
    }
    return result;
}



void aeskw_xor64(u8 *srcA, u8 *srcB, u8 *dst) {
    *(u8 *)dst = *(u8 *)srcA ^ *(u8 *)srcB;
    *(s8 *)((u8 *)dst + 1) = (s8) (*(u8 *)((u8 *)srcA + 1) ^ *(u8 *)((u8 *)srcB + 1));
    *(s8 *)((u8 *)dst + 2) = (s8) (*(u8 *)((u8 *)srcA + 2) ^ *(u8 *)((u8 *)srcB + 2));
    *(s8 *)((u8 *)dst + 3) = (s8) (*(u8 *)((u8 *)srcA + 3) ^ *(u8 *)((u8 *)srcB + 3));
    *(s8 *)((u8 *)dst + 4) = (s8)(*(u8 *)((u8 *)srcA + 4) ^ *(u8 *)((u8 *)srcB + 4));
    *(s8 *)((u8 *)dst + 5) = (s8) (*(u8 *)((u8 *)srcA + 5) ^ *(u8 *)((u8 *)srcB + 5));
    *(s8 *)((u8 *)dst + 6) = (s8) (*(u8 *)((u8 *)srcA + 6) ^ *(u8 *)((u8 *)srcB + 6));
    *(s8 *)((u8 *)dst + 7) = (s8)(*(u8 *)((u8 *)srcA + 7) ^ *(u8 *)((u8 *)srcB + 7));
}

// rijndael-alg-fst.c's tables, defined in the ROM's order (MWCC's sort keeps their order here)
static const u32 rcon[10] = {
    0x01000000U, 0x02000000U, 0x04000000U, 0x08000000U, 0x10000000U, 0x20000000U, 0x40000000U, 0x80000000U,
    0x1b000000U, 0x36000000U,
};

static const u32 Td2[256] = {
    0xa75051f4U, 0x65537e41U, 0xa4c31a17U, 0x5e963a27U, 0x6bcb3babU, 0x45f11f9dU, 0x58abacfaU, 0x03934be3U,
    0xfa552030U, 0x6df6ad76U, 0x769188ccU, 0x4c25f502U, 0xd7fc4fe5U, 0xcbd7c52aU, 0x44802635U, 0xa38fb562U,
    0x5a49deb1U, 0x1b6725baU, 0x0e9845eaU, 0xc0e15dfeU, 0x7502c32fU, 0xf012814cU, 0x97a38d46U, 0xf9c66bd3U,
    0x5fe7038fU, 0x9c951592U, 0x7aebbf6dU, 0x59da9552U, 0x832dd4beU, 0x21d35874U, 0x692949e0U, 0xc8448ec9U,
    0x896a75c2U, 0x7978f48eU, 0x3e6b9958U, 0x71dd27b9U, 0x4fb6bee1U, 0xad17f088U, 0xac66c920U, 0x3ab47dceU,
    0x4a1863dfU, 0x3182e51aU, 0x33609751U, 0x7f456253U, 0x77e0b164U, 0xae84bb6bU, 0xa01cfe81U, 0x2b94f908U,
    0x68587048U, 0xfd198f45U, 0x6c8794deU, 0xf8b7527bU, 0xd323ab73U, 0x02e2724bU, 0x8f57e31fU, 0xab2a6655U,
    0x2807b2ebU, 0xc2032fb5U, 0x7b9a86c5U, 0x08a5d337U, 0x87f23028U, 0xa5b223bfU, 0x6aba0203U, 0x825ced16U,
    0x1c2b8acfU, 0xb492a779U, 0xf2f0f307U, 0xe2a14e69U, 0xf4cd65daU, 0xbed50605U, 0x621fd134U, 0xfe8ac4a6U,
    0x539d342eU, 0x55a0a2f3U, 0xe132058aU, 0xeb75a4f6U, 0xec390b83U, 0xefaa4060U, 0x9f065e71U, 0x1051bd6eU,
    0x8af93e21U, 0x063d96ddU, 0x05aedd3eU, 0xbd464de6U, 0x8db59154U, 0x5d0571c4U, 0xd46f0406U, 0x15ff6050U,
    0xfb241998U, 0xe997d6bdU, 0x43cc8940U, 0x9e7767d9U, 0x42bdb0e8U, 0x8b880789U, 0x5b38e719U, 0xeedb79c8U,
    0x0a47a17cU, 0x0fe97c42U, 0x1ec9f884U, 0x00000000U, 0x86830980U, 0xed48322bU, 0x70ac1e11U, 0x724e6c5aU,
    0xfffbfd0eU, 0x38560f85U, 0xd51e3daeU, 0x3927362dU, 0xd9640a0fU, 0xa621685cU, 0x54d19b5bU, 0x2e3a2436U,
    0x67b10c0aU, 0xe70f9357U, 0x96d2b4eeU, 0x919e1b9bU, 0xc54f80c0U, 0x20a261dcU, 0x4b695a77U, 0x1a161c12U,
    0xba0ae293U, 0x2ae5c0a0U, 0xe0433c22U, 0x171d121bU, 0x0d0b0e09U, 0xc7adf28bU, 0xa8b92db6U, 0xa9c8141eU,
    0x198557f1U, 0x074caf75U, 0xddbbee99U, 0x60fda37fU, 0x269ff701U, 0xf5bc5c72U, 0x3bc54466U, 0x7e345bfbU,
    0x29768b43U, 0xc6dccb23U, 0xfc68b6edU, 0xf163b8e4U, 0xdccad731U, 0x85104263U, 0x22401397U, 0x112084c6U,
    0x247d854aU, 0x3df8d2bbU, 0x3211aef9U, 0xa16dc729U, 0x2f4b1d9eU, 0x30f3dcb2U, 0x52ec0d86U, 0xe3d077c1U,
    0x166c2bb3U, 0xb999a970U, 0x48fa1194U, 0x642247e9U, 0x8cc4a8fcU, 0x3f1aa0f0U, 0x2cd8567dU, 0x90ef2233U,
    0x4ec78749U, 0xd1c1d938U, 0xa2fe8ccaU, 0x0b3698d4U, 0x81cfa6f5U, 0xde28a57aU, 0x8e26dab7U, 0xbfa43fadU,
    0x9de42c3aU, 0x920d5078U, 0xcc9b6a5fU, 0x4662547eU, 0x13c2f68dU, 0xb8e890d8U, 0xf75e2e39U, 0xaff582c3U,
    0x80be9f5dU, 0x937c69d0U, 0x2da96fd5U, 0x12b3cf25U, 0x993bc8acU, 0x7da71018U, 0x636ee89cU, 0xbb7bdb3bU,
    0x7809cd26U, 0x18f46e59U, 0xb701ec9aU, 0x9aa8834fU, 0x6e65e695U, 0xe67eaaffU, 0xcf0821bcU, 0xe8e6ef15U,
    0x9bd9bae7U, 0x36ce4a6fU, 0x09d4ea9fU, 0x7cd629b0U, 0xb2af31a4U, 0x23312a3fU, 0x9430c6a5U, 0x66c035a2U,
    0xbc37744eU, 0xcaa6fc82U, 0xd0b0e090U, 0xd81533a7U, 0x984af104U, 0xdaf741ecU, 0x500e7fcdU, 0xf62f1791U,
    0xd68d764dU, 0xb04d43efU, 0x4d54ccaaU, 0x04dfe496U, 0xb5e39ed1U, 0x881b4c6aU, 0x1fb8c12cU, 0x517f4665U,
    0xea049d5eU, 0x355d018cU, 0x7473fa87U, 0x412efb0bU, 0x1d5ab367U, 0xd25292dbU, 0x5633e910U, 0x47136dd6U,
    0x618c9ad7U, 0x0c7a37a1U, 0x148e59f8U, 0x3c89eb13U, 0x27eecea9U, 0xc935b761U, 0xe5ede11cU, 0xb13c7a47U,
    0xdf599cd2U, 0x733f55f2U, 0xce791814U, 0x37bf73c7U, 0xcdea53f7U, 0xaa5b5ffdU, 0x6f14df3dU, 0xdb867844U,
    0xf381caafU, 0xc43eb968U, 0x342c3824U, 0x405fc2a3U, 0xc372161dU, 0x250cbce2U, 0x498b283cU, 0x9541ff0dU,
    0x017139a8U, 0xb3de080cU, 0xe49cd8b4U, 0xc1906456U, 0x84617bcbU, 0xb670d532U, 0x5c74486cU, 0x5742d0b8U,
};

static const u32 Td3[256] = {
    0xf4a75051U, 0x4165537eU, 0x17a4c31aU, 0x275e963aU, 0xab6bcb3bU, 0x9d45f11fU, 0xfa58abacU, 0xe303934bU,
    0x30fa5520U, 0x766df6adU, 0xcc769188U, 0x024c25f5U, 0xe5d7fc4fU, 0x2acbd7c5U, 0x35448026U, 0x62a38fb5U,
    0xb15a49deU, 0xba1b6725U, 0xea0e9845U, 0xfec0e15dU, 0x2f7502c3U, 0x4cf01281U, 0x4697a38dU, 0xd3f9c66bU,
    0x8f5fe703U, 0x929c9515U, 0x6d7aebbfU, 0x5259da95U, 0xbe832dd4U, 0x7421d358U, 0xe0692949U, 0xc9c8448eU,
    0xc2896a75U, 0x8e7978f4U, 0x583e6b99U, 0xb971dd27U, 0xe14fb6beU, 0x88ad17f0U, 0x20ac66c9U, 0xce3ab47dU,
    0xdf4a1863U, 0x1a3182e5U, 0x51336097U, 0x537f4562U, 0x6477e0b1U, 0x6bae84bbU, 0x81a01cfeU, 0x082b94f9U,
    0x48685870U, 0x45fd198fU, 0xde6c8794U, 0x7bf8b752U, 0x73d323abU, 0x4b02e272U, 0x1f8f57e3U, 0x55ab2a66U,
    0xeb2807b2U, 0xb5c2032fU, 0xc57b9a86U, 0x3708a5d3U, 0x2887f230U, 0xbfa5b223U, 0x036aba02U, 0x16825cedU,
    0xcf1c2b8aU, 0x79b492a7U, 0x07f2f0f3U, 0x69e2a14eU, 0xdaf4cd65U, 0x05bed506U, 0x34621fd1U, 0xa6fe8ac4U,
    0x2e539d34U, 0xf355a0a2U, 0x8ae13205U, 0xf6eb75a4U, 0x83ec390bU, 0x60efaa40U, 0x719f065eU, 0x6e1051bdU,
    0x218af93eU, 0xdd063d96U, 0x3e05aeddU, 0xe6bd464dU, 0x548db591U, 0xc45d0571U, 0x06d46f04U, 0x5015ff60U,
    0x98fb2419U, 0xbde997d6U, 0x4043cc89U, 0xd99e7767U, 0xe842bdb0U, 0x898b8807U, 0x195b38e7U, 0xc8eedb79U,
    0x7c0a47a1U, 0x420fe97cU, 0x841ec9f8U, 0x00000000U, 0x80868309U, 0x2bed4832U, 0x1170ac1eU, 0x5a724e6cU,
    0x0efffbfdU, 0x8538560fU, 0xaed51e3dU, 0x2d392736U, 0x0fd9640aU, 0x5ca62168U, 0x5b54d19bU, 0x362e3a24U,
    0x0a67b10cU, 0x57e70f93U, 0xee96d2b4U, 0x9b919e1bU, 0xc0c54f80U, 0xdc20a261U, 0x774b695aU, 0x121a161cU,
    0x93ba0ae2U, 0xa02ae5c0U, 0x22e0433cU, 0x1b171d12U, 0x090d0b0eU, 0x8bc7adf2U, 0xb6a8b92dU, 0x1ea9c814U,
    0xf1198557U, 0x75074cafU, 0x99ddbbeeU, 0x7f60fda3U, 0x01269ff7U, 0x72f5bc5cU, 0x663bc544U, 0xfb7e345bU,
    0x4329768bU, 0x23c6dccbU, 0xedfc68b6U, 0xe4f163b8U, 0x31dccad7U, 0x63851042U, 0x97224013U, 0xc6112084U,
    0x4a247d85U, 0xbb3df8d2U, 0xf93211aeU, 0x29a16dc7U, 0x9e2f4b1dU, 0xb230f3dcU, 0x8652ec0dU, 0xc1e3d077U,
    0xb3166c2bU, 0x70b999a9U, 0x9448fa11U, 0xe9642247U, 0xfc8cc4a8U, 0xf03f1aa0U, 0x7d2cd856U, 0x3390ef22U,
    0x494ec787U, 0x38d1c1d9U, 0xcaa2fe8cU, 0xd40b3698U, 0xf581cfa6U, 0x7ade28a5U, 0xb78e26daU, 0xadbfa43fU,
    0x3a9de42cU, 0x78920d50U, 0x5fcc9b6aU, 0x7e466254U, 0x8d13c2f6U, 0xd8b8e890U, 0x39f75e2eU, 0xc3aff582U,
    0x5d80be9fU, 0xd0937c69U, 0xd52da96fU, 0x2512b3cfU, 0xac993bc8U, 0x187da710U, 0x9c636ee8U, 0x3bbb7bdbU,
    0x267809cdU, 0x5918f46eU, 0x9ab701ecU, 0x4f9aa883U, 0x956e65e6U, 0xffe67eaaU, 0xbccf0821U, 0x15e8e6efU,
    0xe79bd9baU, 0x6f36ce4aU, 0x9f09d4eaU, 0xb07cd629U, 0xa4b2af31U, 0x3f23312aU, 0xa59430c6U, 0xa266c035U,
    0x4ebc3774U, 0x82caa6fcU, 0x90d0b0e0U, 0xa7d81533U, 0x04984af1U, 0xecdaf741U, 0xcd500e7fU, 0x91f62f17U,
    0x4dd68d76U, 0xefb04d43U, 0xaa4d54ccU, 0x9604dfe4U, 0xd1b5e39eU, 0x6a881b4cU, 0x2c1fb8c1U, 0x65517f46U,
    0x5eea049dU, 0x8c355d01U, 0x877473faU, 0x0b412efbU, 0x671d5ab3U, 0xdbd25292U, 0x105633e9U, 0xd647136dU,
    0xd7618c9aU, 0xa10c7a37U, 0xf8148e59U, 0x133c89ebU, 0xa927eeceU, 0x61c935b7U, 0x1ce5ede1U, 0x47b13c7aU,
    0xd2df599cU, 0xf2733f55U, 0x14ce7918U, 0xc737bf73U, 0xf7cdea53U, 0xfdaa5b5fU, 0x3d6f14dfU, 0x44db8678U,
    0xaff381caU, 0x68c43eb9U, 0x24342c38U, 0xa3405fc2U, 0x1dc37216U, 0xe2250cbcU, 0x3c498b28U, 0x0d9541ffU,
    0xa8017139U, 0x0cb3de08U, 0xb4e49cd8U, 0x56c19064U, 0xcb84617bU, 0x32b670d5U, 0x6c5c7448U, 0xb85742d0U,
};

static const u32 Td4[256] = {
    0x52525252U, 0x09090909U, 0x6a6a6a6aU, 0xd5d5d5d5U, 0x30303030U, 0x36363636U, 0xa5a5a5a5U, 0x38383838U,
    0xbfbfbfbfU, 0x40404040U, 0xa3a3a3a3U, 0x9e9e9e9eU, 0x81818181U, 0xf3f3f3f3U, 0xd7d7d7d7U, 0xfbfbfbfbU,
    0x7c7c7c7cU, 0xe3e3e3e3U, 0x39393939U, 0x82828282U, 0x9b9b9b9bU, 0x2f2f2f2fU, 0xffffffffU, 0x87878787U,
    0x34343434U, 0x8e8e8e8eU, 0x43434343U, 0x44444444U, 0xc4c4c4c4U, 0xdedededeU, 0xe9e9e9e9U, 0xcbcbcbcbU,
    0x54545454U, 0x7b7b7b7bU, 0x94949494U, 0x32323232U, 0xa6a6a6a6U, 0xc2c2c2c2U, 0x23232323U, 0x3d3d3d3dU,
    0xeeeeeeeeU, 0x4c4c4c4cU, 0x95959595U, 0x0b0b0b0bU, 0x42424242U, 0xfafafafaU, 0xc3c3c3c3U, 0x4e4e4e4eU,
    0x08080808U, 0x2e2e2e2eU, 0xa1a1a1a1U, 0x66666666U, 0x28282828U, 0xd9d9d9d9U, 0x24242424U, 0xb2b2b2b2U,
    0x76767676U, 0x5b5b5b5bU, 0xa2a2a2a2U, 0x49494949U, 0x6d6d6d6dU, 0x8b8b8b8bU, 0xd1d1d1d1U, 0x25252525U,
    0x72727272U, 0xf8f8f8f8U, 0xf6f6f6f6U, 0x64646464U, 0x86868686U, 0x68686868U, 0x98989898U, 0x16161616U,
    0xd4d4d4d4U, 0xa4a4a4a4U, 0x5c5c5c5cU, 0xccccccccU, 0x5d5d5d5dU, 0x65656565U, 0xb6b6b6b6U, 0x92929292U,
    0x6c6c6c6cU, 0x70707070U, 0x48484848U, 0x50505050U, 0xfdfdfdfdU, 0xededededU, 0xb9b9b9b9U, 0xdadadadaU,
    0x5e5e5e5eU, 0x15151515U, 0x46464646U, 0x57575757U, 0xa7a7a7a7U, 0x8d8d8d8dU, 0x9d9d9d9dU, 0x84848484U,
    0x90909090U, 0xd8d8d8d8U, 0xababababU, 0x00000000U, 0x8c8c8c8cU, 0xbcbcbcbcU, 0xd3d3d3d3U, 0x0a0a0a0aU,
    0xf7f7f7f7U, 0xe4e4e4e4U, 0x58585858U, 0x05050505U, 0xb8b8b8b8U, 0xb3b3b3b3U, 0x45454545U, 0x06060606U,
    0xd0d0d0d0U, 0x2c2c2c2cU, 0x1e1e1e1eU, 0x8f8f8f8fU, 0xcacacacaU, 0x3f3f3f3fU, 0x0f0f0f0fU, 0x02020202U,
    0xc1c1c1c1U, 0xafafafafU, 0xbdbdbdbdU, 0x03030303U, 0x01010101U, 0x13131313U, 0x8a8a8a8aU, 0x6b6b6b6bU,
    0x3a3a3a3aU, 0x91919191U, 0x11111111U, 0x41414141U, 0x4f4f4f4fU, 0x67676767U, 0xdcdcdcdcU, 0xeaeaeaeaU,
    0x97979797U, 0xf2f2f2f2U, 0xcfcfcfcfU, 0xcecececeU, 0xf0f0f0f0U, 0xb4b4b4b4U, 0xe6e6e6e6U, 0x73737373U,
    0x96969696U, 0xacacacacU, 0x74747474U, 0x22222222U, 0xe7e7e7e7U, 0xadadadadU, 0x35353535U, 0x85858585U,
    0xe2e2e2e2U, 0xf9f9f9f9U, 0x37373737U, 0xe8e8e8e8U, 0x1c1c1c1cU, 0x75757575U, 0xdfdfdfdfU, 0x6e6e6e6eU,
    0x47474747U, 0xf1f1f1f1U, 0x1a1a1a1aU, 0x71717171U, 0x1d1d1d1dU, 0x29292929U, 0xc5c5c5c5U, 0x89898989U,
    0x6f6f6f6fU, 0xb7b7b7b7U, 0x62626262U, 0x0e0e0e0eU, 0xaaaaaaaaU, 0x18181818U, 0xbebebebeU, 0x1b1b1b1bU,
    0xfcfcfcfcU, 0x56565656U, 0x3e3e3e3eU, 0x4b4b4b4bU, 0xc6c6c6c6U, 0xd2d2d2d2U, 0x79797979U, 0x20202020U,
    0x9a9a9a9aU, 0xdbdbdbdbU, 0xc0c0c0c0U, 0xfefefefeU, 0x78787878U, 0xcdcdcdcdU, 0x5a5a5a5aU, 0xf4f4f4f4U,
    0x1f1f1f1fU, 0xddddddddU, 0xa8a8a8a8U, 0x33333333U, 0x88888888U, 0x07070707U, 0xc7c7c7c7U, 0x31313131U,
    0xb1b1b1b1U, 0x12121212U, 0x10101010U, 0x59595959U, 0x27272727U, 0x80808080U, 0xececececU, 0x5f5f5f5fU,
    0x60606060U, 0x51515151U, 0x7f7f7f7fU, 0xa9a9a9a9U, 0x19191919U, 0xb5b5b5b5U, 0x4a4a4a4aU, 0x0d0d0d0dU,
    0x2d2d2d2dU, 0xe5e5e5e5U, 0x7a7a7a7aU, 0x9f9f9f9fU, 0x93939393U, 0xc9c9c9c9U, 0x9c9c9c9cU, 0xefefefefU,
    0xa0a0a0a0U, 0xe0e0e0e0U, 0x3b3b3b3bU, 0x4d4d4d4dU, 0xaeaeaeaeU, 0x2a2a2a2aU, 0xf5f5f5f5U, 0xb0b0b0b0U,
    0xc8c8c8c8U, 0xebebebebU, 0xbbbbbbbbU, 0x3c3c3c3cU, 0x83838383U, 0x53535353U, 0x99999999U, 0x61616161U,
    0x17171717U, 0x2b2b2b2bU, 0x04040404U, 0x7e7e7e7eU, 0xbabababaU, 0x77777777U, 0xd6d6d6d6U, 0x26262626U,
    0xe1e1e1e1U, 0x69696969U, 0x14141414U, 0x63636363U, 0x55555555U, 0x21212121U, 0x0c0c0c0cU, 0x7d7d7d7dU,
};

static const u32 Te0[256] = {
    0xc66363a5U, 0xf87c7c84U, 0xee777799U, 0xf67b7b8dU, 0xfff2f20dU, 0xd66b6bbdU, 0xde6f6fb1U, 0x91c5c554U,
    0x60303050U, 0x02010103U, 0xce6767a9U, 0x562b2b7dU, 0xe7fefe19U, 0xb5d7d762U, 0x4dababe6U, 0xec76769aU,
    0x8fcaca45U, 0x1f82829dU, 0x89c9c940U, 0xfa7d7d87U, 0xeffafa15U, 0xb25959ebU, 0x8e4747c9U, 0xfbf0f00bU,
    0x41adadecU, 0xb3d4d467U, 0x5fa2a2fdU, 0x45afafeaU, 0x239c9cbfU, 0x53a4a4f7U, 0xe4727296U, 0x9bc0c05bU,
    0x75b7b7c2U, 0xe1fdfd1cU, 0x3d9393aeU, 0x4c26266aU, 0x6c36365aU, 0x7e3f3f41U, 0xf5f7f702U, 0x83cccc4fU,
    0x6834345cU, 0x51a5a5f4U, 0xd1e5e534U, 0xf9f1f108U, 0xe2717193U, 0xabd8d873U, 0x62313153U, 0x2a15153fU,
    0x0804040cU, 0x95c7c752U, 0x46232365U, 0x9dc3c35eU, 0x30181828U, 0x379696a1U, 0x0a05050fU, 0x2f9a9ab5U,
    0x0e070709U, 0x24121236U, 0x1b80809bU, 0xdfe2e23dU, 0xcdebeb26U, 0x4e272769U, 0x7fb2b2cdU, 0xea75759fU,
    0x1209091bU, 0x1d83839eU, 0x582c2c74U, 0x341a1a2eU, 0x361b1b2dU, 0xdc6e6eb2U, 0xb45a5aeeU, 0x5ba0a0fbU,
    0xa45252f6U, 0x763b3b4dU, 0xb7d6d661U, 0x7db3b3ceU, 0x5229297bU, 0xdde3e33eU, 0x5e2f2f71U, 0x13848497U,
    0xa65353f5U, 0xb9d1d168U, 0x00000000U, 0xc1eded2cU, 0x40202060U, 0xe3fcfc1fU, 0x79b1b1c8U, 0xb65b5bedU,
    0xd46a6abeU, 0x8dcbcb46U, 0x67bebed9U, 0x7239394bU, 0x944a4adeU, 0x984c4cd4U, 0xb05858e8U, 0x85cfcf4aU,
    0xbbd0d06bU, 0xc5efef2aU, 0x4faaaae5U, 0xedfbfb16U, 0x864343c5U, 0x9a4d4dd7U, 0x66333355U, 0x11858594U,
    0x8a4545cfU, 0xe9f9f910U, 0x04020206U, 0xfe7f7f81U, 0xa05050f0U, 0x783c3c44U, 0x259f9fbaU, 0x4ba8a8e3U,
    0xa25151f3U, 0x5da3a3feU, 0x804040c0U, 0x058f8f8aU, 0x3f9292adU, 0x219d9dbcU, 0x70383848U, 0xf1f5f504U,
    0x63bcbcdfU, 0x77b6b6c1U, 0xafdada75U, 0x42212163U, 0x20101030U, 0xe5ffff1aU, 0xfdf3f30eU, 0xbfd2d26dU,
    0x81cdcd4cU, 0x180c0c14U, 0x26131335U, 0xc3ecec2fU, 0xbe5f5fe1U, 0x359797a2U, 0x884444ccU, 0x2e171739U,
    0x93c4c457U, 0x55a7a7f2U, 0xfc7e7e82U, 0x7a3d3d47U, 0xc86464acU, 0xba5d5de7U, 0x3219192bU, 0xe6737395U,
    0xc06060a0U, 0x19818198U, 0x9e4f4fd1U, 0xa3dcdc7fU, 0x44222266U, 0x542a2a7eU, 0x3b9090abU, 0x0b888883U,
    0x8c4646caU, 0xc7eeee29U, 0x6bb8b8d3U, 0x2814143cU, 0xa7dede79U, 0xbc5e5ee2U, 0x160b0b1dU, 0xaddbdb76U,
    0xdbe0e03bU, 0x64323256U, 0x743a3a4eU, 0x140a0a1eU, 0x924949dbU, 0x0c06060aU, 0x4824246cU, 0xb85c5ce4U,
    0x9fc2c25dU, 0xbdd3d36eU, 0x43acacefU, 0xc46262a6U, 0x399191a8U, 0x319595a4U, 0xd3e4e437U, 0xf279798bU,
    0xd5e7e732U, 0x8bc8c843U, 0x6e373759U, 0xda6d6db7U, 0x018d8d8cU, 0xb1d5d564U, 0x9c4e4ed2U, 0x49a9a9e0U,
    0xd86c6cb4U, 0xac5656faU, 0xf3f4f407U, 0xcfeaea25U, 0xca6565afU, 0xf47a7a8eU, 0x47aeaee9U, 0x10080818U,
    0x6fbabad5U, 0xf0787888U, 0x4a25256fU, 0x5c2e2e72U, 0x381c1c24U, 0x57a6a6f1U, 0x73b4b4c7U, 0x97c6c651U,
    0xcbe8e823U, 0xa1dddd7cU, 0xe874749cU, 0x3e1f1f21U, 0x964b4bddU, 0x61bdbddcU, 0x0d8b8b86U, 0x0f8a8a85U,
    0xe0707090U, 0x7c3e3e42U, 0x71b5b5c4U, 0xcc6666aaU, 0x904848d8U, 0x06030305U, 0xf7f6f601U, 0x1c0e0e12U,
    0xc26161a3U, 0x6a35355fU, 0xae5757f9U, 0x69b9b9d0U, 0x17868691U, 0x99c1c158U, 0x3a1d1d27U, 0x279e9eb9U,
    0xd9e1e138U, 0xebf8f813U, 0x2b9898b3U, 0x22111133U, 0xd26969bbU, 0xa9d9d970U, 0x078e8e89U, 0x339494a7U,
    0x2d9b9bb6U, 0x3c1e1e22U, 0x15878792U, 0xc9e9e920U, 0x87cece49U, 0xaa5555ffU, 0x50282878U, 0xa5dfdf7aU,
    0x038c8c8fU, 0x59a1a1f8U, 0x09898980U, 0x1a0d0d17U, 0x65bfbfdaU, 0xd7e6e631U, 0x844242c6U, 0xd06868b8U,
    0x824141c3U, 0x299999b0U, 0x5a2d2d77U, 0x1e0f0f11U, 0x7bb0b0cbU, 0xa85454fcU, 0x6dbbbbd6U, 0x2c16163aU,
};

static const u32 Te1[256] = {
    0xa5c66363U, 0x84f87c7cU, 0x99ee7777U, 0x8df67b7bU, 0x0dfff2f2U, 0xbdd66b6bU, 0xb1de6f6fU, 0x5491c5c5U,
    0x50603030U, 0x03020101U, 0xa9ce6767U, 0x7d562b2bU, 0x19e7fefeU, 0x62b5d7d7U, 0xe64dababU, 0x9aec7676U,
    0x458fcacaU, 0x9d1f8282U, 0x4089c9c9U, 0x87fa7d7dU, 0x15effafaU, 0xebb25959U, 0xc98e4747U, 0x0bfbf0f0U,
    0xec41adadU, 0x67b3d4d4U, 0xfd5fa2a2U, 0xea45afafU, 0xbf239c9cU, 0xf753a4a4U, 0x96e47272U, 0x5b9bc0c0U,
    0xc275b7b7U, 0x1ce1fdfdU, 0xae3d9393U, 0x6a4c2626U, 0x5a6c3636U, 0x417e3f3fU, 0x02f5f7f7U, 0x4f83ccccU,
    0x5c683434U, 0xf451a5a5U, 0x34d1e5e5U, 0x08f9f1f1U, 0x93e27171U, 0x73abd8d8U, 0x53623131U, 0x3f2a1515U,
    0x0c080404U, 0x5295c7c7U, 0x65462323U, 0x5e9dc3c3U, 0x28301818U, 0xa1379696U, 0x0f0a0505U, 0xb52f9a9aU,
    0x090e0707U, 0x36241212U, 0x9b1b8080U, 0x3ddfe2e2U, 0x26cdebebU, 0x694e2727U, 0xcd7fb2b2U, 0x9fea7575U,
    0x1b120909U, 0x9e1d8383U, 0x74582c2cU, 0x2e341a1aU, 0x2d361b1bU, 0xb2dc6e6eU, 0xeeb45a5aU, 0xfb5ba0a0U,
    0xf6a45252U, 0x4d763b3bU, 0x61b7d6d6U, 0xce7db3b3U, 0x7b522929U, 0x3edde3e3U, 0x715e2f2fU, 0x97138484U,
    0xf5a65353U, 0x68b9d1d1U, 0x00000000U, 0x2cc1ededU, 0x60402020U, 0x1fe3fcfcU, 0xc879b1b1U, 0xedb65b5bU,
    0xbed46a6aU, 0x468dcbcbU, 0xd967bebeU, 0x4b723939U, 0xde944a4aU, 0xd4984c4cU, 0xe8b05858U, 0x4a85cfcfU,
    0x6bbbd0d0U, 0x2ac5efefU, 0xe54faaaaU, 0x16edfbfbU, 0xc5864343U, 0xd79a4d4dU, 0x55663333U, 0x94118585U,
    0xcf8a4545U, 0x10e9f9f9U, 0x06040202U, 0x81fe7f7fU, 0xf0a05050U, 0x44783c3cU, 0xba259f9fU, 0xe34ba8a8U,
    0xf3a25151U, 0xfe5da3a3U, 0xc0804040U, 0x8a058f8fU, 0xad3f9292U, 0xbc219d9dU, 0x48703838U, 0x04f1f5f5U,
    0xdf63bcbcU, 0xc177b6b6U, 0x75afdadaU, 0x63422121U, 0x30201010U, 0x1ae5ffffU, 0x0efdf3f3U, 0x6dbfd2d2U,
    0x4c81cdcdU, 0x14180c0cU, 0x35261313U, 0x2fc3ececU, 0xe1be5f5fU, 0xa2359797U, 0xcc884444U, 0x392e1717U,
    0x5793c4c4U, 0xf255a7a7U, 0x82fc7e7eU, 0x477a3d3dU, 0xacc86464U, 0xe7ba5d5dU, 0x2b321919U, 0x95e67373U,
    0xa0c06060U, 0x98198181U, 0xd19e4f4fU, 0x7fa3dcdcU, 0x66442222U, 0x7e542a2aU, 0xab3b9090U, 0x830b8888U,
    0xca8c4646U, 0x29c7eeeeU, 0xd36bb8b8U, 0x3c281414U, 0x79a7dedeU, 0xe2bc5e5eU, 0x1d160b0bU, 0x76addbdbU,
    0x3bdbe0e0U, 0x56643232U, 0x4e743a3aU, 0x1e140a0aU, 0xdb924949U, 0x0a0c0606U, 0x6c482424U, 0xe4b85c5cU,
    0x5d9fc2c2U, 0x6ebdd3d3U, 0xef43acacU, 0xa6c46262U, 0xa8399191U, 0xa4319595U, 0x37d3e4e4U, 0x8bf27979U,
    0x32d5e7e7U, 0x438bc8c8U, 0x596e3737U, 0xb7da6d6dU, 0x8c018d8dU, 0x64b1d5d5U, 0xd29c4e4eU, 0xe049a9a9U,
    0xb4d86c6cU, 0xfaac5656U, 0x07f3f4f4U, 0x25cfeaeaU, 0xafca6565U, 0x8ef47a7aU, 0xe947aeaeU, 0x18100808U,
    0xd56fbabaU, 0x88f07878U, 0x6f4a2525U, 0x725c2e2eU, 0x24381c1cU, 0xf157a6a6U, 0xc773b4b4U, 0x5197c6c6U,
    0x23cbe8e8U, 0x7ca1ddddU, 0x9ce87474U, 0x213e1f1fU, 0xdd964b4bU, 0xdc61bdbdU, 0x860d8b8bU, 0x850f8a8aU,
    0x90e07070U, 0x427c3e3eU, 0xc471b5b5U, 0xaacc6666U, 0xd8904848U, 0x05060303U, 0x01f7f6f6U, 0x121c0e0eU,
    0xa3c26161U, 0x5f6a3535U, 0xf9ae5757U, 0xd069b9b9U, 0x91178686U, 0x5899c1c1U, 0x273a1d1dU, 0xb9279e9eU,
    0x38d9e1e1U, 0x13ebf8f8U, 0xb32b9898U, 0x33221111U, 0xbbd26969U, 0x70a9d9d9U, 0x89078e8eU, 0xa7339494U,
    0xb62d9b9bU, 0x223c1e1eU, 0x92158787U, 0x20c9e9e9U, 0x4987ceceU, 0xffaa5555U, 0x78502828U, 0x7aa5dfdfU,
    0x8f038c8cU, 0xf859a1a1U, 0x80098989U, 0x171a0d0dU, 0xda65bfbfU, 0x31d7e6e6U, 0xc6844242U, 0xb8d06868U,
    0xc3824141U, 0xb0299999U, 0x775a2d2dU, 0x111e0f0fU, 0xcb7bb0b0U, 0xfca85454U, 0xd66dbbbbU, 0x3a2c1616U,
};

static const u32 Te2[256] = {
    0x63a5c663U, 0x7c84f87cU, 0x7799ee77U, 0x7b8df67bU, 0xf20dfff2U, 0x6bbdd66bU, 0x6fb1de6fU, 0xc55491c5U,
    0x30506030U, 0x01030201U, 0x67a9ce67U, 0x2b7d562bU, 0xfe19e7feU, 0xd762b5d7U, 0xabe64dabU, 0x769aec76U,
    0xca458fcaU, 0x829d1f82U, 0xc94089c9U, 0x7d87fa7dU, 0xfa15effaU, 0x59ebb259U, 0x47c98e47U, 0xf00bfbf0U,
    0xadec41adU, 0xd467b3d4U, 0xa2fd5fa2U, 0xafea45afU, 0x9cbf239cU, 0xa4f753a4U, 0x7296e472U, 0xc05b9bc0U,
    0xb7c275b7U, 0xfd1ce1fdU, 0x93ae3d93U, 0x266a4c26U, 0x365a6c36U, 0x3f417e3fU, 0xf702f5f7U, 0xcc4f83ccU,
    0x345c6834U, 0xa5f451a5U, 0xe534d1e5U, 0xf108f9f1U, 0x7193e271U, 0xd873abd8U, 0x31536231U, 0x153f2a15U,
    0x040c0804U, 0xc75295c7U, 0x23654623U, 0xc35e9dc3U, 0x18283018U, 0x96a13796U, 0x050f0a05U, 0x9ab52f9aU,
    0x07090e07U, 0x12362412U, 0x809b1b80U, 0xe23ddfe2U, 0xeb26cdebU, 0x27694e27U, 0xb2cd7fb2U, 0x759fea75U,
    0x091b1209U, 0x839e1d83U, 0x2c74582cU, 0x1a2e341aU, 0x1b2d361bU, 0x6eb2dc6eU, 0x5aeeb45aU, 0xa0fb5ba0U,
    0x52f6a452U, 0x3b4d763bU, 0xd661b7d6U, 0xb3ce7db3U, 0x297b5229U, 0xe33edde3U, 0x2f715e2fU, 0x84971384U,
    0x53f5a653U, 0xd168b9d1U, 0x00000000U, 0xed2cc1edU, 0x20604020U, 0xfc1fe3fcU, 0xb1c879b1U, 0x5bedb65bU,
    0x6abed46aU, 0xcb468dcbU, 0xbed967beU, 0x394b7239U, 0x4ade944aU, 0x4cd4984cU, 0x58e8b058U, 0xcf4a85cfU,
    0xd06bbbd0U, 0xef2ac5efU, 0xaae54faaU, 0xfb16edfbU, 0x43c58643U, 0x4dd79a4dU, 0x33556633U, 0x85941185U,
    0x45cf8a45U, 0xf910e9f9U, 0x02060402U, 0x7f81fe7fU, 0x50f0a050U, 0x3c44783cU, 0x9fba259fU, 0xa8e34ba8U,
    0x51f3a251U, 0xa3fe5da3U, 0x40c08040U, 0x8f8a058fU, 0x92ad3f92U, 0x9dbc219dU, 0x38487038U, 0xf504f1f5U,
    0xbcdf63bcU, 0xb6c177b6U, 0xda75afdaU, 0x21634221U, 0x10302010U, 0xff1ae5ffU, 0xf30efdf3U, 0xd26dbfd2U,
    0xcd4c81cdU, 0x0c14180cU, 0x13352613U, 0xec2fc3ecU, 0x5fe1be5fU, 0x97a23597U, 0x44cc8844U, 0x17392e17U,
    0xc45793c4U, 0xa7f255a7U, 0x7e82fc7eU, 0x3d477a3dU, 0x64acc864U, 0x5de7ba5dU, 0x192b3219U, 0x7395e673U,
    0x60a0c060U, 0x81981981U, 0x4fd19e4fU, 0xdc7fa3dcU, 0x22664422U, 0x2a7e542aU, 0x90ab3b90U, 0x88830b88U,
    0x46ca8c46U, 0xee29c7eeU, 0xb8d36bb8U, 0x143c2814U, 0xde79a7deU, 0x5ee2bc5eU, 0x0b1d160bU, 0xdb76addbU,
    0xe03bdbe0U, 0x32566432U, 0x3a4e743aU, 0x0a1e140aU, 0x49db9249U, 0x060a0c06U, 0x246c4824U, 0x5ce4b85cU,
    0xc25d9fc2U, 0xd36ebdd3U, 0xacef43acU, 0x62a6c462U, 0x91a83991U, 0x95a43195U, 0xe437d3e4U, 0x798bf279U,
    0xe732d5e7U, 0xc8438bc8U, 0x37596e37U, 0x6db7da6dU, 0x8d8c018dU, 0xd564b1d5U, 0x4ed29c4eU, 0xa9e049a9U,
    0x6cb4d86cU, 0x56faac56U, 0xf407f3f4U, 0xea25cfeaU, 0x65afca65U, 0x7a8ef47aU, 0xaee947aeU, 0x08181008U,
    0xbad56fbaU, 0x7888f078U, 0x256f4a25U, 0x2e725c2eU, 0x1c24381cU, 0xa6f157a6U, 0xb4c773b4U, 0xc65197c6U,
    0xe823cbe8U, 0xdd7ca1ddU, 0x749ce874U, 0x1f213e1fU, 0x4bdd964bU, 0xbddc61bdU, 0x8b860d8bU, 0x8a850f8aU,
    0x7090e070U, 0x3e427c3eU, 0xb5c471b5U, 0x66aacc66U, 0x48d89048U, 0x03050603U, 0xf601f7f6U, 0x0e121c0eU,
    0x61a3c261U, 0x355f6a35U, 0x57f9ae57U, 0xb9d069b9U, 0x86911786U, 0xc15899c1U, 0x1d273a1dU, 0x9eb9279eU,
    0xe138d9e1U, 0xf813ebf8U, 0x98b32b98U, 0x11332211U, 0x69bbd269U, 0xd970a9d9U, 0x8e89078eU, 0x94a73394U,
    0x9bb62d9bU, 0x1e223c1eU, 0x87921587U, 0xe920c9e9U, 0xce4987ceU, 0x55ffaa55U, 0x28785028U, 0xdf7aa5dfU,
    0x8c8f038cU, 0xa1f859a1U, 0x89800989U, 0x0d171a0dU, 0xbfda65bfU, 0xe631d7e6U, 0x42c68442U, 0x68b8d068U,
    0x41c38241U, 0x99b02999U, 0x2d775a2dU, 0x0f111e0fU, 0xb0cb7bb0U, 0x54fca854U, 0xbbd66dbbU, 0x163a2c16U,
};

static const u32 Te3[256] = {
    0x6363a5c6U, 0x7c7c84f8U, 0x777799eeU, 0x7b7b8df6U, 0xf2f20dffU, 0x6b6bbdd6U, 0x6f6fb1deU, 0xc5c55491U,
    0x30305060U, 0x01010302U, 0x6767a9ceU, 0x2b2b7d56U, 0xfefe19e7U, 0xd7d762b5U, 0xababe64dU, 0x76769aecU,
    0xcaca458fU, 0x82829d1fU, 0xc9c94089U, 0x7d7d87faU, 0xfafa15efU, 0x5959ebb2U, 0x4747c98eU, 0xf0f00bfbU,
    0xadadec41U, 0xd4d467b3U, 0xa2a2fd5fU, 0xafafea45U, 0x9c9cbf23U, 0xa4a4f753U, 0x727296e4U, 0xc0c05b9bU,
    0xb7b7c275U, 0xfdfd1ce1U, 0x9393ae3dU, 0x26266a4cU, 0x36365a6cU, 0x3f3f417eU, 0xf7f702f5U, 0xcccc4f83U,
    0x34345c68U, 0xa5a5f451U, 0xe5e534d1U, 0xf1f108f9U, 0x717193e2U, 0xd8d873abU, 0x31315362U, 0x15153f2aU,
    0x04040c08U, 0xc7c75295U, 0x23236546U, 0xc3c35e9dU, 0x18182830U, 0x9696a137U, 0x05050f0aU, 0x9a9ab52fU,
    0x0707090eU, 0x12123624U, 0x80809b1bU, 0xe2e23ddfU, 0xebeb26cdU, 0x2727694eU, 0xb2b2cd7fU, 0x75759feaU,
    0x09091b12U, 0x83839e1dU, 0x2c2c7458U, 0x1a1a2e34U, 0x1b1b2d36U, 0x6e6eb2dcU, 0x5a5aeeb4U, 0xa0a0fb5bU,
    0x5252f6a4U, 0x3b3b4d76U, 0xd6d661b7U, 0xb3b3ce7dU, 0x29297b52U, 0xe3e33eddU, 0x2f2f715eU, 0x84849713U,
    0x5353f5a6U, 0xd1d168b9U, 0x00000000U, 0xeded2cc1U, 0x20206040U, 0xfcfc1fe3U, 0xb1b1c879U, 0x5b5bedb6U,
    0x6a6abed4U, 0xcbcb468dU, 0xbebed967U, 0x39394b72U, 0x4a4ade94U, 0x4c4cd498U, 0x5858e8b0U, 0xcfcf4a85U,
    0xd0d06bbbU, 0xefef2ac5U, 0xaaaae54fU, 0xfbfb16edU, 0x4343c586U, 0x4d4dd79aU, 0x33335566U, 0x85859411U,
    0x4545cf8aU, 0xf9f910e9U, 0x02020604U, 0x7f7f81feU, 0x5050f0a0U, 0x3c3c4478U, 0x9f9fba25U, 0xa8a8e34bU,
    0x5151f3a2U, 0xa3a3fe5dU, 0x4040c080U, 0x8f8f8a05U, 0x9292ad3fU, 0x9d9dbc21U, 0x38384870U, 0xf5f504f1U,
    0xbcbcdf63U, 0xb6b6c177U, 0xdada75afU, 0x21216342U, 0x10103020U, 0xffff1ae5U, 0xf3f30efdU, 0xd2d26dbfU,
    0xcdcd4c81U, 0x0c0c1418U, 0x13133526U, 0xecec2fc3U, 0x5f5fe1beU, 0x9797a235U, 0x4444cc88U, 0x1717392eU,
    0xc4c45793U, 0xa7a7f255U, 0x7e7e82fcU, 0x3d3d477aU, 0x6464acc8U, 0x5d5de7baU, 0x19192b32U, 0x737395e6U,
    0x6060a0c0U, 0x81819819U, 0x4f4fd19eU, 0xdcdc7fa3U, 0x22226644U, 0x2a2a7e54U, 0x9090ab3bU, 0x8888830bU,
    0x4646ca8cU, 0xeeee29c7U, 0xb8b8d36bU, 0x14143c28U, 0xdede79a7U, 0x5e5ee2bcU, 0x0b0b1d16U, 0xdbdb76adU,
    0xe0e03bdbU, 0x32325664U, 0x3a3a4e74U, 0x0a0a1e14U, 0x4949db92U, 0x06060a0cU, 0x24246c48U, 0x5c5ce4b8U,
    0xc2c25d9fU, 0xd3d36ebdU, 0xacacef43U, 0x6262a6c4U, 0x9191a839U, 0x9595a431U, 0xe4e437d3U, 0x79798bf2U,
    0xe7e732d5U, 0xc8c8438bU, 0x3737596eU, 0x6d6db7daU, 0x8d8d8c01U, 0xd5d564b1U, 0x4e4ed29cU, 0xa9a9e049U,
    0x6c6cb4d8U, 0x5656faacU, 0xf4f407f3U, 0xeaea25cfU, 0x6565afcaU, 0x7a7a8ef4U, 0xaeaee947U, 0x08081810U,
    0xbabad56fU, 0x787888f0U, 0x25256f4aU, 0x2e2e725cU, 0x1c1c2438U, 0xa6a6f157U, 0xb4b4c773U, 0xc6c65197U,
    0xe8e823cbU, 0xdddd7ca1U, 0x74749ce8U, 0x1f1f213eU, 0x4b4bdd96U, 0xbdbddc61U, 0x8b8b860dU, 0x8a8a850fU,
    0x707090e0U, 0x3e3e427cU, 0xb5b5c471U, 0x6666aaccU, 0x4848d890U, 0x03030506U, 0xf6f601f7U, 0x0e0e121cU,
    0x6161a3c2U, 0x35355f6aU, 0x5757f9aeU, 0xb9b9d069U, 0x86869117U, 0xc1c15899U, 0x1d1d273aU, 0x9e9eb927U,
    0xe1e138d9U, 0xf8f813ebU, 0x9898b32bU, 0x11113322U, 0x6969bbd2U, 0xd9d970a9U, 0x8e8e8907U, 0x9494a733U,
    0x9b9bb62dU, 0x1e1e223cU, 0x87879215U, 0xe9e920c9U, 0xcece4987U, 0x5555ffaaU, 0x28287850U, 0xdfdf7aa5U,
    0x8c8c8f03U, 0xa1a1f859U, 0x89898009U, 0x0d0d171aU, 0xbfbfda65U, 0xe6e631d7U, 0x4242c684U, 0x6868b8d0U,
    0x4141c382U, 0x9999b029U, 0x2d2d775aU, 0x0f0f111eU, 0xb0b0cb7bU, 0x5454fca8U, 0xbbbbd66dU, 0x16163a2cU,
};

static const u32 Te4[256] = {
    0x63636363U, 0x7c7c7c7cU, 0x77777777U, 0x7b7b7b7bU, 0xf2f2f2f2U, 0x6b6b6b6bU, 0x6f6f6f6fU, 0xc5c5c5c5U,
    0x30303030U, 0x01010101U, 0x67676767U, 0x2b2b2b2bU, 0xfefefefeU, 0xd7d7d7d7U, 0xababababU, 0x76767676U,
    0xcacacacaU, 0x82828282U, 0xc9c9c9c9U, 0x7d7d7d7dU, 0xfafafafaU, 0x59595959U, 0x47474747U, 0xf0f0f0f0U,
    0xadadadadU, 0xd4d4d4d4U, 0xa2a2a2a2U, 0xafafafafU, 0x9c9c9c9cU, 0xa4a4a4a4U, 0x72727272U, 0xc0c0c0c0U,
    0xb7b7b7b7U, 0xfdfdfdfdU, 0x93939393U, 0x26262626U, 0x36363636U, 0x3f3f3f3fU, 0xf7f7f7f7U, 0xccccccccU,
    0x34343434U, 0xa5a5a5a5U, 0xe5e5e5e5U, 0xf1f1f1f1U, 0x71717171U, 0xd8d8d8d8U, 0x31313131U, 0x15151515U,
    0x04040404U, 0xc7c7c7c7U, 0x23232323U, 0xc3c3c3c3U, 0x18181818U, 0x96969696U, 0x05050505U, 0x9a9a9a9aU,
    0x07070707U, 0x12121212U, 0x80808080U, 0xe2e2e2e2U, 0xebebebebU, 0x27272727U, 0xb2b2b2b2U, 0x75757575U,
    0x09090909U, 0x83838383U, 0x2c2c2c2cU, 0x1a1a1a1aU, 0x1b1b1b1bU, 0x6e6e6e6eU, 0x5a5a5a5aU, 0xa0a0a0a0U,
    0x52525252U, 0x3b3b3b3bU, 0xd6d6d6d6U, 0xb3b3b3b3U, 0x29292929U, 0xe3e3e3e3U, 0x2f2f2f2fU, 0x84848484U,
    0x53535353U, 0xd1d1d1d1U, 0x00000000U, 0xededededU, 0x20202020U, 0xfcfcfcfcU, 0xb1b1b1b1U, 0x5b5b5b5bU,
    0x6a6a6a6aU, 0xcbcbcbcbU, 0xbebebebeU, 0x39393939U, 0x4a4a4a4aU, 0x4c4c4c4cU, 0x58585858U, 0xcfcfcfcfU,
    0xd0d0d0d0U, 0xefefefefU, 0xaaaaaaaaU, 0xfbfbfbfbU, 0x43434343U, 0x4d4d4d4dU, 0x33333333U, 0x85858585U,
    0x45454545U, 0xf9f9f9f9U, 0x02020202U, 0x7f7f7f7fU, 0x50505050U, 0x3c3c3c3cU, 0x9f9f9f9fU, 0xa8a8a8a8U,
    0x51515151U, 0xa3a3a3a3U, 0x40404040U, 0x8f8f8f8fU, 0x92929292U, 0x9d9d9d9dU, 0x38383838U, 0xf5f5f5f5U,
    0xbcbcbcbcU, 0xb6b6b6b6U, 0xdadadadaU, 0x21212121U, 0x10101010U, 0xffffffffU, 0xf3f3f3f3U, 0xd2d2d2d2U,
    0xcdcdcdcdU, 0x0c0c0c0cU, 0x13131313U, 0xececececU, 0x5f5f5f5fU, 0x97979797U, 0x44444444U, 0x17171717U,
    0xc4c4c4c4U, 0xa7a7a7a7U, 0x7e7e7e7eU, 0x3d3d3d3dU, 0x64646464U, 0x5d5d5d5dU, 0x19191919U, 0x73737373U,
    0x60606060U, 0x81818181U, 0x4f4f4f4fU, 0xdcdcdcdcU, 0x22222222U, 0x2a2a2a2aU, 0x90909090U, 0x88888888U,
    0x46464646U, 0xeeeeeeeeU, 0xb8b8b8b8U, 0x14141414U, 0xdedededeU, 0x5e5e5e5eU, 0x0b0b0b0bU, 0xdbdbdbdbU,
    0xe0e0e0e0U, 0x32323232U, 0x3a3a3a3aU, 0x0a0a0a0aU, 0x49494949U, 0x06060606U, 0x24242424U, 0x5c5c5c5cU,
    0xc2c2c2c2U, 0xd3d3d3d3U, 0xacacacacU, 0x62626262U, 0x91919191U, 0x95959595U, 0xe4e4e4e4U, 0x79797979U,
    0xe7e7e7e7U, 0xc8c8c8c8U, 0x37373737U, 0x6d6d6d6dU, 0x8d8d8d8dU, 0xd5d5d5d5U, 0x4e4e4e4eU, 0xa9a9a9a9U,
    0x6c6c6c6cU, 0x56565656U, 0xf4f4f4f4U, 0xeaeaeaeaU, 0x65656565U, 0x7a7a7a7aU, 0xaeaeaeaeU, 0x08080808U,
    0xbabababaU, 0x78787878U, 0x25252525U, 0x2e2e2e2eU, 0x1c1c1c1cU, 0xa6a6a6a6U, 0xb4b4b4b4U, 0xc6c6c6c6U,
    0xe8e8e8e8U, 0xddddddddU, 0x74747474U, 0x1f1f1f1fU, 0x4b4b4b4bU, 0xbdbdbdbdU, 0x8b8b8b8bU, 0x8a8a8a8aU,
    0x70707070U, 0x3e3e3e3eU, 0xb5b5b5b5U, 0x66666666U, 0x48484848U, 0x03030303U, 0xf6f6f6f6U, 0x0e0e0e0eU,
    0x61616161U, 0x35353535U, 0x57575757U, 0xb9b9b9b9U, 0x86868686U, 0xc1c1c1c1U, 0x1d1d1d1dU, 0x9e9e9e9eU,
    0xe1e1e1e1U, 0xf8f8f8f8U, 0x98989898U, 0x11111111U, 0x69696969U, 0xd9d9d9d9U, 0x8e8e8e8eU, 0x94949494U,
    0x9b9b9b9bU, 0x1e1e1e1eU, 0x87878787U, 0xe9e9e9e9U, 0xcecececeU, 0x55555555U, 0x28282828U, 0xdfdfdfdfU,
    0x8c8c8c8cU, 0xa1a1a1a1U, 0x89898989U, 0x0d0d0d0dU, 0xbfbfbfbfU, 0xe6e6e6e6U, 0x42424242U, 0x68686868U,
    0x41414141U, 0x99999999U, 0x2d2d2d2dU, 0x0f0f0f0fU, 0xb0b0b0b0U, 0x54545454U, 0xbbbbbbbbU, 0x16161616U,
};

static const u32 Td0[256] = {
    0x51f4a750U, 0x7e416553U, 0x1a17a4c3U, 0x3a275e96U, 0x3bab6bcbU, 0x1f9d45f1U, 0xacfa58abU, 0x4be30393U,
    0x2030fa55U, 0xad766df6U, 0x88cc7691U, 0xf5024c25U, 0x4fe5d7fcU, 0xc52acbd7U, 0x26354480U, 0xb562a38fU,
    0xdeb15a49U, 0x25ba1b67U, 0x45ea0e98U, 0x5dfec0e1U, 0xc32f7502U, 0x814cf012U, 0x8d4697a3U, 0x6bd3f9c6U,
    0x038f5fe7U, 0x15929c95U, 0xbf6d7aebU, 0x955259daU, 0xd4be832dU, 0x587421d3U, 0x49e06929U, 0x8ec9c844U,
    0x75c2896aU, 0xf48e7978U, 0x99583e6bU, 0x27b971ddU, 0xbee14fb6U, 0xf088ad17U, 0xc920ac66U, 0x7dce3ab4U,
    0x63df4a18U, 0xe51a3182U, 0x97513360U, 0x62537f45U, 0xb16477e0U, 0xbb6bae84U, 0xfe81a01cU, 0xf9082b94U,
    0x70486858U, 0x8f45fd19U, 0x94de6c87U, 0x527bf8b7U, 0xab73d323U, 0x724b02e2U, 0xe31f8f57U, 0x6655ab2aU,
    0xb2eb2807U, 0x2fb5c203U, 0x86c57b9aU, 0xd33708a5U, 0x302887f2U, 0x23bfa5b2U, 0x02036abaU, 0xed16825cU,
    0x8acf1c2bU, 0xa779b492U, 0xf307f2f0U, 0x4e69e2a1U, 0x65daf4cdU, 0x0605bed5U, 0xd134621fU, 0xc4a6fe8aU,
    0x342e539dU, 0xa2f355a0U, 0x058ae132U, 0xa4f6eb75U, 0x0b83ec39U, 0x4060efaaU, 0x5e719f06U, 0xbd6e1051U,
    0x3e218af9U, 0x96dd063dU, 0xdd3e05aeU, 0x4de6bd46U, 0x91548db5U, 0x71c45d05U, 0x0406d46fU, 0x605015ffU,
    0x1998fb24U, 0xd6bde997U, 0x894043ccU, 0x67d99e77U, 0xb0e842bdU, 0x07898b88U, 0xe7195b38U, 0x79c8eedbU,
    0xa17c0a47U, 0x7c420fe9U, 0xf8841ec9U, 0x00000000U, 0x09808683U, 0x322bed48U, 0x1e1170acU, 0x6c5a724eU,
    0xfd0efffbU, 0x0f853856U, 0x3daed51eU, 0x362d3927U, 0x0a0fd964U, 0x685ca621U, 0x9b5b54d1U, 0x24362e3aU,
    0x0c0a67b1U, 0x9357e70fU, 0xb4ee96d2U, 0x1b9b919eU, 0x80c0c54fU, 0x61dc20a2U, 0x5a774b69U, 0x1c121a16U,
    0xe293ba0aU, 0xc0a02ae5U, 0x3c22e043U, 0x121b171dU, 0x0e090d0bU, 0xf28bc7adU, 0x2db6a8b9U, 0x141ea9c8U,
    0x57f11985U, 0xaf75074cU, 0xee99ddbbU, 0xa37f60fdU, 0xf701269fU, 0x5c72f5bcU, 0x44663bc5U, 0x5bfb7e34U,
    0x8b432976U, 0xcb23c6dcU, 0xb6edfc68U, 0xb8e4f163U, 0xd731dccaU, 0x42638510U, 0x13972240U, 0x84c61120U,
    0x854a247dU, 0xd2bb3df8U, 0xaef93211U, 0xc729a16dU, 0x1d9e2f4bU, 0xdcb230f3U, 0x0d8652ecU, 0x77c1e3d0U,
    0x2bb3166cU, 0xa970b999U, 0x119448faU, 0x47e96422U, 0xa8fc8cc4U, 0xa0f03f1aU, 0x567d2cd8U, 0x223390efU,
    0x87494ec7U, 0xd938d1c1U, 0x8ccaa2feU, 0x98d40b36U, 0xa6f581cfU, 0xa57ade28U, 0xdab78e26U, 0x3fadbfa4U,
    0x2c3a9de4U, 0x5078920dU, 0x6a5fcc9bU, 0x547e4662U, 0xf68d13c2U, 0x90d8b8e8U, 0x2e39f75eU, 0x82c3aff5U,
    0x9f5d80beU, 0x69d0937cU, 0x6fd52da9U, 0xcf2512b3U, 0xc8ac993bU, 0x10187da7U, 0xe89c636eU, 0xdb3bbb7bU,
    0xcd267809U, 0x6e5918f4U, 0xec9ab701U, 0x834f9aa8U, 0xe6956e65U, 0xaaffe67eU, 0x21bccf08U, 0xef15e8e6U,
    0xbae79bd9U, 0x4a6f36ceU, 0xea9f09d4U, 0x29b07cd6U, 0x31a4b2afU, 0x2a3f2331U, 0xc6a59430U, 0x35a266c0U,
    0x744ebc37U, 0xfc82caa6U, 0xe090d0b0U, 0x33a7d815U, 0xf104984aU, 0x41ecdaf7U, 0x7fcd500eU, 0x1791f62fU,
    0x764dd68dU, 0x43efb04dU, 0xccaa4d54U, 0xe49604dfU, 0x9ed1b5e3U, 0x4c6a881bU, 0xc12c1fb8U, 0x4665517fU,
    0x9d5eea04U, 0x018c355dU, 0xfa877473U, 0xfb0b412eU, 0xb3671d5aU, 0x92dbd252U, 0xe9105633U, 0x6dd64713U,
    0x9ad7618cU, 0x37a10c7aU, 0x59f8148eU, 0xeb133c89U, 0xcea927eeU, 0xb761c935U, 0xe11ce5edU, 0x7a47b13cU,
    0x9cd2df59U, 0x55f2733fU, 0x1814ce79U, 0x73c737bfU, 0x53f7cdeaU, 0x5ffdaa5bU, 0xdf3d6f14U, 0x7844db86U,
    0xcaaff381U, 0xb968c43eU, 0x3824342cU, 0xc2a3405fU, 0x161dc372U, 0xbce2250cU, 0x283c498bU, 0xff0d9541U,
    0x39a80171U, 0x080cb3deU, 0xd8b4e49cU, 0x6456c190U, 0x7bcb8461U, 0xd532b670U, 0x486c5c74U, 0xd0b85742U,
};

static const u32 Td1[256] = {
    0x5051f4a7U, 0x537e4165U, 0xc31a17a4U, 0x963a275eU, 0xcb3bab6bU, 0xf11f9d45U, 0xabacfa58U, 0x934be303U,
    0x552030faU, 0xf6ad766dU, 0x9188cc76U, 0x25f5024cU, 0xfc4fe5d7U, 0xd7c52acbU, 0x80263544U, 0x8fb562a3U,
    0x49deb15aU, 0x6725ba1bU, 0x9845ea0eU, 0xe15dfec0U, 0x02c32f75U, 0x12814cf0U, 0xa38d4697U, 0xc66bd3f9U,
    0xe7038f5fU, 0x9515929cU, 0xebbf6d7aU, 0xda955259U, 0x2dd4be83U, 0xd3587421U, 0x2949e069U, 0x448ec9c8U,
    0x6a75c289U, 0x78f48e79U, 0x6b99583eU, 0xdd27b971U, 0xb6bee14fU, 0x17f088adU, 0x66c920acU, 0xb47dce3aU,
    0x1863df4aU, 0x82e51a31U, 0x60975133U, 0x4562537fU, 0xe0b16477U, 0x84bb6baeU, 0x1cfe81a0U, 0x94f9082bU,
    0x58704868U, 0x198f45fdU, 0x8794de6cU, 0xb7527bf8U, 0x23ab73d3U, 0xe2724b02U, 0x57e31f8fU, 0x2a6655abU,
    0x07b2eb28U, 0x032fb5c2U, 0x9a86c57bU, 0xa5d33708U, 0xf2302887U, 0xb223bfa5U, 0xba02036aU, 0x5ced1682U,
    0x2b8acf1cU, 0x92a779b4U, 0xf0f307f2U, 0xa14e69e2U, 0xcd65daf4U, 0xd50605beU, 0x1fd13462U, 0x8ac4a6feU,
    0x9d342e53U, 0xa0a2f355U, 0x32058ae1U, 0x75a4f6ebU, 0x390b83ecU, 0xaa4060efU, 0x065e719fU, 0x51bd6e10U,
    0xf93e218aU, 0x3d96dd06U, 0xaedd3e05U, 0x464de6bdU, 0xb591548dU, 0x0571c45dU, 0x6f0406d4U, 0xff605015U,
    0x241998fbU, 0x97d6bde9U, 0xcc894043U, 0x7767d99eU, 0xbdb0e842U, 0x8807898bU, 0x38e7195bU, 0xdb79c8eeU,
    0x47a17c0aU, 0xe97c420fU, 0xc9f8841eU, 0x00000000U, 0x83098086U, 0x48322bedU, 0xac1e1170U, 0x4e6c5a72U,
    0xfbfd0effU, 0x560f8538U, 0x1e3daed5U, 0x27362d39U, 0x640a0fd9U, 0x21685ca6U, 0xd19b5b54U, 0x3a24362eU,
    0xb10c0a67U, 0x0f9357e7U, 0xd2b4ee96U, 0x9e1b9b91U, 0x4f80c0c5U, 0xa261dc20U, 0x695a774bU, 0x161c121aU,
    0x0ae293baU, 0xe5c0a02aU, 0x433c22e0U, 0x1d121b17U, 0x0b0e090dU, 0xadf28bc7U, 0xb92db6a8U, 0xc8141ea9U,
    0x8557f119U, 0x4caf7507U, 0xbbee99ddU, 0xfda37f60U, 0x9ff70126U, 0xbc5c72f5U, 0xc544663bU, 0x345bfb7eU,
    0x768b4329U, 0xdccb23c6U, 0x68b6edfcU, 0x63b8e4f1U, 0xcad731dcU, 0x10426385U, 0x40139722U, 0x2084c611U,
    0x7d854a24U, 0xf8d2bb3dU, 0x11aef932U, 0x6dc729a1U, 0x4b1d9e2fU, 0xf3dcb230U, 0xec0d8652U, 0xd077c1e3U,
    0x6c2bb316U, 0x99a970b9U, 0xfa119448U, 0x2247e964U, 0xc4a8fc8cU, 0x1aa0f03fU, 0xd8567d2cU, 0xef223390U,
    0xc787494eU, 0xc1d938d1U, 0xfe8ccaa2U, 0x3698d40bU, 0xcfa6f581U, 0x28a57adeU, 0x26dab78eU, 0xa43fadbfU,
    0xe42c3a9dU, 0x0d507892U, 0x9b6a5fccU, 0x62547e46U, 0xc2f68d13U, 0xe890d8b8U, 0x5e2e39f7U, 0xf582c3afU,
    0xbe9f5d80U, 0x7c69d093U, 0xa96fd52dU, 0xb3cf2512U, 0x3bc8ac99U, 0xa710187dU, 0x6ee89c63U, 0x7bdb3bbbU,
    0x09cd2678U, 0xf46e5918U, 0x01ec9ab7U, 0xa8834f9aU, 0x65e6956eU, 0x7eaaffe6U, 0x0821bccfU, 0xe6ef15e8U,
    0xd9bae79bU, 0xce4a6f36U, 0xd4ea9f09U, 0xd629b07cU, 0xaf31a4b2U, 0x312a3f23U, 0x30c6a594U, 0xc035a266U,
    0x37744ebcU, 0xa6fc82caU, 0xb0e090d0U, 0x1533a7d8U, 0x4af10498U, 0xf741ecdaU, 0x0e7fcd50U, 0x2f1791f6U,
    0x8d764dd6U, 0x4d43efb0U, 0x54ccaa4dU, 0xdfe49604U, 0xe39ed1b5U, 0x1b4c6a88U, 0xb8c12c1fU, 0x7f466551U,
    0x049d5eeaU, 0x5d018c35U, 0x73fa8774U, 0x2efb0b41U, 0x5ab3671dU, 0x5292dbd2U, 0x33e91056U, 0x136dd647U,
    0x8c9ad761U, 0x7a37a10cU, 0x8e59f814U, 0x89eb133cU, 0xeecea927U, 0x35b761c9U, 0xede11ce5U, 0x3c7a47b1U,
    0x599cd2dfU, 0x3f55f273U, 0x791814ceU, 0xbf73c737U, 0xea53f7cdU, 0x5b5ffdaaU, 0x14df3d6fU, 0x867844dbU,
    0x81caaff3U, 0x3eb968c4U, 0x2c382434U, 0x5fc2a340U, 0x72161dc3U, 0x0cbce225U, 0x8b283c49U, 0x41ff0d95U,
    0x7139a801U, 0xde080cb3U, 0x9cd8b4e4U, 0x906456c1U, 0x617bcb84U, 0x70d532b6U, 0x74486c5cU, 0x42d0b857U,
};

int rijndaelKeySetupEnc(u32 *rk, const u8 *cipherKey, int keyBits) {
    int i = 0;
    u32 temp;

    rk[0] = GETU32(cipherKey);
    rk[1] = GETU32(cipherKey + 4);
    rk[2] = GETU32(cipherKey + 8);
    rk[3] = GETU32(cipherKey + 12);
    if (keyBits == 128) {
        for (;;) {
            temp = rk[3];
            rk[4] = rk[0] ^ (Te4[(temp >> 16) & 0xff] & 0xff000000) ^
                    (Te4[(temp >> 8) & 0xff] & 0x00ff0000) ^
                    (Te4[(temp) & 0xff] & 0x0000ff00) ^
                    (Te4[(temp >> 24)] & 0x000000ff) ^ rcon[i];
            rk[5] = rk[1] ^ rk[4];
            rk[6] = rk[2] ^ rk[5];
            rk[7] = rk[3] ^ rk[6];
            if (++i == 10) {
                return 10;
            }
            rk += 4;
        }
    }
    rk[4] = GETU32(cipherKey + 16);
    rk[5] = GETU32(cipherKey + 20);
    if (keyBits == 192) {
        for (;;) {
            temp = rk[5];
            rk[6] = rk[0] ^ (Te4[(temp >> 16) & 0xff] & 0xff000000) ^
                    (Te4[(temp >> 8) & 0xff] & 0x00ff0000) ^
                    (Te4[(temp) & 0xff] & 0x0000ff00) ^
                    (Te4[(temp >> 24)] & 0x000000ff) ^ rcon[i];
            rk[7] = rk[1] ^ rk[6];
            rk[8] = rk[2] ^ rk[7];
            rk[9] = rk[3] ^ rk[8];
            if (++i == 8) {
                return 12;
            }
            rk[10] = rk[4] ^ rk[9];
            rk[11] = rk[5] ^ rk[10];
            rk += 6;
        }
    }
    rk[6] = GETU32(cipherKey + 24);
    rk[7] = GETU32(cipherKey + 28);
    if (keyBits == 256) {
        for (;;) {
            temp = rk[7];
            rk[8] = rk[0] ^ (Te4[(temp >> 16) & 0xff] & 0xff000000) ^
                    (Te4[(temp >> 8) & 0xff] & 0x00ff0000) ^
                    (Te4[(temp) & 0xff] & 0x0000ff00) ^
                    (Te4[(temp >> 24)] & 0x000000ff) ^ rcon[i];
            rk[9] = rk[1] ^ rk[8];
            rk[10] = rk[2] ^ rk[9];
            rk[11] = rk[3] ^ rk[10];
            if (++i == 7) {
                return 14;
            }
            temp = rk[11];
            rk[12] = rk[4] ^ (Te4[(temp >> 24)] & 0xff000000) ^
                     (Te4[(temp >> 16) & 0xff] & 0x00ff0000) ^
                     (Te4[(temp >> 8) & 0xff] & 0x0000ff00) ^
                     (Te4[(temp) & 0xff] & 0x000000ff);
            rk[13] = rk[5] ^ rk[12];
            rk[14] = rk[6] ^ rk[13];
            rk[15] = rk[7] ^ rk[14];
            rk += 8;
        }
    }
    return 0;
}

int rijndaelKeySetupDec(u32 *rk, const u8 *cipherKey, int keyBits) {
    int Nr;
    int i;
    int j;
    u32 temp;

    Nr = rijndaelKeySetupEnc(rk, cipherKey, keyBits);
    for (i = 0, j = 4 * Nr; i < j; i += 4, j -= 4) {
        temp = rk[i];
        rk[i] = rk[j];
        rk[j] = temp;
        temp = rk[i + 1];
        rk[i + 1] = rk[j + 1];
        rk[j + 1] = temp;
        temp = rk[i + 2];
        rk[i + 2] = rk[j + 2];
        rk[j + 2] = temp;
        temp = rk[i + 3];
        rk[i + 3] = rk[j + 3];
        rk[j + 3] = temp;
    }
    for (i = 1; i < Nr; i++) {
        rk += 4;
        rk[0] = Td0[Te4[(rk[0] >> 24)] & 0xff] ^
                Td1[Te4[(rk[0] >> 16) & 0xff] & 0xff] ^
                Td2[Te4[(rk[0] >> 8) & 0xff] & 0xff] ^
                Td3[Te4[(rk[0]) & 0xff] & 0xff];
        rk[1] = Td0[Te4[(rk[1] >> 24)] & 0xff] ^
                Td1[Te4[(rk[1] >> 16) & 0xff] & 0xff] ^
                Td2[Te4[(rk[1] >> 8) & 0xff] & 0xff] ^
                Td3[Te4[(rk[1]) & 0xff] & 0xff];
        rk[2] = Td0[Te4[(rk[2] >> 24)] & 0xff] ^
                Td1[Te4[(rk[2] >> 16) & 0xff] & 0xff] ^
                Td2[Te4[(rk[2] >> 8) & 0xff] & 0xff] ^
                Td3[Te4[(rk[2]) & 0xff] & 0xff];
        rk[3] = Td0[Te4[(rk[3] >> 24)] & 0xff] ^
                Td1[Te4[(rk[3] >> 16) & 0xff] & 0xff] ^
                Td2[Te4[(rk[3] >> 8) & 0xff] & 0xff] ^
                Td3[Te4[(rk[3]) & 0xff] & 0xff];
    }
    return Nr;
}

void rijndaelEncrypt(const u32 *rk, int Nr, const u8 *pt, u8 *ct) {
    u32 s0, s1, s2, s3, t0, t1, t2, t3;
    int r;

    s0 = GETU32(pt) ^ rk[0];
    s1 = GETU32(pt + 4) ^ rk[1];
    s2 = GETU32(pt + 8) ^ rk[2];
    s3 = GETU32(pt + 12) ^ rk[3];
    r = Nr >> 1;
    for (;;) {
        t0 = Te0[(s0 >> 24)] ^ Te1[(s1 >> 16) & 0xff] ^
             Te2[(s2 >> 8) & 0xff] ^ Te3[(s3) & 0xff] ^ rk[4];
        t1 = Te0[(s1 >> 24)] ^ Te1[(s2 >> 16) & 0xff] ^
             Te2[(s3 >> 8) & 0xff] ^ Te3[(s0) & 0xff] ^ rk[5];
        t2 = Te0[(s2 >> 24)] ^ Te1[(s3 >> 16) & 0xff] ^
             Te2[(s0 >> 8) & 0xff] ^ Te3[(s1) & 0xff] ^ rk[6];
        t3 = Te0[(s3 >> 24)] ^ Te1[(s0 >> 16) & 0xff] ^
             Te2[(s1 >> 8) & 0xff] ^ Te3[(s2) & 0xff] ^ rk[7];
        rk += 8;
        if (--r == 0) {
            break;
        }
        s0 = Te0[(t0 >> 24)] ^ Te1[(t1 >> 16) & 0xff] ^
             Te2[(t2 >> 8) & 0xff] ^ Te3[(t3) & 0xff] ^ rk[0];
        s1 = Te0[(t1 >> 24)] ^ Te1[(t2 >> 16) & 0xff] ^
             Te2[(t3 >> 8) & 0xff] ^ Te3[(t0) & 0xff] ^ rk[1];
        s2 = Te0[(t2 >> 24)] ^ Te1[(t3 >> 16) & 0xff] ^
             Te2[(t0 >> 8) & 0xff] ^ Te3[(t1) & 0xff] ^ rk[2];
        s3 = Te0[(t3 >> 24)] ^ Te1[(t0 >> 16) & 0xff] ^
             Te2[(t1 >> 8) & 0xff] ^ Te3[(t2) & 0xff] ^ rk[3];
    }
    s0 = (Te4[(t0 >> 24)] & 0xff000000) ^ (Te4[(t1 >> 16) & 0xff] & 0x00ff0000) ^
         (Te4[(t2 >> 8) & 0xff] & 0x0000ff00) ^ (Te4[(t3) & 0xff] & 0x000000ff) ^ rk[0];
    PUTU32(ct, s0);
    s1 = (Te4[(t1 >> 24)] & 0xff000000) ^ (Te4[(t2 >> 16) & 0xff] & 0x00ff0000) ^
         (Te4[(t3 >> 8) & 0xff] & 0x0000ff00) ^ (Te4[(t0) & 0xff] & 0x000000ff) ^ rk[1];
    PUTU32(ct + 4, s1);
    s2 = (Te4[(t2 >> 24)] & 0xff000000) ^ (Te4[(t3 >> 16) & 0xff] & 0x00ff0000) ^
         (Te4[(t0 >> 8) & 0xff] & 0x0000ff00) ^ (Te4[(t1) & 0xff] & 0x000000ff) ^ rk[2];
    PUTU32(ct + 8, s2);
    s3 = (Te4[(t3 >> 24)] & 0xff000000) ^ (Te4[(t0 >> 16) & 0xff] & 0x00ff0000) ^
         (Te4[(t1 >> 8) & 0xff] & 0x0000ff00) ^ (Te4[(t2) & 0xff] & 0x000000ff) ^ rk[3];
    PUTU32(ct + 12, s3);
}

void rijndaelDecrypt(const u32 *rk, int Nr, const u8 *ct, u8 *pt) {
    u32 s0, s1, s2, s3, t0, t1, t2, t3;
    int r;

    s0 = GETU32(ct) ^ rk[0];
    s1 = GETU32(ct + 4) ^ rk[1];
    s2 = GETU32(ct + 8) ^ rk[2];
    s3 = GETU32(ct + 12) ^ rk[3];
    r = Nr >> 1;
    for (;;) {
        t0 = Td0[(s0 >> 24)] ^ Td1[(s3 >> 16) & 0xff] ^
             Td2[(s2 >> 8) & 0xff] ^ Td3[(s1) & 0xff] ^ rk[4];
        t1 = Td0[(s1 >> 24)] ^ Td1[(s0 >> 16) & 0xff] ^
             Td2[(s3 >> 8) & 0xff] ^ Td3[(s2) & 0xff] ^ rk[5];
        t2 = Td0[(s2 >> 24)] ^ Td1[(s1 >> 16) & 0xff] ^
             Td2[(s0 >> 8) & 0xff] ^ Td3[(s3) & 0xff] ^ rk[6];
        t3 = Td0[(s3 >> 24)] ^ Td1[(s2 >> 16) & 0xff] ^
             Td2[(s1 >> 8) & 0xff] ^ Td3[(s0) & 0xff] ^ rk[7];
        rk += 8;
        if (--r == 0) {
            break;
        }
        s0 = Td0[(t0 >> 24)] ^ Td1[(t3 >> 16) & 0xff] ^
             Td2[(t2 >> 8) & 0xff] ^ Td3[(t1) & 0xff] ^ rk[0];
        s1 = Td0[(t1 >> 24)] ^ Td1[(t0 >> 16) & 0xff] ^
             Td2[(t3 >> 8) & 0xff] ^ Td3[(t2) & 0xff] ^ rk[1];
        s2 = Td0[(t2 >> 24)] ^ Td1[(t1 >> 16) & 0xff] ^
             Td2[(t0 >> 8) & 0xff] ^ Td3[(t3) & 0xff] ^ rk[2];
        s3 = Td0[(t3 >> 24)] ^ Td1[(t2 >> 16) & 0xff] ^
             Td2[(t1 >> 8) & 0xff] ^ Td3[(t0) & 0xff] ^ rk[3];
    }
    s0 = (Td4[(t0 >> 24)] & 0xff000000) ^ (Td4[(t3 >> 16) & 0xff] & 0x00ff0000) ^
         (Td4[(t2 >> 8) & 0xff] & 0x0000ff00) ^ (Td4[(t1) & 0xff] & 0x000000ff) ^ rk[0];
    PUTU32(pt, s0);
    s1 = (Td4[(t1 >> 24)] & 0xff000000) ^ (Td4[(t0 >> 16) & 0xff] & 0x00ff0000) ^
         (Td4[(t3 >> 8) & 0xff] & 0x0000ff00) ^ (Td4[(t2) & 0xff] & 0x000000ff) ^ rk[1];
    PUTU32(pt + 4, s1);
    s2 = (Td4[(t2 >> 24)] & 0xff000000) ^ (Td4[(t1 >> 16) & 0xff] & 0x00ff0000) ^
         (Td4[(t0 >> 8) & 0xff] & 0x0000ff00) ^ (Td4[(t3) & 0xff] & 0x000000ff) ^ rk[2];
    PUTU32(pt + 8, s2);
    s3 = (Td4[(t3 >> 24)] & 0xff000000) ^ (Td4[(t2 >> 16) & 0xff] & 0x00ff0000) ^
         (Td4[(t1 >> 8) & 0xff] & 0x0000ff00) ^ (Td4[(t0) & 0xff] & 0x000000ff) ^ rk[3];
    PUTU32(pt + 12, s3);
}

void MY_MD5Init(MD5_CTX *context) {
    context->count[0] = context->count[1] = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xefcdab89;
    context->state[2] = 0x98badcfe;
    context->state[3] = 0x10325476;
}

void MY_MD5Update(MD5_CTX *context, u8 *input, u32 inputLen) {
    u32 i;
    u32 index;
    u32 partLen;

    index = (context->count[0] >> 3) & 0x3F;
    if ((context->count[0] += inputLen << 3) < (inputLen << 3)) {
        context->count[1]++;
    }
    context->count[1] += inputLen >> 29;
    partLen = 64 - index;
    if (inputLen >= partLen) {
        MD5_memcpy(&context->buffer[index], input, partLen);
        MD5Transform(context->state, context->buffer);
        for (i = partLen; i + 63 < inputLen; i += 64) {
            MD5Transform(context->state, &input[i]);
        }
        index = 0;
    } else {
        i = 0;
    }
    MD5_memcpy(&context->buffer[index], &input[i], inputLen - i);
}

void MY_MD5Final(u8 *digest, MD5_CTX *context) {
    u8 bits[8];
    u32 index;
    u32 padLen;

    Encode(bits, context->count, 8);
    index = (context->count[0] >> 3) & 0x3f;
    padLen = (index < 56) ? (56 - index) : (120 - index);
    MY_MD5Update(context, PADDING, padLen);
    MY_MD5Update(context, bits, 8);
    Encode(digest, context->state, 16);
    MD5_memset((u8 *)context, 0, sizeof(*context));
}

void MD5Transform(u32 *state, u8 *block) {
    u32 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

    Decode(x, block, 64);

    // Round 1
    FF(a, b, c, d, x[0], S11, 0xd76aa478);
    FF(d, a, b, c, x[1], S12, 0xe8c7b756);
    FF(c, d, a, b, x[2], S13, 0x242070db);
    FF(b, c, d, a, x[3], S14, 0xc1bdceee);
    FF(a, b, c, d, x[4], S11, 0xf57c0faf);
    FF(d, a, b, c, x[5], S12, 0x4787c62a);
    FF(c, d, a, b, x[6], S13, 0xa8304613);
    FF(b, c, d, a, x[7], S14, 0xfd469501);
    FF(a, b, c, d, x[8], S11, 0x698098d8);
    FF(d, a, b, c, x[9], S12, 0x8b44f7af);
    FF(c, d, a, b, x[10], S13, 0xffff5bb1);
    FF(b, c, d, a, x[11], S14, 0x895cd7be);
    FF(a, b, c, d, x[12], S11, 0x6b901122);
    FF(d, a, b, c, x[13], S12, 0xfd987193);
    FF(c, d, a, b, x[14], S13, 0xa679438e);
    FF(b, c, d, a, x[15], S14, 0x49b40821);

    // Round 2
    GG(a, b, c, d, x[1], S21, 0xf61e2562);
    GG(d, a, b, c, x[6], S22, 0xc040b340);
    GG(c, d, a, b, x[11], S23, 0x265e5a51);
    GG(b, c, d, a, x[0], S24, 0xe9b6c7aa);
    GG(a, b, c, d, x[5], S21, 0xd62f105d);
    GG(d, a, b, c, x[10], S22, 0x2441453);
    GG(c, d, a, b, x[15], S23, 0xd8a1e681);
    GG(b, c, d, a, x[4], S24, 0xe7d3fbc8);
    GG(a, b, c, d, x[9], S21, 0x21e1cde6);
    GG(d, a, b, c, x[14], S22, 0xc33707d6);
    GG(c, d, a, b, x[3], S23, 0xf4d50d87);
    GG(b, c, d, a, x[8], S24, 0x455a14ed);
    GG(a, b, c, d, x[13], S21, 0xa9e3e905);
    GG(d, a, b, c, x[2], S22, 0xfcefa3f8);
    GG(c, d, a, b, x[7], S23, 0x676f02d9);
    GG(b, c, d, a, x[12], S24, 0x8d2a4c8a);

    // Round 3
    HH(a, b, c, d, x[5], S31, 0xfffa3942);
    HH(d, a, b, c, x[8], S32, 0x8771f681);
    HH(c, d, a, b, x[11], S33, 0x6d9d6122);
    HH(b, c, d, a, x[14], S34, 0xfde5380c);
    HH(a, b, c, d, x[1], S31, 0xa4beea44);
    HH(d, a, b, c, x[4], S32, 0x4bdecfa9);
    HH(c, d, a, b, x[7], S33, 0xf6bb4b60);
    HH(b, c, d, a, x[10], S34, 0xbebfbc70);
    HH(a, b, c, d, x[13], S31, 0x289b7ec6);
    HH(d, a, b, c, x[0], S32, 0xeaa127fa);
    HH(c, d, a, b, x[3], S33, 0xd4ef3085);
    HH(b, c, d, a, x[6], S34, 0x4881d05);
    HH(a, b, c, d, x[9], S31, 0xd9d4d039);
    HH(d, a, b, c, x[12], S32, 0xe6db99e5);
    HH(c, d, a, b, x[15], S33, 0x1fa27cf8);
    HH(b, c, d, a, x[2], S34, 0xc4ac5665);

    // Round 4
    II(a, b, c, d, x[0], S41, 0xf4292244);
    II(d, a, b, c, x[7], S42, 0x432aff97);
    II(c, d, a, b, x[14], S43, 0xab9423a7);
    II(b, c, d, a, x[5], S44, 0xfc93a039);
    II(a, b, c, d, x[12], S41, 0x655b59c3);
    II(d, a, b, c, x[3], S42, 0x8f0ccc92);
    II(c, d, a, b, x[10], S43, 0xffeff47d);
    II(b, c, d, a, x[1], S44, 0x85845dd1);
    II(a, b, c, d, x[8], S41, 0x6fa87e4f);
    II(d, a, b, c, x[15], S42, 0xfe2ce6e0);
    II(c, d, a, b, x[6], S43, 0xa3014314);
    II(b, c, d, a, x[13], S44, 0x4e0811a1);
    II(a, b, c, d, x[4], S41, 0xf7537e82);
    II(d, a, b, c, x[11], S42, 0xbd3af235);
    II(c, d, a, b, x[2], S43, 0x2ad7d2bb);
    II(b, c, d, a, x[9], S44, 0xeb86d391);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    MD5_memset((u8 *)x, 0, sizeof(x));
}

void Encode(u8 *output, u32 *input, u32 len) {
    u32 i;
    u32 j;

    for (i = 0, j = 0; j < len; i++, j += 4) {
        output[j] = (u8)(input[i] & 0xff);
        output[j + 1] = (u8)((input[i] >> 8) & 0xff);
        output[j + 2] = (u8)((input[i] >> 16) & 0xff);
        output[j + 3] = (u8)((input[i] >> 24) & 0xff);
    }
}

void Decode(u32 *output, u8 *input, u32 len) {
    u32 i;
    u32 j;

    for (i = 0, j = 0; j < len; i++, j += 4) {
        output[i] = ((u32)input[j]) | (((u32)input[j + 1]) << 8) | (((u32)input[j + 2]) << 16) | (((u32)input[j + 3]) << 24);
    }
}

void MD5_memcpy(u8 *output, u8 *input, u32 len) {
    u32 i;

    for (i = 0; i < len; i++) {
        output[i] = input[i];
    }
}

void MD5_memset(u8 *output, int value, u32 len) {
    u32 i;

    for (i = 0; i < len; i++) {
        ((char *)output)[i] = (char)value;
    }
}

void MD5Encode(u8 *digest, u8 *input, u32 len) {
    MD5_CTX context;

    MY_MD5Init(&context);
    MY_MD5Update(&context, input, len);
    MY_MD5Final(digest, &context);
}

u64 GetTickCount(void) {
    u64 tick;

    tick = OS_GetTick();
    tick = __aeabi_uldivmod((u32)(tick << 6), (u32)((tick << 6) >> 0x20), 0x82ea, 0);
    return tick;
}

void *Calloc(s32 nmemb, s32 size) {
    void *dest;

        dest = gAtermAlloc(nmemb * size);
    if (dest != NULL) {
        memset(dest, 0, size * nmemb);
    }
    return dest;
}

void Free_atermset(void *ptr) {
    gAtermFree(ptr);
}


s32 PutHex(s8 *lpwrite, s32 ch) {
    s32 i;
    s32 val;
    s8 *str;
    u32 val2;
    u8 val3;

    val3 = ch;
    str = lpwrite;
    val = (s32) (0xF0 & val3) >> 4;
    i = 0;
    do {
        if (val <= 9) {
            val2 = val + 0x30;
        } else {
            val2 = val + 0x37;
        }
        i += 1;
        *str = val2;
        val2 = 1;
        str += val2;
        val = 0xF & val3;
    } while (i < 2);
    *str = 0;
    return str - lpwrite;
}

int PutMac(char *out, const s8 *mac) {
    char *p = out;
    int i;

    for (i = 0; i < 6; i++) {
        p += PutHex(p, *mac++);
        if (i < 5) {
            *p++ = ':';
        }
    }
    *p = 0;
    return p - out;
}


s32 ATERMi_ApConfigStart(s32 prio, s32 maxap, void (*notify)(ATERMApConfigState *), void *(*alloc)(u32), void (*free)(void *), u32 stackSize) {
    s32 ret;

    if (gAtermApState >= 1 && gAtermApState <= 5) {
        return -10;
    }
    gMaxBssIdList = maxap;
    gAtermApState = 7;
    gAtermNotify = notify;
    gAtermAlloc = alloc;
    gAtermFree = free;
    gThreadStackSize = stackSize;
    ret = InitAtermApcNetwork(maxap);
    bInitAtermApNetwork = 1;
    if (ret < 0) {
        gAtermApResult = ret;
        return ret;
    }
    gThreadStack = gAtermAlloc(gThreadStackSize);
    if (gThreadStack == NULL) {
        gAtermApResult = -1;
        return -1;
    }
    if (OS_IsThreadAvailable() != TRUE) {
        gAtermApResult = -9;
        return -9;
    }
    OS_CreateThread(&gAtermThread, (void (*)(void *))AutoConfigThread, NULL,
                    (u8 *)gThreadStack + (gThreadStackSize & ~7),
                    gThreadStackSize, prio);
    gAtermApState = 1;
    gAtermApTimeout = (u32)GetTickCount() + 60000;
    bAtermApCanceled = 0;
    memset(&gAtermApConfigParam, 0, sizeof(ATERMApConfigParam));
    SendNotify();
    OS_WakeupThreadDirect(&gAtermThread);
    gAtermThreadId = 1;
    return 1;
}


u32 ATERMi_ApConfigEnd(void) {
    s32 retv;

    if (gAtermThreadId) {
        s32 laststate = gAtermApState;

        bAtermApCanceled = 1;
        while (gAtermApState >= 1 && gAtermApState <= 5) {
            OS_Sleep(100);
        }
        OS_Sleep(500);
        if (!OS_IsThreadTerminated(&gAtermThread)) {
            do {
                OS_WakeupThreadDirect(&gAtermThread);
                OS_JoinThread(&gAtermThread);
            } while (!OS_IsThreadTerminated(&gAtermThread));
        }
        if (gThreadStack != NULL) {
            gAtermFree(gThreadStack);
            gThreadStack = NULL;
        }
        gAtermThreadId = 0;
        if (laststate != gAtermApState) {
            SendNotify();
        }
    }
    if (bInitAtermApNetwork > 0) {
        retv = FreeAtermApcNetwork();
        bInitAtermApNetwork = 0;
        return retv;
    }
    return -10;
}

s32 ATERMi_ApConfigGetState(ATERMApConfigState *state) {
    s32 timeout = -1;

    state->state = gAtermApState;
    if (gAtermApTimeout == timeout) {
        state->timeout = timeout;
    } else {
        timeout = gAtermApTimeout - (s32)GetTickCount();
        state->timeout = timeout;
    }
    state->result = gAtermApResult;
    return 1;
}

u32 ATERMi_ApConfigGetResult(void *param) {
    memcpy(param, (void *)&gAtermApConfigParam, 0xe8);
    return 1;
}

static inline void func_ov016_021bc804_get(ATERMApConfigState *buf) {
    ATERMi_ApConfigGetState(buf);
}

void SendNotify(void) {
    ATERMApConfigState auStack_10;

    ATERMi_ApConfigGetState(&auStack_10);
    gAtermNotify(&auStack_10);
}
