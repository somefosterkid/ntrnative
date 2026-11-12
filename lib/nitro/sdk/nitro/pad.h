// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/pad.h, as it is (tools/port_lib.py)
#ifndef NITRO_PAD_H_
#define NITRO_PAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/pad/common/pad.h>

#ifdef SDK_ARM9
#else
    #include <nitro/pad/ARM7/xyButton.h>
#endif

#ifdef __cplusplus
}
#endif

#endif
