#include "global.h"

#include "mail_message.h"
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

void sub_0202BD7C(DressUpPhoto *photo, u16 word);
void sub_0202BDA8(DressUpPhoto *dest, const DressUpPhoto *src);

void sub_0202BD7C(DressUpPhoto *photo, u16 word) {
    ((DressUpPhoto_IsValid(photo)) ? (void)0 : GF_AssertFail());

    MailMsg_Init(&photo->title);
    MailMsg_SetFieldI(&photo->title, 0, word);
}

void sub_0202BDA8(DressUpPhoto *dest, const DressUpPhoto *src) {
    ((DressUpPhoto_IsValid(dest)) ? (void)0 : GF_AssertFail());
    memcpy(dest, src, sizeof(DressUpPhoto));
}
