// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gpiCallback.h, with Black's names.
#ifndef BWPORT_DWC_GPICALLBACK_H
#define BWPORT_DWC_GPICALLBACK_H

#include "types.h"
#include "dwc/gpi.h"

// The callbacks of GameSpy's GP that wait to be called (gpiCallback.c), in overlay 11 (see dwc/gp.h for the names)

void func_ov009_02189d78(GPConnection *connection, GPResult result, GPEnum fatal);
GPResult func_ov009_02189dfc(GPConnection *connection, GPICallback callback, void *arg, const GPIOperation *operation,
                        int type);
// Calls the callbacks of an operation (and the errors), or all of them for an id of 0
GPResult func_ov009_0218a0e4(GPConnection *connection, int blockingOperationID);

#endif // BWPORT_DWC_GPICALLBACK_H
