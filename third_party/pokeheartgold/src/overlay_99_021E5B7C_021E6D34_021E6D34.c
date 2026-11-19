#include "global.h"

#include "message_format.h"
#include "overlay_98_sprite_ctx.h"
#include "overlay_99_pokeathlon_course_summary.h"
#include "sprite_system.h"
#include "overlay_99_obj99.h"
#include "overlay_98_ov98text_sys.h"
#include "player_data.h"

s32 ov98_0221EF80(s32 param_1);

u8 ov99_021E7068(const PokeathlonCourseSummary *);
u8 ov99_021E7078(const PokeathlonCourseSummary *);
u8 ov99_021E7088(const PokeathlonCourseSummary *);
PlayerProfile *ov99_021E7098(const PokeathlonCourseSummary *);
u16 ov99_021E70A8(const PokeathlonCourseSummary *);
u16 ov99_021E70B8(const PokeathlonCourseSummary *);
u16 ov99_021E70C8(const PokeathlonCourseSummary *);
u16 ov99_021E70D8(const PokeathlonCourseSummary *);
s32 ov99_021E7100(s32);

u8 ov99_021E7124(Obj99 *);

extern s32 ov99_021E9D8C;

extern s32 ov99_021E9DAC;

void ov99_021E6D34(Obj99 *arg0) {
    MessageFormat *sp2C;
    s32 sp28;
    s32 sp24;
    s32 sp20;
    s32 sp1C;
    u32 sp18;
    s32 sp14;
    s32 sp10;
    MessageFormat *temp_r0_4;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r4;
    s32 temp_r7;
    u8 temp_r6;

    sp1C = ov99_021E70C8(arg0->summary);
    sp20 = ov99_021E70B8(arg0->summary);
    sp24 = ov99_021E70A8(arg0->summary);
    temp_r0 = ov99_021E70D8(arg0->summary);
    sp18 = temp_r0;
    temp_r0_2 = ov99_021E7088(arg0->summary);
    sp28 = temp_r0_2;
    sp14 = ov99_021E7124(arg0);
    temp_r0_3 = ov98_0221EF80(ov99_021E7078(arg0->summary));
    sp10 = temp_r0_3;
    temp_r7 = ov99_021E7068(arg0->summary);
    temp_r4 = ov99_021E7100(temp_r0);
    temp_r6 = temp_r4 + (temp_r7 + (sp14 + temp_r0_3));
    temp_r0_4 = ov98_0221EEFC(arg0->textSys);
    sp2C = temp_r0_4;
    BufferPlayersName(temp_r0_4, 0U, ov99_021E7098(arg0->summary));
    ov98_0221EC08(arg0->textSys, 0, 0xB, 1, 0);
    ov98_0221EBD8(arg0->textSys, 1, 0xC, 0);
    ov98_0221EBD8(arg0->textSys, 2, 0xD, 0);
    ov98_0221EBD8(arg0->textSys, 3, 0xE, 0);
    ov98_0221EBD8(arg0->textSys, 4, 0xF, 0);
    ov98_0221EC08(arg0->textSys, 5, 0x20, 2, 0);
    ov98_0221EBEC(arg0->textSys, 7, 0, 1, 0, 4);
    ov99_021E6CF4(arg0, temp_r6);
    ov98_0221EBD8(arg0->textSys, 8, 0x10, 0);
    ov98_0221EBD8(arg0->textSys, 0xD, 0x1F, 0);
    ov98_0221EBD8(arg0->textSys, 0xE, 1, 0);
    ov98_0221ECD0(arg0->textSys, 0xA, 0x1C, sp1C, 4, 0);
    ov98_0221ECD0(arg0->textSys, 0xB, 0x1D, sp20, 4, 0);
    ov98_0221ECD0(arg0->textSys, 0xC, 0x1E, sp24, 4, 0);
    ov99_021E6D14(arg0, temp_r4);
    ov99_021E6C14(arg0);
    if (temp_r0_2 == 1) {
        ManagedSprite_SetAnim(arg0->sprites[0], 1);
        ManagedSprite_SetPaletteOverride(arg0->sprites[0], 1);
    }
    ov99_021E6C30(arg0, temp_r6, 1, 2, 0xC8, 0x70, 0x20, 1);
    ov99_021E6C30(arg0, sp18, 3, 4, 0x50, 0xA0, 0x20, 0);
    ov98_0221E6F0(arg0->spriteContext, arg0->sprites, &ov99_021E9DAC, sp14, 7, 0x10, 0x28, 1);
    ov98_0221E6F0(arg0->spriteContext, arg0->sprites, &ov99_021E9DAC, sp10, 0x11, 0x10, 0x48, 1);
    ov98_0221E6F0(arg0->spriteContext, arg0->sprites, &ov99_021E9DAC, temp_r7, 0x1B, 0x10, 0x68, 1);
    ov98_0221E6F0(arg0->spriteContext, arg0->sprites, &ov99_021E9DAC, temp_r4, 0x25, 0x10, 0x88, 1);
    ov98_0221E6F0(arg0->spriteContext, arg0->sprites, &ov99_021E9D8C, temp_r4, 0x39, 0x60, 0x78, 0);
}
