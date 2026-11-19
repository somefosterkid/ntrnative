#include "global.h"

#include "sound_02004A44.h"

s32 sub_02005B78(u16, s32, s16 *);

s32 ov96_021EF280(s32 arg0, s32 arg1);

BOOL PokeathlonCourse_GetField3D8_ForCurrentParticipant(u8 printerID);
u8 ov96_021EF2A0(u8 printerID);

s32 ov96_021EF280(s32 arg0, s32 arg1) {
    s16 v0;
    s32 v1;

    sub_02005B78(GF_GetCurrentPlayingBGM(), arg0, &v0);
    v1 = (s32)v0 >= arg1 ? 1 : 0;
    return v1;
}

u8 ov96_021EF2A0(u8 printerID) {
    return PokeathlonCourse_GetField3D8_ForCurrentParticipant(printerID);
}
