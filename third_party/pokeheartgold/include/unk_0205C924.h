#ifndef POKEHEARTGOLD_UNK_0205C924_H
#define POKEHEARTGOLD_UNK_0205C924_H

// Union room chat log: 30 entries, allocated by sub_0205B3B8 (DS size 0x350).

#include "mail_message_types.h"

struct String;

typedef struct UnkStruct_0205C924 {
    struct String *unk_00;
    struct String *unk_04;
    struct String *unk_08;
    u32 unk_0C;
    int unk_10;
    MailMessage unk_14;
} UnkStruct_0205C924;

typedef struct UnkStruct_0205C95C {
    UnkStruct_0205C924 unk_00[30];
    int unk_348;
    int unk_34C;
} UnkStruct_0205C95C;

#endif // POKEHEARTGOLD_UNK_0205C924_H
