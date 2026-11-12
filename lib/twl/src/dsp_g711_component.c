// The TwlSDK's G.711 DSP component: the Teak COFF image built from TwlSDK/build/dspcomponents (processes/audio,
// g711.c) that DSP_OpenStaticComponentG711Core (func_0270723c) opens as a memory file for DSP_LoadG711Core
// (func_02707278), the DSi's voice codec. Its bytes stay out of the C: the build takes them from the base ROM
// (tools/asset.py, config/b_us/assets.tsv).
#include "types.h"

// The component image, 0x26d1d bytes; the linker's 32-byte alignment of .ltdmain pads it to 0x0272e9e0
const u8 data_02707cb4[] = {
#include "asset/dsp_g711_component.inc"
};
