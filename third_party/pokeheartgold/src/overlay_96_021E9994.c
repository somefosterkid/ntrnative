#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "player_data.h"
#include "pokemon_types_def.h"
#include "sys_task.h"

void ov96_021E9994(int arg0, int arg1, void *arg2, void *arg3);

u32 ov96_021E99F4(void);
u32 ov96_021E99F8(void);
u32 ov96_021E99FC(void);
u32 ov96_021E9A04(void);
typedef struct PhotoAccessory {
    u8 unk_00;
    u8 xPos;
    u8 yPos;
    s8 priority;
} PhotoAccessory;
typedef struct PhotoPokemon {
    u32 personality;
    u32 otID;
    u16 species;
    u16 nickname[10 + 1];
    u16 trainerName[7 + 1];
    s8 priority;
    u8 xPos;
    u8 yPos;
    u8 form;
    u8 trainerGender;
} PhotoPokemon;
typedef struct DressUpPhoto {
    u32 integrity;
    PhotoPokemon photoMon;
    u32 unk_3C;
    MailMessage title;
    PhotoAccessory accessories[10];
    u8 unk_70;
    u8 language;
} DressUpPhoto;
u32 ov96_021E9A10(void);

typedef struct BuriedTrap {
    u16 x;
    u16 z;
    u8 trapID;
    u8 spawnedTrapIndex;
} BuriedTrap;
u32 ov96_021E9A18(void);
u32 ov96_021E9A1C(void);
s32 sub_02031B10(void);
u32 ov96_021E9A24(void);

void ov96_021E9994(int arg0, int arg1, void *arg2, void *data) {
    PokeathlonCourse_GetDataCopyArea(data);
    ov96_021E5F24(data);
    PokeathlonCourse_SetStateTransitionType(data, 0xDU);
    PokeathlonCourse_SetStateField07(data, 0x1FU);
}

void ov96_021E99B8(int arg0, int arg1, void *arg2, void *arg3);

void ov96_021E99B8(int arg0, int arg1, void *arg2, void *data) {
    u8 v0;

    PokeathlonCourse_GetDataCopyArea(data);
    PokeathlonCourse_IncrementField1EF(data);
    v0 = PokeathlonCourse_GetParticipantCount(data);
    if (v0 == PokeathlonCourse_GetField1EF(data)) {
        PokeathlonCourse_ResetField1EF(data);
        PokeathlonCourse_SetStateTransitionType(data, 0xFU);
        PokeathlonCourse_SetStateField07(data, 0x24U);
    }
}

u32 ov96_021E99F4(void) {
    return sizeof(u32);
}

u32 ov96_021E99F8(void) {
    return 0x7C;
}

u32 ov96_021E99FC(void) {
    return 0x1F0;
}

u32 ov96_021E9A04(void) {
    return PlayerProfile_sizeof() * 4;
}

u32 ov96_021E9A10(void) {
    return sizeof(DressUpPhoto);
}

u32 ov96_021E9A14(void) {
    return 0x26;
}

u32 ov96_021E9A18(void) {
    return sizeof(BuriedTrap) * 16;
}

u32 ov96_021E9A1C(void) {
    return 0x180;
}

u32 ov96_021E9A24(void) {
    return sub_02031B10();
}
