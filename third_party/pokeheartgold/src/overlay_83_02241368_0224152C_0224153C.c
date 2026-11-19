#include "global.h"

#include "constants/heap.h"

#include "frontier/overlay_80_022340E8.h"

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
#include "unk_02035900.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_main_menu_util.h"
#include "pokemon_summary_app.h"
#include "overlay_81_battle_factory_app.h"
#include "msgdata.h"
#include "sav_chatot.h"
#include "pokepic.h"
#include "overlay_87_scratch_off_card_app.h"
#include "struct_defs/resdat_narc_structs.h"

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

void ov83_02240C8C(BattleFactoryApp *app, Window *window, u32 xOffset, u32 yOffset, u8 font);

void ov83_02240CFC(BattleFactoryApp *, Window *, u32, u32, u8);

void ov83_02240F7C(BattleFactoryApp *, u16 *, u16 *, u16 *, u16 *);

void ov83_0224153C(BattleFactoryApp *app, Window *window);

void ov83_0224153C(BattleFactoryApp *app, Window *window) {
    u16 x, y, playerXOffset, playerYOffset, partnerXOffset, partnerYOffset;

    ov83_02240F7C(app, &playerXOffset, &playerYOffset, &partnerXOffset, &partnerYOffset);

    if (!ov80_02237D8C(app->challengeType)) {
        x = playerXOffset + 4;
        y = playerYOffset + 1;
        ov83_02240C8C(app, window, x, y, 0);
    } else {
        if (sub_0203769C() == 0) {
            x = playerXOffset;
            y = playerYOffset + 1;
            ov83_02240C8C(app, window, x, y, 0);

            x = partnerXOffset;
            y = partnerYOffset + 1;
            ov83_02240CFC(app, window, x, y, 0);
        } else {
            x = playerXOffset;
            y = playerYOffset + 1;
            ov83_02240CFC(app, window, x, y, 0);

            x = partnerXOffset;
            y = partnerYOffset + 1;
            ov83_02240C8C(app, window, x, y, 0);
        }
    }

    ScheduleWindowCopyToVram(window);
}
