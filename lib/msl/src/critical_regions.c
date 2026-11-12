// MSL's critical_regions.c for NITRO: the mutexes of critical_regions.h's inlines, which NitroSDK's MSL start-up
// initializes. Its functions are inline, so only its .bss is in Black. The file's name is pokediamond's object's
// (MSL_ARM_critical_regions_NITRO.o).
#include <critical_regions.h>

int __cs_ref[num_critical_regions];
int __cs_id[num_critical_regions];
OSMutex __cs[num_critical_regions];
