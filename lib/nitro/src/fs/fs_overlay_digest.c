// The digests that fs_overlay.c's FS_StartOverlay checks an overlay against when the game was not booted from the
// ROM: Black's main .data 0x020a8bb8-0x020a9e64. TwlSDK's build makes no object of them: its linker script
// (ARM9-TS.lcf.template, libntr's include/twl/specfiles) leaves room for them at the end of ARM9 main's .data, under
// these labels, and its tools fill them in after the link, the HMAC-SHA1 of each overlay as the ROM stores it. Here
// the room is this file's, with the base ROM's digests (config/b_us/assets.tsv). From pokebw2 (GPL-3.0,
// https://github.com/fuddlesworth/pokebw2), lib/nitro/src/fs/fs_overlay_digest.c; the file name is pokebw2's.
#include "nitro/fs.h"

// The room is in .data, where the linker script leaves it, even where it is zeros
#pragma explicit_zero_data on

// The digest of the overlay table: an HMAC-SHA1 that skips words 6 and 14 of each block, the file IDs of its entries
u8 SDK_OVERLAYTABLE_DIGEST[20] = {
#include "asset/overlay_table_digest.inc"
};

// The digest of the table of the LTD overlays, which Black has none of: zeros
u8 SDK_LTDOVERLAYTABLE_DIGEST[20] = { 0 };

// One digest for each of Black's 237 overlays, in the order of their IDs. SDK_OVERLAY_DIGEST_END is a label of the
// linker script rather than an object: it marks the padding that follows, to the 32-byte boundary where ARM9 main's
// .bss starts
FSOverlayDigest SDK_OVERLAY_DIGEST[237] = {
#include "asset/overlay_digests.inc"
};
