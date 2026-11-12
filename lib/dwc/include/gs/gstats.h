// GameSpy's stats library (gstats.h, gbucket.h): the functions lib/dwc/src/gs/gstats's files call across files
#ifndef BWPORT_GS_GSTATS_H
#define BWPORT_GS_GSTATS_H

#include "types.h"
#include "gs/hashtable.h"

// A set of buckets: the hash table of them
typedef struct bucketset_s {
    HashTable table;
} *bucketset_t;

// gbucket.c's, not written yet: a new bucket of the type (0 int, 1 float, 2 string) in set, or the one named
void *BucketNew(bucketset_t set, char *name, s32 type, void *initialvalue);

#endif // BWPORT_GS_GSTATS_H
