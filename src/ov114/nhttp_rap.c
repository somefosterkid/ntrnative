// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov189/nhttp_rap.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
// nhttp_rap.c: Game Freak's wrapper of HTTP requests to the game's servers. The name is the ROM's own; the
// functions' names are ours

#include "gfl/nhttp_rap.h"
#include "types.h"
#include "constants/language.h"
#include "constants/version.h"
#include "dwc/nhttp.h"
#include "dwc/nhttp_internal.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/net_state.h"
#include "gfl/std.h"
#include "nitro/os.h"

#define NHTTP_RAP_URL_SIZE 0x190
#define NHTTP_RAP_BODY_SIZE 0x400
#define NHTTP_RAP_NUM_REQUESTS 10 // Black's: Black 2 has an 11th, savedata.getbw
#define NHTTP_RAP_REQUEST_CREATE_DATA 4
#define NHTTP_RAP_REQUEST_VALIDATE 7

// Black 2's, not checked against Black
typedef struct NHttpRapLogin {
    u8 unk_00[0x45];
    char token[0x174 - 0x45];
} NHttpRapLogin;

// Black 2's; twinned code confirms the offsets of data, size, login and connection in Black
struct NHttpRap {
    u8 *data;
    u32 size;
    NHttpRapLogin *login;
    int connection;
    u8 body[NHTTP_RAP_BODY_SIZE];
    char url[NHTTP_RAP_URL_SIZE];
    u32 unk_5A0;
    s32 profileId;
};

// Black 2's, not checked against Black
typedef struct NHttpRapRequest {
    const char *url;
    u32 method;
} NHttpRapRequest;

static void *func_ov114_021b95a0(u32 size, int align);
static void func_ov114_021b95b0(void *ptr);
static int func_ov114_021b95c0(int request, int event, void *arg);
void func_ov114_021b984c(NHttpRap *rap, int type);

// The requests' URLs, each with a second 0 at its end, as Black's are (each has 1 to 4 zeros past its text)
static const char data_ov114_021cdd8c[] = {
#include "asset/nhttp_rap_data_ov114_021cdd8c.inc"
};

// pokebw2's sRootCertificates
static void *data_ov114_021ce280[1] = { data_ov009_021a2f2c };
static const char data_ov114_021cdcbc[] = {
#include "asset/nhttp_rap_data_ov114_021cdcbc.inc"
};
static const char data_ov114_021cdd24[] = {
#include "asset/nhttp_rap_data_ov114_021cdd24.inc"
};
static const char data_ov114_021cdc58[] = {
#include "asset/nhttp_rap_data_ov114_021cdc58.inc"
};
static const char data_ov114_021cdc14[] = {
#include "asset/nhttp_rap_data_ov114_021cdc14.inc"
};
static const char data_ov114_021cde5c[] = {
#include "asset/nhttp_rap_data_ov114_021cde5c.inc"
};
static const char data_ov114_021cddf4[] = {
#include "asset/nhttp_rap_data_ov114_021cddf4.inc"
};
static const char data_ov114_021cdbe0[] = {
#include "asset/nhttp_rap_data_ov114_021cdbe0.inc"
};
static const char data_ov114_021cdf34[] = {
#include "asset/nhttp_rap_data_ov114_021cdf34.inc"
};
static const char data_ov114_021cdec8[] = {
#include "asset/nhttp_rap_data_ov114_021cdec8.inc"
};
// pokebw2's sRequests: each request's URL and method (1 posts)
static NHttpRapRequest data_ov114_021ce284[NHTTP_RAP_NUM_REQUESTS] = {
    { data_ov114_021cdd8c, 0 },
    { data_ov114_021cdcbc, 0 },
    { data_ov114_021cdd24, 0 },
    { data_ov114_021cdc58, 1 },
    { data_ov114_021cdc14, 1 },
    { data_ov114_021cde5c, 0 },
    { data_ov114_021cddf4, 1 },
    { data_ov114_021cdbe0, 1 },
    { data_ov114_021cdf34, 1 },
    { data_ov114_021cdec8, 1 },
};
// pokebw2's sNHttpRap
static NHttpRap *data_ov114_021cf960;

