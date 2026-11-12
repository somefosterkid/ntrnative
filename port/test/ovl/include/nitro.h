// port/test/ovl only: libntr's <nitro.h> as far as port/src/ovl.c uses it (the types, in libntr's layout)
#ifndef TEST_NITRO_H
#define TEST_NITRO_H
#include <stdint.h>
#include <stddef.h>
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int BOOL;
#define TRUE 1
#define FALSE 0
typedef enum { MI_PROCESSOR_ARM9 = 0, MI_PROCESSOR_ARM7 = 1 } MIProcessor;
typedef u32 FSOverlayID;
typedef void (*FSOverlayInitFunc)(void);
typedef struct { u32 offset; u32 length; } CARDRomRegion;
typedef struct {
    u32 id;
    u8 *ram_address;
    u32 ram_size;
    u32 bss_size;
    FSOverlayInitFunc *sinit_init;
    FSOverlayInitFunc *sinit_init_end;
    u32 file_id;
    u32 compressed : 24;
    u32 flag : 8;
} FSOverlayInfoHeader;
typedef struct { FSOverlayInfoHeader header; MIProcessor target; CARDRomRegion file_pos; } FSOverlayInfo;
BOOL FS_LoadOverlayInfo(FSOverlayInfo *p_ovi, MIProcessor target, FSOverlayID id);
BOOL FS_LoadOverlay(MIProcessor target, FSOverlayID id);
BOOL FS_UnloadOverlay(MIProcessor target, FSOverlayID id);
#endif
