// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/net_command.h, the lines in use so far.
#ifndef BWPORT_GFL_NET_COMMAND_H
#define BWPORT_GFL_NET_COMMAND_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// The commands machines send each other. The library's own are below 0x100; a game's table takes a base of 0x100 or
// more, the command's high byte, and its commands are the low byte. Data larger than a packet is sent in chunks

typedef struct {
    // Called when the command arrives, with the data and the table's work
    void (*callback)(int netId, int size, const void *data, void *work, NetHandle *handle);
    // Where data that arrives in chunks goes, or NULL to keep it out of the receive buffer
    void *(*getBuffer)(int netId, void *work, int size);
} NetCommand;

// Adds a table of listNum NetCommands at base (pokebw2's func_02040c20)
void func_0203a574(int base, const void *list, int listNum, void *work);

// The library's commands, below which a machine that has not negotiated may send
#define GFL_NET_CMD_BASE_COUNT 0x11

void func_0203a5e4(int base);

// The tables are NetCommand arrays
void func_0203a494(int base, const void *list, int listNum, void *work, HeapID unused);
void func_0203a53c(void);

// Handlers for data in chunks: a chunk, and the header before the chunks
void func_0203a85c(int netId, int size, const void *data, void *work, NetHandle *handle);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Sends data in chunks, returning whether it could start
int func_0203aa94(NetHandle *handle, u8 netIds, u16 command, u32 size, void *data);
void func_0203a858(void);
void func_0203a958(int netId, int size, const void *data, void *work, NetHandle *handle);

#endif // BWPORT_GFL_NET_COMMAND_H
