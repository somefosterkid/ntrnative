// NitroSDK's nitro/hw/ARM9/mmap_global.h, the ARM9's memory map.
// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nitro/include/nitro/hw.h, trimmed to what
// Black's C uses so far.
#ifndef BWPORT_NITRO_HW_ARM9_MMAP_GLOBAL_H
#define BWPORT_NITRO_HW_ARM9_MMAP_GLOBAL_H

// Palette memory: the BG palettes of the main and sub (DB) screens
#define HW_BG_PLTT 0x05000000
#define HW_DB_BG_PLTT 0x05000400

// The main screen's OBJ VRAM
#define HW_OBJ_VRAM 0x06400000
#define HW_OBJ_PLTT 0x05000200
#define HW_DB_OBJ_PLTT 0x05000600

// The instruction TCM's image, which ends where main memory starts, and the size of the data TCM
#define HW_ITCM_IMAGE 0x01ff8000
#define HW_MAIN_MEM 0x02000000

#endif // BWPORT_NITRO_HW_ARM9_MMAP_GLOBAL_H
