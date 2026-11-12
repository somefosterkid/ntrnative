// The types and prototypes that the GameSpy web service files of overlay 175 share (sakeMain.c to AuthService.c,
// once glued as one file): Black's own names, until they take pokebw2's.
#ifndef BWPORT_DWC_WEBSERVICES_INTERNAL_H
#define BWPORT_DWC_WEBSERVICES_INTERNAL_H

#include "types.h"
#include "gs/string.h"
#include "dwc/auth.h"
#include "dwc/competition.h"
#include "dwc/dwc.h"
#include "dwc/ext.h"
#include "dwc/gdb.h"
#include "dwc/gsAvailable.h"
#include "dwc/gsCore.h"
#include "dwc/sc.h"
#include "gs/nonport.h"
#include "gs/md5c.h"
#include "gs/stringutil.h"
#include "gs/xml.h"
#include "gfl/ghttpEncryption.h"
#include "nitro/std.h"

typedef struct Ov175Pend { u8 unk_0[0xc]; void *unk_c; u8 unk_10[8]; s32 unk_18; u8 unk_1c[0x14]; s32 unk_30; u8 unk_34[0xc]; void *unk_40; s32 unk_44; } Ov175Pend;
typedef struct Ov175Cb { void (*unk_0)(s32, u32 *, s32); s32 unk_4; } Ov175Cb;
typedef struct Ov175Blob { u32 unk_0; u32 unk_4; u32 unk_8; u32 unk_c; u32 unk_10; u32 unk_14; u32 unk_18; u32 unk_1c; u8 unk_20[0x1f]; u8 unk_3f[0x15]; u8 unk_54[0x24]; u8 unk_78[0x104]; u8 unk_17c[0x104]; u8 unk_280[0x80]; u8 unk_300[0x80]; } Ov175Blob;
typedef struct Ov175Task { s32 unk_0; u8 unk_4[4]; void (*unk_8)(s32, s32, s32, s32); s32 unk_c; s32 unk_10; u8 unk_14[0x14]; s32 unk_28; u8 unk_2c[0x1c]; s32 unk_48; } Ov175Task;
typedef struct Ov175Call { s32 unk_0; u8 unk_4[8]; void (*unk_c)(s32, s32, s32, s32); u8 unk_10[8]; s32 unk_18; u8 unk_1c[0x14]; s32 unk_30; u8 unk_34[0x14]; s32 unk_48; } Ov175Call;
typedef struct Ov175Job { s32 unk_0; u32 unk_4; u8 unk_8[0xc]; s32 unk_14; u8 unk_18[0x14]; s32 unk_2c; u8 unk_30[0x18]; s32 unk_48; } Ov175Job;
void func_ov175_021e08e8(s32 param_1, u32 param_2, u32 *param_3, Ov175Job *param_4);
typedef struct Ov175_021dd1ecWork6 {
    s32 unk_00;
    u8 unk_04[4];
    s32 unk_08;
    u8 unk_0c[0x3b];
    u8 unk_47[0x341];
    s32 unk_388;
} Ov175_021dd1ecWork6;
void func_ov175_021df4fc(s32 param_1, Ov175_021dd1ecWork6 *param_2);
typedef struct Ov175Pair { s32 unk_0; s32 unk_4; } Ov175Pair;
typedef struct Ov175Inner { s32 unk_0; s32 unk_4; s32 unk_8; s32 unk_c; s32 unk_10; } Ov175Inner;
typedef struct Ov175Rec { s8 *unk_0; s32 unk_4; union { u8 b; s16 h; s32 w; f32 f; s64 i64; u16 *hp; s8 *sp; struct { s8 *data; s32 len; } bin; } v; } Ov175Rec;
typedef void (*Ov175Fn)(u32 param_1, u32 param_2);
typedef struct Ov175Key { u8 unk_0[0x19]; } Ov175Key;
typedef struct Ov175Ctx { u8 unk_0[0x4c]; s32 unk_4c; s32 unk_50; } Ov175Ctx;
typedef struct Ov175Req { s32 unk_0; s8 **unk_4; s32 unk_8; s8 **unk_c; s32 unk_10; s32 unk_14; s32 unk_18; } Ov175Req;
typedef struct Ov175Data { s32 unk_0; s8 unk_4[0x10]; s32 unk_14; s8 unk_18[0xc]; s32 unk_24; s32 unk_28; Ov175Key unk_2c; s32 unk_48; } Ov175Data;
typedef struct Ov175Op Ov175Op;
typedef void (*Ov175ReqCb)(Ov175Data *sake, Ov175Op *request, u32 result, void *input, void *output, void *userData);
typedef struct Ov175Desc { u32 unk_0; s8 *unk_4; s8 *unk_8; s8 *unk_c; s8 *unk_10; u32 (*unk_14)(Ov175Op *); u32 (*unk_18)(Ov175Op *); u32 (*unk_1c)(Ov175Op *); void (*unk_20)(Ov175Op *); void *unk_24; } Ov175Desc;
struct Ov175Op { Ov175Data *unk_0; s32 unk_4; void *unk_8; void *unk_c; Ov175ReqCb unk_10; void *unk_14; void *unk_18; void *unk_1c; Ov175Desc *unk_20; };
typedef struct Ov175SoapTask { u8 unk_0[0x24]; void *unk_24; } Ov175SoapTask;
typedef struct Ov175Slot { union { Ov175Op *req; s32 ghttp; } unk_0; union { s32 w; char *s; } unk_4; union { s32 w; s32 *p; GdbField *fields; char **names; } unk_8; union { s32 w; s32 *p; u8 b; GdbField *fields; } unk_c; union { s32 w; s32 *p; void (*fn)(); char **names; } unk_10; union { s32 w; void (*fn)(); } unk_14; union { s32 w; void (*fn)(); } unk_18; u8 unk_1c[0x18]; void (*unk_34)(); } Ov175Slot;
typedef struct Ov175Sub { u8 unk_0[0x18]; u32 unk_18; u32 unk_1c; u16 unk_20; u16 unk_22; u16 unk_24; u16 unk_26; u16 unk_28; u16 unk_2a; s32 unk_2c; s32 unk_30; s32 unk_34; s32 unk_38; s32 unk_3c; s32 unk_40; } Ov175Sub;
u8 *func_ov175_021e04fc(u8 *param_1, u32 param_2);
u32 func_ov175_021e1830(u32 *param_1);
void func_ov175_021e214c(void);
void func_ov175_021e1948(Ov175Data *param_1, Ov175Op *param_2, u32 param_3, void *param_4, void *param_5, void *param_6);
void func_ov175_021e212c(u32 param_1);
u32 func_ov175_021e1f18(s32 param_1, u32 param_2);
u32 func_ov175_021e2874(Ov175Blob *param_1, s8 *param_2, u32 *param_3);
void func_ov175_021e0c68(s32 param_1, u32 param_2, u32 *param_3, Ov175Task *param_4);
BOOL func_ov009_0216aae0(char *);
s32 func_ov175_021e1eac(s32 param_1, s32 param_2);
u32 func_ov175_021e1ec4(u32 param_1);
void func_ov175_021e216c(s32 param_1);
void func_ov175_021e2354(s32 param_1, u32 param_2, u32 *param_3, Ov175Cb *param_4);
u32 func_ov009_021835d0(u32 *param_1, u8 *param_2);
u32 func_ov175_021e2a5c(Ov175Blob *param_1, u32 *param_2);
BOOL func_ov009_02183560(u32 *);
u32 func_ov009_021837a4(u32 *, char *, u8 *, s32, u32 *);
u32 *func_ov009_0217fe68(u32, u32, u32, u32, u32, u32);
static inline u32 Ov175Swap32(u32 v) { return ((v >> 24) & 0xff) | ((v >> 8) & 0xff00) | ((v << 8) & 0xff0000) | ((v << 24) & 0xff000000); }
static inline u16 Ov175Swap16(u16 v) { return (u16)(((v >> 8) & 0xff) | ((v << 8) & 0xff00)); }
typedef struct Ov175Work { s32 unk_0; s32 unk_4; u16 unk_8; u8 unk_a[6]; u32 unk_10; s32 unk_14[64]; s32 unk_114[3]; s32 unk_120; Ov175Sub *unk_124; } Ov175Work;
Ov175Op *func_ov175_021dd2d4(Ov175Data *param_1, void *param_2, Ov175ReqCb param_3, void *param_4, s32 param_5, u32 (*param_6)(Ov175Op *));
u32 func_ov175_021de024(Ov175Op *param_1);
u32 func_ov175_021de0d4(Ov175Op *param_1);
u32 sakeiStartDeleteRecordRequest(Ov175Op *param_1);
u32 func_ov175_021debe0(Ov175Op *param_1);
u32 sakeiStartSearchForRecordsRequest(Ov175Op *param_1);
u32 func_ov175_021dece8(Ov175Op *param_1);
u32 sakeiStartGetRandomRecordRequest(Ov175Op *param_1);
void func_ov175_021de8ac(s32 param_1, s32 param_2, s32 param_3);
s32 func_ov175_021dd328(Ov175Data *param_1, Ov175Op *param_2);
u32 func_ov175_021dd98c(Ov175Op *param_1);
void func_ov175_021dda98(Ov175Op *param_1);
void func_ov175_021ddbe4(Ov175Op *param_1, Ov175Rec *param_2, s32 param_3);
u32 func_ov009_02183a24(u32 *param_1, u8 *param_2, u32 *param_3);
u32 func_ov175_021de2e0(Ov175Op *param_1, u32 *param_2, s32 *param_3, s32 param_4, s32 param_5);
u32 func_ov175_021dfa6c(Ov175Ctx *param_1);
extern u8 data_ov009_021ae11c;
s32 func_ov175_021df644(s32 param_1, s32 param_2);
u32 func_ov175_021dfab8(s32 param_1, s32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6);
void func_ov175_021df07c(u32 param_1, u32 param_2, u32 param_3, u32 param_4);
u32 func_ov175_021dfbb8(Ov175Ctx *param_1, Ov175Work *param_2, s32 param_3, s32 param_4, s32 param_5, u32 param_6, u32 param_7, u32 param_8);
void func_ov175_021df1a8(u32 param_1, u32 param_2, u32 param_3, u32 param_4);
u32 func_ov175_021dfaa8(s32 param_1, s8 *param_2);
s32 func_ov175_021dfa98(s32 param_1);
s32 func_ov175_021dfaa0(s32 param_1);
extern u8 data_ov009_021ae01c;
BOOL func_ov114_021bf520(u32 *param_1);
typedef struct Ov175_021dd1ecWork2 {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0c;
    u32 unk_10;
    u32 unk_14;
    u32 unk_18;
    u8 unk_1c[0xc];
    u32 unk_28;
    u32 unk_2c;
    u32 unk_30;
    u8 unk_34[0x14];
    u32 unk_48;
    u8 unk_4c[4];
    u32 unk_50;
    u8 unk_54[8];
    u8 unk_5c[0x28];
    u8 unk_84;
} Ov175_021dd1ecWork2;
u32 func_ov175_021e06dc(Ov175_021dd1ecWork2 *param_1, Ov175_021dd1ecWork2 *param_2);
void func_ov175_021e0730(u32 *param_1);
u32 func_ov175_021df7fc(void **client);
u32 func_ov175_021e26bc(Ov175Blob *param_1);
void func_ov175_021e0fe8(s32 param_1, s32 param_2, u32 *param_3, Ov175Call *param_4);
u32 func_ov175_021e0774(Ov175Job *param_1, u32 param_2, s32 param_3, s32 param_4, u32 param_5, u32 param_6, u32 param_7);
u32 func_ov175_021e0040(Ov175Work *param_1);
u32 func_ov175_021e0070(u32 *param_1);
u32 func_ov175_021e0080(u32 *param_1);
u32 func_ov175_021e0000(Ov175Work *param_1);
u32 func_ov175_021e0098(Ov175Work *param_1, s32 param_2, u32 param_3);
u32 func_ov175_021e02f0(Ov175Work *param_1, int param_2, u32 param_3);
u32 func_ov175_021e03f8(Ov175Work *param_1, int param_2, u32 param_3);
typedef struct Ov175_021dd1ecWork3 {
    u32 unk_00;
    u32 unk_04;
    u16 unk_08;
    u8 unk_0a[2];
    u32 unk_0c;
    u32 unk_10;
    u8 unk_14[0x104];
    u32 unk_118;
    u32 unk_11c;
    u32 unk_120;
    u32 unk_124;
} Ov175_021dd1ecWork3;
u32 func_ov175_021dff30(Ov175_021dd1ecWork3 *param_1);
s8 *func_ov175_021df968(void);
// pokebw2's GdbOperation: the operations of the record database client, as the texts of its log name them
typedef enum GdbOperation {
    GDB_OPERATION_NONE,
    GDB_OPERATION_INITIALIZE,
    GDB_OPERATION_CREATE_RECORD,
    GDB_OPERATION_DELETE_RECORD,
    GDB_OPERATION_UPDATE_RECORD,
    GDB_OPERATION_DOWNLOAD_FILE,
    GDB_OPERATION_UPLOAD_FILE,
    GDB_OPERATION_GET_MY_RECORDS,
    GDB_OPERATION_GET_RECORD_LIMIT,
    GDB_OPERATION_GET_SPECIFIC_RECORDS,
    GDB_OPERATION_RATE_RECORD,
    GDB_OPERATION_SEARCH_RECORDS,
    GDB_OPERATION_GET_RANDOM_RECORD,
    GDB_OPERATION_GET_RECORD_COUNT,
    GDB_OPERATION_NUM
} GdbOperation;
typedef struct Ov175Sake { s32 unk_0; Ov175Data *unk_4; s32 unk_8; GdbOperation unk_c; u32 unk_10; s32 unk_14; } Ov175Sake;
void func_ov175_021e17d8(s32 param_1, char *param_2, ...);
void func_ov175_021e13bc(void);
void func_ov175_021e191c(void);
u32 func_ov175_021e20d4(s32 param_1, u32 param_2);
u32 func_ov175_021dfa3c(u32 *param_1);
u32 func_ov175_021df5e4(u32 param_1);
void func_ov175_021defc4(void);
void gsCoreShutdown(void);
void gsCoreThink(u32 param_1);
u32 func_ov175_021e1f6c(u32 param_1, u32 *param_2);
u32 func_ov175_021ddb38(Ov175Rec *param_1, s32 param_2);
u32 func_ov175_021de14c(s8 **param_1, s32 param_2);
s8 *func_ov175_021e22e4(void);
u8 *func_ov175_021dfe20(void);
u32 func_ov175_021dfce8(Ov175Work *param_1);
u32 func_ov175_021dfcf4(u32 *param_1);
u32 func_ov175_021dfd00(u32 *param_1);
u32 func_ov175_021dfd0c(s32 param_1);
u32 func_ov175_021dfe14(Ov175_021dd1ecWork3 *param_1);
u8 *func_ov175_021e05ac(u8 *param_1, u8 *param_2);
u8 *func_ov175_021e054c(u8 *param_1, u32 param_2);
u8 *func_ov175_021e0504(u8 *param_1, s16 param_2);
u32 func_ov175_021dfc8c(Ov175Work *param_1, s32 param_2, s32 param_3, s32 param_4, u32 *param_5);
typedef struct Ov175_021dd1ecWork4 {
    u32 unk_00;
    u32 unk_04;
    u8 unk_08[0x18];
    u16 unk_20;
    u16 unk_22;
    u8 unk_24[8];
    u32 unk_2c;
    u32 unk_30;
    u32 unk_34;
    u8 unk_38[0xffc8];
} Ov175_021dd1ecWork4; // 0x10000 bytes

