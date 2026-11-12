// GameSpy GT2's outgoing buffer (gt2Buffer.c): the shortening its message acknowledgments use.
#ifndef BWPORT_GS_GT2_BUFFER_H
#define BWPORT_GS_GT2_BUFFER_H

#include "types.h"
#include "gs/gt2.h"

void gti2BufferShorten(GTI2Buffer *buffer, int start, int shortenBy);
void gti2BufferWriteUShort(GTI2Buffer *buffer, u16 s);

#endif
