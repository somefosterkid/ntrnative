#include "global.h"

#include "heap.h"
#include "unk_02067A60.h"

struct BoxPokemon;

struct CAPSULE;

struct FieldSystem;

struct Mail;

struct MailMessage;

struct PartyPokemon;

struct SEAL;

struct UnkStruct_02067BF8;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

struct UnkStruct_02067BF8 {
    FieldSystem *fieldSystem;
    u16 state;
    u16 unk06;
    u16 unk08;
    s16 unk0a;
    s16 unk0c;
    u16 unk0e;
    Pokemon *pokemon;
    void *unk14;
    void *unk18;
};

struct UnkStruct_02067BF8 *sub_02067BF8(enum HeapID heapID, FieldSystem *fieldSystem, Pokemon *pokemon, u16 r3, u16 a4, s16 a5, s16 a6) {
    struct UnkStruct_02067BF8 *env = Heap_AllocAtEnd(heapID, sizeof(struct UnkStruct_02067BF8));
    u8 *p = (u8 *)env;
    u32 n = sizeof(struct UnkStruct_02067BF8);
    do {
        *p++ = 0;
    } while (--n);
    env->fieldSystem = fieldSystem;
    env->pokemon = pokemon;
    env->unk08 = a4;
    env->unk0a = a5;
    env->unk0c = a6;
    env->unk0e = r3;
    return env;
}
