#include "global.h"

#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "palette.h"
#include "screen_fade.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_020210A0.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021F297C(void *);
s32 ov14_021F29AC(void *);

s32 ov14_021E783C(void *, s32, s32);
s32 ov14_021E7930(void *, u32);
s32 ov14_021E7BA4(void *);
s32 ov14_021E825C(void *);
s32 ov14_021F2F20(void *);
s32 ov14_021F2F3C(void *);
s32 ov14_021F49C8(void *);
s32 ov14_021F566C(void *);
s32 ov14_021F6A44(void *);
s32 ov14_021F6B10(void *);

void ov14_021E59AC(SysTask *task, void *data);

s32 ov14_021EAFAC(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    void *dest;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_EngineASetPlanes(0U);
    GfGfx_EngineBSetPlanes(0U);
    *(s16 *)REG_BLDCNT_ADDR = 0;
    *(s16 *)REG_DB_BLDCNT_ADDR = 0;
    EnableTouchPad();
    InitializeTouchPad(4);
    *(u16 *)REG_POWCNT_ADDR &= 0xFFFF7FFF;
    Heap_Create(HEAP_ID_3, HEAP_ID_10, 0x80000U);
    dest = Heap_Alloc(HEAP_ID_10, sizeof(struct UnkStruct_ov14_boxsys));
    arg0->sys = dest;
    MI_CpuFill8(dest, 0U, sizeof(struct UnkStruct_ov14_boxsys));
    arg0->sys->unk_450 = NARC_New(NARC_poketool_personal_personal, HEAP_ID_10);
    arg0->sys->unk_454 = NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_10);
    ov14_021E5A60();
    ov14_021E5A70(arg0);
    ov14_021E5E74(arg0);
    ov14_021E5C54(arg0);
    ov14_021E5D78(arg0);
    ov14_021E5DE0(arg0);
    ov14_021F4ED0(arg0);
    ov14_021F297C(arg0);
    ov14_021F2F20(arg0);
    ov14_021F2F3C(arg0);
    ov14_021E783C(arg0, ov14_021E7930(arg0, arg0->unk1F), 2);
    ov14_021E7BA4(arg0);
    v0 = *(s32 *)((u8 *)(arg0->unk_0) + 8);
    if ((v0 != 1) && (v0 != 0)) {
        ov14_021E81FC(arg0);
        ov14_021E825C(arg0);
    }
    ov14_021E82BC(arg0);
    ov14_021E5ED0(arg0);
    ov14_021F5620(arg0);
    ov14_021F566C(arg0);
    ov14_021F49C8(arg0);
    ov14_021F6A44(arg0);
    arg0->sys->unk_0 = SysTask_CreateOnVBlankQueue(ov14_021E59AC, arg0, 0U);
    ov14_021E5EAC(1);
    return arg0->unk30;
}

s32 ov14_021EB0E4(void *arg0) {
    ov14_021E6048(arg0);
    SysTask_Destroy(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_0);
    ov14_021F6B10(arg0);
    ov14_021E5EE8(arg0);
    ov14_021E7D7C(((UnkStruct_ov14_boxapp *)arg0)->sys);
    ov14_021F29AC(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    ov14_021F4F00(arg0);
    ov14_021E5DB8(arg0);
    ov14_021E5E94(arg0);
    ov14_021E5C00(arg0);
    NARC_Delete(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_454);
    NARC_Delete(((UnkStruct_ov14_boxapp *)arg0)->sys->unk_450);
    DisableTouchPad();
    *(s16 *)REG_BLDCNT_ADDR = 0;
    *(s16 *)REG_DB_BLDCNT_ADDR = 0;
    GfGfx_EngineASetPlanes(0U);
    GfGfx_EngineBSetPlanes(0U);
    Heap_Free(((void *)((UnkStruct_ov14_boxapp *)arg0)->sys));
    Heap_Destroy(HEAP_ID_10);
    return *((s32 *)&((UnkStruct_ov14_boxapp *)arg0)->unk30);
}

s32 ov14_021EB170(void *arg0) {
    if (IsPaletteFadeFinished() == 1) {
        return ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_440;
    }
    return 2;
}

s32 ov14_021EB18C(UnkStruct_ov14_boxapp *arg0) {
    if (PaletteData_GetSelectedBuffersBitmask(arg0->sys->unk_18) == 0) {
        return arg0->unk30;
    }
    return 3;
}

s32 ov14_021EB1A4(UnkStruct_ov14_boxapp *arg0) {
    s32 v0;
    UnkStruct_ov14_boxsys *v1;

    v1 = arg0->sys;
    v0 = v1->unk_444;
    if (v0 == 0) {
        return arg0->unk30;
    }
    v1->unk_444 = (s32)(v0 - 1);
    return 4;
}
