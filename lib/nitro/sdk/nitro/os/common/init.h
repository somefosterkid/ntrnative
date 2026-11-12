// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/os/common/init.h, as it is (tools/port_lib.py)
#ifndef NITRO_OS_INIT_H_
#define NITRO_OS_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/misc.h>
#include <nitro/types.h>

void OS_Init(void);
#ifdef SDK_PORT
void OS_Init7(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
