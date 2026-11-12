// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/ssl/ssl.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/cps.h"
#include "nitro/mi.h"
#include "ssl_internal.h"
#include "system/rtc.h"
#include "gfl/std.h"
#include "dwc/ext.h"

// NitroWiFi's SSL layer (SSLv3-like: RC4 with MD5 or SHA1 MACs, RSA key exchange), in overlay 11. No swan name for the
// object or any of its functions; named for their role, after pokediamond's SSL_ssl.o lists the same ones (structure
// reference only, not copied). A connection's own records sit after the socket's TCP ones at soc->con.

// A cache entry remembers a session's id and secret by the peer that offered it, so a later connection can skip the
// full handshake. 4 entries, cleared by CPSi_SslCleanup
// Black 2's, not checked against Black
typedef struct SSLSessionCacheEntry {
    u8 session_id[0x20];
    u8 secret[0x30];
    s32 time; // OS_GetTick() >> 16 when the entry was last used
    CPSInAddr ip;
    u16 port;
    u8 valid;
} SSLSessionCacheEntry;

// A certificate field, or another named value a caller registered on the current thread's connection with
// CPS_SetRootCa, found again by name with get_rootCA
// Black 2's, not checked against Black
typedef struct SSLNamedParam {
    const char *name;
} SSLNamedParam;

// File-local descriptive names; no original SDK type names have been established.
// Black 2's, not checked against Black
typedef struct SSLCertificatePublicKey {
    u8 modulus[0x100];
    s32 modulus_len;
    u8 exponent[8];
    s32 exponent_len;
} SSLCertificatePublicKey;

// Black 2's, not checked against Black
typedef struct SSLRsaPublicKey {
    const char *name;
    s32 modulus_len;
    const u8 *modulus;
    s32 exponent_len;
    const u8 *exponent;
} SSLRsaPublicKey;

// RSA-CRT private key: six alternating length/pointer pairs observed in the original function.
// First/second names avoid assuming a particular p/q ordering for the caller's key material.
typedef struct SSLRsaPrivateKey {
    s32 modulus_len;
    const u8 *modulus;
    s32 first_prime_len;
    const u8 *first_prime;
    s32 second_prime_len;
    const u8 *second_prime;
    s32 first_exponent_len;
    const u8 *first_exponent;
    s32 second_exponent_len;
    const u8 *second_exponent;
    s32 coefficient_len;
    const u8 *coefficient;
} SSLRsaPrivateKey;

// A connection, DER-parsed certificate fields included. 0x830 bytes
// Black 2's; twinned code confirms the offsets of secret, resumed, unk_031, client_random, read_mac_secret, read_rc4,
// handshake_sha1, handshake_sha1_snapshot, handshake_md5, handshake_md5_snapshot, is_server, state,
// application_record_ready, named_params, named_param_count, key_param, read_buf, read_buf_len and read_buf_pos in
// Black
typedef struct SSLContext {
    u8 secret[0x30]; // The pre-master secret, overwritten with the master secret once derived
    u8 resumed;       // TRUE once a cached session's secret was restored
    u8 unk_031;
    u16 cipher_suite;
    u8 client_random[0x20];
    u8 server_random[0x20];
    u8 session_id[0x20];
    u8 unk_094[0x28];
    u8 *write_mac_secret;
    u8 *write_key;
    u8 unk_0c4[4];
    CPSRc4Context write_rc4;
    u8 unk_1ca[2];
    u8 write_seq_num[8]; // The 8-byte big-endian sequence number make_ciphertext folds into an outgoing record's MAC
    u8 *read_mac_secret;
    u8 *read_key;
    u8 unk_1dc[4];
    CPSRc4Context read_rc4;
    u8 unk_2e2[2];
    u8 read_seq_num[8]; // Same, for a received record's MAC; both start at 0 and add1_be8 advances them
    SHA1Context handshake_sha1;          // The running hash of every handshake message so far
    SHA1Context handshake_sha1_snapshot; // Copied for Finished; the snapshot contexts also serve as certificate-hash
                                         // scratch storage
    MD5Context handshake_md5;
    MD5Context handshake_md5_snapshot;
    u8 is_server; // Descriptive role name: nonzero selects the server handshake path
    u8 state;     // SSLI_STATE_*, read back by parse_record's caller
    u8 application_record_ready; // TRUE when read_buf holds a complete application record retained by dispatch
    u8 pad_457;
    s32 signature_algorithm; // 0x458: OID index 3 (MD5) or 4 (SHA1)
    s32 public_key_algorithm;
    const u8 *signed_start; // Borrowed DER span, valid while the input certificate buffer is owned by the caller
    const u8 *signed_end;
    u8 certificate_digest[0x14];
    u32 certificate_digest_len;
    SSLRsaPublicKey chain_key;               // 0x480; borrowed next-certificate key spans, with the name slot unused
    SSLCertificatePublicKey certificate_key; // 0x494, ends at 0x5a4
    const u8 *certificate_signature;
    s32 certificate_signature_len;
    // One byte reused across phases, not two independent flags.
    union {
        u8 parsing_subject;
        u8 certificate_requested;
    } certificate_phase; // 0x5ac
    u8 parsing_rsa_key;
    u8 attribute_oid;
    u8 certificate_date_valid;
    char issuer[0x100];
    char subject[0x100];
    char common_name[0x50];
    const char *hostname;  // 0x800
    const u8 *certificate; // 0x804
    s32 certificate_len;
    u32 current_date;
    int (*validate_certificate)(int result, struct SSLContext *ctx, int index);
    SSLNamedParam *const *named_params; // The caller's registered list, set with CPS_SetRootCa
    u32 named_param_count;
    void *key_param; // Private key consumed by rcv_client_key_exchange; its writer is not recovered
    u8 unk_820[4];
    u8 *read_buf; // Owned incomplete record, or a dispatched application record awaiting consumption
    u32 read_buf_len;
    u32 read_buf_pos;
} SSLContext;

// SSLContext.state
enum {
    SSLI_STATE_INITIAL = 0,
    SSLI_STATE_CLIENT_HELLO_PARSED,
    SSLI_STATE_SERVER_HELLO_PARSED,
    SSLI_STATE_CERTIFICATE_ACCEPTED,
    SSLI_STATE_SERVER_HELLO_DONE = 4,
    SSLI_STATE_MASTER_SECRET_READY = 5,
    SSLI_STATE_FINISHED_VERIFIED = 6,
    SSLI_STATE_EXPECT_FINISHED = 7,
    SSLI_STATE_APPLICATION_DATA = 8,
    SSLI_STATE_ERROR = 9,
};

// Certificate validation combines a low-byte result with these independent flags.
enum {
    SSLI_CERT_RESULT_MASK = 0xff,
    SSLI_CERT_HOSTNAME_MISMATCH = 0x4000,
    SSLI_CERT_DATE_INVALID = 0x8000,
};

// 4 entries, cleared by CPSi_SslCleanup (pokebw2's session)
static SSLSessionCacheEntry session[4];

// set_random's pool: a running SHA1 digest that func_ov009_02162248 stirs entropy into (pokebw2's pool_initialized, sPool)
static u8 pool_initialized;
static u8 pool[0x14];

// The record CPSi_SslWrite2 sends before ClientKeyExchange when the server asked for a certificate and the caller has
// none to offer: alert(fatal, no_certificate) (pokebw2's alert_nocert)
static const u8 alert_nocert[7] = { 0x15, 0x03, 0x00, 0x00, 0x02, 0x01, 0x29 };

// X.509 attribute and algorithm OIDs cert_item recognizes while walking a certificate, and one sentinel that never
// matches: the sentinel, rsaEncryption, 2.5.8.1.1, md5WithRSAEncryption, sha1WithRSAEncryption, commonName
// (pokebw2's object)
static u8 *object[6] = {
    (u8 *)"\xFF\xFF\xFF",
    (u8 *)"\x2A\x86\x48\x86\xF7\x0D\x01\x01\x01",
    (u8 *)"\x55\x08\x01\x01",
    (u8 *)"\x2A\x86\x48\x86\xF7\x0D\x01\x01\x04",
    (u8 *)"\x2A\x86\x48\x86\xF7\x0D\x01\x01\x05",
    (u8 *)"\x55\x04\x03",
};

// The cipher suites CPS_Sslconnect offers (SSL_RSA_WITH_RC4_128_MD5, SSL_RSA_WITH_RC4_128_SHA), and the maximum
// priority a connection's thread may run at while it works (CPS_SetSslHandshakePriority's role), -1 until set
// (pokebw2's sConfig)
static u32 ssl_handshake_priority = (u32)-1;
static u16 pairlist[2] = { 4, 5 };

static void func_ov009_0215fda4(SSLContext *ctx);
static void func_ov009_0215fe3c(SSLContext *ctx, CPSInAddr ip, u16 port);
static void func_ov009_0215fee0(SSLContext *ctx, CPSInAddr ip, u16 port);
static OSIntrMode func_ov009_0215ffd0(SSLContext *ctx);
static u32 func_ov009_0216003c(void);
static u32 enter_computebound(void);
static void exit_computebound(u32 priority);
static const SSLNamedParam *get_rootCA(const SSLContext *ctx, const char *name);
static s32 cert_item_len(const u8 **cursor);
static void make_dn(char *dest, const char *suffix, int repeat);
static u32 parse_time(const u8 *digits, int tag);
static int cert_item(SSLContext *ctx, const u8 **cursor, int depth, int index, int extra);
static int auth_cert(SSLContext *ctx);
static int chars_till_end(const char *host);
static int compare_fqdn(const char *host, const char *pattern);
static void rcv_certificate(SSLContext *ctx, const u8 *body);
static int validate_signature(SSLContext *ctx, const SSLRsaPublicKey *key);
static void rcv_server_hello(SSLContext *ctx, u8 *msg);
static BOOL has_method(const u8 *data, int count, int width, u32 value);
static int select_method(const u8 *data, int count, int width);
static BOOL version_ok(int major, int minor);
static void client_hello_v2(SSLContext *ctx, u8 *msg);
static void client_hello(SSLContext *ctx, u8 *msg);
static void decrypt_premaster_secret(SSLContext *ctx, u8 *encrypted, void *params);
static void create_ms_sub(u8 *out, const char *label, SSLContext *ctx);
static void create_master_secret(SSLContext *ctx);
static void create_key_block(SSLContext *ctx);
static void rcv_client_key_exchange(SSLContext *ctx, u8 *msg);
static void finished_md5(SSLContext *ctx, u8 *out, int sender);
static void finished_sha1(SSLContext *ctx, u8 *out, int sender);
static void rcv_finished(SSLContext *ctx, u8 *msg);
static void add1_be8(u8 *iv);
static u32 decrypt_ssl(SSLContext *ctx, void *buf, u32 len);
static u32 make_plaintext(SSLContext *ctx, u8 *hdr);
static u32 make_ciphertext(SSLContext *ctx, u8 *hdr);
static int tcp_read_raw_nbytes(u8 *dest, int len, CPSSoc *soc);
static void update_digest(SSLContext *ctx, const u8 *data, u32 len);
static void func_ov009_02161c80(SSLContext *ctx, u8 *record);
static int parse_record(CPSSoc *soc);
static void set_random(u8 *buf, int len);
static void func_ov009_021622c0(CPSSoc *soc);
static void func_ov009_02162418(CPSSoc *soc);
static void send_client_key_exchange(CPSSoc *soc);
static BOOL mustget_change_cipher_spec_and_finished(CPSSoc *soc);
static BOOL ssl_connect_try(CPSSoc *soc);
static void try_fill_record(CPSSoc *soc);