// pokebw2's NHttpRap_Alloc
static void *func_ov114_021b95a0(u32 size, int align) {
    return func_ov009_02156f5c(0xd, size, align);
}

// pokebw2's NHttpRap_Free
static void func_ov114_021b95b0(void *ptr) {
    func_ov009_02156fdc(0xd, ptr, 0);
}

// pokebw2's NHttpRap_RequestCallback
static int func_ov114_021b95c0(int request, int event, void *arg) {
    NHttpRap *rap = arg;

    switch (event) {
    case 1:
        break;
    case 2:
    case 3:
        rap->login = NULL;
        break;
    }
    return 0;
}

BOOL func_ov114_021b95d4(u32 index, NHttpRap *rap) {
    int connection;

    if (func_ov114_021be648(func_ov114_021b95a0, func_ov114_021b95b0, 12) != 0) {
        GFL_ASSERT(0);
        return FALSE;
    }
    if (rap == NULL) {
        return FALSE;
    }
    MI_CpuFill8(rap->url, 0, 4);
    if (index == NHTTP_RAP_REQUEST_CREATE_DATA) {
        STD_TSNPrintf(rap->url, sizeof(rap->url), data_ov114_021ce284[index].url, rap->login->token);
    } else {
        STD_TSNPrintf(rap->url, sizeof(rap->url), data_ov114_021ce284[index].url, rap->profileId, GAME_VERSION,
                    GAME_LANGUAGE, 1, rap->login->token);
    }
    connection = func_ov114_021bf04c(rap->url, data_ov114_021ce284[index].method, rap->body, sizeof(rap->body),
                                     func_ov114_021b95c0, rap);
    GFL_ASSERT(connection != 0);
    if (connection == 0) {
        return FALSE;
    }
    rap->connection = connection;
    if (func_ov114_021be9c8(connection, data_ov114_021ce280, 1) < 0) {
        GFL_ASSERT(0);
        return FALSE;
    }
    if (func_ov114_021be6bc(connection, "Accept", "*/*") != 0) {
        GFL_ASSERT(0);
        return FALSE;
    }
    if (func_ov114_021be6bc(connection, "User-Agent", "Nintendo-DS") != 0) {
        GFL_ASSERT(0);
        return FALSE;
    }
    if (func_ov114_021be8e8(connection, "pokemon", "2Phfv9MY") != 0) {
        GFL_ASSERT(0);
        return FALSE;
    }
    return TRUE;
}


// pokebw2's NHttpRap_GetConnection
int func_ov114_021b973c(NHttpRap *rap) {
    if (rap != NULL) {
        return rap->connection;
    }
    return 0;
}

int func_ov114_021b9748(NHttpRap *rap) {
    int error;

    if (rap != NULL) {
        error = func_ov114_021bf308(rap->connection);
        if (error != 0) {
            func_0203c9ac(error, 2, error, 0x3f6);
            return error;
        }
        return 0;
    }
    return 1;
}

void func_ov114_021b9774(NHttpRap *rap) {
    if (rap != NULL && rap->connection != 0) {
        func_ov114_021bf184(rap->connection);
        rap->connection = 0;
        func_ov114_021be698();
    }
}

int func_ov114_021b9790(NHttpRap *rap) {
    int state;
    int error;
    int received;
    int size;
    char *header;

    if (rap == NULL) {
        return -1;
    }
    if (rap->connection == 0) {
        return 0;
    }
    state = func_ov114_021bf488(rap->connection);
    if (state == 15) {
        func_ov114_021bf4b0(rap->connection, &received, &size);
    } else {
        error = func_ov114_021bf488(rap->connection);
        if (error == 0) {
            func_ov114_021be838(rap->connection, &header);
        } else {
            func_0203c9ac(error, 2, error, 0x3f6);
        }
        func_ov114_021b9774(rap);
        return state;
    }
    return state;
}




// pokebw2's NHttpRap_GetAnswer
void *func_ov114_021b97f4(NHttpRap *rap) {
    if (rap == NULL) {
        return NULL;
    }
    return rap->body;
}

