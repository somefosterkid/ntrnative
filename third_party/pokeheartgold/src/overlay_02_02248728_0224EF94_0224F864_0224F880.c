#include "global.h"

#include "field_system.h"
#include "filesystem.h"

void ov02_0224F880(FieldSystemUnkSub120 *arg0, s32 arg1);

void ov02_0224F880(FieldSystemUnkSub120 *arg0, s32 arg1) {
    s32 v0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    FieldSystemUnkSub120_Sub7E4 *v1;
#else
    void *v1;
#endif

    ReadWholeNarcMemberByIdPair(arg0->unk7E4, NARC_a_2_2_1, arg1 - 1);
    v0 = 0;
    arg0->unk868 = 0;
    arg0->unk869 = 0;
    arg0->unk86B = 0;
    arg0->unk86C_3 = 0;
    arg0->unk86A = 0;
    arg0->unk86D = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    v1 = arg0->unk7E4;
    arg0->unk86E = 0;
    while (v1->unk0 != 0xFFFF) {
        v0 += 1;
        v1++;
        if (v0 >= 5) {
            break;
        }
    }
#else
    v1 = arg0;
    arg0->unk86E = 0;
loop_1:
    if (*(u16 *)((u8 *)v1 + 0x7E4) != 0xFFFF) {
        v0 += 1;
        v1 += 8;
        if (v0 < 5) {
            goto loop_1;
        }
    }
#endif
    arg0->unk86C_4 = v0;
}
