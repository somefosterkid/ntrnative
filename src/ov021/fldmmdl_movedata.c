// Black's fldmmdl_movedata.c, as pokebw2 names it: the actors' tables of functions, by move code
// (data_ov021_021d5e04, which fldmmdl.c reads) and by action command, on the grid (ACTOR_ACTION_COMMAND_TABLES) and on
// the rails (ACTOR_ACTION_COMMAND_TABLES_RAIL), and the rows of the commands that differ only by direction
// (ACMD_DIR_LUT). It holds only data, one .rodata run sorted by size after fldmmdl_movecode_pair.c's

#include "types.h"
#include "field/field_actor.h"
#include "field/field_actor_internal.h"
#include "struct_decls.h"

void func_ov021_0219c0e8(FieldActor *param_1);
void func_ov021_0219c118(FieldActor *param_1);
void func_ov010_0216e46c(FieldActor *actor);
void func_ov010_0216e470(FieldActor *actor);
void func_ov021_0219a270(FieldActor *param_1);
void func_ov021_0219a2b8(FieldActor *param_1);
void func_ov021_0219c100(FieldActor *param_1);
void func_ov021_021944bc(FieldActor *param_1);
void func_ov021_021944d8(FieldActor *actor);
void func_ov021_021944c8(FieldActor *param_1);
void func_ov021_0219a114(FieldActor *param_1);
void func_ov021_0219a150(FieldActor *param_1);
void func_ov021_0219a24c(FieldActor *actor);
void func_ov021_0219a120(FieldActor *param_1);
void func_ov021_0219e864(FieldActor *param_1);
void func_ov021_0219e8e8(FieldActor *param_1);
void func_ov021_0219e8f0(FieldActor *param_1);
void func_ov021_0219e854(FieldActor *param_1);
void func_ov021_0219e87c(FieldActor *param_1);
void func_ov021_0219a5c8(FieldActor *param_1);
void func_ov021_0219a5e0(FieldActor *param_1);
void func_ov021_0219e894(FieldActor *param_1);
void func_ov021_0219e8a0(FieldActor *param_1);
void func_ov021_0219aa68(FieldActor *param_1);
void func_ov021_0219ab68(FieldActor *param_1);
void func_ov021_0219e8ac(FieldActor *param_1);
void func_ov021_0219a598(FieldActor *param_1);
void func_ov021_0219a54c(FieldActor *param_1);
void func_ov021_0219e888(FieldActor *param_1);
void func_ov021_0219a574(FieldActor *param_1);
void func_ov021_0219e870(FieldActor *param_1);
void func_ov021_0219aa88(FieldActor *param_1);
void func_ov021_0219e828(FieldActor *param_1);
void func_ov021_0219e834(FieldActor *param_1);
void func_ov021_0219e844(FieldActor *param_1);
void func_ov021_0219a0f0(FieldActor *param_1);
void func_ov021_0219c10c(FieldActor *param_1);
void func_ov021_0219a260(FieldActor *param_1);
void func_ov021_0219c0f4(FieldActor *param_1);
void func_ov010_0216e464(FieldActor *actor);
void func_ov010_0216e468(FieldActor *actor);
void func_ov021_0219bfd0(FieldActor *param_1);
void func_ov021_0219bfdc(FieldActor *param_1);
void func_ov021_0219e8b8(FieldActor *param_1);
void func_ov021_0219aa98(FieldActor *param_1);
void func_ov021_0219aaa8(FieldActor *param_1);
void func_ov021_0219e8c4(FieldActor *param_1);
void func_ov021_0219e8d0(FieldActor *param_1);
void func_ov021_0219aae8(FieldActor *param_1);
void func_ov021_0219e8dc(FieldActor *param_1);
void func_ov021_0219e8f8(FieldActor *param_1);
void func_ov021_0219e910(FieldActor *param_1);
void func_ov021_0219e918(FieldActor *param_1);
void func_ov021_0219ab18(FieldActor *param_1);
void func_ov021_0219ab28(FieldActor *param_1);
void func_ov021_0219e904(FieldActor *param_1);
void func_ov021_0219ad48(FieldActor *param_1);
void func_ov021_0219adc8(FieldActor *param_1);
void func_ov021_0219e920(FieldActor *param_1);
void func_ov021_0219e928(FieldActor *param_1);
void func_ov021_0219e930(FieldActor *param_1);
void func_ov021_0219ad68(FieldActor *param_1);
void func_ov021_0219ad78(FieldActor *param_1);
void func_ov021_0219ad88(FieldActor *param_1);
void func_ov021_0219ad98(FieldActor *param_1);
void func_ov021_0219ad58(FieldActor *param_1);
void func_ov021_0219ada8(FieldActor *param_1);
void func_ov021_0219ab58(FieldActor *param_1);
void func_ov021_0219ab48(FieldActor *param_1);
void func_ov021_0219ab38(FieldActor *param_1);
void func_ov021_0219adb8(FieldActor *param_1);
void func_ov021_0219b8bc(FieldActor *param_1);
void func_ov021_0219b8e8(FieldActor *param_1);
void func_ov021_0219b920(FieldActor *actor);
void func_ov021_0219ab08(FieldActor *param_1);
void func_ov021_0219aaf8(FieldActor *param_1);
void func_ov021_0219bae0(FieldActor *param_1);
void func_ov021_0219bb08(FieldActor *param_1);
void func_ov021_0219bb38(FieldActor *actor);
void func_ov021_0219bb3c(FieldActor *param_1);
void func_ov021_0219aad8(FieldActor *param_1);
void func_ov021_0219aac8(FieldActor *param_1);
void func_ov021_0219aab8(FieldActor *param_1);
void func_ov021_0219be0c(FieldActor *param_1);
void func_ov021_0219be3c(FieldActor *param_1);
void func_ov021_0219be60(FieldActor *param_1);
void func_ov021_0219be70(FieldActor *param_1);
void func_ov021_0219be18(FieldActor *param_1);
void func_ov021_0219be24(FieldActor *param_1);
void func_ov021_0219aa78(FieldActor *param_1);
void func_ov021_0219be30(FieldActor *param_1);
void func_ov021_0219a87c(FieldActor *param_1);
void func_ov021_0219a89c(FieldActor *param_1);
void func_ov021_0219a750(FieldActor *param_1);
void func_ov021_0219a75c(FieldActor *param_1);
void func_ov021_0219a5d4(FieldActor *param_1);
void func_ov021_0219bf7c(FieldActor *param_1);
void func_ov021_0219bf88(FieldActor *param_1);
void func_ov021_0219a58c(FieldActor *param_1);
void func_ov021_0219a580(FieldActor *param_1);
void func_ov021_0219bf94(FieldActor *param_1);
void func_ov021_0219a144(FieldActor *param_1);
void func_ov021_0219a138(FieldActor *param_1);
void func_ov021_0219a12c(FieldActor *param_1);
void func_ov021_0219bfa0(FieldActor *param_1);
void func_ov021_0219bfac(FieldActor *param_1);
void func_ov021_0219a108(FieldActor *param_1);
void func_ov021_0219a0fc(FieldActor *param_1);
void func_ov021_0219bfb8(FieldActor *param_1);
void func_ov021_0219a0e4(FieldActor *param_1);
void func_ov021_0219a0d8(FieldActor *param_1);
void func_ov021_0219a280(FieldActor *param_1);
void func_ov021_0219bfc4(FieldActor *param_1);
void func_ov021_0219a0cc(FieldActor *param_1);
void func_ov021_0219a250(FieldActor *param_1);

