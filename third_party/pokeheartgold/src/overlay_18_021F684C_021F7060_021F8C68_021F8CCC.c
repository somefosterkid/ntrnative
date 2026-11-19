#include "global.h"

#include "constants/gx.h"
#include "constants/heap.h"
#include "constants/pokemon.h"

#include "application/pokedex/pokedex_internal.h"

#include "bg_window.h"
#include "dex_mon_measures.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_3d_loader.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "palette.h"
#include "pm_string.h"
#include "pokedex_util.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "seal_case.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "string_util.h"
#include "sys_task.h"
#include "text.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "wild_encounter.h"
#include "overlay_18_pokedex_entry_popup.h"

struct Background;

struct BgConfig;

struct BgTemplate;

struct BoxPokemon;

struct CAPSULE;

struct G2dRenderer;

struct GF_2DGfxRawResMan;

struct GF_2DGfxRawResObj;

struct GF_2DGfxResMan;

struct Mail;

struct MailMessage;

struct MessageFormat;

struct MessageFormatAttrs;

struct MessageFormatFields;

struct MsgData;

struct NARC;

struct ObjCharTransferTemplate;

struct PaletteBuffer;

struct PaletteData;

struct PartyPokemon;

struct PokedexData;

struct PokedexEntryPopup;

struct PokedexEntryPopupObj;

struct PokedexEntryPopupRodata;

struct Pokepic;

struct PokepicAnimScript;

struct PokepicDrawParam;

struct PokepicManager;

struct PokepicShadow;

struct PokepicTemplate;

struct SEAL;

struct SelectedPaletteData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct String;

struct SysTask;

struct SysTaskQueue;

struct Window;

struct WindowTemplate;

struct _MAT;

struct _MAT_ENTRY;

typedef struct CAPSULE {
    SEAL seals[8];
} CAPSULE;

typedef struct PokedexEntryPopupRodata {
    ObjCharTransferTemplate objCharTransferTemplate;
    BgTemplate bgTemplate2;
    BgTemplate bgTemplate1;
    BgTemplate bgTemplate3;
    WindowTemplate windowTemplates[9];
    u8 typePalIndex[20];
    u32 typeIconCharId[18];
} PokedexEntryPopupRodata;

#if defined(SDK_PORT) || defined(NONMATCHING)
PokedexEntryPopupRodata ov18_021FBD50 __attribute__((section(BWPORT_DATA_SECTION))) = {
    .objCharTransferTemplate = {
        .maxTasks = 0,
        .sizeMain = 0,
        .sizeSub = 0,
        .heapID = HEAP_ID_DEFAULT,
    },
    .bgTemplate2 = {
        .x = 0,
        .y = 0,
        .bufferSize = 0,
        .baseTile = 0,
        .size = 0,
        .colorMode = 0,
        .screenBase = 0,
        .charBase = 0,
        .bgExtPltt = 0,
        .priority = 0,
        .areaOver = 0,
        .dummy = 0,
        .mosaic = 0,
    },
    .bgTemplate1 = {
        .x = 0,
        .y = 0,
        .bufferSize = 0,
        .baseTile = 0,
        .size = 0,
        .colorMode = 0,
        .screenBase = 0,
        .charBase = 0,
        .bgExtPltt = 0,
        .priority = 0,
        .areaOver = 0,
        .dummy = 0,
        .mosaic = 0,
    },
    .bgTemplate3 = {
        .x = 0,
        .y = 0,
        .bufferSize = 0,
        .baseTile = 0,
        .size = 0,
        .colorMode = 0,
        .screenBase = 0,
        .charBase = 0,
        .bgExtPltt = 0,
        .priority = 0,
        .areaOver = 0,
        .dummy = 0,
        .mosaic = 0,
    },
    .windowTemplates = {
        {
            .bgId = 0,
            .left = 0,
            .top = 0,
            .width = 0,
            .height = 0,
            .palette = 0,
            .baseTile = 0,
        },
        {
            .bgId = 0,
            .left = 0,
            .top = 0,
            .width = 0,
            .height = 0,
            .palette = 0,
            .baseTile = 0,
        },
        {
            .bgId = 0,
            .left = 0,
            .top = 0,
            .width = 0,
            .height = 0,
            .palette = 0,
            .baseTile = 0,
        },
        {
            .bgId = 0,
            .left = 0,
            .top = 0,
            .width = 0,
            .height = 0,
            .palette = 0,
            .baseTile = 0,
        },
        {
            .bgId = 0,
            .left = 0,
            .top = 0,
            .width = 0,
            .height = 0,
            .palette = 0,
            .baseTile = 0,
        },
        {
            .bgId = 0,
            .left = 0,
            .top = 0,
            .width = 0,
            .height = 0,
            .palette = 0,
            .baseTile = 0,
        },
        {
            .bgId = 0,
            .left = 0,
            .top = 0,
            .width = 0,
            .height = 0,
            .palette = 0,
            .baseTile = 0,
        },
        {
            .bgId = 0,
            .left = 0,
            .top = 0,
            .width = 0,
            .height = 0,
            .palette = 0,
            .baseTile = 0,
        },
        {
            .bgId = 0,
            .left = 0,
            .top = 0,
            .width = 0,
            .height = 0,
            .palette = 0,
            .baseTile = 0,
        },
    },
    .typePalIndex = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    .typeIconCharId = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0,
    },
};
#endif

