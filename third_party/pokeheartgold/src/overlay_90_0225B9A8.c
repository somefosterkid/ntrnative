#include "global.h"

#include "unk_0202C034.h"
#include "unk_02034354.h"
#include "unk_02037C94.h"
#include "yes_no_prompt.h"
#include "error_handling.h"
#include "overlay_90.h"
#include "overlay_90_unk_struct_ov114_0225_e854_t.h"
#include "overlay_90_unk_struct_ov114_0225_d084.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_90_unk_struct_ov114_0225dac0.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_90_unk_struct_ov114_0225d338.h"
#include "overlay_90_unk_struct_ov114_0225dffc.h"
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

struct SpriteResourceCollection;

struct MessageFormat;

struct PlayerProfile;

struct UnkStruct_ov114_0225E854_t;

struct UnkStruct_ov66_0222DFF8_t;

struct YesNoPrompt;

typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;

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

void sub_0202C554(WiFiList *wiFiList, int param1, int param2);

void sub_0202C584(WiFiList *wiFiList, int param1, int param2);

void sub_0202C5B4(WiFiList *wiFiList, int param1, int param2);

void ov90_0225B9A8(UnkStruct_ov114_0225E854 *param0);

void ov90_0225B9A8(UnkStruct_ov114_0225E854 *param0) {
    WiFiList *v0 = sub_0202C6F4(param0->unk_0C.saveData);
    int v1;
    int v2, v3;
    int v4;
    DWCFriendData *v5;

    for (v1 = 0; v1 < param0->unk_0C.unk_08; v1++) {
        v2 = param0->unk_0C.unk_04[v1];
        v5 = sub_0203484C(v2);
        v3 = sub_0203A084(param0->unk_0C.saveData, v5, &v4);

        switch (v3) {
        case 0:
        case 1:
            switch (param0->unk_07) {
            case UnkEnum_ov66_022324D0_00:
                sub_0202C554(v0, v4, 1);
                break;
            case UnkEnum_ov66_022324D0_01:
                sub_0202C584(v0, v4, 1);
                break;
            case UnkEnum_ov66_022324D0_02:
                sub_0202C5B4(v0, v4, 1);
                break;
            }
            break;
        default:
            break;
        }
    }
}

void *ov90_0225BA14(UnkStruct_ov114_0225E854 *arg0) {
    u8 v0;

    v0 = arg0->unk_07;
    switch (v0) {
    default:
        return arg0;
    case 0:
        return NULL;
    case 1:
        return NULL;
    case 2:
        GF_AssertFail();
        return NULL;
    }
}
