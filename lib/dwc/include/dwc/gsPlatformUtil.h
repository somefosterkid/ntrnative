// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gsPlatformUtil.h, with Black's names.
#ifndef BWPORT_DWC_GSPLATFORMUTIL_H
#define BWPORT_DWC_GSPLATFORMUTIL_H

#include "dwc/nonport.h"
#include <time.h>

// GameSpy's helpers (gsPlatformUtil.c), in overlay 11: 64-bit numbers as texts, the lookup of a host, texts, the clock,
// dates, the generator of random numbers and base 64. A base 64 text has one of three alphabets, which differ in
// their last two digits and their padding: encodingType 0 (+, / and =), 1 ([, ] and _) or 2 (-, _ and =)

// The state of a base 64 encoding that gives four digits at a time: what is left of the input, and the alphabet
typedef struct B64StreamData {
    const char *input;
    int len;
    int encodingType;
} B64StreamData;

// A lookup of a host's address
typedef struct GSIResolveHostnameInfo *GSIResolveHostnameHandle;

gsi_i64 gsiStringToInt64(const char *text);
void gsiInt64ToString(char text[33], gsi_i64 value);
// Looks up a host, at once: 0 with a lookup that func_ov009_0217f1cc reads (and frees), or -1; and frees a lookup
int func_ov009_0217f150(const char *host, GSIResolveHostnameHandle *lookup);
void func_ov009_0217f1c0(GSIResolveHostnameHandle lookup);
unsigned int func_ov009_0217f1cc(GSIResolveHostnameHandle lookup);
// A copy of a text that it allocates, or NULL; and a text made lower case in place, which it returns
char *func_ov009_0217f1e0(const char *text);
char *func_ov009_0217f21c(char *text);
// Start and end the sockets, which needs nothing on the DS
void func_ov009_0217f25c(void);
void func_ov009_0217f260(void);
// The clock in milliseconds and in microseconds, and a sleep
gsi_time current_time(void);
gsi_time current_time_hires(void);
void func_ov009_0217f2bc(gsi_time milliseconds);
// The date of some seconds from 1970 (in a buffer that the next call overwrites, NULL before 1970), and the seconds
// of a date, which it also corrects (-1 for a year before 1969 or after 2039)
struct tm *func_ov009_0217f2c8(const time_t *seconds);
time_t func_ov009_0217f478(struct tm *date);
// Seeds the generator, and gives a number from low up to (not with) high
void Util_RandSeed(unsigned long seed);
int Util_RandInt(int low, int high);
// Decodes a base 64 text of a length, writing the length of the bytes unless that is NULL; encodes bytes as a text
// that it ends; the length of the bytes of a text; and the encoding as a stream, four digits at a time, which gives
// gsi_false once the input has ended
void B64Decode(const char *input, char *output, int length, int *outputLength, int encodingType);
void B64Encode(const char *input, char *output, int length, int encodingType);
int func_ov009_0217fbc8(const char *input, int encodingType);
void B64InitEncodeStream(B64StreamData *data, const char *input, int length, int encodingType);
gsi_bool B64EncodeStream(B64StreamData *data, char output[4]);

#endif // BWPORT_DWC_GSPLATFORMUTIL_H
