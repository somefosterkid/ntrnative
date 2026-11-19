#include "global.h"

#include "pokemon_types_def.h"

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
BOOL sub_0202B614(u32 value);
static inline BOOL DressUpPhoto_IsValid(const DressUpPhoto *photo) {
    return sub_0202B614(photo->integrity);
}

const PhotoAccessory *sub_0202BE2C(const DressUpPhoto *photo, int param1);

const PhotoAccessory *sub_0202BE2C(const DressUpPhoto *photo, int param1) {
    ((param1 < (11 - 1)) ? (void)0 : GF_AssertFail());
    ((photo->unk_3C & (1 << param1)) ? (void)0 : GF_AssertFail());
    ((DressUpPhoto_IsValid(photo)) ? (void)0 : GF_AssertFail());

    return &photo->accessories[param1];
}