// Looks the ctx's session id up in the cache; restores its secret and marks the connection resumed on a hit
// pokebw2's SSLi_RestoreSessionById
static void func_ov009_0215fda4(SSLContext *ctx) {
    int i;
    SSLSessionCacheEntry *entry;
    u32 enabled = OS_DisableInterrupts();

    ctx->resumed = FALSE;
    for (i = 0; i < 4; i++) {
        entry = &session[i];
        if (entry->valid && entry->ip == 0 && entry->port == 0 && memcmp(entry, ctx->session_id, 0x20) == 0) {
            MI_CpuCopy8(entry->secret, ctx->secret, 0x30);
            ctx->resumed = TRUE;
            break;
        }
    }
    OS_RestoreInterrupts(enabled);
}

// Looks a cached session up by the peer it was made with, offering it (ctx->session_id, ctx->resumed) for the next
// ClientHello
// pokebw2's SSLi_FindSessionByPeer
static void func_ov009_0215fe3c(SSLContext *ctx, CPSInAddr ip, u16 port) {
    SSLSessionCacheEntry *entry;
    u32 enabled = OS_DisableInterrupts();
    int i;

    ctx->resumed = FALSE;
    for (i = 0; i < 4; i++) {
        entry = &session[i];
        if (entry->valid && entry->ip == ip && entry->port == port) {
            MI_CpuCopy8(entry->session_id, ctx->session_id, 0x20);
            MI_CpuCopy8(entry->secret, ctx->secret, 0x30);
            entry->time = (u32)(OS_GetTick() >> 16);
            ctx->resumed = TRUE;
            break;
        }
    }
    OS_RestoreInterrupts(enabled);
}

// Stores the connection's current session (id and secret) in the cache, reusing a peer hit, an invalid entry, or the
// oldest entry
// pokebw2's SSLi_SaveSession
static void func_ov009_0215fee0(SSLContext *ctx, CPSInAddr ip, u16 port) {
    int i;
    u32 bestAge;
    SSLSessionCacheEntry *entry;
    u32 enabled;
    SSLSessionCacheEntry *target;
    u32 now;
    u32 age;

    enabled = OS_DisableInterrupts();
    now = (u32)(OS_GetTick() >> 16);
    target = &session[0];
    bestAge = 0;

    for (i = 0; i < 4; i++) {
        entry = &session[i];
        if (entry->valid && ip != 0 && ip == entry->ip && port != 0 && port == entry->port) {
            target = entry;
            break;
        }
        if (bestAge != (u32)-1) {
            if (!entry->valid) {
                bestAge = (u32)-1;
                target = entry;
            } else {
                age = now - entry->time;
                if (age > bestAge) {
                    bestAge = age;
                    target = entry;
                }
            }
        }
    }

    MI_CpuCopy8(ctx->session_id, target->session_id, 0x20);
    MI_CpuCopy8(ctx->secret, target->secret, 0x30);
    target->time = now;
    target->valid = TRUE;
    target->ip = ip;
    target->port = port;
    OS_RestoreInterrupts(enabled);
}

// Marks the cache entry that offered this session id as no longer valid, once the handshake using it has failed
// pokebw2's SSLi_InvalidateSession
static OSIntrMode func_ov009_0215ffd0(SSLContext *ctx) {
    int i;
    SSLSessionCacheEntry *entry;
    u32 enabled = OS_DisableInterrupts();

    for (i = 0; i < 4; i++) {
        entry = &session[i];
        if (entry->valid && memcmp(entry, ctx->session_id, 0x20) == 0) {
            entry->valid = FALSE;
            break;
        }
    }
    return OS_RestoreInterrupts(enabled);
}

// A Unix-ish timestamp for the first 4 bytes of a client random
// pokebw2's SSLi_GetUnixTime
static u32 func_ov009_0216003c(void) {
    RTCDate date;
    RTCTime time;

    RTC_GetDate(&date);
    RTC_GetTime(&time);
    return (u32)RTC_ConvertDateTimeToSecond(&date, &time) + 0x386d4380;
}

// CPS_GetSslHandshakePriority's role: the priority CPS_SetSslHandshakePriority last gave, or -1
// pokebw2's CPS_SslGetMaxPriority
u32 CPS_GetSslHandshakePriority(void) {
    return ssl_handshake_priority;
}

// CPS_SetSslHandshakePriority's role: the priority a connection's thread runs at while enter_computebound holds it up for
// a crypto operation
// pokebw2's CPS_SslSetMaxPriority
void CPS_SetSslHandshakePriority(u32 priority) {
    ssl_handshake_priority = priority;
}

// Raises the current thread's priority to the configured maximum (CPS_SetSslHandshakePriority's role) for a crypto
// operation, returning the priority it had so exit_computebound can put it back; -1 once the slot (0-31) runs out
// pokebw2's SSLi_RaisePriority
static u32 enter_computebound(void) {
    if (ssl_handshake_priority >= 0x20) {
        return (u32)-1;
    }
    {
        OSThread *thread = OS_GetCurrentThread();
        u32 old = OS_GetThreadPriority(thread);
        OS_SetThreadPriority(thread, ssl_handshake_priority);
        return old;
    }
}

// pokebw2's SSLi_RestorePriority
static void exit_computebound(u32 priority) {
    if (priority < 0x20) {
        OS_SetThreadPriority(OS_GetCurrentThread(), priority);
    }
}

// CPS_SetRootCa's role: registers the caller's named params (certificates to pin) on the current thread's
// connection, if it has one
// pokebw2's CPS_SslSetNamedParams
void CPS_SetRootCa(void *const *params, u32 count) {
    OSThread *thread = OS_GetCurrentThread();
    CPSSoc *soc = (CPSSoc *)thread->specific[0];

    if (soc != NULL && soc->con != NULL) {
        SSLContext *ctx = (SSLContext *)soc->con;
        ctx->named_params = (SSLNamedParam *const *)params;
        ctx->named_param_count = count;
    }
}

// Finds the registered param whose name matches, or NULL
// pokebw2's SSLi_FindNamedParam
static const SSLNamedParam *get_rootCA(const SSLContext *ctx, const char *name) {
    int i = 0;
    SSLNamedParam *const *list;
    int count = ctx->named_param_count;
    const SSLNamedParam *param;

    if (0 < count) {
        list = ctx->named_params;
        do {
            param = list[i];
            if (strcmp(param->name, name) == 0) {
                return param;
            }
            i++;
        } while (i < count);
    }
    return NULL;
}

// Reads a BER-style length: a byte with the high bit clear is the value itself; one with it set gives the count of
// following big-endian bytes to read it from. -1 if that count would need the high bits of the result
// pokebw2's SSLi_DecodeLength
static s32 cert_item_len(const u8 **cursor) {
    const u8 *p = *cursor;
    u32 value = *p++;

    if (value & 0x80) {
        u32 count = value & 0x7f;
        value = 0;
        while (count-- != 0) {
            if (value & 0xff000000) {
                return -1;
            }
            value = (value << 8) + *p++;
        }
    }
    *cursor = p;
    return value;
}

// Appends ", *<suffix>" to a hostname pattern (truncating the whole thing to 0xff bytes), or just <suffix> once if
// repeat is 0
// pokebw2's SSLi_BuildHostPattern
static void make_dn(char *dest, const char *suffix, int repeat) {
    char *start = dest;

    if (*dest != '\0') {
        while (*++dest != '\0') {
        }
        if (dest - start >= 0xff) {
            return;
        }
        *dest++ = ',';
        *dest++ = ' ';
    }
    while (repeat-- != 0 && dest - start < 0xff) {
        *dest++ = *suffix++;
    }
    *dest = '\0';
}

// Packs the calendar date as year:16, month:8, day:8. Time of day is ignored.
// pokebw2's SSLi_ParseCertTime
static u32 parse_time(const u8 *digits, int tag) {
    u32 first = digits[0] * 10 + digits[1] - '0' * 11;
    u32 year;

    digits += 2;
    if (tag == 23) {
        if (first < 50) {
            year = first + 2000;
        } else {
            year = first + 1900;
        }
    } else {
        year = first * 100 + (digits[0] * 10 + digits[1] - '0' * 11);
        digits += 2;
    }
    return (year << 16) + ((digits[0] * 10 + digits[1] - '0' * 11) << 8) +
           (digits[2] * 10 + digits[3] - '0' * 11);
}

