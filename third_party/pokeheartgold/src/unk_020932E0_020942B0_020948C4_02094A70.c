#include "global.h"

#include "unk_020932E0.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02094A70(UnkStruct_020932E0 *arg0) {
#else
s32 sub_02094A70(UnkStruct_020932E0 *arg0) {
#endif
    if (arg0->unk_4680 != 0) {
        sub_02094A90(arg0, sub_02093CE4(arg0));
    }
}
