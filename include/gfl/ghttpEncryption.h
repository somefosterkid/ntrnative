#ifndef GFL_GHTTPENCRYPTION_H
#define GFL_GHTTPENCRYPTION_H

#include "types.h"

struct GHIPost;
struct SoapWriter;

// dwc_ghttp.c's and ghttpMain.c's functions that the Sake (gdb.c), sc, gamestats and SOAP code calls
// (dwc/ghttp_internal.h includes this)

// What the game gives to be done with the result of a request
typedef void (*DWCGHTTPCompletedFunc)(char *buffer, int length, int result, void *param);
typedef void (*DWCGHTTPProgressFunc)(int state, const char *buffer, int length, int received, int total, void *param);
// What a request of ghttpMain.c calls with its result (TRUE: the buffer is the caller's to free) and its progress
typedef BOOL (*DWCGHTTPRawCompletedFunc)(int request, int result, char *buffer, int length, void *param);
typedef void (*DWCGHTTPRawProgressFunc)(int request, int state, const char *buffer, int length, int received, int total, void *param);

// pokebw2's DWC_InitGHTTP (the game name is not used)
u32 func_ov114_021bf558(const char *gamename);
// pokebw2's DWC_ShutdownGHTTP
u32 func_ov114_021bf57c(void);
// pokebw2's DWC_ProcessGHTTP
u32 func_ov114_021bf5b8(void);
// pokebw2's DWCi_RemoveGHTTPRootCA
// Removes the root certificates installed for the URL; TRUE when it had some
BOOL func_ov114_021bf540(const char *url);
// pokebw2's DWC_GetGHTTPDataEx: starts a request for a URL; the request, or an error below 0
s32 func_ov114_021bf6c4(const char *url, int bufferSize, BOOL freeBuffer, DWCGHTTPProgressFunc progress, DWCGHTTPCompletedFunc completed, void *param);
// pokebw2's DWCi_HandleGHTTPError
// Turns a GHTTP result into a DWC error, which it sets; the error code
s32 func_ov114_021bf8c0(s32 result);
// pokebw2's ghttpGetExA: starts a request (posting the post when there is one); the request, or -1
s32 func_ov114_021c20a4(const char *url, const char *headers, char *buffer, int bufferSize, struct GHIPost *post, BOOL throttle, BOOL blocking, DWCGHTTPRawProgressFunc progress, DWCGHTTPRawCompletedFunc completed, void *param);
// pokebw2's ghttpRequestThink: runs a request for a frame; FALSE when there is no such request
u32 func_ov114_021c2284(s32 request);
// pokebw2's ghttpGetState
// The state of a request
s32 func_ov114_021c22d4(s32 request);
// pokebw2's ghttpCancelRequest
void func_ov114_021c22a4(s32 request);
// pokebw2's ghttpNewPost: a new post, or NULL
struct GHIPost *func_ov114_021c2304(void);
// pokebw2's ghttpPostSetAutoFree: whether the post is freed with its request
void func_ov114_021c2310(struct GHIPost *post, BOOL autoFree);
// pokebw2's ghttpFreePost: frees the post of a request (does nothing without one)
void func_ov114_021c2324(int request, struct GHIPost *post);
// pokebw2's ghttpPostAddXml: adds the message of an XML writer to the post
BOOL func_ov114_021c2338(struct GHIPost *post, struct SoapWriter *writer);
// pokebw2's ghttpPostAddFileFromMemory: adds a file held in memory to the post
BOOL func_ov114_021c24b4(struct GHIPost *post, const char *name, const void *buffer, int len, const char *reportFilename, const char *contentType);

#endif