// mode 0 retains the leaf key, mode 1 walks subsequent certificates, and mode 2 extracts
// a following certificate's public key by reference for the preceding signature check.
// pokebw2's SSLi_ParseAsn1Field
static int cert_item(SSLContext *ctx, const u8 **cursor, int depth, int index, int mode) {
    const u8 *value;
    int tag;
    s32 len;
    const u8 *end;
    const u8 *start = *cursor;
    int i;
    const u8 *oid;
    int type;

    value = start + 1;
    tag = *start;
    len = cert_item_len(&value);

    if (len < 0 || len > 2000) {
        return 1;
    }
    type = tag & 31;
    switch (type) {
    case 2:
        if (ctx->parsing_rsa_key) {
            if (index == 0) {
                while (*value == 0) {
                    value++;
                    len--;
                }
                if (mode != 0) {
                    if (mode == 2) {
                        ctx->chain_key.modulus_len = len;
                        ctx->chain_key.modulus = value;
                    }
                } else if (len <= 0x100) {
                    MI_CpuCopy8(value, ctx->certificate_key.modulus, len);
                    ctx->certificate_key.modulus_len = len;
                }
            } else if (index == 1) {
                while (*value == 0) {
                    value++;
                    len--;
                }
                if (mode != 0) {
                    if (mode == 2) {
                        ctx->chain_key.exponent_len = len;
                        ctx->chain_key.exponent = value;
                    }
                } else if (len <= 8) {
                    MI_CpuCopy8(value, ctx->certificate_key.exponent, len);
                    ctx->certificate_key.exponent_len = len;
                }
            }
        }
        value += len;
        break;
    case 3:
        i = 0;
        if (depth == 1) {
            if (mode != 2) {
                ctx->certificate_signature = value + 1;
                ctx->certificate_signature_len = len - 1;
            }
        }
        if (ctx->parsing_rsa_key) {
            value++;
            if (cert_item(ctx, &value, depth, i, mode)) {
                return 1;
            }
            ctx->parsing_rsa_key = i;
            break;
        }
        value += len;
        break;
    case 6:
        oid = value;
        for (i = 0; i < 6; i++) {
            if (memcmp(oid, object[i], strlen((const char *)object[i])) == 0) {
                switch (i) {
                case 1:
                case 2:
                    if (mode == 0) {
                        ctx->public_key_algorithm = i;
                    }
                    ctx->parsing_rsa_key = i;
                    break;
                case 3:
                case 4:
                    if (mode != 2) {
                        ctx->signature_algorithm = i;
                    }
                    break;
                case 5:
                    if (mode != 2) {
                        ctx->attribute_oid = i;
                    }
                    break;
                }
                break;
            }
        }
        value += len;
        break;
    case 12:
    case 19:
    case 20:
    case 22:
        i = 0;
        if (mode != 2) {
            if (ctx->certificate_phase.parsing_subject) {
                make_dn(ctx->subject, (const char *)value, len);
                if (ctx->attribute_oid == 5 && len <= 0x4f) {
                    MI_CpuCopy8(value, ctx->common_name, len);
                    ctx->common_name[len] = i;
                }
            } else {
                make_dn(ctx->issuer, (const char *)value, len);
            }
        }
        ctx->attribute_oid = i;
        value += len;
        break;
    case 23:
    case 24:
        i = 1;
        if (mode != 2) {
            u32 date = parse_time(value, type);
            if (index == 0) {
                if (ctx->current_date >= date) {
                    ctx->certificate_date_valid = i;
                }
            } else {
                if (ctx->current_date > date) {
                    ctx->certificate_date_valid = 0;
                }
            }
        }
        value += len;
        ctx->certificate_phase.parsing_subject = i;
        break;
    case 16:
        if (depth == 0 && index == 0 && mode != 2) {
            ctx->signed_start = value;
        }
        i = 0;
        end = value + len;
        while (value < end) {
            if (cert_item(ctx, &value, depth + 1, i++, mode)) {
                return 1;
            }
        }
        if (depth == 1 && index == 0 && mode != 2) {
            ctx->signed_end = value;
        }
        break;
    case 17:
        index = 0;
        end = value + len;
        while (value < end) {
            if (cert_item(ctx, &value, depth + 1, index, mode)) {
                return 1;
            }
        }
        break;
    default:
        i = 0;
        if (tag == 0xa0) {
            end = value + len;
            while (value < end) {
                if (cert_item(ctx, &value, depth + 1, i, mode)) {
                    return 1;
                }
            }
            break;
        }
        value += len;
        break;
    }
    *cursor = value;
    return 0;
}

// Checks the parsed certificate's signature against its signed DER digest, PKCS#1 v1.5 style:
// decrypted = signature^e mod n, then 0x00 0x01 0xff...
// 0x00 <digest>
// pokebw2's SSLi_VerifyCertificateSignature
static int validate_signature(SSLContext *ctx, const SSLRsaPublicKey *key) {
    u16 *digits;
    int n;
    int status;
    int pos;
    u16 *sigDigits, *expDigits, *modDigits;
    u32 priority;

    if (ctx->certificate_signature == NULL || ctx->certificate_signature_len == 0 || key->exponent == NULL ||
        key->exponent_len == 0 || key->modulus == NULL || key->modulus_len == 0) {
        return 2;
    }

    n = (key->modulus_len * 2) / 2;
    digits = (u16 *)data_ov009_021a8280(n * 8);
    if (digits == NULL) {
        return 2;
    }

    sigDigits = digits + n;
    expDigits = sigDigits + n;
    modDigits = expDigits + n;

    CPSi_big_from_char(sigDigits, ctx->certificate_signature, ctx->certificate_signature_len, n);
    CPSi_big_from_char(expDigits, key->exponent, key->exponent_len, n);
    CPSi_big_from_char(modDigits, key->modulus, key->modulus_len, n);
    priority = enter_computebound();
    CPSi_big_power(digits, sigDigits, expDigits, n, modDigits);
    exit_computebound(priority);
    CPSi_char_from_big((u8 *)sigDigits, digits, key->modulus_len, n);

    status = 0;
    if (((u8 *)sigDigits)[0] != 0 || ((u8 *)sigDigits)[1] != 1) {
        status = 2;
    } else {
        int len = key->modulus_len;

        for (pos = 2; pos < len; pos++) {
            if (((u8 *)sigDigits)[pos] != 0xff) {
                break;
            }
        }
        if (pos + 1 >= len || ((u8 *)sigDigits)[pos] != 0 || ((u8 *)sigDigits)[pos + 1] != 0x30 ||
            memcmp(ctx->certificate_digest, (u8 *)sigDigits + len - ctx->certificate_digest_len,
                   ctx->certificate_digest_len) != 0) {
            status = 2;
        }
    }
    data_ov009_021a82b0(digits);
    return status;
}

// Hashes the certificate's signed DER span and checks its signature using the registered issuer key.
// The handshake snapshot contexts serve as temporary digest storage here; this does not restore a session.
// pokebw2's SSLi_CheckCertificate
static int auth_cert(SSLContext *ctx) {
    int flags = ctx->certificate_date_valid ? 0 : SSLI_CERT_DATE_INVALID;
    const SSLNamedParam *param;

    if (ctx->public_key_algorithm == -1) {
        return 4 | flags;
    }
    switch (ctx->signature_algorithm) {
    case 3:
        CPSi_md5_init(&ctx->handshake_md5_snapshot);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, ctx->signed_start, ctx->signed_end - ctx->signed_start);
        CPSi_md5_result(&ctx->handshake_md5_snapshot, ctx->certificate_digest);
        ctx->certificate_digest_len = 0x10;
        break;
    case 4:
        CPSi_sha1_init(&ctx->handshake_sha1_snapshot);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, ctx->signed_start, ctx->signed_end - ctx->signed_start);
        CPSi_sha1_result(&ctx->handshake_sha1_snapshot, ctx->certificate_digest);
        ctx->certificate_digest_len = 0x14;
        break;
    default:
        return 3 | flags;
    }
    param = get_rootCA(ctx, ctx->issuer);
    if (param == NULL) {
        return 1 | flags;
    }
    return validate_signature(ctx, (const SSLRsaPublicKey *)param) | flags;
}

// How many bytes until the next '.' or the end of the string
// pokebw2's SSLi_LabelLength
static int chars_till_end(const char *host) {
    const char *start = host;
    while (*host != '.' && *host != '\0') {
        host++;
    }
    return host - start;
}

// Compares the requested hostname with the certificate pattern (the second
// argument may contain a wildcard). Like strcmp, zero means a match.
// pokebw2's SSLi_CompareHostname
static int compare_fqdn(const char *host, const char *pattern) {
    char p, h;

    for (;;) {
        while ((h = *host++) == (p = *pattern++)) {
            if (h == 0) {
                return 0;
            }
        }
        if (p != '*') {
            return 1;
        }
        {
            int hostLen, patternLen;
            host--;
            hostLen = chars_till_end(host);
            patternLen = chars_till_end(pattern);
            if (patternLen > hostLen) {
                return 1;
            }
            host += hostLen - patternLen;
        }
    }
}

// Processes the Certificate message's BE24-length-prefixed chain. The callback can
// override validation; continuation depends on the original low-byte result as well.
// pokebw2's SSLi_ParseCertificateChain
static void rcv_certificate(SSLContext *ctx, const u8 *body) {
    int mode;
    int result;
    int remaining;
    int index;
    RTCDate date;

    remaining = (((body[0] << 8) + body[1]) << 8) + body[2];
    body += 3;
    ctx->public_key_algorithm = -1;
    RTC_GetDate(&date);
    mode = 0;
    index = 0;
    ctx->current_date = ((date.year + 2000) << 16) + (date.month << 8) + date.day;
    ctx->subject[0] = 0;
    ctx->certificate_key.exponent_len = 0;
    ctx->certificate_key.modulus_len = 0;
    for (;;) {
        int len = (((body[0] << 8) + body[1]) << 8) + body[2];
        body += 3;
        remaining -= len + 3;
        ctx->signature_algorithm = -1;
        ctx->parsing_rsa_key = 0;
        ctx->certificate_phase.parsing_subject = 0;
        ctx->certificate_date_valid = 0;
        ctx->subject[0] = 0;
        ctx->issuer[0] = 0;
        ctx->common_name[0] = 0;
        ctx->certificate = body;
        ctx->certificate_len = len;
        if (cert_item(ctx, &body, 0, 0, mode) || (u32)ctx->certificate_key.modulus_len < 0x33 ||
            ctx->certificate_key.exponent_len == 0) {
            ctx->state = SSLI_STATE_ERROR;
            return;
        }
        result = auth_cert(ctx);
        if (index == 0 && ctx->hostname && compare_fqdn(ctx->hostname, ctx->common_name)) {
            result |= SSLI_CERT_HOSTNAME_MISMATCH;
        }
        mode = result & SSLI_CERT_RESULT_MASK;
        if (mode == 1 && remaining != 0) {
            const u8 *next = body + 3;
            ctx->parsing_rsa_key = 0;
            if (cert_item(ctx, &next, 0, 0, 2)) {
                ctx->state = SSLI_STATE_ERROR;
                return;
            }
            result = (result & ~SSLI_CERT_RESULT_MASK) | validate_signature(ctx, &ctx->chain_key);
        }
        if (ctx->validate_certificate)
            result = ctx->validate_certificate(result, ctx, index);
        index++;
        if (mode != 0 && result == 0 && remaining != 0) {
            mode = 1;
            continue;
        }
        if (result == 0) {
            ctx->state = SSLI_STATE_CERTIFICATE_ACCEPTED;
        } else {
            ctx->state = SSLI_STATE_ERROR;
        }
        return;
    }
}

