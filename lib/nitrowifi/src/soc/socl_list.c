// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitrowifi/src/soc/socl_list.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "nitrowifi/soc.h"
#include "socl_internal.h"
#include "gfl/std.h"
#include "nitro/os.h"

// SOCL's lists of sockets: the live ones, and the closed ones that func_ov009_0215ebb4 frees. The singly linked lists
// chain through SOCLSocket.next. pokediamond's NitroWiFi names the object socl_list.o.

static void func_ov009_0215f2f4(SOCLSocket **list, SOCLSocket *socket);
static void func_ov009_0215f334(SOCLSocket **list, SOCLSocket *socket);
static SOCLSocket **func_ov009_0215f36c(SOCLSocket **list, SOCLSocket *socket);

// pokebw2's SOCLiSocketListTrash
SOCLSocket *data_ov009_021aa4b8;
// pokebw2's SOCLiSocketList
SOCLSocket *data_ov009_021aa4b4;

// pokebw2's SOCLi_SocketRegister
void func_ov009_0215f2dc(SOCLSocket *socket) {
    func_ov009_0215f2f4(&data_ov009_021aa4b4, socket);
}

// pokebw2's SOCLi_SocketRegisterList
static void func_ov009_0215f2f4(SOCLSocket **list, SOCLSocket *socket) {
    socket->next = *list;
    *list = socket;
}

// pokebw2's SOCLi_SocketRegisterTrash
void func_ov009_0215f304(SOCLSocket *socket) {
    func_ov009_0215f2f4(&data_ov009_021aa4b8, socket);
}

// pokebw2's SOCLi_SocketUnregister
void func_ov009_0215f31c(SOCLSocket *socket) {
    func_ov009_0215f334(&data_ov009_021aa4b4, socket);
}

// pokebw2's SOCLi_SocketUnregisterList
static void func_ov009_0215f334(SOCLSocket **list, SOCLSocket *socket) {
    u32 irq = OS_DisableInterrupts();
    SOCLSocket **link = func_ov009_0215f36c(list, socket);

    if (link != NULL) {
        *link = socket->next;
    }
    OS_RestoreInterrupts(irq);
}

// The pointer that points to the socket (the list's head or a socket's next), NULL when it is not in the list
// pokebw2's SOCLi_SocketGetNextPtr
static SOCLSocket **func_ov009_0215f36c(SOCLSocket **list, SOCLSocket *socket) {
    SOCLSocket **link = list;
    SOCLSocket *s;
    u32 irq = OS_DisableInterrupts();

    for (s = *link; s != NULL; s = *link) {
        if (s == socket) {
            OS_RestoreInterrupts(irq);
            return link;
        }
        link = &s->next;
    }
    OS_RestoreInterrupts(irq);
    return NULL;
}

// pokebw2's SOCLi_SocketUnregisterTrash
void func_ov009_0215f3b8(SOCLSocket *socket) {
    func_ov009_0215f334(&data_ov009_021aa4b8, socket);
}

// pokebw2's SOCL_SocketIsInvalid
BOOL func_ov009_0215f3d0(const SOCLSocket *socket) {
    if ((int)socket <= 0 || func_ov009_0215f36c(&data_ov009_021aa4b4, (SOCLSocket *)socket) == NULL) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's SOCL_SocketIsInTrash
BOOL func_ov009_0215f404(const SOCLSocket *socket) {
    if (func_ov009_0215f36c(&data_ov009_021aa4b8, (SOCLSocket *)socket) != NULL) {
        return TRUE;
    }
    return FALSE;
}
