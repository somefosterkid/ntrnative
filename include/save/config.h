// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/config.h: the lines in use so far.
#ifndef BWPORT_SAVE_CONFIG_H
#define BWPORT_SAVE_CONFIG_H

#include "types.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of textSpeed, soundFlags, unk6, unk7, kanji, unk9 and cgearOn, and its
// size in Black
// The options, which a save keeps at the start of the block that getTrainerDataBlkAddress returns, so a pointer to one
// is that block's. The field names are guessed, apart from kanji and cgearOn, which the code's use shows
struct Config {
    u16 textSpeed : 4;
    u16 soundFlags : 2;
    u16 unk6 : 1;
    u16 unk7 : 1;
    // The kana or kanji text of the Japanese version, which also sets the message language
    u16 kanji : 1;
    u16 unk9 : 1;
    // Whether the C-Gear is on, which a continue sets from the answer to the start menu's question
    u16 cgearOn : 1;
    u16 : 5;
    u16 unk2;
};

void func_02008320(Config *config);

u32 getSoundFlags(const Config *config);
Config *func_020082ec(u32 heapId);
u32 func_02008374(const Config *config);
void func_0200837c(Config *config, u32 value);
void func_02008398(Config *config, u32 value);
BOOL func_020083ac(const Config *config);
void func_020083b4(Config *config, u32 value);
BOOL func_020083c8(const Config *config);
void func_020083d0(Config *config, u32 value);
u32 func_020083e4(const Config *config);
// Also sets the message language
void func_020083ec(Config *config, u32 value);
void func_02008414(const Config *config);
u32 func_02008428(const Config *config);
void func_02008430(Config *config, u32 value);
u32 func_02008448(const Config *config);
void func_02008450(Config *config, u32 value);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// Copies a config into the save's
void initConfig(const Config *config, Config *dest);

#endif // BWPORT_SAVE_CONFIG_H
