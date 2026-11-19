#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_81_battle_factory_app.h"
#include "struct_defs/resdat_narc_structs.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "list_menu_items.h"
#include "list_menu_2d.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokepic.h"
#include "pokemon_summary_app.h"
#include "sav_chatot.h"
#include "overlay_87_scratch_off_card_app.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct ListMenuCursor ListMenuCursor;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct SpriteAnimFrame SpriteAnimFrame;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};

struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
};

typedef u32 TextColor;

struct ListMenuCursor {
    TextColor color;
    String *string;
};

struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
};

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void ov81_02240628(BattleFactoryApp *, u32, u8, u8, u8);

void ov81_022405F0(BattleFactoryApp *app);

void ov81_022405F0(BattleFactoryApp *app) {
    if ((app->wheelPaletteCounter & 3) == 0) {
        if ((3 + 1) <= (app->wheelPaletteCounter >> 2)) {
            app->wheelPaletteCounter = 0;
        }
        ov81_02240628(app, 7, ((app->wheelPaletteCounter >> 2) + 2) & 0xff, 0x20, 0x20);
    }
    app->wheelPaletteCounter += 1;
    return;
}
