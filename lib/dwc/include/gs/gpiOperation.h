#ifndef GS_GPIOPERATION_H
#define GS_GPIOPERATION_H

#include "gs/gp.h"

// gpiOperation.c
GPResult gpiFailedOpCallback(GPConnection *connection, const GPIOperation *operation);
GPIBool gpiFindOperationByID(const GPConnection *connection, GPIOperation **operation, int id);
GPIBool gpiOperationsAreBlocking(const GPConnection *connection);
GPResult gpiProcessOperation(GPConnection *connection, GPIOperation *operation, const char *input);

#endif
