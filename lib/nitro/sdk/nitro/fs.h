// From libntr (MIT, https://github.com/cybervisi0n/libntr) at 1555d91: include/nitro/fs.h, as it is (tools/port_lib.py)
#ifndef NITRO_FS_H_
#define NITRO_FS_H_

#include <nitro/misc.h>
#include <nitro/types.h>

#include <nitro/fs/archive.h>
#include <nitro/fs/file.h>
#include <nitro/fs/overlay.h>
#if SDK_VERSION_MAJOR == 5
#include <nitro/fs/types.h>
#include <nitro/fs/romfat.h>
#include <nitro/fs/hook.h>
#include <nitro/fs/api.h>
#endif

#endif
