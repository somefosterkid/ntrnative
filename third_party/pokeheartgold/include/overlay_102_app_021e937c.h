#ifndef POKEHEARTGOLD_OVERLAY_102_APP_021E937C_H
#define POKEHEARTGOLD_OVERLAY_102_APP_021E937C_H

// Ov102App_021E937C: the Easy Chat view (Heap_Alloc 0x234 in ov102_021E909C). Its HGSS layout differs from
// the platinum UnkStruct_ov20_021D2128 copies past 0x24; pads where no code reads a field yet.

#include "bg_window.h"

typedef struct Ov102App_021E937CRec_214 {
    struct TextOBJ *unk_0;
    u8 unk_4[0xC];
} Ov102App_021E937CRec_214; // 0x10

typedef struct Ov102App_021E937C {
    u8 unk_0[0x18];
    void *unk_18; // EasyChatApp (ov102_021E909C arg0)
    s32 unk_1C;
    BgConfig *unk_20;
    u8 unk_24[0x1AC];
    void *unk_1D0[2];
    void *unk_1D8[2];
    void *unk_1E0; // ov102_021EA314 (sentence view)
    void *unk_1E4; // ov102_021EB1A4 (Ov102App_021EB1E0)
    void *unk_1E8; // ov102_021EB694
    void *unk_1EC; // ov102_021EBC34 (Ov102App_021EBC34)
    void *unk_1F0; // ov102_021EC20C
    u8 unk_1F4;
    u8 unk_1F5[0xF];
    Window unk_204;
    Ov102App_021E937CRec_214 unk_214[2];
} Ov102App_021E937C;

// Task data of the Easy Chat view commands (Heap_Alloc 0x18 in ov102_021E940C).
struct EasyChatApp;
struct EasyChatWordList;
typedef struct UnkStruct_ov102_021E978C {
    void *unk_0; // the view (Ov102App_021E937C)
    struct EasyChatApp *unk_4;
    struct EasyChatWordList *unk_8;
    u32 unk_C; // command
    int unk_10; // view task slot
    int unk_14; // state
} UnkStruct_ov102_021E978C;

// The DS layout (ILP32): the checks run only under mwcc.
#ifdef __MWERKS__
typedef char Ov102App_021E937C_size_check[(sizeof(Ov102App_021E937C) == 0x234) ? 1 : -1];
typedef char Ov102App_021E937C_1E0_check[(offsetof(Ov102App_021E937C, unk_1E0) == 0x1E0) ? 1 : -1];
typedef char Ov102App_021E937C_1F4_check[(offsetof(Ov102App_021E937C, unk_1F4) == 0x1F4) ? 1 : -1];
typedef char Ov102App_021E937C_204_check[(offsetof(Ov102App_021E937C, unk_204) == 0x204) ? 1 : -1];
typedef char UnkStruct_ov102_021E978C_size_check[(sizeof(UnkStruct_ov102_021E978C) == 0x18) ? 1 : -1];
#endif

#endif // POKEHEARTGOLD_OVERLAY_102_APP_021E937C_H
