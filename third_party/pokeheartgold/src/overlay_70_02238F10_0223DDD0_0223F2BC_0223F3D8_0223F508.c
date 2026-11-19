#include "global.h"
#include "constants/heap.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "bg_window.h"
#include "overlay_70_gtsapplication_state.h"
#include "string_util.h"
#include "message_format.h"

struct BoxPokemon;

struct GTSPokemonCriteria;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct String;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct Window;

enum CharsetMode {

    CHARSET_MODE_JP,

    CHARSET_MODE_EN,
};

void String_Delete(String *string);

u32 GetBoxMonData(BoxPokemon *boxMon, int param, void *dest);

u32 CalcBoxMonLevel(BoxPokemon *boxMon);

void FillWindowPixelBuffer(Window *window, u8 val);

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

void ReadMsgDataIntoString(const struct MsgData *loader, u32 entryID, String *string);

String *NewString_ReadMsgData(const struct MsgData *loader, u32 entryID);

enum Gender {
    GENDER_MALE = 0,
    GENDER_FEMALE = 1,
    GENDER_NONE = 2,
};

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

typedef u32 TextColor;

extern const int ov70_02245910[];

void Window_DrawAlignedSystemText(Window *window, String *string, int x, int y, int centered, TextColor textColor);

String *ReadMsgData_ExpandPlaceholders(MessageFormat *template, struct MsgData *loader, u32 entryID, u32 heapID);

extern TextColor ov70_022465EC[2];

u16 *GTS_LoadAlphabeticalPokedex(enum HeapID heapID, int unused, int *pokedexLength);

u8 *GTS_LoadNationalDexLookup(enum HeapID heapID);

extern void ov70_0223F508(struct MsgData *gtsMessageLoader, MessageFormat *template, Window windows[], BoxPokemon *boxMon, GTSPokemonCriteria *criteria);

void ov70_02245084(Window *window, String *string, int x, int y, int centered, TextColor textColor);

void ov70_0223F508(struct MsgData *gtsMessageLoader, MessageFormat *template, Window windows[], BoxPokemon *boxMon, GTSPokemonCriteria *criteria) {
    String *offerString, *levelString;
    String *nicknameString = String_New(10 + 1, HEAP_ID_61);
    String *genderString = String_New(10 + 1, HEAP_ID_61);

    GetBoxMonData(boxMon, MON_DATA_NICKNAME_STRING, nicknameString);

    int gender = GetBoxMonData(boxMon, MON_DATA_GENDER, 0L) + 1;
    int level = CalcBoxMonLevel(boxMon);
    offerString = NewString_ReadMsgData(gtsMessageLoader, 104);

    BufferIntegerAsString(template, 3, level, 3, (enum PrintingMode)(0), (enum CharsetMode)(1));
    levelString = ReadMsgData_ExpandPlaceholders(template, gtsMessageLoader, 108, HEAP_ID_61);

    if (gender != GENDER_NONE + 1) {
        ReadMsgDataIntoString(gtsMessageLoader, ov70_02245910[gender], genderString);
    }

    for (int i = 0; i < 3; i++) {
        FillWindowPixelBuffer(&windows[i], 0x0);
    }

    ov70_02245084(&windows[0], offerString, 0, 0, 0, ((TextColor)(((15 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
    ov70_02245084(&windows[1], nicknameString, 0, 0, 0, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))));
    ov70_02245084(&windows[2], levelString, 0, 0, 2, ((TextColor)(((1 & 0xFF) << 16) | ((2 & 0xFF) << 8) | ((0 & 0xFF) << 0))));

    if (gender != GENDER_NONE + 1) {
        ov70_02245084(&windows[1], genderString, 70, 0, 0, ov70_022465EC[gender - 1]);
    }

    criteria->species = GetBoxMonData(boxMon, MON_DATA_SPECIES, 0L);
    criteria->gender = gender;
    criteria->level = level;

    String_Delete(levelString);
    String_Delete(genderString);
    String_Delete(nicknameString);
    String_Delete(offerString);
}
