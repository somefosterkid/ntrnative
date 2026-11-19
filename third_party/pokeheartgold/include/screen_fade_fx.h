#ifndef POKEHEARTGOLD_SCREEN_FADE_FX_H
#define POKEHEARTGOLD_SCREEN_FADE_FX_H

#include "global.h"

#include "heap.h"
#include "screen_fade_internal.h"
#include "screen_fade_shape_globals.h"

// The per-effect work records of the screen fade (what ScreenFade.data points to for each FadeType family) and the effect parameter
// tables they are started from. Pointer members, so offsets differ between DS and 64-bit hosts: use the fields, sizeof(T) for allocation.

typedef struct UnkStruct_02010318 {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    enum DSScreen screen;
} UnkStruct_02010318;

typedef struct UnkStruct_020101B0 {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
} UnkStruct_020101B0;

typedef struct UnkStruct_02010EA4 {
    UnkStruct_020101B0 unk_00;
    UnkStruct_020101B0 unk_10;
    UnkStruct_020101B0 unk_20;
    enum DSScreen screen;
    int unk_34;
    int unk_38;
    int unk_3C;
    int unk_40;
    int unk_44;
    HardwareWindowSettings *unk_48;
} UnkStruct_02010EA4;

typedef struct UnkStruct_02010FC0 {
    short unk_00[2][192];
    short unk_300[2][192];
    int unk_600;
} UnkStruct_02010FC0;

typedef struct UnkStruct_02010588 {
    UnkStruct_02010FC0 *unk_00;
    int unk_04;
    enum DSScreen screen;
} UnkStruct_02010588;

typedef struct UnkStruct_0201109C {
    UnkStruct_02010588 unk_00;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    enum HeapID heapID;
    int unk_2C;
    HardwareWindowSettings *unk_30;
    ScreenFadeHBlanks *unk_34;
} UnkStruct_0201109C;

typedef struct UnkStruct_0200FB7C {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_0200FB7C;

typedef struct UnkStruct_02011360 {
    UnkStruct_02010588 unk_00;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    HardwareWindowSettings *unk_24;
    ScreenFadeHBlanks *unk_28;
    enum HeapID heapID;
} UnkStruct_02011360;

typedef struct UnkStruct_0200FC2C {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_0200FC2C;

typedef struct UnkStruct_02011568 {
    UnkStruct_02010588 unk_00;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    HardwareWindowSettings *unk_28;
    ScreenFadeHBlanks *unk_2C;
    enum HeapID heapID;
} UnkStruct_02011568;

typedef struct UnkStruct_0200FD34 {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
} UnkStruct_0200FD34;

typedef struct UnkStruct_0201184C_sub1 {
    UnkStruct_020101B0 unk_00;
    UnkStruct_020101B0 unk_10;
    UnkStruct_020101B0 unk_20;
} UnkStruct_0201184C_sub1;

typedef struct UnkStruct_0201184C {
    UnkStruct_02010588 unk_00;
    UnkStruct_0201184C_sub1 *unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    int unk_20;
    HardwareWindowSettings *unk_24;
    ScreenFadeHBlanks *unk_28;
    enum HeapID heapID;
} UnkStruct_0201184C;

typedef struct UnkStruct_02011738 {
    const UnkStruct_0200FD8C *unk_00;
    const UnkStruct_0200FD8C *unk_04;
    u16 unk_08;
    u16 unk_0A;
    u8 unk_0C;
    u8 unk_0D;
    u16 unk_0E;
} UnkStruct_02011738;

typedef struct UnkStruct_02011C7C {
    int unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_02011C7C;

typedef struct UnkStruct_02011AFC {
    UnkStruct_02010588 unk_00;
    UnkStruct_02011C7C unk_0C;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
    enum HeapID heapID;
    HardwareWindowSettings *unk_30;
    ScreenFadeHBlanks *unk_34;
} UnkStruct_02011AFC;

typedef struct UnkStruct_0200FEA4 {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04;
    u8 unk_05;
    u16 unk_06;
} UnkStruct_0200FEA4;

typedef struct UnkStruct_02011F2C {
    int unk_00;
    int unk_04;
    int unk_08;
} UnkStruct_02011F2C;

typedef struct UnkStruct_02011E04 {
    UnkStruct_02010588 unk_00;
    UnkStruct_02011F2C unk_0C;
    int unk_18;
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
    enum HeapID heapID;
    HardwareWindowSettings *unk_30;
    ScreenFadeHBlanks *unk_34;
} UnkStruct_02011E04;

typedef struct UnkStruct_0200FF30 {
    u16 unk_00;
    u16 unk_02;
    u8 unk_04;
    u8 unk_05;
    u16 unk_06;
} UnkStruct_0200FF30;

typedef struct UnkStruct_02012174 {
    u8 unk_00[192];
    u8 unk_C0[192];
    int unk_180;
} UnkStruct_02012174;

typedef struct UnkStruct_0201076C {
    UnkStruct_02012174 unk_00[2];
    u8 unk_308;
    u8 screen;
} UnkStruct_0201076C;

typedef struct UnkStruct_020120D4 {
    UnkStruct_0201076C unk_00;
    const UnkStruct_0200F898 *unk_30C;
    int unk_310;
    int unk_314;
    int unk_318;
    int unk_31C;
    int unk_320;
    int unk_324;
    enum HeapID heapID;
    HardwareWindowSettings *unk_32C;
    ScreenFadeHBlanks *unk_330;
} UnkStruct_020120D4;

typedef struct UnkStruct_02012290 {
    UnkStruct_02010EA4 unk_00;
    UnkStruct_020120D4 unk_4C;
    UnkStruct_0201006C *unk_380;
    u8 unk_384;
    u8 unk_385;
    u8 unk_386;
    u8 unk_387;
} UnkStruct_02012290;

typedef struct UnkStruct_02010D94 {
    UnkStruct_02010EA4 unk_00;
    UnkStruct_02010EA4 unk_4C;
} UnkStruct_02010D94;

#endif // POKEHEARTGOLD_SCREEN_FADE_FX_H
