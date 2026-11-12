// MSL's scanf.c: only __StringRead, the reader of strtoul.c's and strtold.c's functions, is in Black (khdays-decomp's
// CharCursor_Control, CC0-1.0, has the same code). The file's name is pokediamond's object's (MSL_Common_scanf.o).
#include <stdio.h>

int __StringRead(void *isc, int ch, int Action) {
    char RetVal;
    __InStrCtrl *Iscp = (__InStrCtrl *) isc;

    switch (Action) {
        case __GetAChar:
            RetVal = *(Iscp->NextChar);
            if (RetVal == '\0') {
                Iscp->NullCharDetected = 1;
                return EOF;
            } else {
                Iscp->NextChar++;
                return (unsigned char) RetVal;
            }
        case __UngetAChar:
            if (Iscp->NullCharDetected == 0) {
                Iscp->NextChar--;
            } else {
                Iscp->NullCharDetected = 0;
            }
            return ch;
        case __TestForError:
            return Iscp->NullCharDetected;
    }
    return 0;
}
