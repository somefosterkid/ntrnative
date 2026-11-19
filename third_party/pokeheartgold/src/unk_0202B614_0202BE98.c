#include "global.h"

#include "pokemon_types_def.h"
#include "pm_string.h"

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
u16 MailMsg_GetFieldI(const MailMessage *sentence, int slot);
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
typedef u16 charcode_t;
void CopyU16ArrayToString(String *dst, const charcode_t *src);

u16 sub_0202BE98(const DressUpPhoto *photo);
BOOL sub_0202BEA4(const DressUpPhoto *photo);
void sub_0202BECC(const PhotoPokemon *photoMon, String *name);

u16 sub_0202BE98(const DressUpPhoto *photo) {
    return MailMsg_GetFieldI(&photo->title, 0);
}

BOOL sub_0202BEA4(const DressUpPhoto *photo) {
    ((DressUpPhoto_IsValid(photo)) ? (void)0 : GF_AssertFail());

    if (photo->integrity == (0x2345)) {
        return 1;
    }

    return 0;
}

void sub_0202BECC(const PhotoPokemon *photoMon, String *name) {
    CopyU16ArrayToString(name, photoMon->trainerName);
}
