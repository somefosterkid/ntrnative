// DWC's pool of the reliable data that GT2 could not take yet, sent again later: its own file in Black, before
// dwc_transport.c (its .bss is reached from a base of its own, which dwc_transport.c's sTransInfo is not). NitroDWC 2.2
// has no send pool; the code and its names are pokebw2's (GPL-3.0, https://github.com/fuddlesworth/pokebw2),
// lib/dwc/src/transport/dwc_transport.c, which keeps it in dwc_transport.c. The file is named from its functions.
#include "types.h"
#include "dwc/dwc.h"
#include "gs/gt2.h"
#include "nitro/mi.h"

// GT2's send, which the pool sends again through
typedef GT2Result (*DWCTransportSendFunc)(GT2Connection connection, const GT2Byte *message, int length,
                                          GT2Bool reliable);

void DWCs_InitSendPool(void);
void DWCs_ClearSendPool(void);
BOOL DWCs_HasSendPoolSpace(void);
BOOL DWCs_FlushSendPool(void);
BOOL DWCs_FlushSendPoolWith(DWCTransportSendFunc send);
BOOL DWCs_AddToSendPool(GT2Connection connection, const void *data, int size);

// A copy of data that GT2 could not take yet, to be sent again to its connection
typedef struct TransportSendNode {
    GT2Connection connection;
    void *data;
    int size;
    struct TransportSendNode *next;
} TransportSendNode;

// The data that GT2 could not take, and its total size
static int sSendPoolSize;
static TransportSendNode *sSendPoolHead;

void DWCs_InitSendPool(void) {
    sSendPoolHead = NULL;
    sSendPoolSize = 0;
}

void DWCs_ClearSendPool(void) {
    TransportSendNode *node = sSendPoolHead;

    while (node != NULL) {
        sSendPoolHead = node->next;
        DWC_Free(4, node->data, node->size);
        DWC_Free(4, node, 0);
        node = sSendPoolHead;
    }
    sSendPoolHead = NULL;
    sSendPoolSize = 0;
}

BOOL DWCs_HasSendPoolSpace(void) {
    if (sSendPoolSize < 0x6000) {
        return TRUE;
    }
    return FALSE;
}

BOOL DWCs_FlushSendPool(void) {
    return DWCs_FlushSendPoolWith(gt2Send);
}

BOOL DWCs_FlushSendPoolWith(DWCTransportSendFunc send) {
    TransportSendNode *node = sSendPoolHead;
    int tries;

    while (node != NULL) {
        for (tries = 0; tries < 3; tries++) {
            if (!send(node->connection, node->data, node->size, TRUE)) {
                sSendPoolSize -= node->size;
                DWC_Free(4, node->data, node->size);
                node->data = NULL;
                node->size = 0;
                node->connection = NULL;
                break;
            }
        }
        if (tries == 3) {
            break;
        }
        node = node->next;
    }
    node = sSendPoolHead;
    if (node != NULL) {
        do {
            if (node->data != NULL) {
                break;
            }
            sSendPoolHead = node->next;
            DWC_Free(4, node, 0);
            node = sSendPoolHead;
        } while (node != NULL);
    }
    return node == NULL;
}

BOOL DWCs_AddToSendPool(GT2Connection connection, const void *data, int size) {
    TransportSendNode **link = &sSendPoolHead;
    TransportSendNode *node;

    if (sSendPoolSize + size > 0x8000) {
        return FALSE;
    }
    for (node = sSendPoolHead; node != NULL; node = node->next) {
        link = &node->next;
    }
    *link = DWC_Alloc(4, sizeof(TransportSendNode));
    if (*link == NULL) {
        return FALSE;
    }
    (*link)->data = DWC_Alloc(4, size);
    if ((*link)->data == NULL) {
        DWC_Free(4, *link, sizeof(TransportSendNode));
        *link = NULL;
        return FALSE;
    }
    (*link)->connection = connection;
    (*link)->size = size;
    (*link)->next = NULL;
    MIi_CpuCopy32(data, (*link)->data, size);
    sSendPoolSize += size;
    return TRUE;
}
