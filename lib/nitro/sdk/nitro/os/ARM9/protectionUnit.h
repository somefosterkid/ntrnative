// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/os/ARM9/protectionUnit.h, as it is (tools/port_lib.py)
#ifndef NITRO_OS_PROTECTIONUNIT_H_
#define NITRO_OS_PROTECTIONUNIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/misc.h>
#include <nitro/types.h>

void OS_EnableProtectionUnit(void);
void OS_DisableProtectionUnit(void);

#ifdef __cplusplus
}
#endif

#endif
