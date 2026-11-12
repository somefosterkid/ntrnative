// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_string.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTL_STRING_H
#define BWPORT_BATTLE_BTL_STRING_H

#include "types.h"
#include "struct_decls.h"

void func_ov093_021eee80(StrBuf *strbuf, u16 message, const u32 *args);
void func_ov093_021eeef4(StrBuf *strbuf, u16 message, const u32 *args);
void func_ov093_021ef694(StrBuf *strbuf, u16 message);

// The six values of a level up's stat window, two digits each
void func_ov093_021ef71c(StrBuf *strbuf, int hp, int attack, int defense, int spAttack, int spDefense, int speed);
// The same with three digits each
void func_ov093_021ef7ac(StrBuf *strbuf, int hp, int attack, int defense, int spAttack, int spDefense, int speed);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_ov093_021eecd8(void);
void func_ov093_021eec78(BtlMainModule *mainModule, u8 clientId, const BtlPokeCon *pokeCon, HeapID heapId);
// The message that a Pokémon used a move
void func_ov093_021ef5e8(StrBuf *strbuf, u8 monId, u16 move);
// A standard message with count arguments that follow
void func_ov093_021eee24(StrBuf *strbuf, u16 message, u32 count, ...);

void func_ov093_021ef6a8(StrBuf *strbuf, u16 message, const u32 *args);
void func_ov093_021ef6e8(StrBuf *strbuf, u16 message, const u32 *args);
void func_ov093_021ef664(StrBuf *strbuf, u16 arg1, u32 arg2);

#endif // BWPORT_BATTLE_BTL_STRING_H