u32 func_ov175_021dfe2c(u8 *unused, u32 param_1, s32 param_2, s32 param_3, u32 *param_5);
u32 func_ov175_021e0a7c(Ov175Task *param_1, u32 param_2, u32 *param_3, u32 param_4, u32 param_5, Ov175Blob *param_6, u8 *param_7, u32 param_8, u32 param_9, u32 param_10);
u32 func_ov175_021dff78(Ov175Work *param_1, s32 param_2, u8 *param_3);
u32 func_ov175_021dffa0(Ov175Work *param_1, s32 param_2, u32 param_3);
u32 func_ov175_021dffc8(Ov175Work *param_1, s32 param_2, u32 param_3);
u32 func_ov175_021dfff8(Ov175Work *param_1, s32 param_2, s32 param_3, s32 param_4);
Ov175Op *func_ov175_021dd56c(Ov175Data *param_1, s32 param_2, void *param_3, Ov175ReqCb param_4, void *param_5);
void func_ov175_021dd604(Ov175Op *param_1);
u32 func_ov175_021dfb18(Ov175Work *param_1, s8 *param_2, s32 param_3, s32 param_4, s32 param_5, u32 param_6, u32 param_7, u32 param_8);
u32 func_ov175_021dfd24(Ov175Work *param_1, s32 param_2, u8 *param_3, s32 param_4, s32 param_5, s32 param_6, s32 param_7, s32 param_8);
void func_ov175_021df110(u32 param_1, u32 param_2, u32 param_3, u32 param_4);
u32 func_ov175_021dfd18(s32 param_1, s32 param_2, u32 param_3);
u32 func_ov175_021dfdfc(s32 *param_1, s16 param_2, u32 param_3);
u32 func_ov175_021dfe08(s32 *param_1, s16 param_2, u8 param_3);
u32 func_ov175_021e0db8(Ov175Pend *param_1, u32 param_2, s8 *param_3, s8 *param_4, Ov175Work *param_5, u8 *param_6, Ov175Blob *param_7, u8 *param_8, u32 param_9, u32 param_10, u32 param_11);
void func_ov175_021e0764();
void gsCoreInitialize(void);
void func_ov175_021df734(s32 param_1);
u32 DWCi_GetAuthInfo(s32 *, s32 *);
u32 func_ov175_021e2510(u32 param_1, u32 param_2, s8 *param_3, s8 *param_4, u32 param_5, u32 param_6);
void func_ov175_021df4a4(s32 param_1, char *param_2, ...);
u32 DWC_GetLastError(u32 *param_1);
u32 func_ov175_021df9d8(u32 gameId, u32 **client);
s32 func_ov175_021df65c(s32 param_1);
u32 func_ov009_021834e8(u32 *param_1, u8 *param_2);
void func_ov009_02181d84(s32 *param_1);
typedef struct Ov175_021dd1ecWork5 {
    s32 unk_00;
    u8 unk_04[4];
    s32 unk_08;
    s32 unk_0c;
} Ov175_021dd1ecWork5;
typedef struct Ov175I64 { s32 unk_0; s32 unk_4; } Ov175I64;
typedef struct Ov175Bytes8 { u8 unk_0[8]; } Ov175Bytes8;
typedef struct Ov175Auth {
    u32 status;
    u32 res;
    Ov175Blob blob;
    u8 bufA[0x104];
    u8 bufC[0x104];
    u8 digest[0x14];
} Ov175Auth;

