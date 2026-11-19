#include "global.h"

#include "overlay_01_021FE590.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_021FE64C(BerryPatchEffectCounter *arg0);
#else
s32 ov01_021FE64C(BerryPatchEffectCounter *arg0);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_021FE64C(BerryPatchEffectCounter *arg0) {
#else
s32 ov01_021FE64C(BerryPatchEffectCounter *arg0) {
#endif
    if (arg0->counter == 0) {
        ov01_021FE5CC(arg0);
    }
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_021FE65C(BerryPatchEffectCounter *arg0);
#else
unsigned int ov01_021FE65C(BerryPatchEffectCounter *arg0);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_021FE65C(BerryPatchEffectCounter *arg0) {
#else
unsigned int ov01_021FE65C(BerryPatchEffectCounter *arg0) {
#endif
    int new_var;
    new_var = 0;
    if (arg0->counter == new_var) {
        ov01_021FE61C(arg0);
    }
}
