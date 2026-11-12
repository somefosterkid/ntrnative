// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/nhttp.h: its types and
// constants, and the prototypes of Black's named functions.
#ifndef BWPORT_DWC_NHTTP_H
#define BWPORT_DWC_NHTTP_H

#include "types.h"

typedef void *(*NHttpAllocFunc)(u32 size, int align);
typedef void (*NHttpFreeFunc)(void *ptr);

// Called with the connection, the event and the argument given to NHTTPCreateConnection. The events are the post data
// being sent (1), the buffer being full (2), the body being received (3) and the connection ending (4)
typedef int (*NHttpCallback)(int connection, int event, void *arg);

// pokebw2's NHTTPStartup
// Starts the library with the allocator it uses and its thread's priority; 0 when it started
int func_ov114_021be648(NHttpAllocFunc alloc, NHttpFreeFunc free, u32 priority);
// pokebw2's NHTTPGetError
int func_ov114_021be6ac(void);
// pokebw2's NHTTPAddHeaderField
int func_ov114_021be6bc(int connection, const char *label, const char *value);
// pokebw2's NHTTPSetBasicAuthorization
int func_ov114_021be8e8(int connection, const char *user, const char *password);
// pokebw2's NHTTPSetRootCA
int func_ov114_021be9c8(int connection, void *certificates, int count);
// pokebw2's NHTTPCreateConnection
// A request of the URL whose answer goes in the buffer, 0 when it couldn't be made
int func_ov114_021bf04c(const char *url, int method, void *buffer, u32 bufferSize, NHttpCallback callback, void *arg);

// The root certificate the requests trust, in ov009's data
extern u8 data_ov009_021a2f2c[];

// pokebw2's NHTTPSetPostDataRaw
// Sets the data that a request posts
int func_ov114_021be79c(int connection, const void *data, u32 size);

// pokebw2's NHTTPGetResultCode
// The answer's HTTP status, or -1
int func_ov114_021be880(int connection);

#endif // BWPORT_DWC_NHTTP_H
