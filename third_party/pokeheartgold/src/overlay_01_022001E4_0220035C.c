#include "global.h"

#include "overlay_01_022001E4.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_0220035C(UnkOv01_022001E4 *arg0) {
#else
u32 ov01_0220035C(UnkOv01_022001E4 *arg0) {
#endif
    UnkOv01_022001E4 *new_var;
    int new_var2;
    new_var2 = 0;
    new_var = arg0;
    if (arg0->unk_00 == new_var2) {
        ov01_02200260(arg0);
    }
    arg0 = arg0;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_0220036C(UnkOv01_022001E4 *arg0) {
#else
int ov01_0220036C(UnkOv01_022001E4 *arg0) {
#endif
    if (arg0->unk_04 == 0) {
        ov01_022002AC(arg0);
    }
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_0220037C(UnkOv01_022001E4 *arg0) {
#else
unsigned char ov01_0220037C(UnkOv01_022001E4 *arg0) {
#endif
    if (arg0->unk_00 == 0) {
        ov01_022002FC(arg0);
    }
}
