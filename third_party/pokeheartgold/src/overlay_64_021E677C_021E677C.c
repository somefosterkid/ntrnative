#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "font_types_def.h"
#include "gf_gfx_loader.h"
#include "hall_of_fame.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_64_hall_of_fame_showcase.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "string_util.h"
#include "text.h"
#include "unk_02009D48.h"
#include "unk_02013FDC.h"
#include "wild_encounter.h"

typedef enum GFPalSlotOffset GFPalSlotOffset;

typedef enum GFPalLoadLocation GFPalLoadLocation;

typedef enum Font Font;

struct Background;

struct BgConfig;

struct HOFMon;

struct HOFParty;

struct HallOfFame;

struct HofDisplayMon;

struct ManagedSprite;

struct MessageFormat;

struct MessageFormatAttrs;

struct MessageFormatFields;

struct MsgData;

struct NARC;

struct PokepicTemplate;

struct Sprite;

struct SpriteList;

struct SpriteResourceHeaderList;

struct SpriteResourcesHeader;

struct String;

struct TextPrinterTemplate;

struct UnkStruct_02014E30;

struct Window;

struct _MAT;

struct _MAT_ENTRY;


extern const UnkStruct_02014E30 ov64_021E6E98;

extern const u8 ov64_021E6EA8[];

void ov64_021E6754(HallOfFameShowcase *arg0);

void ov64_021E5AC8(void *arg0, u32 offset, u32 arg2);

void ov64_021E677C(HallOfFameShowcase *work, u32 idx);

void ov64_021E677C(HallOfFameShowcase *work, u32 idx) {
    u32 gender;
    u32 which;
    u32 i;
    void *buf;
    s8 height;
    PokepicTemplate tmpl;
    int base;
    u32 shiny;
    u8 picHeight;
    u32 loc;
    u32 width;
    u32 pid;
    UnkStruct_02014E30 rect;

    rect = ov64_021E6E98;

    if (idx >= work->numMons) {
        ov64_021E6754(work);
        return;
    }
    ManagedSprite_SetDrawFlag(work->sprites[0], 0);
    ManagedSprite_SetDrawFlag(work->sprites[1], 0);

    if (work->monDetailSet == 0) {
        which = 0;
        base = 0;
    } else {
        which = 1;
        base = 8;
    }

    Save_HOF_GetMonStatsByIndexPair(work->hallOfFame, work->record, idx, &work->mon);
    gender = GetGenderBySpeciesAndPersonality(work->mon.species, work->mon.personality);
    sub_020729D8(work->pokegraNarc, &height, work->mon.species);
    picHeight = GetMonPicHeightBySpeciesGenderForm(work->mon.species, gender, 2, work->mon.form, work->mon.personality);
    ManagedSprite_SetPositionXYWithSubscreenOffset(work->sprites[which], 0x40, (picHeight + 0x40) - height, 0x200000);

    if (CalcShininessByOtIdAndPersonality(work->mon.otid, work->mon.personality) != 0) {
        shiny = 1;
    } else {
        shiny = 0;
    }
    buf = Heap_AllocAtEnd(HEAP_ID_59, 0xC80);
    pid = work->mon.personality;
    GetMonSpriteCharAndPlttNarcIdsEx(&tmpl, work->mon.species, GetGenderBySpeciesAndPersonality(work->mon.species, pid), 2, shiny, work->mon.form, pid);
    sub_02014510((NarcId)(tmpl.narcID), tmpl.charDataID, HEAP_ID_59, &rect, buf, work->mon.personality, 0, 2, work->mon.species);
    loc = NNS_G2dGetImageLocation(Sprite_GetImageProxy(*(Sprite **)work->sprites[which]), (NNS_G2D_VRAM_TYPE)(2));
    ov64_021E5AC8(buf, loc, 0xC80);
    Heap_Free(buf);
    loc = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(*(Sprite **)work->sprites[which]), (NNS_G2D_VRAM_TYPE)(2));
    GfGfxLoader_GXLoadPal((NarcId)(tmpl.narcID), tmpl.palDataID, (GFPalLoadLocation)(5), (GFPalSlotOffset)(loc), 0x20, HEAP_ID_59);
    ManagedSprite_SetDrawFlag(work->sprites[which], 1);

    for (i = 0; i < 7; i++) {
        FillWindowPixelBuffer(&work->windows[base + i], 0);
    }
    AddTextPrinterParameterizedWithColor(&work->windows[base], (Font)(0), work->mon.nickname, 0, (Font)(0), 0xFF, 0xF0200, 0L);
    ReadMsgDataIntoString(work->speciesNames, work->mon.species, work->stringBuf);
    AddTextPrinterParameterizedWithColor(&work->windows[base + 1], (Font)(0), work->stringBuf, 0, 0, 0xFF, 0xF0200, 0L);
    if (work->mon.species != 0x1D && work->mon.species != 0x20) {
        if (gender == 0) {
            ReadMsgDataIntoString(work->msgData, 3, work->stringBuf2);
            width = FontID_String_GetWidth((Font)(0), work->stringBuf, 0);
            AddTextPrinterParameterizedWithColor(&work->windows[base + 1], (Font)(0), work->stringBuf2, width + 8, 0, 0xFF, 0xF0200, 0L);
        } else if (gender == 1) {
            ReadMsgDataIntoString(work->msgData, 4, work->stringBuf2);
            width = FontID_String_GetWidth((Font)(0), work->stringBuf, 0);
            AddTextPrinterParameterizedWithColor(&work->windows[base + 1], (Font)(0), work->stringBuf2, width + 8, 0, 0xFF, 0xF0200, 0L);
        }
    }
    ReadMsgDataIntoString(work->msgData, 1, work->stringBuf);
    BufferIntegerAsString(work->msgFormat, 0, work->mon.level, 3, (PrintingMode)(0), 1);
    StringExpandPlaceholders(work->msgFormat, work->stringBuf2, work->stringBuf);
    AddTextPrinterParameterizedWithColor(&work->windows[base + 1], (Font)(0), work->stringBuf2, 0, 0x10, 0xFF, 0xF0200, 0L);
    ReadMsgDataIntoString(work->msgData, 2, work->stringBuf);
    AddTextPrinterParameterizedWithColor(&work->windows[base + 2], (Font)(0), work->stringBuf, 0, 0, 0xFF, 0xF0200, 0L);
    width = FontID_String_GetWidth((Font)(0), work->stringBuf, 0);
    AddTextPrinterParameterizedWithColor(&work->windows[base + 2], (Font)(0), work->mon.otname, width, 0, 0xFF, 0xF0200, 0L);
    for (i = 0; i < 4; i++) {
        if (work->mon.moves[i] != 0) {
            ReadMsgDataIntoString(work->moveNames, work->mon.moves[i], work->stringBuf);
            width = FontID_String_GetWidth((Font)(0), work->stringBuf, 0);
            AddTextPrinterParameterizedWithColor(&work->windows[base + 3 + i], (Font)(0), work->stringBuf, 0x38 - (width >> 1), 0, 0xFF, 0xF0200, 0L);
        }
    }
    for (i = 0; i < 7; i++) {
        CopyWindowPixelsToVram_TextMode(&work->windows[base + i]);
        ScheduleWindowCopyToVram(&work->windows[base + i]);
    }
    work->monDetailSet = work->monDetailSet ^ 1;
}
