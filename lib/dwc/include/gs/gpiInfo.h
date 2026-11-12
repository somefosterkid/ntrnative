#ifndef GS_GPIINFO_H
#define GS_GPIINFO_H

#include "gs/gp.h"

// Black's: GameSpy's gpiAddLocalInfo
GPResult func_ov009_0218c5fc(GPConnection *connection, GPIBuffer *outputBuffer);
// Black's: GameSpy's gpiProcessGetInfo
s32 func_ov009_0218bdb8(GPConnection *connection, GPIOperation *operation, const char *input);
GPResult gpiSetInfos(GPConnection *connection, GPEnum info, const char *value);
GPResult gpiSendGetInfo(GPConnection *connection, int profileid, int operationid);
GPResult gpiGetInfo(GPConnection *connection, GPProfile profile, GPEnum checkCache, GPEnum blocking,
                    GPCallback callback, void *param);

#endif
