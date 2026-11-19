#include "global.h"
#include "sprite_resources_header.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_0200A090.h"
#include "overlay_90_unk_struct_ov114_0225_e1_a4.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "player_data.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
#include "overlay_90_unk_struct_ov114_0225cdb4.h"
#include "overlay_90_unk_struct_ov114_0225ccd0.h"
#include "overlay_90_unk_struct_ov114_0225cfcc.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225e0f8.h"

struct BufferManager;
struct MsgData;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct PlayerProfile;
struct UnkStruct_ov114_0225D678_t;
struct UnkStruct_ov66_0222DFF8_t;
typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;
enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};
struct UnkStruct_ov114_0225D678_t {
    u8 unk_00;
    u8 unk_01;
    u16 heapID;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    UnkStruct_ov114_0225C76C unk_08;
    UnkStruct_ov114_0225E0F8 unk_18;
    UnkStruct_ov114_0225CEF0 unk_30;
    UnkStruct_ov114_0225D084 unk_3C;
    UnkStruct_ov114_0225D338 unk_4C;
    UnkStruct_ov114_0225CFCC unk_84;
    UnkStruct_ov114_0225CCD0 unk_214;
    UnkStruct_ov114_0225CDB4 *unk_22C;
    UnkStruct_ov114_0225E1A4 unk_230;
    void *unk_260;
    NNSG2dScreenData *unk_264;
    UnkStruct_ov114_0225DAC0 unk_268[4];
    UnkStruct_ov114_0225DFFC unk_348;
    SysTask *unk_650;
    SysTask *unk_654;
};
UnkStruct_ov114_0225D678 *ov90_02259588(const UnkStruct_ov114_0225C76C *param0, s16 param1, u32 heapID);
void ov90_02258E10(UnkStruct_ov114_0225CCD0 *param0);
void ov90_02259084(UnkStruct_ov114_0225CEF0 *param0);
void ov90_02259158(UnkStruct_ov114_0225CFCC *param0);
void ov90_022591D4(UnkStruct_ov114_0225D084 *param0);
void ov90_02259434(UnkStruct_ov114_0225D338 *param0);
void ov90_02259EAC(UnkStruct_ov114_0225DAC0 *param0);
void ov90_0225A108(UnkStruct_ov114_0225DFFC *param0);
void ov90_0225A340(UnkStruct_ov114_0225E1A4 *param0);
void ov90_02259784(UnkStruct_ov114_0225D678 *param0);

UnkStruct_ov114_0225D678 *ov90_0225892C(const UnkStruct_ov114_0225C76C *param0, u32 heapID);
void ov90_02258938(UnkStruct_ov114_0225D678 *param0);

UnkStruct_ov114_0225D678 *ov90_0225892C(const UnkStruct_ov114_0225C76C *param0, u32 heapID) {
    return ov90_02259588(param0, UnkEnum_ov66_022324D0_02, heapID);
}

void ov90_02258938(UnkStruct_ov114_0225D678 *param0) {
    SysTask_Destroy(param0->unk_650);
    SysTask_Destroy(param0->unk_654);

    ov90_0225A108(&param0->unk_348);
    ov90_0225A340(&param0->unk_230);

    {
        int v0;

        for (v0 = 0; v0 < param0->unk_08.unk_08; v0++) {
            ov90_02259EAC(&param0->unk_268[v0]);
        }
    }

    ov90_02259434(&param0->unk_4C);
    ov90_02259784(param0);
    ov90_022591D4(&param0->unk_3C);
    ov90_02258E10(&param0->unk_214);
    ov90_02259158(&param0->unk_84);
    ov90_02259084(&param0->unk_30);

    Heap_Free(param0);
}