static const u32 data_ov021_021d55fc[] = { 0x85, 0x86, 0x87, 0x88 };
static const u32 data_ov021_021d553c[] = { 0x89, 0x8a, 0x8b, 0x8c };
static const u32 data_ov021_021d552c[] = { 0x18, 0x19, 0x1a, 0x1b };
static const u32 data_ov021_021d566c[] = { 0x81, 0x82, 0x83, 0x84 };
static const u32 data_ov021_021d560c[] = { 0x75, 0x76, 0x77, 0x78 };
static const u32 data_ov021_021d55ec[] = { 0x14, 0x15, 0x16, 0x17 };
static const u32 data_ov021_021d567c[] = { 0x69, 0x6a, 0x6b, 0x6c };
static const u32 data_ov021_021d556c[] = { 0x20, 0x21, 0x22, 0x23 };
static const u32 data_ov021_021d561c[] = { 0xa3, 0xa4, 0xa5, 0xa6 };
static const u32 data_ov021_021d548c[] = { 0x10, 0x11, 0x12, 0x13 };
static const u32 data_ov021_021d568c[] = { 0x24, 0x25, 0x26, 0x27 };
static const u32 data_ov021_021d54bc[] = { 0x60, 0x61, 0x62, 0x63 };
static const u32 data_ov021_021d551c[] = { 0xc, 0xd, 0xe, 0xf };
static const u32 data_ov021_021d569c[] = { 0x28, 0x29, 0x2a, 0x2b };
static const u32 data_ov021_021d557c[] = { 0x2c, 0x2d, 0x2e, 0x2f };
static const u32 data_ov021_021d54ec[] = { 0x71, 0x72, 0x73, 0x74 };
static const u32 data_ov021_021d55dc[] = { 0x6d, 0x6e, 0x6f, 0x70 };
static const u32 data_ov021_021d54cc[] = { 0x8, 0x9, 0xa, 0xb };
static const u32 data_ov021_021d56ac[] = { 0x9b, 0x9c, 0x9d, 0x9e };
static const u32 data_ov021_021d562c[] = { 0x5e, 0x5f, 0x5e, 0x5f };
static const u32 data_ov021_021d55cc[] = { 0x4, 0x5, 0x6, 0x7 };
static const u32 data_ov021_021d56bc[] = { 0x34, 0x35, 0x36, 0x37 };
static const u32 data_ov021_021d558c[] = { 0xa7, 0xa8, 0xa9, 0xaa };
static const u32 data_ov021_021d563c[] = { 0x30, 0x31, 0x32, 0x33 };
static const u32 data_ov021_021d554c[] = { 0x0, 0x1, 0x2, 0x3 };
static const u32 data_ov021_021d56cc[] = { 0x91, 0x92, 0x93, 0x94 };
static const u32 data_ov021_021d550c[] = { 0x7d, 0x7e, 0x7f, 0x80 };
static const u32 data_ov021_021d55bc[] = { 0x5c, 0x5d, 0x5c, 0x5d };
static const u32 data_ov021_021d549c[] = { 0x4c, 0x4d, 0x4e, 0x4f };
static const u32 data_ov021_021d559c[] = { 0x95, 0x96, 0x97, 0x98 };
static const u32 data_ov021_021d54fc[] = { 0x50, 0x51, 0x52, 0x53 };
static const u32 data_ov021_021d54ac[] = { 0x54, 0x55, 0x56, 0x57 };
static const u32 data_ov021_021d564c[] = { 0x38, 0x39, 0x3a, 0x3b };
static const u32 data_ov021_021d565c[] = { 0x58, 0x59, 0x5a, 0x5b };
static const u32 data_ov021_021d54dc[] = { 0x1c, 0x1d, 0x1e, 0x1f };
static const u32 data_ov021_021d55ac[] = { 0x8d, 0x8e, 0x8f, 0x90 };
static const u32 data_ov021_021d555c[] = { 0x79, 0x7a, 0x7b, 0x7c };

