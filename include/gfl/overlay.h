// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/gfl/overlay.h, the lines in use so far.
#ifndef BWPORT_GFL_OVERLAY_H
#define BWPORT_GFL_OVERLAY_H

#include "types.h"
#include "gfl/heap.h"

// The linker script defines OVERLAY_<n>_ID for every overlay, so that overlay IDs in the code are relocations; its
// value is the overlay's number, which a PC target (the port: no such link, and an address doesn't fit a u32 constant
// initializer on x64) takes as it is
#if !defined(__MWERKS__) && !defined(__INTELLISENSE__) && !defined(__arm__)
#define OVERLAY_ID(n) ((u32)(n))
#else
#define OVERLAY_ID(n) ((u32)OVERLAY_##n##_ID)
#endif
// For procs in the main program, which need no overlay
#define OVERLAY_NONE (-1)

extern u32 OVERLAY_74_ID[];

extern u32 OVERLAY_1_ID[];
extern u32 OVERLAY_36_ID[];
extern u32 OVERLAY_117_ID[];
extern u32 OVERLAY_136_ID[];
extern u32 OVERLAY_118_ID[];
extern u32 OVERLAY_100_ID[];
extern u32 OVERLAY_7_ID[];
extern u32 OVERLAY_125_ID[];
extern u32 OVERLAY_4_ID[];
extern u32 OVERLAY_6_ID[];
extern u32 OVERLAY_8_ID[];
extern u32 OVERLAY_29_ID[];
extern u32 OVERLAY_30_ID[];
extern u32 OVERLAY_31_ID[];
extern u32 OVERLAY_32_ID[];
extern u32 OVERLAY_33_ID[];
extern u32 OVERLAY_34_ID[];
extern u32 OVERLAY_35_ID[];
extern u32 OVERLAY_9_ID[];
extern u32 OVERLAY_10_ID[];
extern u32 OVERLAY_11_ID[];
extern u32 OVERLAY_12_ID[];
extern u32 OVERLAY_13_ID[];
extern u32 OVERLAY_129_ID[];
extern u32 OVERLAY_132_ID[];
extern u32 OVERLAY_14_ID[];
extern u32 OVERLAY_15_ID[];
extern u32 OVERLAY_16_ID[];
extern u32 OVERLAY_17_ID[];
extern u32 OVERLAY_18_ID[];
extern u32 OVERLAY_19_ID[];
extern u32 OVERLAY_21_ID[];
extern u32 OVERLAY_22_ID[];
extern u32 OVERLAY_23_ID[];
extern u32 OVERLAY_137_ID[];
extern u32 OVERLAY_95_ID[];
extern u32 OVERLAY_96_ID[];
extern u32 OVERLAY_56_ID[];
extern u32 OVERLAY_63_ID[];
extern u32 OVERLAY_67_ID[];
extern u32 OVERLAY_69_ID[];
extern u32 OVERLAY_72_ID[];
extern u32 OVERLAY_82_ID[];
extern u32 OVERLAY_83_ID[];
extern u32 OVERLAY_90_ID[];
extern u32 OVERLAY_92_ID[];
extern u32 OVERLAY_93_ID[];
extern u32 OVERLAY_186_ID[];
extern u32 OVERLAY_183_ID[];
extern u32 OVERLAY_184_ID[];
extern u32 OVERLAY_185_ID[];
extern u32 OVERLAY_230_ID[];
extern u32 OVERLAY_91_ID[];
extern u32 OVERLAY_94_ID[];
extern u32 OVERLAY_97_ID[];
extern u32 OVERLAY_214_ID[];
extern u32 OVERLAY_88_ID[];
extern u32 OVERLAY_107_ID[];
extern u32 OVERLAY_121_ID[];
extern u32 OVERLAY_168_ID[];
extern u32 OVERLAY_110_ID[];
extern u32 OVERLAY_112_ID[];
extern u32 OVERLAY_114_ID[];
extern u32 OVERLAY_115_ID[];
extern u32 OVERLAY_116_ID[];
extern u32 OVERLAY_119_ID[];
extern u32 OVERLAY_120_ID[];
extern u32 OVERLAY_123_ID[];
extern u32 OVERLAY_127_ID[];
extern u32 OVERLAY_133_ID[];
extern u32 OVERLAY_135_ID[];
extern u32 OVERLAY_131_ID[];
extern u32 OVERLAY_170_ID[];
extern u32 OVERLAY_196_ID[];
extern u32 OVERLAY_197_ID[];
extern u32 OVERLAY_198_ID[];
extern u32 OVERLAY_187_ID[];
extern u32 OVERLAY_195_ID[];
extern u32 OVERLAY_199_ID[];
extern u32 OVERLAY_203_ID[];
extern u32 OVERLAY_206_ID[];
extern u32 OVERLAY_205_ID[];
extern u32 OVERLAY_217_ID[];
extern u32 OVERLAY_220_ID[];
extern u32 OVERLAY_228_ID[];
extern u32 OVERLAY_98_ID[];
extern u32 OVERLAY_227_ID[];
extern u32 OVERLAY_231_ID[];
extern u32 OVERLAY_188_ID[];
extern u32 OVERLAY_194_ID[];
extern u32 OVERLAY_204_ID[];
extern u32 OVERLAY_207_ID[];
extern u32 OVERLAY_93_ID[];
extern u32 OVERLAY_213_ID[];
extern u32 OVERLAY_139_ID[];
extern u32 OVERLAY_102_ID[];
extern u32 OVERLAY_103_ID[];
extern u32 OVERLAY_104_ID[];
extern u32 OVERLAY_105_ID[];
extern u32 OVERLAY_106_ID[];
extern u32 OVERLAY_175_ID[];
extern u32 OVERLAY_176_ID[];
extern u32 OVERLAY_177_ID[];
extern u32 OVERLAY_178_ID[];
extern u32 OVERLAY_179_ID[];

// The overlay manager (gf_overlay.c), with room for as many overlays loaded at once to main memory, ITCM and DTCM
void GFL_OvlManagerInit(HeapID heapId, u32 mainCount, u32 itcmCount, u32 dtcmCount);
// Loads an overlay, unless it would overwrite one that is loaded. OVERLAY_NONE loads nothing
BOOL GFL_OvlLoad(u32 overlayId);
void GFL_OvlUnload(u32 overlayId);

extern u32 OVERLAY_216_ID[];
extern u32 OVERLAY_3_ID[];
extern u32 OVERLAY_57_ID[];

extern u32 OVERLAY_101_ID[];
extern u32 OVERLAY_219_ID[];
extern u32 OVERLAY_212_ID[];
extern u32 OVERLAY_70_ID[];
extern u32 OVERLAY_111_ID[];
extern u32 OVERLAY_68_ID[];
extern u32 OVERLAY_128_ID[];

#endif // BWPORT_GFL_OVERLAY_H
