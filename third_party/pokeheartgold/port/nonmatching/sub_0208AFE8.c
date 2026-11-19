#include "global.h"
#include "bg_window.h"
#include "unk_0201956C.h"
#include "unk_02088288.h"
struct BgConfig;
struct UnkStruct_0201956C;
struct UnkStruct_unk_02088288;
struct UnkStruct_unk_02088288_7C8;
typedef struct UnkStruct_unk_02088288_7C8 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4_0 : 4;
    u8 unk4_4 : 4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
} UnkStruct_unk_02088288_7C8;
typedef struct UnkStruct_unk_02088288 {
    BgConfig *bgConfig;
    u8 unk4[0x7C4];
    UnkStruct_unk_02088288_7C8 unk7C8;
    UnkStruct_0201956C *unk7D0;
} UnkStruct_unk_02088288;

void sub_0208AFE8(UnkStruct_unk_02088288 *a0, u8 a1, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, u8 a8);
void sub_0208AFE8(UnkStruct_unk_02088288 *a0, u8 a1, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, u8 a8) {
    a0->unk7C8.unk0 = a2;
    a0->unk7C8.unk4_0 = a6;
    a0->unk7C8.unk1 = a3;
    a0->unk7C8.unk2 = a4;
    a0->unk7C8.unk3 = a5;
    a0->unk7C8.unk4_4 = a7;
    a0->unk7C8.unk5 = a1;
    a0->unk7C8.unk6 = 0;
    a0->unk7C8.unk7 = a8;
}
