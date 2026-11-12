// TwlSDK 5's nitro/card/types.h (libntr's include/nitro/card/types.h): the ROM headers. nitro/card.h includes it, and
// so does lib/nitro/sdk's nitro/card/rom.h (the SDK 4 headers that libraries/mb is built with), after its own
// CARDRomRegion and CARDRomHeader: NITRO_CARD_ROM_H_, its guard, leaves them out here, and CARDRomHeaderTWL is
// the one definition.
#ifndef BWPORT_NITRO_CARD_TYPES_H
#define BWPORT_NITRO_CARD_TYPES_H

#ifndef NITRO_CARD_ROM_H_
// From libntr (MIT, https://github.com/cybervisi0n/libntr), include/nitro/card/types.h: the ROM header that the card
// leaves in the shared memory, and the part of the DSi's that the access level reads
typedef struct CARDRomRegion {
    u32 offset;
    u32 length;
} CARDRomRegion;

typedef struct CARDRomHeader {
    char game_name[12];
    u32 game_code;
    u16 maker_code;
    u8 product_id;
    u8 device_type;
    u8 device_size;
    u8 reserved_A[9];
    u8 game_version;
    u8 property;

    void *main_rom_offset;
    void *main_entry_address;
    void *main_ram_address;
    u32 main_size;
    void *sub_rom_offset;
    void *sub_entry_address;
    void *sub_ram_address;
    u32 sub_size;

    CARDRomRegion fnt;
    CARDRomRegion fat;

    CARDRomRegion main_ovt;
    CARDRomRegion sub_ovt;

    u32 normal_command_control;
    u32 secure_command_control;
    u32 banner_offset;
    u16 secure_crc;
    u8 rom_param_B[2];

    void *main_autoload_done;
    void *sub_autoload_done;

    u8 rom_param_C[8];
    u32 rom_size;
    u32 header_size;
    u32 main_module_param_offset;
    u32 sub_module_param_offset;

    u16 normal_area_rom_offset;
    u16 twl_ltd_area_rom_offset;
    u8 reserved_B[0x2C];

    u8 logo_data[0x9C];
    u16 logo_crc;
    u16 header_crc;
} CARDRomHeader;
#endif // NITRO_CARD_ROM_H_

// The DSi's ROM header, 0x378 bytes
typedef struct CARDRomHeaderTWL {
    CARDRomHeader ntr;
    u8 debugger_reserved[0x20];
    u8 config1[0x34];

    struct {
        u32 : 5;
        u32 game_card_on : 1;
        u32 : 2;
        u32 game_card_nitro_mode : 1;
        u32 : 2;
        u32 photo_access_read : 1;
        u32 photo_access_write : 1;
        u32 sdmc_access_read : 1;
        u32 sdmc_access_write : 1;
        u32 backup_access_read : 1;
        u32 backup_access_write : 1;
        u32 : 0;
    } access_control;
    u8 reserved_0x1B8[8];
    u32 main_ltd_rom_offset;
    u8 reserved_0x1C4[4];
    void *main_ltd_ram_address;
    u32 main_ltd_size;
    u32 sub_ltd_rom_offset;
    u8 reserved_0x1D4[4];
    void *sub_ltd_ram_address;
    u32 sub_ltd_size;
    // The hashed areas of the DS and the DSi images, the tables of the sectors' and of the blocks' hashes, a sector's
    // size and the sectors of a block (card_hash.c)
    CARDRomRegion digest_area_ntr;
    CARDRomRegion digest_area_ltd;
    CARDRomRegion digest_tabel1;
    CARDRomRegion digest_tabel2;
    u32 digest_table1_size;
    u32 digest_table2_sectors;
    u8 config2[0xf8];
    u8 main_static_digest[0x14];
    u8 sub_static_digest[0x14];
    u8 digest_tabel2_digest[0x14];
    u8 banner_digest[0x14];
    u8 main_ltd_static_digest[0x14];
    u8 sub_ltd_static_digest[0x14];
} CARDRomHeaderTWL;

#endif // BWPORT_NITRO_CARD_TYPES_H
