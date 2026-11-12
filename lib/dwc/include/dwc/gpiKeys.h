// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gpiKeys.h, with Black's names.
#ifndef BWPORT_DWC_GPIKEYS_H
#define BWPORT_DWC_GPIKEYS_H

#include "types.h"
#include "dwc/gpi.h"

// The keys and values of the status of GameSpy's GP (gpiKeys.c), in overlay 11, kept in sorted arrays of GPIKey (see
// dwc/gp.h for the names)

void func_ov009_0218d7cc(void *element);
GPResult func_ov009_0218d7f4(GPConnection *connection);
void func_ov009_0218d840(GPConnection *connection);
GPResult func_ov009_0218d878(GPConnection *connection, DArray keys, const char *keyName, const char *keyValue);
GPResult func_ov009_0218d900(GPConnection *connection, DArray keys, const char *keyName, const char *newValue);
GPResult func_ov009_0218d9a8(GPConnection *connection, DArray keys, const char *keyName, char **keyValue);
// Writes the keys of the connection's status as "\\keys\\<count>" and the base-64 names and values
GPResult gpiSaveKeysToBuffer(GPConnection *connection, char **buffer);

#endif // BWPORT_DWC_GPIKEYS_H
