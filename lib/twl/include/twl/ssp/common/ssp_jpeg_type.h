// TwlSDK's twl/ssp/common/ssp_jpeg_type.h, the JPEG codec's constants. From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2),
// lib/twl/include/twl/ssp.h: its types and constants, and the prototypes of Black's named functions.
#ifndef BWPORT_TWL_SSP_COMMON_SSP_JPEG_TYPE_H
#define BWPORT_TWL_SSP_COMMON_SSP_JPEG_TYPE_H

#include "types.h"

#define SSP_JPEG_RGB555 0x4

// The encoder's sampling of the chrominance: 4:4:4 (1x1), 4:2:0 (2x2) or 4:2:2 (2x1)
#define SSP_JPEG_OUTPUT_YUV444 1
#define SSP_JPEG_OUTPUT_YUV420 2
#define SSP_JPEG_OUTPUT_YUV422 3

#endif // BWPORT_TWL_SSP_COMMON_SSP_JPEG_TYPE_H
