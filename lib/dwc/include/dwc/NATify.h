// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/NATify.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_NATIFY_H
#define BWPORT_DWC_NATIFY_H

#include "types.h"
#include "dwc/nonport.h"
#include "gs/gp.h"

typedef enum {
    no_nat,
    firewall_only,
    full_cone,
    restricted_cone,
    port_restricted_cone,
    symmetric,
    unknown,
    NUM_NAT_TYPES
} NatType;

typedef enum {
    promiscuous,
    not_promiscuous,
    port_promiscuous,
    ip_promiscuous,
    promiscuity_not_applicable,
    NUM_PROMISCUITY_TYPES
} NatPromiscuity;

typedef enum {
    unrecognized,
    private_as_public,
    consistent_port,
    incremental,
    mixed,
    NUM_MAPPING_SCHEMES
} NatMappingScheme;

typedef struct _AddressMapping {
    unsigned int privateIp;
    unsigned short privatePort;
    unsigned int publicIp;
    unsigned short publicPort;
} AddressMapping;

// What was found of the NAT. The mappings are those of the address checks 0 (the first), 1 and 2 (to the other
// servers) and 3 (to the first again)
typedef struct _NAT {
    char brand[32];
    char model[32];
    char firmware[64];
    gsi_bool ipRestricted;
    gsi_bool portRestricted;
    NatPromiscuity promiscuity;
    NatType natType;
    NatMappingScheme mappingScheme;
    AddressMapping mappings[4];
    gsi_bool qr2Compatible;
} NAT;

// pokebw2's AddressToString
// "ip:port" (or just the ip or the port) in string, or in one of two buffers that it uses in turn when it is NULL
const char *func_ov009_02198850(unsigned int ip, unsigned short port, char string[22]);
// pokebw2's OutputMapping
void func_ov009_0219898c(const AddressMapping *mapping);

#endif // BWPORT_DWC_NATIFY_H
