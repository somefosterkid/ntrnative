// Black's code at ov114 0x021bbb28-0x021bbe94. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
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
// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they
// belong in the library's header
#define NHTTP_FIRST_FIELD 12


#pragma thumb off
// Frees a response: the blocks of its header, the copies that were made of it, the buffer of the body (by the user's
// function) and the response itself
void func_ov114_021bbb28(NHttpMutexInfo *mutexInfo, NHttpResponse *response) {
    NHttpHeaderBlock *block = response->blocks;
    NHttpConnection *connection;

    if (block != NULL) {
        do {
            NHttpHeaderBlock *next = block->next;

            func_ov114_021ba850(block);
            block = next;
            response->blocks = next;
        } while (block != NULL);
    }
    if (response->headerCopy != NULL) {
        func_ov114_021ba850(response->headerCopy);
    }
    if (response->valueCopy != NULL) {
        func_ov114_021ba850(response->valueCopy);
    }
    if (response->release != NULL) {
        response->release(response->buffer, func_ov114_021ba850, response->userParam);
        response->buffer = NULL;
        response->bufferSize = 0;
    }
    connection = func_ov114_021bebf0(mutexInfo, response);
    if (connection != NULL) {
        connection->response = NULL;
    }
    func_ov114_021ba850(response);
}

int func_ov114_021bbbc8(const NHttpResponse *response, const char *name, int *valueStart) {
    int colon;
    int lineEnd = 0;
    int line;
    int next;

    line = func_ov114_021bafa0(response, NHTTP_FIRST_FIELD, response->headerSize, &colon, &lineEnd);
    while (line > 0) {
        next = func_ov114_021bafa0(response, line, response->headerSize, &colon, &lineEnd);
        if (colon > 0 && func_ov114_021bb12c(response, line, colon, name, 0) == 0) {
            if (colon + 1 < response->headerSize) {
                int valueEnd;
                int start;

                next = func_ov114_021bafa0(response, colon + 1, response->headerSize, NULL, &lineEnd);
                if (next <= 0) {
                    valueEnd = response->headerSize;
                } else {
                    if (next < lineEnd) {
                        return -1;
                    }
                    valueEnd = next - lineEnd;
                }
                start = func_ov114_021bb0bc(response, colon + 1, valueEnd);
                if (start < 0) {
                    start = valueEnd;
                }
                *valueStart = start;
                return valueEnd - start;
            }
            return 0;
        }
        line = next;
    }
    return -1;
}

static char data_ov114_021ce414[0x10] = "HTTPSTATUSCODE";
// Copies the value of a header field (a text that stays until the next call), and gives its length. The name
// HTTPSTATUSCODE gives the status code of the answer
int func_ov114_021bbcf0(NHttpResponse *response, const char *name, char **value) {
    int start;
    int length;

    if (!response->headerDone) {
        return -1;
    }
    if (response->valueCopy != NULL) {
        func_ov114_021ba850(response->valueCopy);
        response->valueCopy = NULL;
    }
    length = func_ov114_021bbbc8(response, name, &start);
    if (length >= 0) {
        response->valueCopy = func_ov114_021ba820(length + 1, 4);
        if (response->valueCopy == NULL) {
            return -1;
        }
        response->valueCopy[length] = 0;
        func_ov114_021bb210(response, response->valueCopy, start, length);
        *value = response->valueCopy;
        return length;
    }
    if (func_ov114_021ba130(data_ov114_021ce414, name) == 0) {
        response->valueCopy = func_ov114_021ba820(4, 4);
        if (response->valueCopy == NULL) {
            return -1;
        }
        response->valueCopy[3] = 0;
        func_ov114_021bb210(response, response->valueCopy, 9, 3);
        *value = response->valueCopy;
        return 3;
    }
    return -1;
}

// Gives the whole header: the part of the response if it fits there, or a copy
int func_ov114_021bbdfc(NHttpState *state, NHttpResponse *response, char **header) {
    if (!response->headerDone || response->headerSize == 0) {
        return -1;
    }
    if (response->headerSize < NHTTP_HEADER_SIZE) {
        *header = response->header;
    } else {
        if (response->headerCopy == NULL) {
            response->headerCopy = func_ov114_021ba820(response->headerSize, 4);
            if (response->headerCopy == NULL) {
                func_ov114_021ba874(state, 1);
                return -1;
            }
            func_ov114_021bb210(response, response->headerCopy, 0, response->headerSize);
        }
        *header = response->headerCopy;
    }
    return response->headerSize;
}