NHttpRap *func_ov114_021b9800(HeapID heapId, s32 profileId, void *loginBuffer) {
    NHttpRap *rap = GFL_HeapAllocate(heapId, sizeof(NHttpRap), TRUE, "nhttp_rap.c", 0x1aa);

    data_ov114_021cf960 = rap;
    rap->profileId = profileId;
    rap->login = loginBuffer;
    return rap;
}


// pokebw2's NHttpRap_Destroy
void func_ov114_021b9838(NHttpRap *rap) {
    if (rap != NULL) {
        func_ov114_021b99b8(rap);
        GFL_HeapFree(rap);
    }
}

// pokebw2's NHttpRap_WriteHeader
void func_ov114_021b984c(NHttpRap *rap, int type) {
    u16 command = type;
    char ch;

    if (rap != NULL) {
        rap->size = 0;
        while ((ch = rap->login->token[rap->size]) != '\0') {
            rap->data[rap->size] = ch;
            rap->size++;
        }
        rap->data[rap->size++] = '\0';
        rap->data[rap->size++] = command >> 8;
        rap->data[rap->size++] = command;
    }
}

void func_ov114_021b98a0(NHttpRap *rap, HeapID heapId, u32 size, u32 type) {
    if (rap != NULL) {
        rap->data = allocConfigDSSoftwareFeature(heapId, size + 0x130, "nhttp_rap.c", 0x1f5);
        MI_CpuFill8(rap->data, 0, size + 0x130);
        func_ov114_021b984c(rap, type);
    }
}


// pokebw2's NHttpRap_AddPostData
void func_ov114_021b98d8(NHttpRap *rap, const void *data, u32 size) {
    if (rap != NULL) {
        MI_CpuCopy8(data, rap->data + rap->size, size);
        rap->size += size;
    }
}

// pokebw2's NHttpRap_SendValidate
BOOL func_ov114_021b98f8(NHttpRap *rap) {
    // read folded, but still emitted: the 4 bytes first in the file's .rodata (pokebw2's)
    static const u32 sValidateRequest = NHTTP_RAP_REQUEST_VALIDATE;
    int connection;

    if (rap == NULL) {
        return FALSE;
    }
    if (func_ov114_021be648(func_ov114_021b95a0, func_ov114_021b95b0, 12) != 0) {
        GFL_ASSERT(0);
        return FALSE;
    }
    MI_CpuFill8(rap->url, 0, 4);
    STD_CopyString(rap->url, data_ov114_021ce284[sValidateRequest].url);
    connection = func_ov114_021bf04c(data_ov114_021ce284[sValidateRequest].url,
                                     data_ov114_021ce284[sValidateRequest].method, rap->body,
                                     sizeof(rap->body), func_ov114_021b95c0, NULL);
    if (connection == 0) {
        func_ov114_021be6ac();
        GFL_ASSERT(connection != 0);
        return FALSE;
    }
    rap->connection = connection;
    if (func_ov114_021be9c8(connection, data_ov114_021ce280, 1) < 0) {
        GFL_ASSERT(0);
        return FALSE;
    }
    func_ov114_021be79c(connection, rap->data, rap->size);
    return TRUE;
}

// pokebw2's NHttpRap_FreePostData
void func_ov114_021b99b8(NHttpRap *rap) {
    if (rap != NULL) {
        if (rap->data != NULL) {
            func_0203d7fc(rap->data);
        }
        rap->data = NULL;
        rap->size = 0;
    }
    data_ov114_021cf960 = NULL;
}

// pokebw2's NHttpRap_GetStatus
int func_ov114_021b99dc(NHttpRap *rap) {
    if (rap != NULL) {
        return func_ov114_021be880(rap->connection);
    }
    return 0;
}

u8 func_ov114_021b99f0(const void *answer) {
    return *(u8 *)answer;
}


// pokebw2's NHttpRap_GetCheckResult
u32 func_ov114_021b99f4(const void *body, int index) {
    const u8 *result = (const u8 *)body + 1 + index * 4;

    return (result[0] << 24) | (result[1] << 16) | (result[2] << 8) | result[3];
}

const void *func_ov114_021b9a14(const void *answer, int count) {
    return (const u8 *)answer + 1 + count * 4;
}


