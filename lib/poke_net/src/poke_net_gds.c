// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/poke_net/src/poke_net_gds.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// The GDS client's own functions: starting and ending it, the state of the request, and the requests themselves, each
// of which builds its data and hands it to the request engine. The ROM names nothing here; the file's name and its
// functions' are those of the Generation 4 library, with our own for the requests that Generation 5 changed.

#include "types.h"
#include "poke_net/poke_net_gds.h"
#include "poke_net_internal.h"
#include "dwc/ext.h"

typedef struct {
    u16 SearchType;
    u16 SearchOpt;
    s64 SearchParam;
} POKE_NET_GDS_REQUEST_MUSICALSHOT_GET;

typedef struct {
    u64 Code;
    u32 ServerVersion;
} POKE_NET_GDS_REQUEST_BATTLEDATA_GET;

typedef struct {
    u64 Code;
} POKE_NET_GDS_REQUEST_BATTLEDATA_FAVORITE;

#define POKE_NET_GDS_DEBUG_MESSAGE_SIZE 0x400

// pokebw2's POKE_NET_GetRequestSize


BOOL func_ov125_021d8598(POKE_NET_REQUESTCOMMON_AUTH *auth, const char *url, u16 port) {
    return func_ov125_021d7770(url, port, auth);
}

void func_ov125_021d85b4(void) {
    func_ov125_021d770c();
}

int func_ov125_021d85c0(void) {
    int status;

    func_ov125_021d7cac();
    status = func_ov125_021d7bcc();
    func_ov125_021d7cc0();
    return status;
}


int func_ov125_021d85dc(void) {
    return func_ov125_021d7c48();
}

void *func_ov125_021d85e8(void) {
    return func_ov125_021d7bdc();
}

s32 func_ov125_021d85f4(void) {
    return func_ov125_021d7c14();
}

s32 func_ov125_021d8600(s32 reqno) {
    return func_ov125_021d89ec(reqno);
}

BOOL func_ov125_021d860c(POKE_NET_GDS_MUSICALSHOT_SEND *data, void *response) {
    POKE_NET_GDS_MUSICALSHOT_SEND request;

    request = *data;
    return func_ov125_021d7a10(POKE_NET_GDS_REQCODE_MUSICALSHOT_REGIST, &request, response, -1);
}

BOOL func_ov125_021d8654(s32 monsno, void *response) {
    POKE_NET_GDS_REQUEST_MUSICALSHOT_GET request;

    request.SearchType = 0;
    request.SearchOpt = 0;
    request.SearchParam = monsno;
    return func_ov125_021d7a10(POKE_NET_GDS_REQCODE_MUSICALSHOT_GET, &request, response, -1);
}

BOOL func_ov125_021d8694(BattleRecVideo *data, u8 *sign, s32 signSize, void *response) {
    POKE_NET_GDS_REQUEST_BATTLEDATA_REGIST request = {0};

    if (signSize > POKE_NET_GDS_BATTLEDATA_SIGN_SIZE) {
        return FALSE;
    }
    request.Data = *data;
    request.SignSize = signSize;
    memcpy(request.Sign, sign, signSize);
    return func_ov125_021d7a10(POKE_NET_GDS_REQCODE_BATTLEDATA_REGIST, &request, response, -1);
}

BOOL func_ov125_021d8754(POKE_NET_GDS_BATTLEDATA_SEARCHCONDITION *condition, void *response) {
    POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCH request;

    request.SearchType = POKE_NET_GDS_BATTLEDATA_SEARCHTYPE_CONDITION;
    request.ConditionParam = *condition;
    request.SearchType = POKE_NET_GDS_BATTLEDATA_SEARCHTYPE_CONDITION;
    request.SearchOpt = 0;
    return func_ov125_021d7a10(POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH, &request, response, -1);
}

BOOL func_ov125_021d879c(u32 serverVersion, void *response) {
    POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCH request = {0};

    request.SearchType = POKE_NET_GDS_BATTLEDATA_SEARCHTYPE_LIST1;
    request.ConditionParam.ServerVersion = serverVersion;
    request.SearchParam = 0;
    request.SearchOpt = 0;
    return func_ov125_021d7a10(POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH, &request, response, -1);
}

BOOL func_ov125_021d87fc(u32 serverVersion, void *response) {
    POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCH request = {0};

    request.SearchType = POKE_NET_GDS_BATTLEDATA_SEARCHTYPE_LIST3;
    request.ConditionParam.ServerVersion = serverVersion;
    request.SearchParam = 0;
    request.SearchOpt = 0;
    return func_ov125_021d7a10(POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH, &request, response, -1);
}

