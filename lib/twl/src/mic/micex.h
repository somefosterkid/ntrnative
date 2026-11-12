// TwlSDK's micex.h, a header of the SPI library's own (libntr's libraries/spi/include/micex.h), beside its source.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/twl/include/twl/mic.h, with Black's names.
#ifndef BWPORT_TWL_SRC_MIC_MICEX_H
#define BWPORT_TWL_SRC_MIC_MICEX_H

#include "types.h"
#include "nitro/spi/ARM9/mic.h"

// The DSi's limited sampling of the microphone in the LTD autoload (lib/twl/src/mic/mic_twl.c), which NitroSDK's
// MIC_StartLimitedSampling, MIC_StartLimitedSamplingAsync and MIC_StopLimitedSampling call when the console runs in its
// DSi mode. They take the sampling rates 0x2bf, 0x3ff, 0x57f, 0x800, 0x83f, 0xb00, 0xbff and MIC_SAMPLING_RATE_8180
// only. Their names are guesses after NitroSDK's
MICResult MICEXi_StartLimitedSampling(const MICAutoParam *param);
MICResult MICEXi_StartLimitedSamplingAsync(const MICAutoParam *param, MICCallback callback, void *arg);
MICResult MICEXi_StopLimitedSampling(void);

#endif // BWPORT_TWL_SRC_MIC_MICEX_H