static const FieldActorMoveCode data_ov021_021d5ab0 = { 3, func_ov021_0219adb8, func_ov021_0219adc8, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5a9c = { 3, func_ov021_0219ab38, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5a88 = { 3, func_ov021_0219ab48, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5a74 = { 3, func_ov021_0219ab58, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5a60 = { 3, func_ov021_0219ada8, func_ov021_0219adc8, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d59fc = { 3, func_ov021_0219ad78, func_ov021_0219adc8, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5a10 = { 3, func_ov021_0219ad88, func_ov021_0219adc8, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5a38 = { 3, func_ov021_0219ad98, func_ov021_0219adc8, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5a4c = { 3, func_ov021_0219ad58, func_ov021_0219adc8, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5b14 = { 3, func_ov021_0219aad8, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5b28 = { 3, func_ov021_0219aac8, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5b50 = { 51, func_ov021_0219be0c, func_ov021_0219be3c, func_ov021_0219be60, func_ov021_0219be70 };
static const FieldActorMoveCode data_ov021_021d5b64 = { 52, func_ov021_0219be18, func_ov021_0219be3c, func_ov021_0219be60, func_ov021_0219be70 };
static const FieldActorMoveCode data_ov021_021d5ba0 = { 54, func_ov021_0219be30, func_ov021_0219be3c, func_ov021_0219be60, func_ov021_0219be70 };
static const FieldActorMoveCode data_ov021_021d5bb4 = { 3, func_ov021_0219a87c, func_ov021_0219a89c, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5bdc = { 3, func_ov021_0219a5d4, func_ov021_0219a5e0, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5bf0 = { 55, func_ov021_0219bf7c, func_ov021_0219bfdc, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5c40 = { 57, func_ov021_0219bf94, func_ov021_0219bfdc, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5c54 = { 3, func_ov021_0219a144, func_ov021_0219a150, func_ov021_0219a24c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5c7c = { 3, func_ov021_0219a12c, func_ov021_0219a150, func_ov021_0219a24c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5c90 = { 58, func_ov021_0219bfa0, func_ov021_0219bfdc, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5ccc = { 3, func_ov021_0219a0fc, func_ov021_0219a150, func_ov021_0219a24c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5ce0 = { 60, func_ov021_0219bfb8, func_ov021_0219bfdc, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5d08 = { 3, func_ov021_0219a0d8, func_ov021_0219a150, func_ov021_0219a24c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5d1c = { 3, func_ov021_0219a280, func_ov021_0219a2b8, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d58bc = { 62, func_ov021_0219bfd0, func_ov021_0219bfdc, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d58a8 = { 1, func_ov010_0216e464, func_ov010_0216e468, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5894 = { 64, func_ov021_0219c0f4, func_ov021_0219c118, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5880 = { 3, func_ov021_0219a260, func_ov021_0219a2b8, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d586c = { 66, func_ov021_0219c10c, func_ov021_0219c118, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5858 = { 3, func_ov021_0219a0f0, func_ov021_0219a150, func_ov021_0219a24c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5844 = { 69, func_ov021_0219e828, func_ov021_0219e834, func_ov021_0219e844, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d5830 = { 3, func_ov021_0219aa88, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d581c = { 71, func_ov021_0219e870, func_ov021_0219e8e8, func_ov021_0219e8f0, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d5808 = { 3, func_ov021_0219a574, func_ov021_0219a54c, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d57f4 = { 73, func_ov021_0219e888, func_ov021_0219e8e8, func_ov021_0219e8f0, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d57e0 = { 3, func_ov021_0219a598, func_ov021_0219a54c, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d57a4 = { 75, func_ov021_0219e8a0, func_ov021_0219e8e8, func_ov021_0219e8f0, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d57b8 = { 3, func_ov021_0219aa68, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d58d0 = { 77, func_ov021_0219e8b8, func_ov021_0219e8e8, func_ov021_0219e8f0, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d58e4 = { 3, func_ov021_0219aa98, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5934 = { 3, func_ov021_0219aae8, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5948 = { 80, func_ov021_0219e8dc, func_ov021_0219e8e8, func_ov021_0219e8f0, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d5970 = { 3, func_ov021_0219ab18, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5984 = { 3, func_ov021_0219ab28, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d59c0 = { 3, func_ov021_0219ad48, func_ov021_0219adc8, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d59d4 = { 84, func_ov021_0219e920, func_ov021_0219e928, func_ov021_0219e930, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d5a24 = { 0, func_ov010_0216e464, func_ov010_0216e468, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5ac4 = { 3, func_ov021_0219b8bc, func_ov021_0219b8e8, func_ov021_0219b920, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5b3c = { 3, func_ov021_0219aab8, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5b78 = { 53, func_ov021_0219be24, func_ov021_0219be3c, func_ov021_0219be60, func_ov021_0219be70 };
static const FieldActorMoveCode data_ov021_021d5bc8 = { 3, func_ov021_0219a750, func_ov021_0219a75c, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5c04 = { 56, func_ov021_0219bf88, func_ov021_0219bfdc, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5c68 = { 3, func_ov021_0219a138, func_ov021_0219a150, func_ov021_0219a24c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5ca4 = { 59, func_ov021_0219bfac, func_ov021_0219bfdc, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5cf4 = { 3, func_ov021_0219a0e4, func_ov021_0219a150, func_ov021_0219a24c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5d30 = { 61, func_ov021_0219bfc4, func_ov021_0219bfdc, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d56dc = { 63, func_ov021_0219c0e8, func_ov021_0219c118, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d56f0 = { 3, func_ov021_0219a270, func_ov021_0219a2b8, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5718 = { 68, func_ov021_021944bc, func_ov021_021944d8, func_ov021_021944c8, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d572c = { 3, func_ov021_0219a114, func_ov021_0219a150, func_ov021_0219a24c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5768 = { 72, func_ov021_0219e87c, func_ov021_0219e8e8, func_ov021_0219e8f0, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d577c = { 3, func_ov021_0219a5c8, func_ov021_0219a5e0, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d57cc = { 76, func_ov021_0219e8ac, func_ov021_0219e8e8, func_ov021_0219e8f0, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d58f8 = { 3, func_ov021_0219aaa8, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d595c = { 81, func_ov021_0219e8f8, func_ov021_0219e910, func_ov021_0219e918, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d5998 = { 82, func_ov021_0219e904, func_ov021_0219e910, func_ov021_0219e918, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d59e8 = { 3, func_ov021_0219ad68, func_ov021_0219adc8, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5ad8 = { 3, func_ov021_0219ab08, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5b8c = { 3, func_ov021_0219aa78, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5c18 = { 3, func_ov021_0219a58c, func_ov021_0219a54c, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5cb8 = { 3, func_ov021_0219a108, func_ov021_0219a150, func_ov021_0219a24c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5d44 = { 2, func_ov021_0219a0cc, func_ov021_0219a150, func_ov021_0219a24c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5704 = { 65, func_ov021_0219c100, func_ov021_0219c118, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5740 = { 3, func_ov021_0219a120, func_ov021_0219a150, func_ov021_0219a24c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5790 = { 74, func_ov021_0219e894, func_ov021_0219e8e8, func_ov021_0219e8f0, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d590c = { 78, func_ov021_0219e8c4, func_ov021_0219e8e8, func_ov021_0219e8f0, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d59ac = { 83, func_ov021_0219e904, func_ov021_0219e910, func_ov021_0219e918, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d5aec = { 3, func_ov021_0219aaf8, func_ov021_0219ab68, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5c2c = { 3, func_ov021_0219a580, func_ov021_0219a54c, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5d58 = { 3, func_ov021_0219a250, func_ov021_0219a2b8, func_ov010_0216e46c, func_ov010_0216e470 };
static const FieldActorMoveCode data_ov021_021d5754 = { 70, func_ov021_0219e864, func_ov021_0219e8e8, func_ov021_0219e8f0, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d5920 = { 79, func_ov021_0219e8d0, func_ov021_0219e8e8, func_ov021_0219e8f0, func_ov021_0219e854 };
static const FieldActorMoveCode data_ov021_021d5b00 = { 3, func_ov021_0219bae0, func_ov021_0219bb08, func_ov021_0219bb38, func_ov021_0219bb3c };

const u32 *const ACMD_DIR_LUT[] = {
    data_ov021_021d554c,
    data_ov021_021d551c,
    data_ov021_021d565c,
    data_ov021_021d55ec,
    data_ov021_021d548c,
    data_ov021_021d558c,
    data_ov021_021d561c,
    data_ov021_021d55cc,
    data_ov021_021d54cc,
    data_ov021_021d552c,
    data_ov021_021d54dc,
    data_ov021_021d556c,
    data_ov021_021d568c,
    data_ov021_021d569c,
    data_ov021_021d557c,
    data_ov021_021d563c,
    data_ov021_021d56bc,
    data_ov021_021d564c,
    data_ov021_021d549c,
    data_ov021_021d54fc,
    data_ov021_021d54ac,
    data_ov021_021d55bc,
    data_ov021_021d562c,
    data_ov021_021d54bc,
    data_ov021_021d567c,
    data_ov021_021d55dc,
    data_ov021_021d560c,
    data_ov021_021d54ec,
    data_ov021_021d555c,
    data_ov021_021d550c,
    data_ov021_021d566c,
    data_ov021_021d55fc,
    data_ov021_021d553c,
    data_ov021_021d55ac,
    data_ov021_021d56cc,
    data_ov021_021d559c,
    data_ov021_021d56ac,
    NULL,
};

const FieldActorMoveCode *const data_ov021_021d5e04[] = {
    &data_ov021_021d5a24,
    &data_ov021_021d58a8,
    &data_ov021_021d5d44,
    &data_ov021_021d5d58,
    &data_ov021_021d5880,
    &data_ov021_021d56f0,
    &data_ov021_021d5d08,
    &data_ov021_021d5cf4,
    &data_ov021_021d5858,
    &data_ov021_021d5ccc,
    &data_ov021_021d5cb8,
    &data_ov021_021d572c,
    &data_ov021_021d5740,
    &data_ov021_021d5c7c,
    &data_ov021_021d5808,
    &data_ov021_021d5c2c,
    &data_ov021_021d5c18,
    &data_ov021_021d57e0,
    &data_ov021_021d577c,
    &data_ov021_021d5bdc,
    &data_ov021_021d5bb4,
    &data_ov021_021d57b8,
    &data_ov021_021d5b8c,
    &data_ov021_021d5830,
    &data_ov021_021d58e4,
    &data_ov021_021d58f8,
    &data_ov021_021d5b3c,
    &data_ov021_021d5b28,
    &data_ov021_021d5b14,
    &data_ov021_021d5934,
    &data_ov021_021d5aec,
    &data_ov021_021d5ad8,
    &data_ov021_021d5970,
    &data_ov021_021d5984,
    &data_ov021_021d5a9c,
    &data_ov021_021d5a88,
    &data_ov021_021d5a74,
    &data_ov021_021d59c0,
    &data_ov021_021d5a4c,
    &data_ov021_021d59e8,
    &data_ov021_021d59fc,
    &data_ov021_021d5a10,
    &data_ov021_021d5a38,
    &data_ov021_021d5a60,
    &data_ov021_021d5ab0,
    &data_ov021_021d5c68,
    &data_ov021_021d5c54,
    NULL,
    &data_ov021_021d5ac4,
    &data_ov021_021d5bc8,
    &data_ov021_021d5b00,
    &data_ov021_021d5b50,
    &data_ov021_021d5b64,
    &data_ov021_021d5b78,
    &data_ov021_021d5ba0,
    &data_ov021_021d5bf0,
    &data_ov021_021d5c04,
    &data_ov021_021d5c40,
    &data_ov021_021d5c90,
    &data_ov021_021d5ca4,
    &data_ov021_021d5ce0,
    &data_ov021_021d5d30,
    &data_ov021_021d58bc,
    &data_ov021_021d56dc,
    &data_ov021_021d5894,
    &data_ov021_021d5704,
    &data_ov021_021d586c,
    &data_ov021_021d5d1c,
    &data_ov021_021d5718,
    &data_ov021_021d5844,
    &data_ov021_021d5754,
    &data_ov021_021d581c,
    &data_ov021_021d5768,
    &data_ov021_021d57f4,
    &data_ov021_021d5790,
    &data_ov021_021d57a4,
    &data_ov021_021d57cc,
    &data_ov021_021d58d0,
    &data_ov021_021d590c,
    &data_ov021_021d5920,
    &data_ov021_021d5948,
    &data_ov021_021d595c,
    &data_ov021_021d5998,
    &data_ov021_021d59ac,
    &data_ov021_021d59d4,
};

const FieldActorFunc *const ACTOR_ACTION_COMMAND_TABLES[] = {
    data_ov021_021d4180,
    data_ov021_021d41c0,
    data_ov021_021d4170,
    data_ov021_021d4198,
    data_ov021_021d4a14,
    data_ov021_021d4a08,
    data_ov021_021d49fc,
    data_ov021_021d49f0,
    data_ov021_021d49e4,
    data_ov021_021d49d8,
    data_ov021_021d49cc,
    data_ov021_021d49c0,
    data_ov021_021d49b4,
    data_ov021_021d49a8,
    data_ov021_021d499c,
    data_ov021_021d4990,
    data_ov021_021d4984,
    data_ov021_021d4978,
    data_ov021_021d496c,
    data_ov021_021d4960,
    data_ov021_021d4954,
    data_ov021_021d4948,
    data_ov021_021d493c,
    data_ov021_021d4930,
    data_ov021_021d48f4,
    data_ov021_021d48e8,
    data_ov021_021d48dc,
    data_ov021_021d48d0,
    data_ov021_021d48c4,
    data_ov021_021d48b8,
    data_ov021_021d48ac,
    data_ov021_021d48a0,
    data_ov021_021d4894,
    data_ov021_021d4888,
    data_ov021_021d487c,
    data_ov021_021d4870,
    data_ov021_021d4864,
    data_ov021_021d4858,
    data_ov021_021d484c,
    data_ov021_021d4840,
    data_ov021_021d4834,
    data_ov021_021d4828,
    data_ov021_021d481c,
    data_ov021_021d4810,
    data_ov021_021d4804,
    data_ov021_021d47f8,
    data_ov021_021d47ec,
    data_ov021_021d47e0,
    data_ov021_021d47d4,
    data_ov021_021d4468,
    data_ov021_021d47bc,
    data_ov021_021d47b0,
    data_ov021_021d47a4,
    data_ov021_021d4798,
    data_ov021_021d478c,
    data_ov021_021d4780,
    data_ov021_021d4774,
    data_ov021_021d4768,
    data_ov021_021d475c,
    data_ov021_021d4750,
    data_ov021_021d472c,
    data_ov021_021d47c8,
    data_ov021_021d46fc,
    data_ov021_021d46f0,
    data_ov021_021d46e4,
    data_ov021_021d46d8,
    data_ov021_021d46cc,
    data_ov021_021d46c0,
    data_ov021_021d43d8,
    data_ov021_021d41b0,
    data_ov021_021d4160,
    data_ov021_021d4190,
    data_ov021_021d4168,
    data_ov021_021d41a0,
    data_ov021_021d4188,
    data_ov021_021d4660,
    data_ov021_021d4624,
    data_ov021_021d4618,
    data_ov021_021d460c,
    data_ov021_021d4600,
    data_ov021_021d45c4,
    data_ov021_021d45b8,
    data_ov021_021d45ac,
    data_ov021_021d45a0,
    data_ov021_021d4924,
    data_ov021_021d4918,
    data_ov021_021d490c,
    data_ov021_021d4900,
    data_ov021_021d4594,
    data_ov021_021d4588,
    data_ov021_021d457c,
    data_ov021_021d4570,
    data_ov021_021d4744,
    data_ov021_021d4738,
    data_ov021_021d4708,
    data_ov021_021d4714,
    data_ov021_021d4564,
    data_ov021_021d4558,
    data_ov021_021d454c,
    data_ov021_021d4540,
    data_ov021_021d4504,
    data_ov021_021d44a4,
    data_ov021_021d4498,
    data_ov021_021d4654,
    data_ov021_021d448c,
    data_ov021_021d4474,
    data_ov021_021d4414,
    data_ov021_021d4438,
    data_ov021_021d4444,
    data_ov021_021d445c,
    data_ov021_021d4678,
    data_ov021_021d469c,
    data_ov021_021d46a8,
    data_ov021_021d4720,
    data_ov021_021d4a20,
    data_ov021_021d43fc,
    data_ov021_021d43f0,
    data_ov021_021d4384,
    data_ov021_021d424c,
    data_ov021_021d436c,
    data_ov021_021d4360,
    data_ov021_021d4354,
    data_ov021_021d4348,
    data_ov021_021d433c,
    data_ov021_021d4330,
    data_ov021_021d4324,
    data_ov021_021d4318,
    data_ov021_021d430c,
    data_ov021_021d4300,
    data_ov021_021d42f4,
    data_ov021_021d42e8,
    data_ov021_021d42c4,
    data_ov021_021d42d0,
    data_ov021_021d4408,
    data_ov021_021d4420,
    data_ov021_021d4450,
    data_ov021_021d4684,
    data_ov021_021d46b4,
    data_ov021_021d4a2c,
    data_ov021_021d4288,
    data_ov021_021d41c8,
    data_ov021_021d4270,
    data_ov021_021d43b4,
    data_ov021_021d4258,
    data_ov021_021d41ec,
    data_ov021_021d43e4,
    data_ov021_021d427c,
    data_ov021_021d43cc,
    data_ov021_021d43c0,
    data_ov021_021d466c,
    data_ov021_021d4264,
    data_ov021_021d439c,
    data_ov021_021d4390,
    data_ov021_021d4240,
    data_ov021_021d4234,
    data_ov021_021d421c,
    data_ov021_021d4228,
    data_ov021_021d42a0,
    data_ov021_021d42ac,
    data_ov021_021d43a8,
    data_ov021_021d463c,
    data_ov021_021d4630,
    data_ov021_021d4480,
    data_ov021_021d45f4,
    data_ov021_021d4378,
    data_ov021_021d45dc,
    data_ov021_021d45d0,
    data_ov021_021d4534,
    data_ov021_021d4528,
    data_ov021_021d451c,
    data_ov021_021d4510,
    data_ov021_021d42dc,
    data_ov021_021d41a8,
    data_ov021_021d4690,
    data_ov021_021d4a38,
    data_ov021_021d41b8,
    data_ov021_021d41d4,
    data_ov021_021d4294,
    data_ov021_021d42b8,
    data_ov021_021d442c,
    data_ov021_021d4178,
    data_ov021_021d44f8,
    data_ov021_021d44ec,
    data_ov021_021d44e0,
    data_ov021_021d44bc,
    data_ov021_021d44b0,
    data_ov021_021d44d4,
    data_ov021_021d44c8,
    data_ov021_021d41f8,
};

const FieldActorFunc *const ACTOR_ACTION_COMMAND_TABLES_RAIL[] = {
    data_ov021_021d8104, data_ov021_021d810c, data_ov021_021d80fc, data_ov021_021d80f4, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8308, data_ov021_021d82fc,
    data_ov021_021d82f0, data_ov021_021d82e4, data_ov021_021d82d8, data_ov021_021d82cc, data_ov021_021d82c0,
    data_ov021_021d82b4, data_ov021_021d82a8, data_ov021_021d829c, data_ov021_021d8290, data_ov021_021d8284,
    data_ov021_021d8278, data_ov021_021d8248, data_ov021_021d8254, data_ov021_021d826c, data_ov021_021d8134,
    data_ov021_021d8350, data_ov021_021d8200, data_ov021_021d81f4, data_ov021_021d81e8, data_ov021_021d81dc,
    data_ov021_021d81d0, data_ov021_021d81c4, data_ov021_021d81b8, data_ov021_021d8194, data_ov021_021d81a0,
    data_ov021_021d8260, data_ov021_021d8320, data_ov021_021d817c, data_ov021_021d811c, data_ov021_021d820c,
    data_ov021_021d8158, data_ov021_021d8140, data_ov021_021d814c, data_ov021_021d81ac, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d832c, data_ov021_021d8218, data_ov021_021d8164,
    data_ov021_021d8188, data_ov021_021d8338, data_ov021_021d8128, data_ov021_021d8344, data_ov021_021d8170,
    data_ov021_021d472c, data_ov021_021d47c8, data_ov021_021d46fc, data_ov021_021d46f0, data_ov021_021d46e4,
    data_ov021_021d46d8, data_ov021_021d46cc, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d41b0,
    data_ov021_021d4160, data_ov021_021d4190, data_ov021_021d4168, data_ov021_021d41a0, data_ov021_021d4188,
    data_ov021_021d4660, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8314, data_ov021_021d823c,
    data_ov021_021d8230, data_ov021_021d8224, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d4234,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d4480, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
    data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114, data_ov021_021d8114,
};
