#include "global.h"

#include "error_handling.h"
#include "text.h"

int ov52_021E92E8(int printerID);

int ov52_021E92E8(int printerID) {
    return !TextPrinterCheckActive(printerID);
}

void ov52_021E9300(u8 *signature1BppOut, u8 *signature4Bpp);

void ov52_021E9300(u8 *signature1BppOut, u8 *signature4Bpp) {
    int bytesWritten = 0;
    int bitIndex = 0;
    *signature1BppOut = 0;

    for (int i = 0; i < 24 * 8 * (8 * (8 / 2)); i++) {
        int pixel = signature4Bpp[i] & 0xF;

        if (pixel == 1) {
            *signature1BppOut |= (1 << bitIndex);
        }

        bitIndex++;
        pixel = signature4Bpp[i] >> 4;

        if (pixel == 1) {
            *signature1BppOut |= (1 << bitIndex);
        }

        bitIndex++;

        if (bitIndex == 8) {
            bytesWritten++;
            signature1BppOut++;
            *signature1BppOut = 0;
            bitIndex = 0;
        }

        if (bytesWritten >= 24 * 8 * (8 * (8 / 2))) {
            ((0) ? (void)0 : GF_AssertFail());
        }
    }
}
