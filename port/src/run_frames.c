// tools/port_run.py's frame counter: the address of the V-blank count (OS_GetVBlankCount's word, which libntr's
// frame adds 1 to after the V-blank handler). The harness reads the count through /proc/<pid>/mem while the game runs,
// with no breakpoint and no code in the frame. Nothing in the game uses it.

const volatile u32 *const bwport_vblank_count = (const volatile u32 *)HW_VBLANK_COUNT_BUF;
