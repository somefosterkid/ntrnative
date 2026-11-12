#ifndef GS_GPIUNIQUE_H
#define GS_GPIUNIQUE_H

#include "gs/gp.h"

GPResult gpiProcessRegisterUniqueNick(GPConnection *connection, GPIOperation *operation, const char *input);
// Black's: the register-cdkey twin of gpiProcessRegisterUniqueNick
GPResult func_ov009_0219316c(GPConnection *connection, GPIOperation *operation, const char *input);

#endif
