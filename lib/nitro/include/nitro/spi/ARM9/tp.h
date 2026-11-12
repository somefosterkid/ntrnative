// NitroSDK's nitro/spi/ARM9/tp.h, the touch panel.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/tp.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_NITRO_SPI_ARM9_TP_H
#define BWPORT_NITRO_SPI_ARM9_TP_H

#include "types.h"

typedef struct {
    u16 x;
    u16 y;
    u16 touch;
    u16 validity;
} TPData;

typedef struct {
    s16 x0;
    s16 y0;
    s16 xDotSize;
    s16 yDotSize;
} TPCalibrateParam;

// Whether the stylus touches the screen
#define TP_TOUCH_OFF 0
#define TP_TOUCH_ON 1

// Which coordinates of a sample are valid
#define TP_VALIDITY_VALID 0

#define TP_VALIDITY_INVALID_X 1
#define TP_VALIDITY_INVALID_Y 2
#define TP_VALIDITY_INVALID_XY 3
// NitroSDK's enum of the request flags, int-sized
typedef int TPRequestCommandFlag;
#define TP_REQUEST_COMMAND_FLAG_AUTO_ON 0x2
#define TP_REQUEST_COMMAND_FLAG_AUTO_OFF 0x4

void TP_RequestSamplingAsync(void);
u32 TP_WaitRawResult(TPData *result);
void TP_RequestAutoSamplingStartAsync(u16 vcount, u16 frequence, TPData *samplingBufs, u16 bufSize);
void TP_RequestAutoSamplingStopAsync(void);
void TP_GetLatestRawPointInAuto(TPData *point);
u16 TP_GetLatestIndexInAuto(void);
void TP_GetCalibratedPoint(TPData *disp, const TPData *raw);
void TP_WaitBusy(TPRequestCommandFlag command_flgs);
u32 TP_CheckError(TPRequestCommandFlag command_flgs);

void TP_Init(void);
BOOL TP_GetUserInfo(TPCalibrateParam *calibrate);
void TP_SetCalibrateParam(const TPCalibrateParam *param);

// NitroSDK's static inline functions that Black's headers lacked, as a final ROM compiles them (no assertions).
// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/spi/ARM9/tp.h
static inline u32 TP_RequestAutoSamplingStart (u16 vcount, u16 frequence, TPData samplingBufs[], u16 bufSize)
{
	TP_RequestAutoSamplingStartAsync(vcount, frequence, samplingBufs, bufSize);
	TP_WaitBusy(TP_REQUEST_COMMAND_FLAG_AUTO_ON);
	return TP_CheckError(TP_REQUEST_COMMAND_FLAG_AUTO_ON);
}
static inline u32 TP_RequestAutoSamplingStop (void)
{
	TP_RequestAutoSamplingStopAsync();
	TP_WaitBusy(TP_REQUEST_COMMAND_FLAG_AUTO_OFF);
	return TP_CheckError(TP_REQUEST_COMMAND_FLAG_AUTO_OFF);
}
static inline u32 TP_RequestRawSampling (TPData * result)
{
	TP_RequestSamplingAsync();
	return TP_WaitRawResult(result);
}

#endif // BWPORT_NITRO_SPI_ARM9_TP_H
