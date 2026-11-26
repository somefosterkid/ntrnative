#ifndef POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_02266AF0_H
#define POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_02266AF0_H

#include "global.h"

#include "list_menu.h"
#include "sys_task.h"

struct UnkStruct_ov70_0225DEE8_t;
struct UnkStruct_ov70_02263344_t;

// Countdown display state of the Wi-Fi Plaza menus (ov49_02262D70 .. ov49_02262E10): 8 bytes,
// allocated alone by ov49_02262B14 (ov49_0225EF40 slot) or embedded at 0x3C of UnkStruct_ov70_02266AF0.
typedef struct UnkStruct_ov70_02266C68 {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    s16 unk_04; // seconds shown (frames / 30)
    u8 unk_06;
    u8 unk_07; // redraw flags
} UnkStruct_ov70_02266C68;

// sizeof(UnkStruct_ov70_02266C68) == 0x8

// Work of the Wi-Fi Plaza menu state machine ov49_02262028: allocated 0x44 bytes by ov49_0225EF40,
// fetched back with ov49_0225EF84.
typedef struct UnkStruct_ov70_02266AF0 {
    s16 unk_00;                     // 0x00
    u16 unk_02;                     // 0x02
    u8 unk_04;                      // 0x04 state set by ov49_02262BF8 / ov49_02262C20
    s8 unk_05;                      // 0x05
    u8 unk_06;                      // 0x06
    u8 unk_07;                      // 0x07
    u16 unk_08;                     // 0x08
    u16 unk_0A;                     // 0x0A frame counter
    s32 unk_0C;                     // 0x0C result of ov45_0222A548; sub_02037C0C sends 0x46 bytes from here
    s32 unk_10;                     // 0x10 timer (0x384 frames)
    BOOL unk_14;                    // 0x14
    ListMenuTemplate unk_18;        // 0x18
    u32 unk_38;                     // 0x38 list item count
    UnkStruct_ov70_02266C68 unk_3C; // 0x3C
} UnkStruct_ov70_02266AF0;

// sizeof(UnkStruct_ov70_02266AF0) == 0x44

#ifndef SDK_PORT
LAYOUT_CHECK(UnkStruct_ov70_02266C68_size, sizeof(UnkStruct_ov70_02266C68) == 0x8);
LAYOUT_CHECK(UnkStruct_ov70_02266AF0_size, sizeof(UnkStruct_ov70_02266AF0) == 0x44);
#endif

void ov49_02262BF8(UnkStruct_ov70_02266AF0 *work, struct UnkStruct_ov70_02263344_t * slotMgr, struct UnkStruct_ov70_0225DEE8_t *plaza, s32 msgId, s32 state);
void ov49_02262C20(UnkStruct_ov70_02266AF0 *work, struct UnkStruct_ov70_02263344_t * slotMgr, s8 state, s8 arg3);
void ov49_02262C38(UnkStruct_ov70_02266AF0 *work, struct UnkStruct_ov70_0225DEE8_t *plaza, u32 count, u32 firstMsg);
void ov49_02262CA8(UnkStruct_ov70_02266AF0 *work, void *plaza);
s32 ov49_02262CB4(UnkStruct_ov70_02266AF0 *work, struct UnkStruct_ov70_0225DEE8_t *plaza, void *arg2, struct UnkStruct_ov70_02263344_t * slotMgr, s32 arg4);
void ov49_02262D70(UnkStruct_ov70_02266C68 *countdown, struct UnkStruct_ov70_0225DEE8_t *plaza, u32 arg2, u32 arg3, s32 frames);
void ov49_02262DB8(UnkStruct_ov70_02266C68 *countdown, struct UnkStruct_ov70_0225DEE8_t *plaza);
void ov49_02262DD4(UnkStruct_ov70_02266C68 *countdown, s32 frames);
void ov49_02262DF8(UnkStruct_ov70_02266C68 *countdown, struct UnkStruct_ov70_0225DEE8_t *plaza, s32 arg2);
void ov49_02262E04(UnkStruct_ov70_02266C68 *countdown, struct UnkStruct_ov70_0225DEE8_t *plaza, s32 arg2);
void ov49_02262E10(UnkStruct_ov70_02266C68 *countdown, struct UnkStruct_ov70_0225DEE8_t *plaza, s32 arg2, s32 arg3);

#endif // POKEHEARTGOLD_OVERLAY_49_UNK_STRUCT_OV70_02266AF0_H
