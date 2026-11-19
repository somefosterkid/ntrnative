#include "global.h"

#include "unk_02034354_internal.h"

int sub_02034520(void);

void sub_02034524(void);

int sub_02034520(void) {
    return sizeof(CommPlayerInfo);
}

void sub_02034524(void) {
    if (_021D4130 != NULL) {
        _021D4130->dataFinishedReading = 1;
    }
}
