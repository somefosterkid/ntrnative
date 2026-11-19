#include "global.h"

#include "overlay_80_0222FD08.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "yes_no_prompt.h"
#include "overlay_80_battle_factory.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "sprite.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
#include "overlay_90_unk_struct_ov114_0225_e854_t.h"
#include "overlay_90_unk_struct_ov114_0225e0f8.h"
#include "overlay_90_unk_struct_ov114_0225cef0.h"
#include "overlay_90_unk_struct_ov114_02260284.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225cbf0.h"
#include "overlay_90_unk_struct_ov114_0225f270.h"
#include "overlay_90_unk_struct_ov114_0225f578.h"


struct BufferManager;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct PlayerProfile;
struct UnkStruct_ov114_0225E854_t;
struct UnkStruct_ov66_0222DFF8_t;
struct YesNoPrompt;
typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;
BOOL ov80_02230790(const UnkStruct_ov114_0225E854 *param0);

u8 ov80_02230784(BattleFactory *factory) {
    factory->currentBattle = (u8)(factory->currentBattle + 1);
    return factory->currentBattle;
}

BOOL ov80_02230790(const UnkStruct_ov114_0225E854 *param0) {
    return param0->unk_06;
}
