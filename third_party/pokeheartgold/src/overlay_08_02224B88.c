#include "global.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "unk_02020A0C.h"
#include "yes_no_prompt.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "player_data.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_90_unk_struct_ov114_0225c76c.h"
#include "overlay_90_unk_struct_ov114_0225c9a8.h"
#include "sprite.h"
#include "overlay_07_bg_scroll_context.h"
#include "msgdata.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_90_unk_struct_ov114_0225e0f8.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
#include "overlay_90_unk_struct_ov114_0225f270.h"
#include "overlay_90_unk_struct_ov114_0225f578.h"
#include "overlay_90_unk_struct_ov114_0225_e854_t.h"
#include "overlay_08_battle_sub_menu_cursor.h"

u8 ov08_02224B88(BattleSubMenuCursor *cursor);

struct BufferManager;
struct BattleCursor;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct PlayerProfile;
struct UnkStruct_ov114_0225E854_t;
struct UnkStruct_ov66_0222DFF8_t;
struct YesNoPrompt;
typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;
typedef struct WiFiPlazaQuestion {
    u32 questionNumber;
    u32 selectedAnswer;
} WiFiPlazaQuestion;
typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;
void ov12_0226BAFC(struct BattleCursor *cursorRenderer, int x1, int x2, int y1, int y2);
BOOL ov08_02224B8C(BattleSubMenuCursor *cursor);
void ov08_02224B90(BattleSubMenuCursor *cursor, BOOL isVisible);
void ov08_02224B94(BattleSubMenuCursor *cursor, struct BattleCursor *sprites);
void ov08_02224B98(BattleSubMenuCursor *cursor, u8 positionIndex);

u8 ov08_02224B88(BattleSubMenuCursor *cursor) {
    return cursor->currentPositionIndex;
}

BOOL ov08_02224B8C(BattleSubMenuCursor *cursor) {
    return cursor->isVisible;
}

void ov08_02224B90(BattleSubMenuCursor *cursor, BOOL isVisible) {
    cursor->isVisible = isVisible;
}

void ov08_02224B94(BattleSubMenuCursor *cursor, struct BattleCursor *sprites) {
    cursor->sprites = sprites;
}

void ov08_02224B98(BattleSubMenuCursor *cursor, u8 positionIndex) {
    cursor->currentPositionIndex = positionIndex;

    if (cursor->isVisible == 1) {
        ov12_0226BAFC(cursor->sprites, cursor->positions[cursor->currentPositionIndex].left, cursor->positions[cursor->currentPositionIndex].width, cursor->positions[cursor->currentPositionIndex].top, cursor->positions[cursor->currentPositionIndex].height);
    }
}
