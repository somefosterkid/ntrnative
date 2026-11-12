// NitroSDK's nitro/spi/ARM9/mic.h, the microphone.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/mic.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_NITRO_SPI_ARM9_MIC_H
#define BWPORT_NITRO_SPI_ARM9_MIC_H

#include "types.h"

typedef enum {
    MIC_RESULT_SUCCESS,
    MIC_RESULT_BUSY,
    MIC_RESULT_ILLEGAL_PARAMETER,
    MIC_RESULT_SEND_ERROR,
    MIC_RESULT_INVALID_COMMAND,
    MIC_RESULT_ILLEGAL_STATUS,
    MIC_RESULT_FATAL_ERROR,
} MICResult;

// How the ARM7 samples: 8 or 12 bits, signed or not, and with its filter on or off
typedef enum {
    MIC_SAMPLING_TYPE_8BIT,
    MIC_SAMPLING_TYPE_12BIT,
    MIC_SAMPLING_TYPE_SIGNED_8BIT,
    MIC_SAMPLING_TYPE_SIGNED_12BIT,
    MIC_SAMPLING_TYPE_12BIT_FILTER_OFF,
    MIC_SAMPLING_TYPE_SIGNED_12BIT_FILTER_OFF,
} MICSamplingType;

// 8180 Hz, in ARM7 clocks
#define MIC_SAMPLING_RATE_8180 0x1001

typedef void (*MICCallback)(MICResult result, void *arg);

typedef struct {
    int type;
    void *buffer;
    u32 size;
    u32 rate;
    BOOL loop_enable;
    MICCallback full_callback;
    void *full_arg;
} MICAutoParam;

#define PM_AMP_OFF 0
#define PM_AMP_ON 1
// NitroSDK's enum, int-sized
typedef int PMAmpSwitch;

// NitroSDK's gains of the microphone amplifier
typedef enum {
    PM_AMPGAIN_20 = 0,
    PM_AMPGAIN_40 = 1,
    PM_AMPGAIN_80 = 2,
    PM_AMPGAIN_160 = 3,
    PM_AMPGAIN_DEFAULT = PM_AMPGAIN_40
} PMAmpGain;

MICResult MIC_StartAutoSamplingAsync(const MICAutoParam *param, MICCallback callback, void *arg);
// TwlSDK's: on a DSi in TWL mode these use the codec's own microphone code (MICEX), else the NitroSDK's
MICResult MIC_StartLimitedSamplingAsync(const MICAutoParam *param, MICCallback callback, void *arg);
MICResult MIC_StartLimitedSampling(const MICAutoParam *param);
MICResult MIC_StopLimitedSampling(void);

u32 PM_SetAmp(PMAmpSwitch status);
// The amplifier switch in *swBuf (when not NULL)
u32 PM_GetAmp(PMAmpSwitch *swBuf);
void MIC_Init(void);                     // MIC_Init

void PM_Init(void);     // PM_Init
u32 PM_SetAmpGain(PMAmpGain gain);
// TwlSDK's: the gain as a level from 0 to 119
u32 PM_SetAmpGainLevel(u8 level);
void *MIC_GetLastSamplingAddress(void);

// The sampling rate is the number of ARM7 clocks (33.51 MHz) between two samples, and the buffer and its size are
// multiples of 32 bytes, which the cache needs
#define MIC_SAMPLING_RATE_MIN 0x400

// The library's state, which the DSi's MICEX code in the LTD autoload reaches through MICi_GetSysWork. The busy flag is
// set from the call that starts or stops the sampling until the ARM7 answers
typedef struct {
    BOOL busy;
    MICCallback callback;
    void *callbackArg;
    MICResult result;
    MICCallback fullCallback;
    void *fullArg;
    u16 *sampleDest;
} MICWork;

MICResult MIC_StopAutoSamplingAsync(MICCallback callback, void *arg);
MICWork *MICi_GetSysWork(void);

#endif // BWPORT_NITRO_SPI_ARM9_MIC_H