extern const PokedexEntryPopupRodata sRodata;



void *ov18_021F9694(u32 species, enum HeapID heapId);

void ov18_021F9648(void *arg0, MsgData *msgData, s32 strno, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

void ov18_021F95FC(Window *window, String *string, u32 arg2, u32 y, enum Font fontId, u32 color, s32 arg6);


extern const WindowTemplate ov18_021FBDB4[9];

void ov18_021F8CCC(PokedexEntryPopup *popup) {
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *strbuf;
    String *string;
    u32 i;
    int x;
    for (i = 0; i < (sizeof(popup->windows) / sizeof(*(popup->windows))); i++) {
        AddWindow(popup->bgConfig, &popup->windows[i], &ov18_021FBDB4[i]);
        FillWindowPixelBuffer(&popup->windows[i], 0);
    }

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 802, popup->heapId);
    msgFormat = MessageFormat_New(popup->heapId);
    strbuf = String_New(0x400, popup->heapId);

    ov18_021F9648((u8 *)(&popup->windows[0]), msgData, (s32)(0x90), (s32)(0x70), (s32)(0), (s32)(4), (s32)((((2) & 0xFF) << 16) | (((1) & 0xFF) << 8) | (((0) & 0xFF) << 0)), (s32)(2));

    BufferIntegerAsString(msgFormat, 0, Pokedex_ConvertToCurrentDexNo(popup->natDexEnabled, popup->species), 3, PRINTING_MODE_LEADING_ZEROS, 1);
    string = NewString_ReadMsgData(msgData, 9);
    StringExpandPlaceholders(msgFormat, strbuf, string);
    ov18_021F95FC(&popup->windows[1], strbuf, 1, 0, (enum Font)(4), ((((2) & 0xFF) << 16) | (((1) & 0xFF) << 8) | (((0) & 0xFF) << 0)), 0);
    String_Delete(string);

    string = ov18_021E590C(popup->species, 2, popup->heapId);
    ov18_021F95FC(&popup->windows[2], string, 0, 0, (enum Font)(4), ((((2) & 0xFF) << 16) | (((1) & 0xFF) << 8) | (((0) & 0xFF) << 0)), 0);
    String_Delete(string);

    string = ov18_021E595C(popup->species, 2, popup->heapId);
    x = GetWindowWidth(&popup->windows[3]) * 8 - 4;
    ov18_021F95FC(&popup->windows[3], string, x, 0, (enum Font)(4), ((((2) & 0xFF) << 16) | (((1) & 0xFF) << 8) | (((0) & 0xFF) << 0)), 1);
    String_Delete(string);

    string = ov18_021E59A8(popup->species, 2, 0, popup->heapId);
    x = (GetWindowWidth(&popup->windows[4]) * 8 - FontID_String_GetWidthMultiline((enum Font)(0), string, 0)) / 2;
    ov18_021F95FC(&popup->windows[4], string, x, 0, (enum Font)(0), ((((2) & 0xFF) << 16) | (((1) & 0xFF) << 8) | (((0) & 0xFF) << 0)), 0);
    String_Delete(string);

    ov18_021F9648((u8 *)(&popup->windows[5]), msgData, (s32)(0xA), (s32)(0x14), (s32)(0), (s32)(0), (s32)((((2) & 0xFF) << 16) | (((1) & 0xFF) << 8) | (((0) & 0xFF) << 0)), (s32)(2));
    ov18_021F9648((u8 *)(&popup->windows[7]), msgData, (s32)(0xB), (s32)(0x14), (s32)(0), (s32)(0), (s32)((((2) & 0xFF) << 16) | (((1) & 0xFF) << 8) | (((0) & 0xFF) << 0)), (s32)(2));

    MessageFormat_Delete(msgFormat);
    DestroyMsgData(msgData);

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, GetDexHeightMsgBank(), popup->heapId);
    ov18_021F9648((u8 *)(&popup->windows[6]), msgData, (s32)(popup->species), (s32)(4), (s32)(0), (s32)(0), (s32)((((2) & 0xFF) << 16) | (((1) & 0xFF) << 8) | (((0) & 0xFF) << 0)), (s32)(0));
    DestroyMsgData(msgData);

    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, GetDexWeightMsgBank(), popup->heapId);
    ov18_021F9648((u8 *)(&popup->windows[8]), msgData, (s32)(popup->species), (s32)(4), (s32)(0), (s32)(0), (s32)((((2) & 0xFF) << 16) | (((1) & 0xFF) << 8) | (((0) & 0xFF) << 0)), (s32)(0));
    DestroyMsgData(msgData);

    String_Delete(strbuf);

    for (i = 0; i < (sizeof(popup->windows) / sizeof(*(popup->windows))); i++) {
        ScheduleWindowCopyToVram(&popup->windows[i]);
    }
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_src_overlay_18_021F684C_021F7060_021F8C68_021F8CCC_c_0(void) {
    PORT_AssetAdd("IPKE", "src/overlay_18_021F684C_021F7060_021F8C68_021F8CCC.c:ov18_021FBD50", (void *)&ov18_021FBD50, sizeof(ov18_021FBD50), "u8");
    PORT_AssetAdd("IPGE", "src/overlay_18_021F684C_021F7060_021F8C68_021F8CCC.c:ov18_021FBD50", (void *)&ov18_021FBD50, sizeof(ov18_021FBD50), "u8");
}
