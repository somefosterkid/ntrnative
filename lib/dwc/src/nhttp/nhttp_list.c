// Black's code at ov114 0x021bacec-0x021baed4. Split out of ghttpEncryption.c by the files of pokebw2's overlay 189
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
void func_ov114_021bacec(NHttpListInfo *info) {
    info->head = NULL;
    info->count = 0;
}

// Puts a request at the end of the list and gives it an id, or returns -1 if there is no memory
int func_ov114_021bacfc(NHttpListInfo *info, NHttpRequest *request) {
    int id = -1;
    NHttpQueueNode *node = func_ov114_021ba820(sizeof(NHttpQueueNode), 4);

    if (node != NULL) {
        if (info->head != NULL) {
            node->prev = info->head->prev;
            node->next = info->head;
            info->head->prev->next = node;
            info->head->prev = node;
        } else {
            node->prev = node;
            node->next = node;
            info->head = node;
        }
        id = info->count++;
        node->id = id;
        node->request = request;
        node->socket = -1;
        if (info->count < 0) {
            info->count = 0;
        }
    }
    return id;
}

// Finds the request that has an id
NHttpQueueNode *func_ov114_021bad94(NHttpListInfo *info, int id) {
    NHttpQueueNode *found = NULL;
    NHttpQueueNode *head = info->head;
    NHttpQueueNode *node;

    if (head != NULL) {
        if (head->id == id) {
            found = head;
        } else {
            node = head->next;
            while (node != head) {
                if (node->id == id) {
                    found = node;
                    break;
                }
                node = node->next;
            }
        }
    }
    return found;
}

// Takes a request out of the list and ends its connection
BOOL func_ov114_021bade0(NHttpListInfo *info, NHttpMutexInfo *mutexInfo, int id) {
    BOOL freed = FALSE;
    NHttpQueueNode *node = func_ov114_021bad94(info, id);

    if (node != NULL) {
        NHttpConnection *connection;

        if (info->head != info->head->prev) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            if (info->head == node) {
                info->head = node->next;
            }
        } else {
            info->head = NULL;
        }
        connection = func_ov114_021bebe0(mutexInfo, node->request);
        func_ov114_021bb934(mutexInfo, node->request);
        func_ov114_021ba850(node);
        if (connection != NULL) {
            connection->status = 8;
            func_ov114_021bee1c(mutexInfo, connection);
            func_ov114_021bf1b0(connection);
        }
        freed = TRUE;
    }
    return freed;
}

void func_ov114_021bae90(NHttpListInfo *info, NHttpMutexInfo *mutexInfo) {
    NHttpQueueNode *node = info->head;

    if (node != NULL) {
        do {
            func_ov114_021bade0(info, mutexInfo, node->id);
            node = info->head;
        } while (node != NULL);
    }
}

// Takes the request that runs next off the list
NHttpQueueNode *func_ov114_021baec8(NHttpListInfo *info) {
    return (NHttpQueueNode *)NHTTPi_getHdrFromList(info);
}
