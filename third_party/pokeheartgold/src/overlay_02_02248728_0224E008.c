#include "global.h"
#include "heap.h"
#include "sys_task_api.h"
#include "bag_cursor.h"
#include "bg_window.h"
#include "camera.h"
#include "field/dynamic_terrain_height.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"
#include "list_menu.h"
#include "map_matrix.h"
#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "unk_0200A090.h"
#include "overlay_02_field3d_effects.h"



struct UnkStruct_ov5_021D1BEC_t;
typedef struct UnkStruct_ov5_021D1BEC_t UnkStruct_ov5_021D1BEC;
typedef struct {
    UnkStruct_ov5_021D1BEC *unk_00;
    u16 *unk_04;
    u32 unk_08;
    u32 unk_0C;
} UnkStruct_ov6_02248948;
typedef void (*UnkFuncPtr_ov6_02249834)(UnkStruct_ov5_021D1BEC *);
typedef BOOL (*UnkFuncPtr_ov6_0224984C)(UnkStruct_ov5_021D1BEC *);
extern const UnkFuncPtr_ov6_02249834 ov02_02253A04[6];
extern const UnkFuncPtr_ov6_0224984C ov02_02253A34[6];
void ov02_0224E020(SysTask *param0, void *param1);

void ov02_0224E008(Ov02App_0224DCB0 *env) {
    int i;

    for (i = 0; i < 18; i++) {
        ov02_0224DE6C(&env->slots[i]);
    }
}

void ov02_0224E020(SysTask *param0, void *param1) {
    UnkStruct_ov6_02248948 *v0 = param1;

    switch (v0->unk_08) {
    case 0:
        if (ov02_02253A34[v0->unk_0C](v0->unk_00) == 1) {
            v0->unk_08++;
        }
        break;
    case 1:
        ov02_02253A04[v0->unk_0C](v0->unk_00);
        *v0->unk_04 = 1;
        Heap_Free(v0);
        SysTask_Destroy(param0);
        break;
    }
}
