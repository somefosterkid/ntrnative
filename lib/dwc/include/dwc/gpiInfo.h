// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gpiInfo.h, with Black's names.
#ifndef BWPORT_DWC_GPIINFO_H
#define BWPORT_DWC_GPIINFO_H

#include "types.h"
#include "dwc/gpi.h"

// The info of the profiles of GameSpy's GP (gpiInfo.c), in overlay 11: the server's answers, the cache of them and the
// changes to the player's own (see dwc/gp.h for the names)

GPResult func_ov009_0218bdb8(GPConnection *connection, GPIOperation *operation, const char *input);
// Adds the changes waiting in the update buffers to a message
GPResult func_ov009_0218c5fc(GPConnection *connection, GPIBuffer *buffer);
GPResult gpiSetInfos(GPConnection *connection, GPEnum info, const char *value);
GPResult gpiSendGetInfo(GPConnection *connection, int profileid, int operationid);
GPResult gpiGetInfo(GPConnection *connection, GPProfile profile, GPEnum checkCache, GPEnum blocking,
                    GPCallback callback, void *param);
void gpiFreeInfoCache(GPIProfile *profile);

#endif // BWPORT_DWC_GPIINFO_H
