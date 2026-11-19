#include "global.h"

#include "overlay_01_simple_renderers.h"

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_021FEA90(UnkStruct_ov5_021F2874 *arg0);
#else
unsigned short ov01_021FEA90(UnkStruct_ov5_021F2874 *arg0);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_021FEA90(UnkStruct_ov5_021F2874 *arg0) {
#else
unsigned short ov01_021FEA90(UnkStruct_ov5_021F2874 *arg0) {
#endif
    if (arg0->unk_00 == 0) {
        ov01_021FEA48(arg0);
    }
}

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_021FEAA0(UnkStruct_ov5_021F2874 *arg0);
#else
unsigned int ov01_021FEAA0(UnkStruct_ov5_021F2874 *arg0);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov01_021FEAA0(UnkStruct_ov5_021F2874 *arg0) {
#else
unsigned int ov01_021FEAA0(UnkStruct_ov5_021F2874 *arg0) {
#endif
    if (arg0->unk_00 == 0) {
        ov01_021FEA7C(arg0);
    }
}