// Handles the ServerHello received by the client: peer random, selected session ID and cipher suite.
// pokebw2's SSLi_ParseServerHello
static void rcv_server_hello(SSLContext *ctx, u8 *msg) {
    long sidLen;
    BOOL wasResumed;
    u8 *sid;
    u8 origSidLen;

    MI_CpuCopy8(msg + 2, ctx->server_random, 0x20);
    wasResumed = ctx->resumed;
    sidLen = msg[0x22];
    origSidLen = sidLen;
    sid = msg + 0x23;
    if (wasResumed && origSidLen == 0x20 && memcmp(ctx->session_id, sid, 0x20) == 0) {
        ctx->unk_031 = 1;
    } else {
        if (wasResumed) {
            func_ov009_0215ffd0(ctx);
        }
        if (sidLen == 0) {
            ctx->resumed = FALSE;
        } else {
            MI_CpuCopy8(sid, ctx->session_id, 0x20);
            ctx->resumed = TRUE;
        }
        ctx->unk_031 = 0;
    }
    ctx->cipher_suite = (sid[sidLen] << 8) + sid[sidLen + 1];
    if (ctx->cipher_suite != 4 && ctx->cipher_suite != 5) {
        ctx->state = SSLI_STATE_ERROR;
        return;
    }
    ctx->state = SSLI_STATE_SERVER_HELLO_PARSED;
}

// Whether value is among the count big-endian values of width bytes each (2 or 3) in data
// pokebw2's SSLi_ContainsU16
static BOOL has_method(const u8 *data, int count, int width, u32 value) {
    int i;

    for (i = 0; i < count; i++, data += width) {
        u32 entry = (data[0] << 8) + data[1];
        if (width == 3) {
            entry = (entry << 8) + data[2];
        }
        if (entry == value) {
            return TRUE;
        }
    }
    return FALSE;
}

// Picks the first of this connection's offered cipher suites that the count entries of width bytes each in data
// also list, or 0
// pokebw2's SSLi_PickCipherSuite
static int select_method(const u8 *data, int count, int width) {
    u32 i;

    for (i = 0; i < 2; i++) {
        if (has_method(data, count, width, pairlist[i])) {
            return pairlist[i];
        }
    }
    return 0;
}

// Whether major is SSLv3's (3); only version 3.x handshakes are understood
// pokebw2's SSLi_IsSsl3Version
static BOOL version_ok(int major, int minor) {
    (void)minor;
    return major == 3;
}

// Handles the SSLv2-compatible ClientHello: three-byte cipher entries, a session ID and a challenge
// copied into the client-random storage. The normal SSLv3 ClientHello uses client_hello.
// pokebw2's SSLi_ParseSsl2ClientHello
static void client_hello_v2(SSLContext *ctx, u8 *msg) {
    int cipherLen;
    int offset;
    int sessionLen;
    int challengeLen;
    u16 cipher;

    if (!version_ok(msg[0], msg[1])) {
        return;
    }
    cipherLen = (msg[2] << 8) + msg[3];
    cipher = select_method(msg + 8, cipherLen / 3, 3);
    if (cipher == 0) {
        return;
    }
    ctx->cipher_suite = cipher;
    sessionLen = (msg[4] << 8) + msg[5];
    challengeLen = (msg[6] << 8) + msg[7];
    ctx->resumed = FALSE;
    offset = cipherLen + 8 + sessionLen;
    if (challengeLen >= 0x20) {
        MI_CpuCopy8(msg + offset, ctx->client_random, 0x20);
    } else {
        MI_CpuFill8(ctx->client_random, 0, 0x20 - challengeLen);
        MI_CpuCopy8(msg + offset, ctx->client_random + 0x20 - challengeLen, challengeLen);
    }
    ctx->state = SSLI_STATE_CLIENT_HELLO_PARSED;
}

// Handles the ClientHello received by the server: client random, offered session ID and two-byte cipher entries.
// The offered session ID can restore a cached session.
// pokebw2's SSLi_ParseClientHello
static void client_hello(SSLContext *ctx, u8 *msg) {
    int sidLen;
    u8 *sid;
    u8 *cipherList;
    u16 cipher;

    if (!version_ok(msg[0], msg[1])) {
        return;
    }
    MI_CpuCopy8(msg + 2, ctx->client_random, 0x20);

    sidLen = msg[0x22];
    sid = msg + 0x23;
    if (sidLen != 0x20) {
        ctx->resumed = FALSE;
    } else {
        MI_CpuCopy8(sid, ctx->session_id, 0x20);
        func_ov009_0215fda4(ctx);
    }

    cipherList = sid + sidLen;
    cipher = select_method(cipherList + 2, ((cipherList[0] << 8) + cipherList[1]) / 2, 2);
    ctx->cipher_suite = cipher;
    if (cipher != 0) {
        ctx->state = SSLI_STATE_CLIENT_HELLO_PARSED;
    }
}

// Decrypts with both prime/exponent pairs, recombines their results, then reduces by the full modulus.
// Seven n-digit regions and CPSi_big_div's three-n-digit scratch occupy exactly 20*n allocated bytes.
// pokebw2's SSLi_DecryptPreMasterSecret
static void decrypt_premaster_secret(SSLContext *ctx, u8 *encrypted, void *keyParam) {
    const SSLRsaPrivateKey *key = (const SSLRsaPrivateKey *)keyParam;
    int n;
    u16 *buf;
    u16 *operand, *intermediate, *firstResult, *secondResult, *prime, *result, *work;
    u32 priority;

    if (key == NULL || key->modulus_len == 0) {
        return;
    }

    n = (key->modulus_len * 2) / 2 + 1;
    buf = (u16 *)data_ov009_021a8280(20 * n);
    if (buf == NULL) {
        return;
    }

    operand = buf + n;
    intermediate = operand + n;
    firstResult = intermediate + n;
    secondResult = firstResult + n;
    prime = secondResult + n;
    result = prime + n;
    work = result + n;

    CPSi_big_from_char(buf, encrypted, key->modulus_len, n);
    CPSi_big_from_char(operand, key->first_exponent, key->first_exponent_len, n);
    CPSi_big_from_char(prime, key->first_prime, key->first_prime_len, n);
    priority = enter_computebound();
    CPSi_big_montpower(firstResult, buf, operand, n, prime);
    CPSi_big_from_char(operand, key->second_exponent, key->second_exponent_len, n);
    CPSi_big_from_char(prime, key->second_prime, key->second_prime_len, n);
    CPSi_big_montpower(secondResult, buf, operand, n, prime);
    exit_computebound(priority);

    CPSi_big_sub(buf, firstResult, secondResult, n);
    CPSi_big_from_char(operand, key->coefficient, key->coefficient_len, n);
    CPSi_big_mult(intermediate, buf, operand, n);
    CPSi_big_from_char(operand, key->second_prime, key->second_prime_len, n);
    CPSi_big_mult(buf, intermediate, operand, n);
    CPSi_big_add(intermediate, buf, secondResult, n);
    CPSi_big_from_char(operand, key->modulus, key->modulus_len, n);

    if (CPSi_big_sign(intermediate, n) < 0) {
        CPSi_big_negate(intermediate, n);
        CPSi_big_div(NULL, intermediate, operand, result, n, work);
        CPSi_big_sub(result, operand, result, n);
    } else {
        CPSi_big_div(NULL, intermediate, operand, result, n, work);
    }
    CPSi_char_from_big((u8 *)ctx, result, 0x30, n);
    data_ov009_021a82b0(buf);
}

// One block of SSLv3's key material generation: MD5(secret + SHA1(label + secret + client_random + server_random)),
// reusing the handshake hash snapshot slots as scratch since they aren't needed yet this early
// pokebw2's SSLi_HashKeyMaterialBlock
static void create_ms_sub(u8 *out, const char *label, SSLContext *ctx) {
    u8 digest[20];

    CPSi_sha1_init(&ctx->handshake_sha1_snapshot);
    CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, (const u8 *)label, strlen(label));
    CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, ctx->secret, 0x30);
    CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, ctx->client_random, 0x40);
    CPSi_sha1_result(&ctx->handshake_sha1_snapshot, digest);

    CPSi_md5_init(&ctx->handshake_md5_snapshot);
    CPSi_md5_calc(&ctx->handshake_md5_snapshot, ctx->secret, 0x30);
    CPSi_md5_calc(&ctx->handshake_md5_snapshot, digest, 0x14);
    CPSi_md5_result(&ctx->handshake_md5_snapshot, out);
}

// Turns the pre-master secret (ctx->secret) into the 0x30-byte master secret, overwriting it in place:
// MD5(pms+SHA1("A"+pms+randoms)) || MD5(pms+SHA1("BB"+pms+randoms)) || MD5(pms+SHA1("CCC"+pms+randoms))
// pokebw2's SSLi_GenerateMasterSecret
static void create_master_secret(SSLContext *ctx) {
    u8 block[0x30];

    create_ms_sub(block, "A", ctx);
    create_ms_sub(block + 0x10, "BB", ctx);
    create_ms_sub(block + 0x20, "CCC", ctx);
    MI_CpuCopy8(block, ctx->secret, 0x30);
}

// Generates the key_block (SSLv3's generate_key_material, repeating "A", "BB", "CCC", ... for each round) into
// ctx->session_id and the scratch past it, which the handshake has no further use for, then splits it into this
// side's write and read MAC secrets and RC4 keys depending on which half of each pair ctx->is_server (the role) picks,
// and sets both RC4 contexts up (there is no IV: this cipher suite is always RC4)
// pokebw2's SSLi_GenerateKeyBlock
static inline void SSLi_KeyBlockRound(SSLContext *ctx, u8 *out, int round) {
    u8 buf[0x15];
    int i;

    CPSi_sha1_init(&ctx->handshake_sha1_snapshot);
    buf[0] = (u8)(0x41 + round);
    for (i = 0; i < round + 1; i++) {
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, buf, 1);
    }
    CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, ctx->secret, 0x30);
    CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, ctx->server_random, 0x20);
    CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, ctx->client_random, 0x20);
    CPSi_sha1_result(&ctx->handshake_sha1_snapshot, buf + 1);

    CPSi_md5_init(&ctx->handshake_md5_snapshot);
    CPSi_md5_calc(&ctx->handshake_md5_snapshot, ctx->secret, 0x30);
    CPSi_md5_calc(&ctx->handshake_md5_snapshot, buf + 1, 0x14);
    CPSi_md5_result(&ctx->handshake_md5_snapshot, out);
}

