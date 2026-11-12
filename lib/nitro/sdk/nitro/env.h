// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/env.h, as it is (tools/port_lib.py)
#ifndef NITROSDK_ENV_H_
#define NITROSDK_ENV_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SDK_ARM9
    #include <nitro/env/env_system.h>
#endif

#ifdef SDK_X86
#include <nitro/env/env_system.h>
#endif

#ifdef SDK_AARCH64
#include <nitro/env/env_system.h>
#endif

#ifdef __cplusplus
}
#endif

#endif
