// Black's code at ov114 0x021ba7dc-0x021baa1c. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
// (the same functions in the same order); the name is that file's.

#include <string.h>
#include <stdio.h>
#include "types.h"
#include "dwc/dwc.h"
#include "dwc/ext.h"
#include "dwc/nhttp.h"
#include "dwc/base.h"
#include "gfl/std.h"
#include "nitro/os.h"
#include "nitro/std.h"
#include "nitrowifi/cps.h"
#include "nitrowifi/soc.h"
#include "field/unity_tower.h"
#include "save/player_info.h"
#include "struct_decls.h"
#include "system/str_tool.h"
#include "gs/nonport.h"
#include "gs/darray.h"
#include "nitro/os.h"
#include "gfl/blact.h"
#include "dwc/nhttp_internal.h"

#pragma thumb off
void func_ov114_021ba7dc(NHttpState *state) {
    state->alloc = NULL;
    state->free = NULL;
    state->started = FALSE;
    state->socket = -1;
    func_ov114_021ba87c(state, 0);
    func_ov114_021ba874(state, 0);
    state->quit = FALSE;
    state->stack = NULL;
    state->unk_20 = 0;
}

void *func_ov114_021ba820(u32 size, int align) {
    NHttpState *state = &func_ov114_021bef08()->state;

    if (state->alloc != NULL) {
        return state->alloc(size, align);
    }
    return NULL;
}

void func_ov114_021ba850(void *ptr) {
    NHttpState *state = &func_ov114_021bef08()->state;

    if (state->free != NULL) {
        state->free(ptr);
    }
}

void func_ov114_021ba874(NHttpState *state, int error) {
    state->error = error;
}

void func_ov114_021ba87c(NHttpState *state, int error) {
    state->sslError = error;
}

int func_ov114_021ba884(NHttpState *state) {
    return state->sslError;
}

// Starts the library: its lists and its mutex, the stack of the thread that runs the requests and the thread. Returns
// whether it did, and the error says why not
BOOL func_ov114_021ba88c(NHttpRoot *root, NHttpAllocFunc alloc, NHttpFreeFunc free, int priority) {
    NHttpState *state = func_ov114_021bef3c(root);
    NHttpListInfo *list = func_ov114_021bef40(root);
    NHttpReqInfo *req = func_ov114_021bef48(root);
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(root);
    NHttpThreadInfo *thread = func_ov114_021bef50(root);

    state->alloc = alloc;
    state->free = free;
    state->error = 0;
    state->sslError = 0;
    state->quit = FALSE;
    state->unk_24 = 0;
    func_ov114_021bacec(list);
    func_ov114_021bb3c0(req);
    func_ov114_021befa4(mutexInfo);
    func_ov114_021bef6c();
    state->socket = -1;
    state->stack = func_ov114_021ba820(0x2000, 8);
    if (state->stack == NULL) {
        func_ov114_021ba874(state, 1);
        func_ov114_021befc8();
        return FALSE;
    }
    if (!func_ov114_021b9b64(thread, priority, state->stack)) {
        func_ov114_021ba874(state, 9);
        func_ov114_021ba850(state->stack);
        state->stack = NULL;
        func_ov114_021befc8();
        return FALSE;
    }
    state->started = TRUE;
    return TRUE;
}

// Ends the library: cancels the requests, ends the thread and frees its stack, then calls back
void func_ov114_021ba988(NHttpRoot *root, void (*callback)(void)) {
    NHttpState *state = func_ov114_021bef3c(root);
    NHttpThreadInfo *thread = func_ov114_021bef50(root);

    func_ov114_021b9c1c(thread, TRUE);
    func_ov114_021bbaac(root);
    func_ov114_021b9bd4(thread, state);
    func_ov114_021ba850(state->stack);
    state->stack = NULL;
    func_ov114_021befc8();
    state->started = FALSE;
    if (callback != NULL) {
        callback();
    }
    func_ov114_021bec48();
    if (state->socket >= 0) {
        func_ov114_021b9cfc(NULL, NULL, state->socket);
        state->socket = -1;
    }
}

int func_ov114_021baa14(NHttpState *state) {
    return state->error;
}
