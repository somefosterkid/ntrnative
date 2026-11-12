// Black's code at ov021 0x021c7f70-0x021c8018. The name is the file name its asserts pass (tools/name_files.py).

#include "types.h"
#include "field/field_skill_map_eff.h"
#include "gfl/heap.h"
#include "field/field_flash.h"
#include "field/ov021_021c7de0.h"

// pokebw2's FieldSkillMapEff: the field moves' map effects, of which there is only Flash's
typedef struct FieldSkillMapEff {
    FieldFlash *flash;
} FieldSkillMapEff;

void FieldSkillMapEff_SetupFlash(FieldSkillMapEff *param_1, u32 param_2, HeapID param_3);
void func_ov021_021c7ff0(FieldSkillMapEff *param_1);
void func_ov021_021c8000(FieldSkillMapEff *param_1);
void func_ov021_021c800c(FieldSkillMapEff *param_1);

u32 *func_ov021_021c7f70(u32 param_1, HeapID param_2) {
    FieldSkillMapEff *ptr;

    ptr = GFL_HeapAllocate(param_2, sizeof(FieldSkillMapEff), 1, "fieldskill_mapeff.c", 0x49);
    FieldSkillMapEff_SetupFlash(ptr, param_1, param_2);
    return (u32 *)ptr;
}

void func_ov021_021c7f9c(u32 *param_1) {
    func_ov021_021c7ff0((FieldSkillMapEff *)param_1);
    GFL_HeapFree(param_1);
}

void func_ov021_021c7fac(s32 *param_1) {
    func_ov021_021c8000((FieldSkillMapEff *)param_1);
}

void func_ov021_021c7fb4(u32 *param_1) {
    func_ov021_021c800c((FieldSkillMapEff *)param_1);
}

void *FieldSkillMapEff_GetFlash(void *skillMapEff) {
    FieldSkillMapEff *effect = skillMapEff;

    return effect->flash;
}

void FieldSkillMapEff_SetupFlash(FieldSkillMapEff *param_1, u32 param_2, HeapID param_3) {
    FieldFlash *ptr;

    if ((param_2 & 3) != 0) {
        ptr = FieldFlash_Create(param_3);
        param_1->flash = ptr;
        if ((param_2 & 2) != 0) {
            func_ov021_021c7e50(ptr, 2);
        } else {
            func_ov021_021c7e50(ptr, 0);
        }
    } else {
        param_1->flash = NULL;
    }
}

void func_ov021_021c7ff0(FieldSkillMapEff *param_1) {
    func_ov021_021c7de0(param_1->flash);
    param_1->flash = NULL;
}

void func_ov021_021c8000(FieldSkillMapEff *param_1) {
    func_ov021_021c7dfc(param_1->flash);
}

void func_ov021_021c800c(FieldSkillMapEff *param_1) {
    func_ov021_021c7e30(param_1->flash);
}
