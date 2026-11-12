// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/config.c, ported by tools/port_file.py.
#include "types.h"
#include "save/config.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/std.h"

// The options (the ROM's string for the file, "config.c"). The names of the fields are guessed.

Config *func_020082ec(u32 heapId) {
    Config *config = GFL_HeapAllocate(heapId, sizeof(Config), FALSE, "config.c", 77);
    func_02008320(config);
    return config;
}

void initConfig(const Config *config, Config *dest) {
    MI_CpuCopy8(config, dest, sizeof(Config));
}

// The original clears each option after the memset
void func_02008320(Config *config) {
    MI_CpuFill8(config, 0, sizeof(Config));
    config->textSpeed = 1;
    config->soundFlags = 0;
    config->unk6 = 0;
    config->unk7 = 0;
    config->kanji = 0;
    config->unk9 = 1;
    config->cgearOn = 0;
}

u32 func_02008374(const Config *config) {
    return config->textSpeed;
}

void func_0200837c(Config *config, u32 value) {
    config->textSpeed = value;
}

u32 getSoundFlags(const Config *config) {
    return config->soundFlags;
}

void func_02008398(Config *config, u32 value) {
    config->soundFlags = value;
}

BOOL func_020083ac(const Config *config) {
    return config->unk7;
}

void func_020083b4(Config *config, u32 value) {
    config->unk7 = value;
}

BOOL func_020083c8(const Config *config) {
    return config->unk6;
}

void func_020083d0(Config *config, u32 value) {
    config->unk6 = value;
}

u32 func_020083e4(const Config *config) {
    return config->kanji;
}

void func_020083ec(Config *config, u32 value) {
    config->kanji = value;
    func_020461d4(config->kanji);
}

void func_02008414(const Config *config) {
    func_020461d4(config->kanji);
}

u32 func_02008428(const Config *config) {
    return config->unk9;
}

void func_02008430(Config *config, u32 value) {
    config->unk9 = value;
}

u32 func_02008448(const Config *config) {
    return config->cgearOn;
}

void func_02008450(Config *config, u32 value) {
    config->cgearOn = value;
}
