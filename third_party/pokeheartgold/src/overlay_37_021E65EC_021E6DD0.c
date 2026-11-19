#include "global.h"

#include "unk_02095EAC.h"

#include "message_format.h"
#include "unk_02034354.h"
#include "unk_020379A0.h"

s32 ov37_021E68AC(void *);

s32 ov37_021E762C(UnkStruct_02095EAC *, s32, s32);

s32 ov37_021E7844(void *, s32);

s32 ov37_021E6DD0(UnkStruct_02095EAC *arg0, s32 arg1);

s32 ov37_021E6DD0(UnkStruct_02095EAC *arg0, s32 arg1) {
    BufferPlayersName(arg0->unk_0C, 0U, sub_02034818(0U));
    ov37_021E762C(arg0, 3, 1);
    ov37_021E7844(arg0, 0x10);
    ov37_021E68AC(arg0);
    return arg1;
}

s32 ov37_021E76A0(s32);

s32 ov37_021E6E04(UnkStruct_02095EAC *arg0, s32 arg1);

s32 ov37_021E6E04(UnkStruct_02095EAC *arg0, s32 arg1) {
    if (ov37_021E76A0(arg0->unk_30) != 0) {
        ov37_021E7844(arg0, 0x11);
        sub_02037AC0(0xC8U);
    }
    ov37_021E68AC(arg0);
    return arg1;
}
