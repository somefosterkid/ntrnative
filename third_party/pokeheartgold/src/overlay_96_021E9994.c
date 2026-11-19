#include "global.h"

#include "pokeathlon/pokeathlon.h"
#include "player_data.h"
#include "pokemon_types_def.h"
#include "sys_task.h"

void ov96_021E9994(s32 arg0, s32 arg1, s32 arg2, PokeathlonCourseData *arg3);

int ov96_021E99F4(void);
s32 ov96_021E99F8(void);
s32 ov96_021E99FC(void);
s32 ov96_021E9A04(void);
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
int ov96_021E9A10(void);

typedef struct BuriedTrap {
    u16 x;
    u16 z;
    u8 trapID;
    u8 spawnedTrapIndex;
} BuriedTrap;
int ov96_021E9A18(void);
s32 ov96_021E9A1C(void);
int sub_02031B10(void);
int ov96_021E9A24(void);

void ov96_021E9994(s32 arg0, s32 arg1, s32 arg2, PokeathlonCourseData *data) {
    PokeathlonCourse_GetDataCopyArea(data);
    ov96_021E5F24(data);
    PokeathlonCourse_SetStateTransitionType(data, 0xDU);
    PokeathlonCourse_SetStateField07(data, 0x1FU);
}

void ov96_021E99B8(s32 arg0, s32 arg1, s32 arg2, PokeathlonCourseData *arg3);

void ov96_021E99B8(s32 arg0, s32 arg1, s32 arg2, PokeathlonCourseData *data) {
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

int ov96_021E99F4(void) {
    return sizeof(u32);
}

s32 ov96_021E99F8(void) {
    return 0x7C;
}

s32 ov96_021E99FC(void) {
    return 0x1F0;
}

s32 ov96_021E9A04(void) {
    return PlayerProfile_sizeof() * 4;
}

int ov96_021E9A10(void) {
    return sizeof(DressUpPhoto);
}

void *ov96_021E9A14(void) {
    return (void *)0x26;
}

int ov96_021E9A18(void) {
    return sizeof(BuriedTrap) * 16;
}

s32 ov96_021E9A1C(void) {
    return 0x180;
}

int ov96_021E9A24(void) {
    return sub_02031B10();
}