static void create_key_block(SSLContext *ctx) {
    s32 i, j;
    u8 c;
    u8 tmp[20];
    void *p;
    s32 size1;
    s32 size2;
    s32 size3;
    s32 len;

    /* Not a fake: the prebuilt libssl.a's DWARF lists size1, size2 (sp+0) and size3 (r2)
       as this function's locals, set per cipher (the library's switch probably had more suites); a switch without a
       default leaves them unset otherwise, so MWCC keeps them as variables like the original */
    switch (ctx->cipher_suite) {
    case 4:
        size1 = 16;
        size2 = 16;
        size3 = 0;
        break;
    case 5:
        size1 = 20;
        size2 = 16;
        size3 = 0;
        break;
    }
    len = (size1 + size2 + size3) * 2;
    for (i = 0; i * 16 < len; i++) {
        p = &ctx->handshake_sha1_snapshot;
        CPSi_sha1_init(p);
        c = 'A' + i;
        for (j = 0; j < i + 1; j++) {
            CPSi_sha1_calc(p, &c, 1);
        }
        CPSi_sha1_calc(p, ctx->secret, 48);
        CPSi_sha1_calc(p, ctx->server_random, 32);
        CPSi_sha1_calc(p, ctx->client_random, 32);
        CPSi_sha1_result(p, tmp);
        p = &ctx->handshake_md5_snapshot;
        CPSi_md5_init(p);
        CPSi_md5_calc(p, ctx->secret, 48);
        CPSi_md5_calc(p, tmp, 20);
        CPSi_md5_result(p, ctx->session_id + i * 16);
    }
    if (ctx->is_server) {
        ctx->read_mac_secret = ctx->session_id;
        ctx->read_key = ctx->session_id + size1 * 2;
        ctx->write_mac_secret = ctx->session_id + size1;
        ctx->write_key = ctx->write_mac_secret + size1 + size2;
    } else {
        ctx->write_mac_secret = ctx->session_id;
        ctx->write_key = ctx->session_id + size1 * 2;
        ctx->read_mac_secret = ctx->session_id + size1;
        ctx->read_key = ctx->read_mac_secret + size1 + size2;
    }
    CPSi_rc4_init(&ctx->read_rc4, ctx->read_key, 16);
    CPSi_rc4_init(&ctx->write_rc4, ctx->write_key, 16);
}

// Finishes a ClientKeyExchange that carried an RSA-encrypted pre-master secret with this connection's own key
// (for the quick-resume ticket's key exchange; see decrypt_premaster_secret), derives the master secret and
// key block from it, remembers the session and sets this side's role for the key block's halves
// pokebw2's SSLi_FinishKeyExchange
static void rcv_client_key_exchange(SSLContext *ctx, u8 *msg) {
    decrypt_premaster_secret(ctx, msg, ctx->key_param);
    create_master_secret(ctx);
    func_ov009_0215fee0(ctx, 0, 0);
    create_key_block(ctx);
    ctx->state = SSLI_STATE_MASTER_SECRET_READY;
}

// Half of a Finished message's MAC (SHA1 is finished_sha1): HMAC-SSLv3 style, with the running MD5
// handshake hash's digest (not continued afterward: the snapshot is for a later quick resume, not this) as the
// inner hash's last input, under the given sender's 4-byte label ("CLNT" or "SRVR")
// pokebw2's SSLi_ComputeFinishedMd5
static void finished_md5(SSLContext *ctx, u8 *out, int sender) {
    u8 pad[0x30];

    if ((ctx->is_server ^ sender) != 0) {
        CPSi_md5_calc(&ctx->handshake_md5, (const u8 *)"SRVR", 4);
    } else {
        CPSi_md5_calc(&ctx->handshake_md5, (const u8 *)"CLNT", 4);
    }
    CPSi_md5_calc(&ctx->handshake_md5, ctx->secret, 0x30);
    MI_CpuFill8(pad, 0x36, 0x30);
    CPSi_md5_calc(&ctx->handshake_md5, pad, 0x30);
    CPSi_md5_result(&ctx->handshake_md5, out);

    CPSi_md5_init(&ctx->handshake_md5);
    CPSi_md5_calc(&ctx->handshake_md5, ctx->secret, 0x30);
    MI_CpuFill8(pad, 0x5c, 0x30);
    CPSi_md5_calc(&ctx->handshake_md5, pad, 0x30);
    CPSi_md5_calc(&ctx->handshake_md5, out, 0x10);
    CPSi_md5_result(&ctx->handshake_md5, out);
}

// pokebw2's SSLi_ComputeFinishedSha1
static void finished_sha1(SSLContext *ctx, u8 *out, int sender) {
    u8 pad[0x28];

    if ((ctx->is_server ^ sender) != 0) {
        CPSi_sha1_calc(&ctx->handshake_sha1, (const u8 *)"SRVR", 4);
    } else {
        CPSi_sha1_calc(&ctx->handshake_sha1, (const u8 *)"CLNT", 4);
    }
    CPSi_sha1_calc(&ctx->handshake_sha1, ctx->secret, 0x30);
    MI_CpuFill8(pad, 0x36, 0x28);
    CPSi_sha1_calc(&ctx->handshake_sha1, pad, 0x28);
    CPSi_sha1_result(&ctx->handshake_sha1, out);

    CPSi_sha1_init(&ctx->handshake_sha1);
    CPSi_sha1_calc(&ctx->handshake_sha1, ctx->secret, 0x30);
    MI_CpuFill8(pad, 0x5c, 0x28);
    CPSi_sha1_calc(&ctx->handshake_sha1, pad, 0x28);
    CPSi_sha1_calc(&ctx->handshake_sha1, out, 0x14);
    CPSi_sha1_result(&ctx->handshake_sha1, out);
}

// Checks the received Finished with the original fixed sender selector 1, restoring the running
// handshake hash after each half so processing can subsequently include this message.
// pokebw2's SSLi_VerifyFinished
static void rcv_finished(SSLContext *ctx, u8 *msg) {
    u8 computed[0x14];

    MI_CpuCopy8(&ctx->handshake_md5, &ctx->handshake_md5_snapshot, sizeof(MD5Context));
    finished_md5(ctx, computed, 1);
    MI_CpuCopy8(&ctx->handshake_md5_snapshot, &ctx->handshake_md5, sizeof(MD5Context));
    if (memcmp(msg, computed, 0x10) != 0) {
        ctx->state = SSLI_STATE_ERROR;
        return;
    }

    MI_CpuCopy8(&ctx->handshake_sha1, &ctx->handshake_sha1_snapshot, sizeof(SHA1Context));
    finished_sha1(ctx, computed, 1);
    MI_CpuCopy8(&ctx->handshake_sha1_snapshot, &ctx->handshake_sha1, sizeof(SHA1Context));
    if (memcmp(msg + 0x10, computed, 0x14) != 0) {
        ctx->state = SSLI_STATE_ERROR;
        return;
    }
    ctx->state = SSLI_STATE_FINISHED_VERIFIED;
}

// Adds 1 to an 8-byte big-endian sequence number, carrying leftward
// pokebw2's SSLi_IncrementCounter
static void add1_be8(u8 *iv) {
    u8 val;
    s32 val2;

    val2 = 8;
    do {
        iv = iv + -1;
        val = *iv;
        *iv = val + 1;
        if ((u8)(val + 1) != 0) {
            return;
        }
        val2 = val2 + -1;
    } while (val2 != 0);
}

// Decrypts received bytes with the read RC4 state and returns len unchanged.
// pokebw2's SSLi_Rc4Crypt
static u32 decrypt_ssl(SSLContext *ctx, void *buf, u32 len) {
    CPSi_rc4_crypt(&ctx->read_rc4, buf, len);
    return len;
}

// RC4-decrypts a received record and checks its MAC, the mirror of make_ciphertext: hdr's length still covers the
// MAC, which this strips off after recomputing it over the shorter content and comparing. Sets ctx->state to an
// error on a mismatch either way, so the caller doesn't need to check this call's return for that
// pokebw2's SSLi_CheckRecordMac
static u32 make_plaintext(SSLContext *ctx, u8 *hdr) {
    int len = (u32)decrypt_ssl(ctx, hdr + 5, (hdr[3] << 8) + hdr[4]);
    u8 mac[0x14];
    u8 pad[0x30];
    int macLen;

    switch (ctx->cipher_suite) {
    case 4:
        len -= 0x10;
        hdr[3] = (u8)(len >> 8);
        hdr[4] = (u8)len;

        CPSi_md5_init(&ctx->handshake_md5_snapshot);
        macLen = 0x10;
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, ctx->read_mac_secret, macLen);
        MI_CpuFill8(pad, 0x36, 0x30);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, pad, 0x30);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, ctx->read_seq_num, 8);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, hdr, 1);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, hdr + 3, len + 2);
        CPSi_md5_result(&ctx->handshake_md5_snapshot, mac);

        CPSi_md5_init(&ctx->handshake_md5_snapshot);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, ctx->read_mac_secret, macLen);
        MI_CpuFill8(pad, 0x5c, 0x30);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, pad, 0x30);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, mac, macLen);
        CPSi_md5_result(&ctx->handshake_md5_snapshot, mac);
        break;
    case 5:
        len -= 0x14;
        hdr[3] = (u8)(len >> 8);
        hdr[4] = (u8)len;

        CPSi_sha1_init(&ctx->handshake_sha1_snapshot);
        macLen = 0x14;
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, ctx->read_mac_secret, macLen);
        MI_CpuFill8(pad, 0x36, 0x28);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, pad, 0x28);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, ctx->read_seq_num, 8);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, hdr, 1);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, hdr + 3, len + 2);
        CPSi_sha1_result(&ctx->handshake_sha1_snapshot, mac);

        CPSi_sha1_init(&ctx->handshake_sha1_snapshot);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, ctx->read_mac_secret, macLen);
        MI_CpuFill8(pad, 0x5c, 0x28);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, pad, 0x28);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, mac, macLen);
        CPSi_sha1_result(&ctx->handshake_sha1_snapshot, mac);
        break;
    }

    if (memcmp(hdr + 5 + len, mac, macLen) != 0) {
        ctx->state = SSLI_STATE_ERROR;
    }
    add1_be8(ctx->read_seq_num + 8);
    return len + 5;
}

