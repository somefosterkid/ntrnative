// Black's code at ov114 0x021be648-0x021bea20. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
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

static u8 data_ov114_021ce53c[0x4] = {
    0x3a, 0x00, 0x00, 0x00
};
#pragma thumb off
int func_ov114_021be648(NHttpAllocFunc alloc, NHttpFreeFunc free, u32 priority) {
    return -(func_ov114_021ba88c(func_ov114_021bef08(), alloc, free, priority) == 0);
}

void func_ov114_021be680(void (*callback)(void)) {
    func_ov114_021ba988(func_ov114_021bef08(), callback);
}

void func_ov114_021be698(void) {
    func_ov114_021ba988(func_ov114_021bef08(), NULL);
}

int func_ov114_021be6ac(void) {
    return func_ov114_021baa14(&func_ov114_021bef08()->state);
}

int func_ov114_021be6bc(int connection, const char *name, const char *value) {
    NHttpMutexInfo *mutexInfo = func_ov114_021bef58(func_ov114_021bef08());

    return func_ov114_021be6ec(func_ov114_021bec10(mutexInfo, (NHttpConnection *)connection), name, value);
}

int func_ov114_021be6ec(NHttpRequest *request, const char *name, const char *value) {
    NHttpState *state = func_ov114_021bef3c(func_ov114_021bef08());

    if (request == NULL) {
        return -1;
    }
    if (request->sent != 0) {
        return -1;
    }
    return -(func_ov114_021bac38(request, state, name, value) == 0);
}

int func_ov114_021be748(int connection, const char *name, const char *value) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpState *state = func_ov114_021bef3c(root);
    NHttpRequest *request = func_ov114_021bec10(func_ov114_021bef58(root), (NHttpConnection *)connection);

    return -(func_ov114_021bac58(request, state, name, value) == 0);
}

// Sets the data that a request posts as it is, instead of as parts
int func_ov114_021be79c(int connection, const void *data, u32 size) {
    NHttpRequest *request = func_ov114_021bec10(func_ov114_021bef58(func_ov114_021bef08()), (NHttpConnection *)connection);

    if (request == NULL) {
        return -1;
    }
    if (request->sent != 0) {
        return -1;
    }
    if (request->postData != NULL) {
        return -1;
    }
    request->rawPost = TRUE;
    request->rawData = data;
    request->rawSize = size;
    return 0;
}

int func_ov114_021be7fc(int connection, const char *name, char **value) {
    NHttpResponse *response =
        func_ov114_021bec2c(func_ov114_021bef58(func_ov114_021bef08()), (NHttpConnection *)connection);

    if (response == NULL) {
        return -1;
    }
    return func_ov114_021bbcf0(response, name, value);
}

int func_ov114_021be838(int connection, char **header) {
    NHttpRoot *root = func_ov114_021bef08();
    NHttpState *state = func_ov114_021bef3c(root);
    NHttpResponse *response = func_ov114_021bec2c(func_ov114_021bef58(root), (NHttpConnection *)connection);

    if (response == NULL) {
        return -1;
    }
    return func_ov114_021bbdfc(state, response, header);
}

// The status of the answer, once its header has come, or -1
int func_ov114_021be880(int connection) {
    NHttpResponse *response =
        func_ov114_021bec2c(func_ov114_021bef58(func_ov114_021bef08()), (NHttpConnection *)connection);

    if (response == NULL) {
        return -1;
    }
    if (response->headerDone) {
        return response->statusCode;
    }
    return -1;
}

int func_ov114_021be8b8(int connection, int option) {
    NHttpRequest *request = func_ov114_021bec10(func_ov114_021bef58(func_ov114_021bef08()), (NHttpConnection *)connection);

    if (request == NULL) {
        return -1;
    }
    request->verifyOption = option;
    return 0;
}

// Sets the user and the password of the request's basic authorization (32 characters at most each)
int func_ov114_021be8e8(int connection, const char *user, const char *password) {
    NHttpRequest *request = func_ov114_021bec10(func_ov114_021bef58(func_ov114_021bef08()), (NHttpConnection *)connection);
    char text[0x41];
    int userLength;
    int passwordLength;

    if (request == NULL || user == NULL || password == NULL) {
        return -1;
    }
    userLength = func_ov114_021ba124(user, 0x21);
    passwordLength = func_ov114_021ba124(password, 0x21);
    if (userLength <= 0x20 && passwordLength <= 0x20) {
        func_ov114_021ba13c(text, sizeof(text));
        func_ov114_021ba100(text, user, userLength);
        func_ov114_021ba100(text + userLength, data_ov114_021ce53c, 1);
        func_ov114_021ba100(text + (userLength + 1), password, passwordLength);
        request->authorizationSize = func_ov114_021ba714(request->authorization, text);
    } else {
        return -1;
    }
    return 0;
}

int func_ov114_021be9c8(int connection, void *certificates, int count) {
    NHttpRequest *request = func_ov114_021bec10(func_ov114_021bef58(func_ov114_021bef08()), (NHttpConnection *)connection);

    return func_ov114_021be9f8(request, certificates, count);
}

int func_ov114_021be9f8(NHttpRequest *request, void *certificates, int count) {
    if (request == NULL) {
        return -1;
    }
    request->unk_C8 = 0;
    request->unk_DC = 0;
    request->rootCerts = certificates;
    request->rootCertCount = count;
    return 0;
}
