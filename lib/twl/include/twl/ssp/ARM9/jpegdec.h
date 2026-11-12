// TwlSDK's twl/ssp/ARM9/jpegdec.h, the JPEG decoder. From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2),
// lib/twl/include/twl/ssp.h: its types and constants, and the prototypes of Black's named functions.
#ifndef BWPORT_TWL_SSP_ARM9_JPEGDEC_H
#define BWPORT_TWL_SSP_ARM9_JPEGDEC_H

#include "types.h"
#include "twl/ssp/common/ssp_jpeg_type.h"

BOOL SSP_StartJpegDecoder(u8 *data, u32 size, void *dst, s16 *width, s16 *height, u32 option);

#endif // BWPORT_TWL_SSP_ARM9_JPEGDEC_H