// Appends this record's MAC to its content (SSLv3's: hash(mac_secret + pad1 + seq_num + type + length + content),
// then hash(mac_secret + pad2 + that)) and RC4-crypts the whole thing, advancing the sequence number. hdr is the
// record header (type, version, length) with the content right after it; returns the header's own size (5)
// plus the content and MAC's combined length
// pokebw2's SSLi_ComputeMac
static u32 make_ciphertext(SSLContext *ctx, u8 *hdr) {
    u8 *content = hdr + 5;
    s32 len = (hdr[3] << 8) + hdr[4];
    u8 pad[0x30];

    switch (ctx->cipher_suite) {
    case 4:
        CPSi_md5_init(&ctx->handshake_md5_snapshot);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, ctx->write_mac_secret, 0x10);
        MI_CpuFill8(pad, 0x36, 0x30);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, pad, 0x30);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, ctx->write_seq_num, 8);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, hdr, 1);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, hdr + 3, len + 2);
        CPSi_md5_result(&ctx->handshake_md5_snapshot, content + len);

        CPSi_md5_init(&ctx->handshake_md5_snapshot);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, ctx->write_mac_secret, 0x10);
        MI_CpuFill8(pad, 0x5c, 0x30);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, pad, 0x30);
        CPSi_md5_calc(&ctx->handshake_md5_snapshot, content + len, 0x10);
        CPSi_md5_result(&ctx->handshake_md5_snapshot, content + len);
        len += 0x10;
        break;
    case 5:
        CPSi_sha1_init(&ctx->handshake_sha1_snapshot);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, ctx->write_mac_secret, 0x14);
        MI_CpuFill8(pad, 0x36, 0x28);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, pad, 0x28);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, ctx->write_seq_num, 8);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, hdr, 1);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, hdr + 3, len + 2);
        CPSi_sha1_result(&ctx->handshake_sha1_snapshot, content + len);

        CPSi_sha1_init(&ctx->handshake_sha1_snapshot);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, ctx->write_mac_secret, 0x14);
        MI_CpuFill8(pad, 0x5c, 0x28);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, pad, 0x28);
        CPSi_sha1_calc(&ctx->handshake_sha1_snapshot, content + len, 0x14);
        CPSi_sha1_result(&ctx->handshake_sha1_snapshot, content + len);
        len += 0x14;
        break;
    }

    hdr[3] = (u8)(len >> 8);
    hdr[4] = (u8)len;
    CPSi_rc4_crypt(&ctx->write_rc4, hdr + 5, len);
    add1_be8(ctx->write_seq_num + 8);
    return len + 5;
}

// Reads exactly len bytes from the socket's TCP stream into dest, waiting across as many reads as it takes; -1 if
// the connection ends first
// pokebw2's SSLi_ReadExact
static int tcp_read_raw_nbytes(u8 *dest, int len, CPSSoc *soc) {
    do {
        u32 avail;
        u8 *data = func_ov009_0215ae18(&avail, soc);

        if (avail == 0) {
            return -1;
        }
        if (avail > len) {
            avail = len;
        }
        MI_CpuCopy8(data, dest, avail);
        func_ov009_0215af14(avail, soc);
        dest += avail;
        len -= avail;
    } while (len > 0);
    return 0;
}

// Feeds data into both running handshake hashes, so Finished covers every handshake message once it's sent
// pokebw2's SSLi_UpdateHandshakeHash
static void update_digest(SSLContext *ctx, const u8 *data, u32 len) {
    CPSi_sha1_calc(&ctx->handshake_sha1, data, len);
    CPSi_md5_calc(&ctx->handshake_md5, data, len);
}

