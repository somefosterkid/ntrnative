// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gpiUnique.h, with Black's names.
#ifndef BWPORT_DWC_GPIUNIQUE_H
#define BWPORT_DWC_GPIUNIQUE_H

#include "types.h"
#include "dwc/gpi.h"

// The answers of the server to the registrations of a unique nick and of a CD key, in GameSpy's GP (gpiUnique.c), in
// overlay 11 (see dwc/gp.h for the names)

GPResult gpiProcessRegisterUniqueNick(GPConnection *connection, GPIOperation *operation, const char *input);
GPResult func_ov009_0219316c(GPConnection *connection, GPIOperation *operation, const char *input);

#endif // BWPORT_DWC_GPIUNIQUE_H
