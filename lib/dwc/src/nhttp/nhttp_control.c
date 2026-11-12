// Black's code at ov114 0x021baa1c-0x021bacec. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
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
BOOL addHdrList(NHttpHeader **list, NHttpState *state, const char *name, const char *value) {
    BOOL isFound = FALSE;
    NHttpHeader *found;
    NHttpHeader *node;

    node = *list;
    if (node != NULL) {
        found = node;
        if (func_ov114_021ba590(name, node->name) != 0) {
            found = node->next;
            if (found != *list) {
                do {
                    if (func_ov114_021ba590(name, found->name) == 0) {
                        isFound = TRUE;
                        break;
                    }
                    found = found->next;
                } while (found != *list);
            }
        } else {
            isFound = TRUE;
        }
    }
    if (isFound) {
        found->value = (char *)value;
    } else {
        NHttpHeader *created = func_ov114_021ba820(sizeof(NHttpHeader), 4);
        if (created == NULL) {
            func_ov114_021ba874(state, 1);
            return FALSE;
        }
        created->name = (char *)name;
        created->value = (char *)value;
        created->length = 0;
        created->binary = 0;
        if (*list == NULL) {
            created->next = created;
            created->prev = created;
            *list = created;
        } else {
            created->prev = (*list)->prev;
            created->next = *list;
            (*list)->prev->next = created;
            (*list)->prev = created;
        }
    }
    return TRUE;
}

char func_ov114_021bab24(u8 c) {
    u8 next = c + 1;

    if (next == '{') {
        next = '0';
    } else if (next == '[') {
        next = 'a';
    } else if (next == ':') {
        next = 'A';
    }
    return next;
}

const char data_ov114_021cdfc8[0x18] = {
    45, 45, 116, 57, 83, 102, 52, 121, 102, 106, 102, 49, 82, 116, 118, 68, 117, 51, 65, 65, 0, 0, 0, 0
};
#pragma thumb reset
BOOL func_ov114_021bab58(NHttpRequest *request, const char *data, int length) {
    char *name;
    int i;
    int c;
    char original;

    if (NHTTPi_memfind(data, length, request->boundary + 2, NHTTP_BOUNDARY_SIZE - 2) < 0) {
        return TRUE;
    }
    i = NHTTP_BOUNDARY_SIZE - 1;
    name = request->boundary + 2;
    for (; i >= 2; i--) {
        c = request->boundary[i];
        original = data_ov114_021cdfc8[i];
        do {
            c = func_ov114_021bab24(c);
            request->boundary[i] = c;
            if (c == original) {
                break;
            }
            if (NHTTPi_memfind(data, length, name, NHTTP_BOUNDARY_SIZE - 2) < 0) {
                return TRUE;
            }
        } while (TRUE);
    }
    return FALSE;
}

#pragma thumb off
NHttpHeader *NHTTPi_getHdrFromList(void *list) {
    NHttpHeader **headPtr = list;
    NHttpHeader *head;

    head = *headPtr;
    if (head != NULL) {
        if (head == head->prev) {
            *headPtr = NULL;
        } else {
            head->prev->next = head->next;
            head->next->prev = head->prev;
            *headPtr = head->next;
        }
    }
    return head;
}

BOOL func_ov114_021bac38(NHttpRequest *request, NHttpState *state, const char *name, const char *value) {
    if (request->sent != 0) {
        return FALSE;
    }
    return addHdrList(&request->headers, state, name, value);
}

BOOL func_ov114_021bac58(NHttpRequest *request, NHttpState *state, const char *name, const char *value) {
    BOOL added = FALSE;
    int length = 0;

    if (request->sent != 0) {
        return FALSE;
    }
    if (request->rawPost != 0) {
        return FALSE;
    }
    if (value != NULL) {
        length = func_ov114_021ba118(value);
    }
    if (func_ov114_021bab58(request, value, length)) {
        added = addHdrList(&request->postData, state, name, value);
        if (added) {
            request->postData->prev->length = length;
        }
    }
    return added;
}