// Takes ownership of a complete record. Application data retains the allocation in
// read_buf; every other path frees it after processing its contents.
// pokebw2's SSLi_ProcessRecord
static void func_ov009_02161c80(SSLContext *ctx, u8 *record) {
    int type;
    u8 *body;
    u32 remaining;
    if (ctx->state == SSLI_STATE_ERROR) {
        data_ov009_021a82b0(record);
        return;
    }
    type = record[0];
    remaining = (record[3] << 8) + record[4] + 5;
    if (ctx->read_mac_secret != NULL &&
        (((ctx->state == SSLI_STATE_EXPECT_FINISHED || ctx->state == SSLI_STATE_APPLICATION_DATA) && type != 0x15) ||
         (type == 0x15 && remaining > 7))) {
        remaining = make_plaintext(ctx, record);
        if (ctx->state == SSLI_STATE_ERROR) {
            data_ov009_021a82b0(record);
            return;
        }
    }
    body = record + 5;
    remaining -= 5;
    switch (type) {
    case 0x14:
        if (ctx->read_mac_secret != NULL) {
            MI_CpuFill8(ctx->read_seq_num, 0, 8);
            ctx->state = SSLI_STATE_EXPECT_FINISHED;
        } else {
            ctx->state = SSLI_STATE_ERROR;
        }
        break;
    case 0x15:
        if (remaining != 2 || body[0] == 2)
            ctx->state = SSLI_STATE_ERROR;
        break;
    case 0x16:
        if (remaining < 4) {
            ctx->state = SSLI_STATE_ERROR;
            break;
        }
        // BUG: The original compares a body length against bytes including its four-byte header,
        // then subtracts both. Later iterations also read a header without checking that four bytes remain.
        do {
#ifdef BUGFIX
            if (remaining < 4) {
                ctx->state = SSLI_STATE_ERROR;
                break;
            }
#endif
            int messageType = body[0];
            int len = (body[1] << 16) + (body[2] << 8) + body[3];
#ifdef BUGFIX
            if (len > remaining - 4) {
#else
            if (len > remaining) {
#endif
                ctx->state = SSLI_STATE_ERROR;
                break;
            }
            body += 4;
            switch (messageType) {
            case 1:
                if (ctx->is_server && ctx->state == SSLI_STATE_INITIAL)
                    client_hello(ctx, body);
                else
                    ctx->state = SSLI_STATE_ERROR;
                break;
            case 16:
                if (ctx->is_server)
                    rcv_client_key_exchange(ctx, body);
                else
                    ctx->state = SSLI_STATE_ERROR;
                break;
            case 2:
                if (len >= 0x26 && !ctx->is_server)
                    rcv_server_hello(ctx, body);
                else
                    ctx->state = SSLI_STATE_ERROR;
                break;
            case 11:
                if (ctx->state == SSLI_STATE_SERVER_HELLO_PARSED && !ctx->is_server) {
                    rcv_certificate(ctx, body);
                    ctx->certificate_phase.certificate_requested = 0;
                } else
                    ctx->state = SSLI_STATE_ERROR;
                break;
            case 13:
                if (ctx->state == SSLI_STATE_CERTIFICATE_ACCEPTED && !ctx->is_server)
                    ctx->certificate_phase.certificate_requested = 1;
                else
                    ctx->state = SSLI_STATE_ERROR;
                break;
            case 14:
                if (!ctx->is_server && ctx->state == SSLI_STATE_CERTIFICATE_ACCEPTED)
                    ctx->state = SSLI_STATE_SERVER_HELLO_DONE;
                else
                    ctx->state = SSLI_STATE_ERROR;
                break;
            case 20:
                rcv_finished(ctx, body);
                break;
            default:
                ctx->state = SSLI_STATE_ERROR;
                break;
            }
            if (ctx->state != SSLI_STATE_ERROR) {
                update_digest(ctx, body - 4, len + 4);
                body += len;
                remaining -= len + 4;
            }
        } while (remaining != 0 && ctx->state != SSLI_STATE_ERROR);
        break;
    case 0x17:
        if (ctx->state != SSLI_STATE_APPLICATION_DATA) {
            ctx->state = SSLI_STATE_ERROR;
            break;
        }
        ctx->read_buf = record;
        ctx->read_buf_pos = 5;
        ctx->read_buf_len = remaining + 5;
        ctx->application_record_ready = 1;
        return;
    default:
        ctx->state = SSLI_STATE_ERROR;
        break;
    }
    data_ov009_021a82b0(record);
}

// Peeks the framing before allocating. The SSLv2-compatible ClientHello form has
// a two-byte header; standard records retain their five-byte header in the buffer.
// pokebw2's SSLi_ReceiveRecord
static int parse_record(CPSSoc *soc) {
    SSLContext *ctx = (SSLContext *)soc->con;
    u32 len;
    u8 *peek, *record;

    for (;;) {
        peek = func_ov009_0215ae18(&len, soc);
        if (len >= 5) {
            break;
        }
        if (len == 0 || soc->state == 0) {
            ctx->state = SSLI_STATE_ERROR;
            return SSLI_STATE_ERROR;
        }
    }
    if (peek[0] == 0x80) {
        if (ctx->is_server && ctx->state == 0) {
            len = peek[1];
            func_ov009_0215af14(2, soc);
            record = (u8 *)data_ov009_021a8280(len);
            if (record == NULL) {
                ctx->state = SSLI_STATE_ERROR;
                return SSLI_STATE_ERROR;
            }
            if (tcp_read_raw_nbytes(record, len, soc) == 0 && record[0] == 1) {
                client_hello_v2(ctx, record + 1);
            } else {
                ctx->state = SSLI_STATE_ERROR;
            }
            update_digest(ctx, record, len);
            data_ov009_021a82b0(record);
        } else {
            ctx->state = SSLI_STATE_ERROR;
        }
    } else {
        len = (peek[3] << 8) + peek[4] + 5;
        if (len > 0x4805) {
            ctx->state = SSLI_STATE_ERROR;
            return SSLI_STATE_ERROR;
        }
        record = (u8 *)data_ov009_021a8280(len);
        if (record == NULL) {
            ctx->state = SSLI_STATE_ERROR;
            return SSLI_STATE_ERROR;
        }
        if (tcp_read_raw_nbytes(record, len, soc) != 0) {
            data_ov009_021a82b0(record);
            ctx->state = SSLI_STATE_ERROR;
            return SSLI_STATE_ERROR;
        }
        func_ov009_02161c80(ctx, record);
    }
    return ctx->state;
}

// Draws len bytes from pool, a running SHA1 digest func_ov009_02162248 stirs seed into, reseeding first from
// data_ov009_021a82e4 (the stack's own generator) if nothing ever has. pool is updated (a carry-add of a fresh SHA1 digest of
// itself) a 20-byte block at a time, and a zero byte never comes out since that cipher suite byte would be
// mistaken for a string terminator downstream
// pokebw2's SSLi_GetRandomBytes
static void set_random(u8 *dest, int len) {
    int produced, pos;
    u32 tmp;
    u8 digest[0x14];

    if (!pool_initialized) {
        tmp = MATH_Rand32(&data_ov009_021a82e4, 0);
        func_ov009_02162248(&tmp, 4);
    }
    produced = 0;
    pos = 0x14;
    while (produced < len) {
        if (pos == 0x14) {
            SHA1Context sha1;
            u32 enabled;
            int i;
            u32 carry;

            CPSi_sha1_init(&sha1);
            enabled = OS_DisableInterrupts();
            CPSi_sha1_calc(&sha1, pool, 0x14);
            CPSi_sha1_result_prng(&sha1, digest);

            carry = 1;
            for (i = 0x13; i >= 0; i--) {
                tmp = pool[i] + digest[i] + carry;
                pool[i] = (u8)tmp;
                carry = tmp >> 8;
            }
            OS_RestoreInterrupts(enabled);
            pos = 0;
        }
        if (digest[pos] != 0) {
            dest[produced++] = digest[pos];
        }
        pos++;
    }
}

// func_ov009_02162248's role: stirs more entropy into set_random's pool (a plain SHA1 of the old pool and
// the new seed) and marks it seeded, so the next draw no longer reseeds from data_ov009_021a82e4 on its own
// pokebw2's CPS_SslAddRandomSeed
void func_ov009_02162248(const void *seed, u32 len) {
    SHA1Context sha1;
    u32 enabled;

    CPSi_sha1_init(&sha1);
    enabled = OS_DisableInterrupts();
    CPSi_sha1_calc(&sha1, pool, 0x14);
    CPSi_sha1_calc(&sha1, seed, len);
    CPSi_sha1_result(&sha1, pool);
    OS_RestoreInterrupts(enabled);
    pool_initialized = TRUE;
}

// Sends ChangeCipherSpec (switching this side to the keys create_key_block made) and Finished (the two
// Finished MAC halves, encrypted and MACed with the new write keys since ChangeCipherSpec already took effect)
// pokebw2's SSLi_SendChangeCipherSpecAndFinished
static void func_ov009_021622c0(CPSSoc *soc) {
    SSLContext *ctx = (SSLContext *)soc->con;
    u8 *msg = (u8 *)data_ov009_021a8280(0x83);

    if (msg == NULL) {
        ctx->state = SSLI_STATE_ERROR;
        return;
    }

    msg[0] = 0x14;
    msg[1] = 3;
    msg[2] = 0;
    msg[3] = 0;
    msg[4] = 1;
    msg[5] = 1;
    MI_CpuFill8(ctx->write_seq_num, 0, 8);

    msg[6] = 0x16;
    msg[7] = 3;
    msg[8] = 0;
    msg[9] = 0;
    msg[10] = 0x28;
    msg[11] = 0x14;
    msg[12] = 0;
    msg[13] = 0;
    msg[14] = 0x24;

    MI_CpuCopy8(&ctx->handshake_md5, &ctx->handshake_md5_snapshot, sizeof(MD5Context));
    finished_md5(ctx, msg + 15, 0);
    MI_CpuCopy8(&ctx->handshake_md5_snapshot, &ctx->handshake_md5, sizeof(MD5Context));
    MI_CpuCopy8(&ctx->handshake_sha1, &ctx->handshake_sha1_snapshot, sizeof(SHA1Context));
    finished_sha1(ctx, msg + 0x1f, 0);
    MI_CpuCopy8(&ctx->handshake_sha1_snapshot, &ctx->handshake_sha1, sizeof(SHA1Context));
    update_digest(ctx, msg + 11, 0x28);

    {
        u32 len = make_ciphertext(ctx, msg + 6);
        func_ov009_0215b198(msg, len + 6, NULL, 0, soc);
    }
    data_ov009_021a82b0(msg);
}

// Sends ClientHello: version, a fresh client random (a Unix-ish timestamp, then 28 more random bytes), the cached
// session id for this peer if there is one, and the 2 cipher suites this connection offers
// pokebw2's SSLi_SendClientHello
static void func_ov009_02162418(CPSSoc *soc) {
    SSLContext *ctx = (SSLContext *)soc->con;
    u8 *msg = (u8 *)data_ov009_021a8280(0x98);
    u8 *p;
    u32 time;
    u32 i;
    int len;
    int bodyLen;
    int total;

    if (msg == NULL) {
        ctx->state = SSLI_STATE_ERROR;
        return;
    }

    msg[9] = 3;
    msg[10] = 0;
    time = func_ov009_0216003c();
    ctx->client_random[0] = (u8)(time >> 24);
    ctx->client_random[1] = (u8)(time >> 16);
    ctx->client_random[2] = (u8)(time >> 8);
    ctx->client_random[3] = (u8)time;
    set_random(ctx->client_random + 4, 0x1c);

    MI_CpuCopy8(ctx->client_random, msg + 11, 0x20);
    func_ov009_0215fe3c(ctx, soc->remote_ip, soc->remote_port);

    if (ctx->resumed) {
        msg[0x2b] = 0x20;
        MI_CpuCopy8(ctx->session_id, msg + 0x2c, 0x20);
        p = msg + 0x4c;
    } else {
        msg[0x2b] = 0;
        p = msg + 0x2c;
    }
    p[0] = 0;
    p[1] = 4;
    p += 2;
    for (i = 0; i < 2; i++) {
        p[0] = (u8)(pairlist[i] >> 8);
        p[1] = (u8)pairlist[i];
        p += 2;
    }
    p[0] = 1;
    p[1] = 0;

    total = p + 2 - msg;
    len = total - 5;
    bodyLen = len - 4;
    msg[0] = 0x16;
    msg[1] = 3;
    msg[2] = 0;
    msg[3] = (u8)(len >> 8);
    msg[4] = (u8)len;
    msg[5] = 1;
    msg[6] = (u8)(bodyLen >> 16);
    msg[7] = (u8)(bodyLen >> 8);
    msg[8] = (u8)bodyLen;
    func_ov009_0215b198(msg, len + 5, NULL, 0, soc);
    update_digest(ctx, msg + 5, len);
    data_ov009_021a82b0(msg);
}

// RSA-encrypts the pre-master secret using the inline key retained from the leaf
// certificate, then sends and hashes the complete ClientKeyExchange message.
// pokebw2's SSLi_SendClientKeyExchange
static void send_client_key_exchange(CPSSoc *soc) {
    SSLContext *ctx = (SSLContext *)soc->con;
    u16 *digits, *modulus;
    int n;
    u8 *padded;
    int len, i;
    u8 *packet, *out;
    u16 *base, *exponent;
    u32 priority;
    if (ctx->certificate_phase.certificate_requested)
        func_ov009_0215b198((u8 *)alert_nocert, 7, NULL, 0, soc);
    ctx->secret[0] = 3;
    ctx->secret[1] = 0;
    set_random(ctx->secret + 2, 0x2e);
    len = ctx->certificate_key.modulus_len;
    n = (len * 2) / 2;
    padded = (u8 *)data_ov009_021a8280(len);
    if (padded == NULL) {
        ctx->state = SSLI_STATE_ERROR;
        return;
    }
    padded[0] = 0;
    padded[1] = 2;
    set_random(padded + 2, len - 0x33);
    padded[len - 0x31] = 0;
    MI_CpuCopy8(ctx->secret, padded + len - 0x30, 0x30);
    digits = (u16 *)data_ov009_021a8280(n * 8);
    if (digits == NULL) {
        data_ov009_021a82b0(padded);
        ctx->state = SSLI_STATE_ERROR;
        return;
    }
    base = digits + n;
    exponent = base + n;
    modulus = exponent + n;
    CPSi_big_from_char(base, padded, len, n);
    CPSi_big_from_char(exponent, ctx->certificate_key.exponent, ctx->certificate_key.exponent_len, n);
    CPSi_big_from_char(modulus, ctx->certificate_key.modulus, len, n);
    priority = enter_computebound();
    CPSi_big_power(digits, base, exponent, n, modulus);
    exit_computebound(priority);
    packet = (u8 *)data_ov009_021a8280(len + 0x49);
    if (packet == NULL) {
        data_ov009_021a82b0(padded);
        data_ov009_021a82b0(digits);
        ctx->state = SSLI_STATE_ERROR;
        return;
    }
    packet[0] = 0x16;
    packet[1] = 3;
    packet[2] = 0;
    packet[3] = (len + 4) >> 8;
    packet[4] = len + 4;
    packet[5] = 0x10;
    packet[6] = len >> 16;
    packet[7] = len >> 8;
    packet[8] = len;
    out = packet + 9;
    if (len & 1)
        *out++ = digits[len / 2];
    for (i = len / 2 - 1; i >= 0; i--) {
        *out++ = digits[i] >> 8;
        *out++ = digits[i];
    }
    func_ov009_0215b198(packet, len + 9, NULL, 0, soc);
    update_digest(ctx, packet + 5, len + 4);
    data_ov009_021a82b0(packet);
    data_ov009_021a82b0(digits);
    data_ov009_021a82b0(padded);
}

// Waits for the server's ChangeCipherSpec and Finished, in that order; TRUE if the connection ended first (the
// Finished check itself sets ctx->state to an error rather than failing this)
// pokebw2's SSLi_WaitForChangeCipherSpecAndFinished
static BOOL mustget_change_cipher_spec_and_finished(CPSSoc *soc) {
    if (parse_record(soc) != SSLI_STATE_EXPECT_FINISHED) {
        return TRUE;
    }
    if (parse_record(soc) != SSLI_STATE_FINISHED_VERIFIED) {
        return TRUE;
    }
    return FALSE;
}

// Drives a full or abbreviated handshake: ClientHello, then everything up to ServerHelloDone (full) or straight to
// ChangeCipherSpec (a session the server resumed); then ClientKeyExchange, or not, and both sides' ChangeCipherSpec
// and Finished. State 8 allows application records after the exchange.
// pokebw2's SSLi_DoHandshake
static BOOL ssl_connect_try(CPSSoc *soc) {
    SSLContext *ctx = (SSLContext *)soc->con;
    int state;

    func_ov009_02162418(soc);
    do {
        state = parse_record(soc);
        if (state == SSLI_STATE_ERROR)
            return TRUE;
    } while (state != SSLI_STATE_SERVER_HELLO_DONE && !ctx->unk_031);

    if (ctx->unk_031) {
        create_key_block(ctx);
        if (mustget_change_cipher_spec_and_finished(soc))
            return TRUE;
        func_ov009_021622c0(soc);
    } else {
        send_client_key_exchange(soc);
        create_master_secret(ctx);
        if (ctx->resumed)
            func_ov009_0215fee0(ctx, soc->remote_ip, soc->remote_port);
        create_key_block(ctx);
        func_ov009_021622c0(soc);
        if (mustget_change_cipher_spec_and_finished(soc))
            return TRUE;
    }
    ctx->state = SSLI_STATE_APPLICATION_DATA;
    return FALSE;
}

// CPSi_SslConnect's role: connects the socket's TCP first if it isn't already, then runs the handshake
// pokebw2's CPS_SslConnect
int CPSi_SslConnect(CPSSoc *soc) {
    u8 state = soc->state;
    SSLContext *ctx = (SSLContext *)soc->con;

    if (state != CPS_STT_ESTABLISHED) {
        if (func_ov009_0215abb0(soc) != 0) {
            return TRUE;
        }
    }

    ctx->state = SSLI_STATE_INITIAL;
    ctx->read_mac_secret = NULL;
    ctx->is_server = 0;
    CPSi_sha1_init(&ctx->handshake_sha1);
    CPSi_md5_init(&ctx->handshake_md5);
    return ssl_connect_try(soc);
}

// CPSi_SslRead's role: a pointer into the application data the handshake has buffered and not yet given out, and
// *len to how much of it there is; NULL (with *len 0) once the connection has nothing left and has closed
// pokebw2's CPS_SslRead
u8 *CPSi_SslRead(u32 *len, CPSSoc *soc) {
    SSLContext *ctx = (SSLContext *)soc->con;

    if (ctx->read_buf != NULL && !ctx->application_record_ready) {
        u8 *data = ctx->read_buf + ctx->read_buf_pos;
        u32 remaining = ctx->read_buf_len - ctx->read_buf_pos;

        if (tcp_read_raw_nbytes(data, remaining, soc) != 0) {
            data_ov009_021a82b0(ctx->read_buf);
            ctx->read_buf = NULL;
            *len = 0;
            return NULL;
        }
        func_ov009_02161c80(ctx, ctx->read_buf);
        if (!ctx->application_record_ready) {
            ctx->read_buf = NULL;
        }
    }

    while (ctx->read_buf == NULL) {
        if (parse_record(soc) == SSLI_STATE_ERROR) {
            *len = 0;
            return NULL;
        }
    }

    *len = ctx->read_buf_len - ctx->read_buf_pos;
    return ctx->read_buf + ctx->read_buf_pos;
}

// CPSi_SslConsume's role: marks len bytes of what CPSi_SslRead last returned as taken, freeing the
// buffer once all of it has been
// pokebw2's CPS_SslReadRelease
void CPSi_SslConsume(u32 len, CPSSoc *soc) {
    SSLContext *ctx = (SSLContext *)soc->con;

    if (len >= ctx->read_buf_len - ctx->read_buf_pos) {
        if (ctx->read_buf != NULL) {
            data_ov009_021a82b0(ctx->read_buf);
        }
        ctx->read_buf = NULL;
        return;
    }
    ctx->read_buf_pos += len;
}

// Copies currently available TCP bytes into an incomplete record without waiting
// for the rest. Once complete, dispatch transfers or frees the allocation.
// pokebw2's SSLi_FillReadBuffer
static void try_fill_record(CPSSoc *soc) {
    SSLContext *ctx = (SSLContext *)soc->con;
    u32 len, remaining;
    u8 *data;
    BOOL complete;
    if (ctx->read_buf == NULL) {
        if (soc->rcvbuf_len < 5)
            return;
        data = func_ov009_0215ae18(&len, soc);
        len = (data[3] << 8) + data[4] + 5;
        if (len > 0x4805) {
            ctx->state = SSLI_STATE_ERROR;
            return;
        }
        ctx->read_buf = (u8 *)data_ov009_021a8280(len);
        if (ctx->read_buf == NULL) {
            ctx->state = SSLI_STATE_ERROR;
            return;
        }
        ctx->read_buf_len = len;
        ctx->read_buf_pos = 0;
        ctx->application_record_ready = 0;
    } else if (soc->rcvbuf_len == 0) {
        return;
    }
    data = func_ov009_0215ae18(&len, soc);
    remaining = ctx->read_buf_len - ctx->read_buf_pos;
    if (len >= remaining) {
        len = remaining;
        complete = TRUE;
    } else {
        complete = FALSE;
    }
    MI_CpuCopy8(data, ctx->read_buf + ctx->read_buf_pos, len);
    func_ov009_0215af14(len, soc);
    if (complete) {
        func_ov009_02161c80(ctx, ctx->read_buf);
        if (!ctx->application_record_ready)
            ctx->read_buf = NULL;
    } else {
        ctx->read_buf_pos += len;
    }
}

// CPSi_SslGetLength's role: how many bytes CPSi_SslRead would give out right now, or -1 once the connection has
// closed with nothing left
// pokebw2's CPS_SslCanRead
s32 CPSi_SslGetLength(CPSSoc *soc) {
    SSLContext *ctx = (SSLContext *)soc->con;

    if (ctx->read_buf == NULL || !ctx->application_record_ready) {
        try_fill_record(soc);
    }
    if (ctx->read_buf != NULL && ctx->application_record_ready) {
        return (s32)(ctx->read_buf_len - ctx->read_buf_pos);
    }
    if (ctx->read_buf == NULL) {
        if (soc->state != CPS_STT_ESTABLISHED || ctx->state == SSLI_STATE_ERROR) {
            return -1;
        }
    }
    return 0;
}

// CPSi_SslWrite2's role: sends data as application data records, each up to 0xb4f bytes of content, MACed and
// RC4-crypted by make_ciphertext. data2/len2 is a second buffer func_ov009_0215b470-style callers hand this to send right
// after the first, as one write; returns the total bytes sent, which may be less than len+len2 if data_ov009_021a8280 or the
// connection itself runs out partway
// pokebw2's CPS_SslWrite
u32 CPSi_SslWrite2(u8 *data, u32 len, u8 *data2, u32 len2, CPSSoc *soc) {
    SSLContext *con = (SSLContext *)soc->con;
    u8 *tmp;
    s32 total_len;
    s32 this_len;
    s32 part1_len;
    s32 part2_len;
    s32 total_sent_len;
    s32 this_sent_len;

    total_len = len + len2;
    total_sent_len = 0;
    do {
        if (total_len > 0xb4f) {
            this_len = 0xb4f;
        } else {
            this_len = total_len;
        }
        tmp = (u8 *)data_ov009_021a8280(this_len + 0x19);
        if (tmp == NULL) {
            break;
        }
        part1_len = this_len;
        if (len < part1_len) {
            part1_len = len;
        }
        part2_len = this_len - part1_len;
        MI_CpuCopy8(data, tmp + 5, part1_len);
        data += part1_len;
        len -= part1_len;
        MI_CpuCopy8(data2, tmp + 5 + part1_len, part2_len);
        data2 += part2_len;

        tmp[0] = 0x17;
        tmp[1] = 3;
        tmp[2] = 0;
        tmp[3] = (u8)(this_len >> 8);
        tmp[4] = (u8)this_len;
        this_sent_len = make_ciphertext(con, tmp);

        if (func_ov009_0215b198(tmp, this_sent_len, NULL, 0, soc) < this_sent_len) {
            this_len = 0;
        }
        data_ov009_021a82b0(tmp);
        total_sent_len += this_len;
        total_len -= this_len;
    } while (total_len != 0 && this_len != 0);
    return total_sent_len;
}

// This file's signature, "[SDK+UBIQUITOUS:SSL]", which CPS_SetSsl refers to purely to keep it linked in; the SDK's
// linker script gathers such strings into the main module (at 0x020050f0, after CPS's and SOCL's)
extern const char SSLi_Signature[];

// CPSi_SslShutdown's role: sends a close_notify alert if the connection ended in one already (SSLv3's own way to
// say goodbye), then resets the state so a later CPSi_SslConnect can reuse the connection
// pokebw2's CPS_SslShutdown
void CPSi_SslShutdown(CPSSoc *soc) {
    SSLContext *ctx = (SSLContext *)soc->con;

    if (ctx->state == SSLI_STATE_APPLICATION_DATA) {
        u8 msg[0x1c];

        msg[0] = 0x15;
        msg[1] = 3;
        msg[2] = 0;
        msg[3] = 0;
        msg[4] = 2;
        msg[5] = 1;
        msg[6] = 0;
        func_ov009_0215b198(msg, make_ciphertext(ctx, msg), NULL, 0, soc);
    }
    ctx->state = SSLI_STATE_INITIAL;
}

// CPSi_SslClose's role: frees the connection's read buffer, if it has one, and resets its state
// pokebw2's CPS_SslClose
void CPSi_SslClose(CPSSoc *soc) {
    SSLContext *ctx = (SSLContext *)soc->con;

    ctx->state = SSLI_STATE_INITIAL;
    if (ctx->read_buf != NULL) {
        data_ov009_021a82b0(ctx->read_buf);
    }
    ctx->read_buf = NULL;
}

// CPS_SetSsl's role: turns the SSL layer on for the caller's socket (func_ov009_0215fc80's own caller already set
// soc->con before this)
// pokebw2's CPS_SetSsl
void CPS_SetSsl(BOOL enable) {
    OSThread *thread;
    CPSSoc *soc;

    OSi_ReferSymbol(SSLi_Signature);
    thread = OS_GetCurrentThread();
    soc = (CPSSoc *)thread->specific[0];
    if (soc != NULL) {
        soc->ssl = (u8)enable;
    }
}

// CPSi_SslPeriodical's role: called once a second by CPS's own housekeeping thread. Expires session cache entries that
// have sat unused too long, and wakes any thread that has been blocked reading an SSL socket for too long, so its
// own timeout logic can run
// pokebw2's CPS_SslTick
void CPSi_SslPeriodical(s32 now) {
    u32 enabled = OS_DisableInterrupts();
    int i;
    OSThread *thread;

    for (i = 0; i < 4; i++) {
        SSLSessionCacheEntry *entry = &session[i];
        if (entry->valid && now - entry->time > 0x3bd) {
            entry->valid = FALSE;
        }
    }
    OS_RestoreInterrupts(enabled);

    for (thread = OSi_ThreadInfo.list; thread != NULL; thread = thread->next) {
        CPSSoc *soc = (CPSSoc *)thread->specific[0];
        if (soc != NULL && soc->thread != NULL && soc->ssl != 0 && soc->state == CPS_STT_ESTABLISHED &&
            ((SSLContext *)soc->con)->state < SSLI_STATE_APPLICATION_DATA && now - soc->when > 0xef &&
            soc->block_type == CPS_BLOCK_TCPREAD) {
            soc->state = 0;
            soc->block_type = CPS_BLOCK_NONE;
            OS_WakeupThreadDirect(soc->thread);
        }
    }
}

// CPSi_SslCleanup's role: clears the session cache, once at func_ov009_021573c4
// pokebw2's CPS_SslInit
void CPSi_SslCleanup(void) {
    MI_CpuFill8(session, 0, sizeof(session));
}
