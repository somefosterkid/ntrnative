#ifndef BWPORT_NITROWIFI_SSL_H
#define BWPORT_NITROWIFI_SSL_H

#include "types.h"

// NitroWiFi's nitroWiFi/ssl.h (the CPS's SSL): a root certificate a connection accepts, its distinguished name, RSA
// modulus and exponent (DWC's dwc_ca_*.c files define them)
typedef struct CPSCaInfo {
    char *dn;
    int modulus_len;
    u8 *modulus;
    int exponent_len;
    u8 *exponent;
} CPSCaInfo;

#endif
