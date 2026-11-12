// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/auth.h: its types and constants,
// and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_AUTH_H
#define BWPORT_DWC_AUTH_H

#include "types.h"
#include "dwc/soap.h"
#include "dwc/http.h"

// The player's certificate, as the service writes it. `valid` tells that the signature is right (it is made by the
// check, not read); the signature is over the fields in between and the server data
typedef struct AuthCertificate {
    int valid;
    int length;
    int version;
    int partnerCode;
    int namespaceId;
    int userId;
    int profileId;
    int expireTime;
    char profileNick[0x1f];
    char uniqueNick[0x15];
    char cdkeyHash[0x21];
    BigNum peerKeyModulus;
    BigNum peerKeyExponent;
    u8 signature[0x80];
    u8 serverData[0x80];
} AuthCertificate;

// The key pair of the session: the modulus and the private key, and the hash of the private key that is the proof
typedef struct AuthKeys {
    BigNum modulus;
    BigNum privateKey;
    u8 proof[0x10];
} AuthKeys;

// What the login brings: the error (0, or one of the codes below), the answer's response code, the certificate and
// the keys
typedef struct AuthResult {
    int error;
    int responseCode;
    AuthCertificate certificate;
    AuthKeys keys;
    int unk_5a0;
} AuthResult;

// The errors of the login that are the library's own: 0x64 for an answer that could not be taken, up to 0x6b for a
// canceled request
typedef enum AuthError {
    AUTH_ERROR_NONE = 0,
    AUTH_ERROR_BAD_ANSWER = 0x64,
    AUTH_ERROR_BAD_RESPONSE = 0x65,
    AUTH_ERROR_BAD_CERTIFICATE = 0x66,
    AUTH_ERROR_NO_MEMORY = 0x68,
    AUTH_ERROR_NOT_AVAILABLE = 0x6a,
    AUTH_ERROR_CANCELED = 0x6b,
} AuthError;

// Called when the login has ended, with the status of the request, the result and the user data
typedef void (*AuthResultFunc)(int status, AuthResult *result, void *userData);

#define DWCAUTH_INGAMESN_LEN (25 + 1)
#define DWCAUTH_GSBRCDBYTES (11 + 1)

// What the NAS answered: the returncd, the time, the locator, the token and challenge the login uses, a cookie
// (DWCAuthResult of the SDK)
typedef struct DWCAuthResult {
    int returncode;
    char retcode[3 + 1];
    char datetime[14 + 1];
    char locator[50 + 1];
    char token[300 + 1];
    char challenge[8 + 1];
    char cookie[64 + 1];
} DWCAuthResult;

// What DWC_Auth_Create copies into its work: the in-game screen name, the branch code and the allocator
// (DWCAuthParam of the SDK)
typedef struct DWCAuthParam {
    u16 ingamesn[DWCAUTH_INGAMESN_LEN];
    char gsbrcd[DWCAUTH_GSBRCDBYTES];
    DWCHttpAllocFunc alloc;
    DWCHttpFreeFunc free;
} DWCAuthParam;

u32 DWC_Auth_Create(DWCAuthParam *param, void *http);
void DWC_Auth_Destroy(void);
void DWC_Auth_Abort(void);
void DWC_Auth_Join(void);
u32 DWC_Auth_GetError(void);
void DWC_Auth_GetResult(DWCAuthResult *result);
u32 DWC_Auth_GetCalInfoFromNVRAM(u8 *info);
// The request's calibration info (dwc_cal.c's record) from a Wi-Fi info
typedef struct NasAuthInfo NasAuthInfo;
struct DWCWiFiInfo;
BOOL DWC_Auth_GetCalInfoFromWiFiInfo(NasAuthInfo *info, struct DWCWiFiInfo *wifi);
BOOL DWC_Auth_SetCalInfoToHttp(DWCHttp *http, const void *info, u32 flag);
// dwc_netcheck.o's flag the other requests pass DWC_Auth_SetCalInfoToHttp
extern u32 data_ov009_021ace70;
// dwc_auth.o's request parameters (the NAS url, the buffer size, a timeout)
extern DWCHttpParam data_ov009_021a2358;
void DWC_Auth_SetCustomNas(const char *nasaddr);
const char *func_ov009_02177ed8(void);
BOOL func_ov009_02178d14(const u16 *ingamesn, const char *gsbrcd, u64 param_3, DWCHttpAllocFunc alloc, DWCHttpFreeFunc free);
void func_ov009_02178ea0(char *token, char *challenge);
s64 func_ov009_02178f00(void);

#endif // BWPORT_DWC_AUTH_H
