// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gpiOperation.h, with Black's names.
#ifndef BWPORT_DWC_GPIOPERATION_H
#define BWPORT_DWC_GPIOPERATION_H

#include "dwc/gpi.h"
#include "dwc/nonport.h"

// The operations in progress of GameSpy's GP (gpiOperation.c), in overlay 11 (see dwc/gp.h for the names)

// Calls the app's callback of an operation that failed, with only the result
GPResult gpiFailedOpCallback(GPConnection *connection, const GPIOperation *operation);
GPResult gpiAddOperation(GPConnection *connection, int type, void *data, GPIOperation **op, GPEnum blocking,
                         GPCallback callback, void *param);
void gpiRemoveOperation(GPConnection *connection, GPIOperation *operation);
GPIBool gpiFindOperationByID(const GPConnection *connection, GPIOperation **operation, int id);
GPIBool gpiOperationsAreBlocking(const GPConnection *connection);
GPResult gpiProcessOperation(GPConnection *connection, GPIOperation *operation, const char *input);

#endif // BWPORT_DWC_GPIOPERATION_H
