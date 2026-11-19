#include "global.h"

s32 ov59_0223A0B8(void *arg0);

s32 ov59_0223A120(void *, s32 *);

s32 ov59_02239EF8(void *arg0);

s32 ov59_02239EF8(void *arg0) {
    s32 sp0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    s32 ret;
#endif
    sp0 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    ret = ov59_0223A120(arg0, &sp0);
    if (sp0 == 0) {
        ret = ov59_0223A0B8(arg0);
    }
    return ret; /* r0 on DS = result of the last call made */
#else
    ov59_0223A120(arg0, &sp0);
    if (sp0 == 0) {
        ov59_0223A0B8(arg0);
    }
#endif
}
