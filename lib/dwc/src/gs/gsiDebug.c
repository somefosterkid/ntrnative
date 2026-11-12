// Black's code at ov009 0x0217d188-0x0217d3ac. GameSpy's debug output (header gsiDebug.h): the gsDebug functions
// with their hexdump helper and default callback, found by their signatures.

#include "types.h"
#include "gs/nonport.h"
#include <ctype.h>
#include "gs/string.h"

void func_ov009_0218402c(s8 *param_1, s8 *param_2);

static s32 data_ov009_021ad020[17];
static OSMutex data_ov009_021ad008;
static u32 data_ov009_021ad004;
static s32 data_ov009_021ad000;
void func_ov009_0217f0fc(OSMutex *mutex);
void func_ov009_0217f108(OSMutex *mutex);
void func_ov009_0217f114(OSMutex *mutex);
void func_ov009_0217d280(const u8 *data, char *line, u32 len);

void func_ov009_0217d188(u32 param_1, u32 param_2, u32 param_3, u32 param_4, s8 *arg4) {
    func_ov009_0218402c((s8 *) param_4, arg4);
}

void gsDebugVaList(u32 param_1, u32 param_2, u32 param_3, u32 param_4, s8 *param_5) {
    u8 *ptr;
    if (*(s32 *)((u8 *)((u8 *)(&data_ov009_021ad000)) + 4) == 0) {
        *(s32 *)((u8 *)((u8 *)(&data_ov009_021ad000)) + 4) = 1;
        func_ov009_0217f0fc((OSMutex *)((u8 *)(&data_ov009_021ad008)));
    }
    func_ov009_0217f108((OSMutex *)((u8 *)(&data_ov009_021ad008)));
    ptr = (s32 *)(param_2 + ((u8 *)(data_ov009_021ad020) + param_1 * 5));
    if (*ptr & param_3) {
        if (*(s32 (**)(u32, u32, u32, u32, s8 *))((u8 *)(&data_ov009_021ad000)) != NULL) {
            (*(s32 (**)(u32, u32, u32, u32, s8 *))((u8 *)(&data_ov009_021ad000)))(param_1, param_2, param_3, param_4,
                    param_5);
        } else {
            func_ov009_0217d188(param_1, param_2, param_3, param_4, param_5);
        }
    }
    func_ov009_0217f114((OSMutex *)((u8 *)(&data_ov009_021ad008)));
}

void gsDebugFormat(int category, int type, int level, const char *format, ...) {
    va_list args;

    va_start(args, format);
    gsDebugVaList(category, type, level, (u32)format, args);
    va_end(args);
}

void func_ov009_0217d280(const u8 *data, char *line, u32 len) {
    char *hex = line;
    char *text = line + 0x29;
    u32 odd = len & 1;

    memset(line, ' ', 0x40);
    while (len-- != 0) {
        u8 c = *data++;

        OS_SPrintf(hex, "%02X", c);
        hex += 2;
        if (isgraph(c)) {
            *text++ = c;
        } else {
            *text++ = '.';
        }
        if ((len & 1) == odd) {
            *hex++ = ' ';
        }
    }
    *hex = ' ';
    *text = '\0';
}


void gsDebugBinary(int category, int type, int level, const u8 *data, int size) {
    int left = size;
    char line[0x50];

    while (left > 0) {
        int count = left < 16 ? left : 16;

        func_ov009_0217d280(data, line, count);
        gsDebugFormat(category, type, level, "  %s\r\n", line);
        data += count;
        left -= count;
    }
}


