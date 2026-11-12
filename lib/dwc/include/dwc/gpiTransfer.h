// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gpiTransfer.h, with Black's names.
#ifndef BWPORT_DWC_GPITRANSFER_H
#define BWPORT_DWC_GPITRANSFER_H

#include "types.h"
#include "dwc/gpi.h"

// What is left of the file transfers of GameSpy's GP (gpiTransfer.c) in overlay 11: every transfer that a peer asks
// for is refused (see dwc/gp.h for the names)

void gpiHandleTransferMessage(GPConnection *connection, GPIPeer *peer, int type, const char *headers,
                              const char *buffer, int len);

#endif // BWPORT_DWC_GPITRANSFER_H