extern char sakeiSoapUrl[128];
extern char scServiceURL[128];
extern char wsAuthServiceURL[128];

u32 func_ov175_021dd1ec(Ov175Data **param_1);
void func_ov175_021dd250(Ov175Data *param_1);
void func_ov175_021dd25c(Ov175Data *param_1, s8 *param_2, u32 param_3, s8 *param_4);
void func_ov175_021dd290(Ov175Data *param_1, u32 param_2, const u8 *param_3);
u32 func_ov175_021dd2cc(Ov175Data *param_1);
Ov175Op *func_ov175_021dd358(Ov175Data *param_1, void *param_2, Ov175ReqCb param_3, void *param_4);
Ov175Op *func_ov175_021dd380(Ov175Data *param_1, void *param_2, Ov175ReqCb param_3, void *param_4);
Ov175Op *func_ov175_021dd3a8(Ov175Data *param_1, void *param_2, Ov175ReqCb param_3, void *param_4);
Ov175Op *func_ov175_021dd3d0(Ov175Data *param_1, void *param_2, Ov175ReqCb param_3, void *param_4);
u32 func_ov175_021dd3f8(Ov175Data *param_1, s8 *param_2);
u32 func_ov175_021dd420(Ov175Data *param_1, u32 param_2, s8 *param_3);
u32 func_ov175_021dd4b0(Ov175Data *param_1, s8 *param_2);
u32 func_ov175_021dd4d8(Ov175Data *param_1, s8 *param_2);
void func_ov175_021dd560(Ov175Data *param_1, Ov175Op *param_2);
s32 func_ov175_021ddb10(Ov175Op *param_1, Ov175Desc *param_2);
u32 func_ov175_021df884(Ov175_021dd1ecWork2 *param_1);
void func_ov175_021df8d4(u32 *param_1);
void func_ov175_021df928(s32 param_1, s8 *param_2);
void func_ov175_021df948(s32 param_1, s8 *param_2);

#endif
