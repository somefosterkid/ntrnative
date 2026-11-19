#include "global.h"

void ov74_02230030(void *arg);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov74_02231448(WMErrCode param0);
#define ov74_02231448() ov74_02231448((WMErrCode)0)
#else
void ov74_02231448();
#endif

s32 ov74_022314A0(void);

s32 ov74_022314A0(void) {
    if (WM_Enable(ov74_02230030) != WM_ERRCODE_OPERATING) {
        ov74_02231448();
        return 0;
    }
    return 1;
}

void ov74_02230070(void *arg);

s32 ov74_022314BC(void);

s32 ov74_022314BC(void) {
    if (WM_Disable(ov74_02230070) != WM_ERRCODE_OPERATING) {
        ov74_02231448();
        OS_Terminate();
        return 0;
    }
    return 1;
}