BOOL func_ov125_021d885c(u32 serverVersion, void *response) {
    POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCH request = {0};

    request.SearchType = POKE_NET_GDS_BATTLEDATA_SEARCHTYPE_LIST2;
    request.ConditionParam.ServerVersion = serverVersion;
    request.SearchParam = 0;
    request.SearchOpt = 0;
    return func_ov125_021d7a10(POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH, &request, response, -1);
}

BOOL func_ov125_021d88bc(u64 code, u32 serverVersion, void *response) {
    POKE_NET_GDS_REQUEST_BATTLEDATA_GET request;

    request.Code = code;
    request.ServerVersion = serverVersion;
    return func_ov125_021d7a10(POKE_NET_GDS_REQCODE_BATTLEDATA_GET, &request, response, -1);
}

BOOL func_ov125_021d88f0(u64 code, void *response) {
    POKE_NET_GDS_REQUEST_BATTLEDATA_FAVORITE request;

    request.Code = code;
    return func_ov125_021d7a10(POKE_NET_GDS_REQCODE_BATTLEDATA_FAVORITE, &request, response, -1);
}














s32 func_ov125_021d891c(s32 reqno) {
    s32 size;

    switch (reqno) {
    case POKE_NET_GDS_REQCODE_DEBUG_MESSAGE:
        size = POKE_NET_GDS_DEBUG_MESSAGE_SIZE;
        break;
    case POKE_NET_GDS_REQCODE_MUSICALSHOT_REGIST:
        size = sizeof(POKE_NET_GDS_MUSICALSHOT_SEND);
        break;
    case POKE_NET_GDS_REQCODE_MUSICALSHOT_GET:
        size = sizeof(POKE_NET_GDS_REQUEST_MUSICALSHOT_GET);
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_REGIST:
        size = sizeof(POKE_NET_GDS_REQUEST_BATTLEDATA_REGIST);
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH:
        size = sizeof(POKE_NET_GDS_REQUEST_BATTLEDATA_SEARCH);
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_GET:
        size = sizeof(POKE_NET_GDS_REQUEST_BATTLEDATA_GET);
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_FAVORITE:
        size = sizeof(POKE_NET_GDS_REQUEST_BATTLEDATA_FAVORITE);
        break;
    default:
        return -1;
    }
    return size + POKE_NET_REQUEST_HEADER_SIZE;
}

// pokebw2's func_ov125_021d89ec
// pokebw2's POKE_NET_GetResponseMaxSize
s32 func_ov125_021d89ec(s32 reqno) {
    s32 reqnos[] = {
        POKE_NET_GDS_REQCODE_DEBUG_MESSAGE,
        POKE_NET_GDS_REQCODE_MUSICALSHOT_REGIST,
        POKE_NET_GDS_REQCODE_MUSICALSHOT_GET,
        POKE_NET_GDS_REQCODE_BATTLEDATA_REGIST,
        POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH,
        POKE_NET_GDS_REQCODE_BATTLEDATA_GET,
        POKE_NET_GDS_REQCODE_BATTLEDATA_FAVORITE,
        -1,
    };
    s32 size;
    s32 i;

    switch (reqno) {
    case POKE_NET_GDS_REQCODE_ADMIN:
        // The largest of all, each of which has its header counted already
        size = -1;
        for (i = 0;; i++) {
            s32 max;

            if (reqnos[i] == -1) {
                break;
            }
            max = func_ov125_021d89ec(reqnos[i]);
            if (max > size) {
                size = max;
            }
        }
        if (size == -1) {
            return -1;
        }
        break;
    case POKE_NET_GDS_REQCODE_DEBUG_MESSAGE:
        size = POKE_NET_GDS_DEBUG_MESSAGE_SIZE;
        break;
    case POKE_NET_GDS_REQCODE_MUSICALSHOT_REGIST:
        size = sizeof(POKE_NET_GDS_RESPONSE_REGIST);
        break;
    case POKE_NET_GDS_REQCODE_MUSICALSHOT_GET:
        size = sizeof(POKE_NET_GDS_RESPONSE_MUSICALSHOT_GET);
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_REGIST:
        size = sizeof(POKE_NET_GDS_RESPONSE_REGIST);
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_SEARCH:
        size = sizeof(POKE_NET_GDS_RESPONSE_BATTLEDATA_SEARCH);
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_GET:
        size = sizeof(POKE_NET_GDS_RESPONSE_BATTLEDATA_GET);
        break;
    case POKE_NET_GDS_REQCODE_BATTLEDATA_FAVORITE:
        size = 0;
        break;
    default:
        return -1;
    }
    return size + POKE_NET_RESPONSE_HEADER_SIZE;
}
