// TwlSDK's twl/ssp/ARM9/jpegenc.h, the JPEG encoder. From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2),
// lib/twl/include/twl/ssp.h: its types and constants, and the prototypes of Black's named functions.
#ifndef BWPORT_TWL_SSP_ARM9_JPEGENC_H
#define BWPORT_TWL_SSP_ARM9_JPEGENC_H

#include "types.h"
#include "twl/ssp/common/ssp_jpeg_type.h"

u32 SSP_GetJpegEncoderBufferSize(u32 width, u32 height, u32 sampling, u32 option);
u32 SSP_StartJpegEncoder(const void *src, u8 *dst, u32 limit, u8 *work, u32 width, u32 height, u32 quality,
                         u32 sampling, u32 option);

#endif // BWPORT_TWL_SSP_ARM9_JPEGENC_H
