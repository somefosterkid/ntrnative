// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/dwc/include/dwc/gpiUtility.h, with Black's names.
#ifndef BWPORT_DWC_GPIUTILITY_H
#define BWPORT_DWC_GPIUTILITY_H

#include "dwc/gpi.h"
#include "dwc/nonport.h"

// The helpers of GameSpy's GP (gpiUtility.c), in overlay 11: reading the values of the server's "\\key\\value"
// messages, the errors and the encoding of passwords (see dwc/gp.h for the names)

// strncpy that always ends the text
void strzcpy(char *dest, const char *src, size_t len);
// Whether the server sent an "\\error\\", which it records (and passes to the error callback if asked to)
GPIBool gpiCheckForError(GPConnection *connection, const char *input, GPIBool callErrorCallback);
// Read the value after a key, up to the next character like the key's first; the first from index on, which moves
// index past the value
GPIBool func_ov009_02193378(const char *command, const char *key, int *index, char *value, int len);
GPIBool gpiValueForKey(const char *command, const char *key, char *value, int len);
// Whether a socket connected: state is 0 (not yet), 3 (connected) or 4 (refused)
GPResult func_ov009_02193488(GPConnection *connection, SOCKET sock, int *state);
GPResult gpiReadKeyAndValue(GPConnection *connection, const char *buffer, int *index, char key[512], char value[512]);
void gpiSetError(GPConnection *connection, GPErrorCode errorCode, const char *errorString);
void gpiSetErrorString(GPConnection *connection, const char *errorString);
// Scrambles a text with the generator seeded with "gsp" and writes it in base 64
void func_ov009_02193748(const char *unencodedString, char *encodedString);

#endif // BWPORT_DWC_GPIUTILITY_H
