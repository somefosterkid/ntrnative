// The billboard actors' animations, by animation set (data_ov021_021d7f68, which fldmmdl_blact.c reads), each set a
// table of animations by animation number; the functions of each kind of scene node (data_ov021_021d770c, which
// fldmmdl.c reads); and the face size and scale of each billboard size. Black's ov021 .rodata 0x021d6540-0x021d8058
// and .data 0x021ddbf0-0x021ddd70, data only, before fldmmdl_blact.c's: pokebw2's fldmmdl_drawdata.c (GPL-3.0,
// https://github.com/fuddlesworth/pokebw2, src/ov036), whose name the ROM doesn't give. Black's sets share their
// animations (pokebw2's Black 2 has a copy for each set), and its small sets' tables are in .data; the animations are
// defined in Black's address order with equal sizes in the places that give MWCC's layout Black's.
#include "types.h"
#include "field/field_actor.h"
#include "field/field_actor_internal.h"
#include "field/fldmmdl_drawdata.h"
#include "gfl/blact.h"


const FieldActorSceneNodeVTable *const data_ov021_021d770c[21] = {
    &data_ov021_021d86c0, &data_ov021_021d85b8, &data_ov021_021d8660, &data_ov021_021d85d0, &data_ov021_021d8678,
    &data_ov021_021d8648, &data_ov021_021d8600, &data_ov021_021d85e8, &data_ov021_021d8588, &data_ov021_021d8630,
    &data_ov021_021d8570, &data_ov021_021d8570, &data_ov021_021d86f0, &data_ov021_021d8690, &data_ov021_021d85a0,
    &data_ov021_021d8648, &data_ov021_021d86a8, &data_ov021_021d8558, &data_ov021_021d86d8, &data_ov021_021d8618,
    &data_ov021_021d8648,
};

// The size of a billboard actor's faces in pixels, and its scale, by its billboard size
const u16 MMDL_BILLBOARD_SIZES_INT[3][2] = { { 32, 32 }, { 16, 16 }, { 64, 64 } };

const u16 data_ov021_021d75d0[3][2] = { { 0x2000, 0x2000 }, { 0x1000, 0x1000 }, { 0x4000, 0x4000 } };

static const BlActAnimStep data_ov021_021d65ec[] = { { 0, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d6544[] = { { 0, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d6574[] = { { 0, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d65ac[] = { { 12, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d654c[] = { { 3, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d65c4[] = { { 2, 1, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d656c[] = { { 6, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d6584[] = { { 0, 0, 0, 2 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d6554[] = { { 9, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d65dc[] = { { 4, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d65b4[] = { { 4, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d65bc[] = { { 0, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d658c[] = { { 9, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d65a4[] = { { 4, 1, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d6564[] = { { 2, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d657c[] = { { 12, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d65e4[] = { { 2, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d6604[] = { { 13, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d65fc[] = { { 6, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d655c[] = { { 0, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d65cc[] = { { 0, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d659c[] = { { 1, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d65d4[] = { { 14, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d6594[] = { { 15, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d65f4[] = { { 3, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d6674[] = {
    { 0, 0, 0, 10 }, { 1, 0, 0, 10 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6620[] = { { 1, 0, 0, 4 }, { 2, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d665c[] = {
    { 12, 0, 0, 16 }, { 13, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6680[] = { { 5, 0, 0, 4 }, { 6, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d66a4[] = {
    { 4, 0, 0, 10 }, { 5, 0, 0, 10 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d662c[] = {
    { 4, 0, 0, 10 }, { 5, 0, 0, 10 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6668[] = {
    { 4, 1, 0, 10 }, { 5, 1, 0, 10 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6638[] = {
    { 0, 0, 0, 10 }, { 1, 0, 0, 10 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6644[] = { { 0, 0, 0, 5 }, { 1, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d6698[] = {
    { 9, 1, 0, 4 }, { 10, 1, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d66b0[] = {
    { 6, 0, 0, 10 }, { 7, 0, 0, 10 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6650[] = { { 0, 0, 0, 4 }, { 1, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 } };

static const BlActAnimStep data_ov021_021d668c[] = {
    { 9, 0, 0, 4 }, { 10, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d674c[] = {
    { 10, 0, 0, 4 }, { 9, 0, 0, 4 }, { 8, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d66dc[] = {
    { 2, 0, 0, 5 }, { 3, 0, 0, 10 }, { 2, 0, 0, 5 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d66ec[] = {
    { 12, 0, 0, 4 }, { 13, 0, 0, 4 }, { 14, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d675c[] = {
    { 12, 0, 0, 2 }, { 13, 0, 0, 2 }, { 14, 0, 0, 2 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d677c[] = {
    { 14, 0, 0, 2 }, { 13, 0, 0, 2 }, { 12, 0, 0, 2 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d672c[] = {
    { 2, 0, 0, 4 }, { 1, 0, 0, 4 }, { 0, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d66cc[] = {
    { 2, 0, 0, 5 }, { 3, 0, 0, 10 }, { 2, 0, 0, 5 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d670c[] = {
    { 6, 0, 0, 4 }, { 5, 0, 0, 4 }, { 4, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d66fc[] = {
    { 10, 1, 0, 4 }, { 9, 1, 0, 4 }, { 8, 1, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d673c[] = {
    { 6, 0, 0, 4 }, { 5, 0, 0, 4 }, { 4, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d671c[] = {
    { 10, 0, 0, 4 }, { 9, 0, 0, 4 }, { 8, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d676c[] = {
    { 10, 1, 0, 4 }, { 9, 1, 0, 4 }, { 8, 1, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d66bc[] = {
    { 2, 0, 0, 4 }, { 1, 0, 0, 4 }, { 0, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7268[] = {
    { 10, 0, 0, 3 }, { 9, 0, 0, 3 }, { 11, 0, 0, 3 }, { 9, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7254[] = {
    { 7, 0, 0, 3 }, { 6, 0, 0, 3 }, { 8, 0, 0, 3 }, { 6, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7240[] = {
    { 4, 0, 0, 3 }, { 3, 0, 0, 3 }, { 5, 0, 0, 3 }, { 3, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d722c[] = {
    { 1, 0, 0, 3 }, { 0, 0, 0, 3 }, { 2, 0, 0, 3 }, { 0, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7218[] = {
    { 10, 0, 0, 6 }, { 9, 0, 0, 6 }, { 11, 0, 0, 6 }, { 9, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6a34[] = {
    { 7, 0, 0, 8 }, { 6, 0, 0, 8 }, { 8, 0, 0, 8 }, { 6, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d71f0[] = {
    { 10, 0, 0, 1 }, { 9, 0, 0, 1 }, { 11, 0, 0, 1 }, { 9, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d71dc[] = {
    { 7, 0, 0, 1 }, { 6, 0, 0, 1 }, { 8, 0, 0, 1 }, { 6, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d71c8[] = {
    { 5, 1, 0, 16 }, { 4, 1, 0, 16 }, { 6, 1, 0, 16 }, { 4, 1, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d71b4[] = {
    { 1, 0, 0, 8 }, { 0, 0, 0, 8 }, { 1, 1, 0, 8 }, { 0, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d71a0[] = {
    { 3, 0, 0, 8 }, { 2, 0, 0, 8 }, { 3, 1, 0, 8 }, { 2, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d69f8[] = {
    { 7, 0, 0, 16 }, { 6, 0, 0, 16 }, { 8, 0, 0, 16 }, { 6, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6994[] = {
    { 3, 0, 0, 4 }, { 2, 0, 0, 4 }, { 3, 1, 0, 4 }, { 2, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d69a8[] = {
    { 5, 0, 0, 4 }, { 4, 0, 0, 4 }, { 6, 0, 0, 4 }, { 4, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7150[] = {
    { 4, 0, 0, 6 }, { 3, 0, 0, 6 }, { 5, 0, 0, 6 }, { 3, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d713c[] = {
    { 10, 0, 0, 2 }, { 9, 0, 0, 2 }, { 11, 0, 0, 2 }, { 9, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7128[] = {
    { 5, 1, 0, 4 }, { 4, 1, 0, 4 }, { 6, 1, 0, 4 }, { 4, 1, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6a84[] = {
    { 4, 0, 0, 4 }, { 3, 0, 0, 4 }, { 5, 0, 0, 4 }, { 3, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7100[] = {
    { 3, 0, 0, 2 }, { 2, 0, 0, 2 }, { 3, 1, 0, 2 }, { 2, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d70ec[] = {
    { 5, 0, 0, 2 }, { 4, 0, 0, 2 }, { 6, 0, 0, 2 }, { 4, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6b24[] = {
    { 1, 0, 0, 4 }, { 0, 0, 0, 3 }, { 2, 0, 0, 4 }, { 0, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6b38[] = {
    { 7, 0, 0, 2 }, { 6, 0, 0, 1 }, { 8, 0, 0, 2 }, { 6, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d70b0[] = {
    { 3, 0, 0, 1 }, { 2, 0, 0, 1 }, { 3, 1, 0, 1 }, { 2, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d709c[] = {
    { 5, 0, 0, 1 }, { 4, 0, 0, 1 }, { 6, 0, 0, 1 }, { 4, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7088[] = {
    { 5, 1, 0, 1 }, { 4, 1, 0, 1 }, { 6, 1, 0, 1 }, { 4, 1, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6bc4[] = {
    { 7, 0, 0, 4 }, { 6, 0, 0, 4 }, { 8, 0, 0, 4 }, { 6, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7060[] = {
    { 3, 0, 0, 3 }, { 2, 0, 0, 3 }, { 3, 1, 0, 3 }, { 2, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d704c[] = {
    { 5, 0, 0, 3 }, { 4, 0, 0, 3 }, { 6, 0, 0, 3 }, { 4, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6c64[] = {
    { 3, 0, 0, 2 }, { 2, 0, 0, 1 }, { 3, 1, 0, 2 }, { 2, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6c78[] = {
    { 5, 0, 0, 2 }, { 4, 0, 0, 1 }, { 6, 0, 0, 2 }, { 4, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7010[] = {
    { 7, 0, 0, 2 }, { 6, 0, 0, 2 }, { 8, 0, 0, 2 }, { 6, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6ffc[] = {
    { 1, 0, 0, 6 }, { 0, 0, 0, 6 }, { 2, 0, 0, 6 }, { 0, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6fe8[] = {
    { 5, 1, 0, 2 }, { 4, 1, 0, 1 }, { 6, 1, 0, 2 }, { 4, 1, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6cb4[] = {
    { 7, 0, 0, 2 }, { 6, 0, 0, 2 }, { 8, 0, 0, 2 }, { 6, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6fc0[] = {
    { 3, 0, 0, 4 }, { 2, 0, 0, 3 }, { 3, 1, 0, 4 }, { 2, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6fac[] = {
    { 4, 0, 0, 2 }, { 3, 0, 0, 2 }, { 5, 0, 0, 2 }, { 3, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6cf0[] = {
    { 1, 0, 0, 3 }, { 0, 0, 0, 3 }, { 2, 0, 0, 3 }, { 0, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6d04[] = {
    { 3, 0, 0, 6 }, { 2, 0, 0, 6 }, { 3, 1, 0, 6 }, { 2, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6f70[] = {
    { 1, 0, 0, 2 }, { 0, 0, 0, 2 }, { 2, 0, 0, 2 }, { 0, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6f5c[] = {
    { 10, 0, 0, 4 }, { 9, 0, 0, 3 }, { 11, 0, 0, 4 }, { 9, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6f48[] = {
    { 5, 1, 0, 6 }, { 4, 1, 0, 6 }, { 6, 1, 0, 6 }, { 4, 1, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6d40[] = {
    { 7, 0, 0, 3 }, { 6, 0, 0, 3 }, { 8, 0, 0, 3 }, { 6, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6f20[] = {
    { 7, 0, 0, 4 }, { 6, 0, 0, 4 }, { 8, 0, 0, 4 }, { 6, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6d68[] = {
    { 16, 0, 0, 8 }, { 17, 0, 0, 8 }, { 18, 0, 0, 8 }, { 19, 0, 0, 8 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6d90[] = {
    { 4, 0, 0, 2 }, { 3, 0, 0, 1 }, { 5, 0, 0, 2 }, { 3, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6da4[] = {
    { 4, 0, 0, 8 }, { 3, 0, 0, 8 }, { 5, 0, 0, 8 }, { 3, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6ed0[] = {
    { 1, 0, 0, 4 }, { 0, 0, 0, 4 }, { 2, 0, 0, 4 }, { 0, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6ebc[] = {
    { 4, 0, 0, 4 }, { 3, 0, 0, 3 }, { 5, 0, 0, 4 }, { 3, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6ea8[] = {
    { 1, 0, 0, 16 }, { 0, 0, 0, 16 }, { 2, 0, 0, 16 }, { 0, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6de0[] = {
    { 7, 0, 0, 2 }, { 6, 0, 0, 1 }, { 8, 0, 0, 2 }, { 6, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6e80[] = {
    { 7, 0, 0, 16 }, { 6, 0, 0, 16 }, { 8, 0, 0, 16 }, { 6, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6e08[] = {
    { 10, 0, 0, 2 }, { 9, 0, 0, 1 }, { 11, 0, 0, 2 }, { 9, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6e30[] = {
    { 4, 0, 0, 8 }, { 3, 0, 0, 8 }, { 5, 0, 0, 8 }, { 3, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6e44[] = {
    { 1, 0, 0, 4 }, { 0, 0, 0, 3 }, { 2, 0, 0, 4 }, { 0, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6e6c[] = {
    { 10, 0, 0, 8 }, { 9, 0, 0, 8 }, { 11, 0, 0, 8 }, { 9, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6e94[] = {
    { 4, 0, 0, 16 }, { 3, 0, 0, 16 }, { 5, 0, 0, 16 }, { 3, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6ef8[] = {
    { 7, 0, 0, 4 }, { 6, 0, 0, 3 }, { 8, 0, 0, 4 }, { 6, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6f0c[] = {
    { 10, 0, 0, 3 }, { 9, 0, 0, 3 }, { 11, 0, 0, 3 }, { 9, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6f84[] = {
    { 1, 0, 0, 6 }, { 0, 0, 0, 6 }, { 1, 1, 0, 6 }, { 0, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6f98[] = {
    { 5, 1, 0, 4 }, { 4, 1, 0, 3 }, { 6, 1, 0, 4 }, { 4, 1, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7024[] = {
    { 1, 0, 0, 2 }, { 0, 0, 0, 1 }, { 1, 1, 0, 2 }, { 0, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7038[] = {
    { 5, 1, 0, 3 }, { 4, 1, 0, 3 }, { 6, 1, 0, 3 }, { 4, 1, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d70c4[] = {
    { 1, 0, 0, 1 }, { 0, 0, 0, 1 }, { 1, 1, 0, 1 }, { 0, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d70d8[] = {
    { 5, 1, 0, 2 }, { 4, 1, 0, 2 }, { 6, 1, 0, 2 }, { 4, 1, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7164[] = {
    { 1, 0, 0, 1 }, { 0, 0, 0, 1 }, { 2, 0, 0, 1 }, { 0, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7178[] = {
    { 4, 0, 0, 1 }, { 3, 0, 0, 1 }, { 5, 0, 0, 1 }, { 3, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7204[] = {
    { 1, 0, 0, 16 }, { 0, 0, 0, 16 }, { 1, 1, 0, 16 }, { 0, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d727c[] = {
    { 0, 0, 0, 1 }, { 0, 0, 0, 1 }, { 0, 0, 0, 1 }, { 0, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d72a4[] = {
    { 1, 0, 0, 2 }, { 0, 0, 0, 1 }, { 2, 0, 0, 2 }, { 0, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d72b8[] = {
    { 4, 0, 0, 2 }, { 3, 0, 0, 1 }, { 5, 0, 0, 2 }, { 3, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d72e0[] = {
    { 10, 0, 0, 2 }, { 9, 0, 0, 1 }, { 11, 0, 0, 2 }, { 9, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d72f4[] = {
    { 1, 0, 0, 4 }, { 0, 0, 0, 3 }, { 2, 0, 0, 4 }, { 0, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d731c[] = {
    { 4, 0, 0, 4 }, { 3, 0, 0, 3 }, { 5, 0, 0, 4 }, { 3, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7330[] = {
    { 7, 0, 0, 4 }, { 6, 0, 0, 3 }, { 8, 0, 0, 4 }, { 6, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7358[] = {
    { 16, 0, 0, 4 }, { 15, 0, 0, 4 }, { 17, 0, 0, 4 }, { 15, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d736c[] = {
    { 10, 0, 0, 4 }, { 9, 0, 0, 3 }, { 11, 0, 0, 4 }, { 9, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7394[] = {
    { 4, 0, 0, 4 }, { 3, 0, 0, 4 }, { 5, 0, 0, 4 }, { 3, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d73bc[] = {
    { 4, 0, 0, 2 }, { 3, 0, 0, 1 }, { 5, 0, 0, 2 }, { 3, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6c50[] = {
    { 1, 0, 0, 2 }, { 0, 0, 0, 2 }, { 2, 0, 0, 2 }, { 0, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6c3c[] = {
    { 8, 0, 0, 4 }, { 9, 0, 0, 4 }, { 10, 0, 0, 4 }, { 11, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6c28[] = {
    { 1, 0, 0, 6 }, { 0, 0, 0, 6 }, { 2, 0, 0, 6 }, { 0, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d67a0[] = {
    { 10, 0, 0, 6 }, { 9, 0, 0, 6 }, { 11, 0, 0, 6 }, { 9, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6c00[] = {
    { 7, 0, 0, 6 }, { 6, 0, 0, 6 }, { 8, 0, 0, 6 }, { 6, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6bec[] = {
    { 10, 0, 0, 4 }, { 9, 0, 0, 4 }, { 11, 0, 0, 4 }, { 9, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d67dc[] = {
    { 1, 0, 0, 1 }, { 0, 0, 0, 1 }, { 2, 0, 0, 1 }, { 0, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d67f0[] = {
    { 0, 0, 0, 2 }, { 1, 0, 0, 2 }, { 2, 0, 0, 2 }, { 3, 0, 0, 2 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

// Animation set 9
static const BlActAnimStep *const data_ov021_021d660c[] = { data_ov021_021d6574, data_ov021_021d6644 };

static const BlActAnimStep data_ov021_021d6b9c[] = {
    { 7, 0, 0, 8 }, { 6, 0, 0, 8 }, { 8, 0, 0, 8 }, { 6, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6b88[] = {
    { 7, 0, 0, 1 }, { 6, 0, 0, 1 }, { 8, 0, 0, 1 }, { 6, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d682c[] = {
    { 4, 0, 0, 2 }, { 3, 0, 0, 2 }, { 5, 0, 0, 2 }, { 3, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6b60[] = {
    { 7, 0, 0, 3 }, { 6, 0, 0, 3 }, { 8, 0, 0, 3 }, { 6, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6b4c[] = {
    { 1, 0, 0, 2 }, { 0, 0, 0, 1 }, { 2, 0, 0, 2 }, { 0, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6868[] = {
    { 10, 0, 0, 4 }, { 9, 0, 0, 3 }, { 11, 0, 0, 4 }, { 9, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d687c[] = {
    { 1, 0, 0, 6 }, { 0, 0, 0, 6 }, { 2, 0, 0, 6 }, { 0, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6b10[] = {
    { 7, 0, 0, 4 }, { 6, 0, 0, 3 }, { 8, 0, 0, 4 }, { 6, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6afc[] = {
    { 4, 0, 0, 4 }, { 5, 0, 0, 4 }, { 6, 0, 0, 4 }, { 7, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6ae8[] = {
    { 7, 0, 0, 6 }, { 6, 0, 0, 6 }, { 8, 0, 0, 6 }, { 6, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d68b8[] = {
    { 10, 0, 0, 8 }, { 9, 0, 0, 8 }, { 11, 0, 0, 8 }, { 9, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6ac0[] = {
    { 19, 0, 0, 4 }, { 18, 0, 0, 4 }, { 20, 0, 0, 4 }, { 18, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6aac[] = {
    { 22, 0, 0, 4 }, { 21, 0, 0, 4 }, { 23, 0, 0, 4 }, { 21, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d68f4[] = {
    { 22, 0, 0, 4 }, { 21, 0, 0, 4 }, { 23, 0, 0, 4 }, { 21, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6908[] = {
    { 10, 0, 0, 4 }, { 9, 0, 0, 4 }, { 11, 0, 0, 4 }, { 9, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6a70[] = {
    { 1, 0, 0, 4 }, { 0, 0, 0, 4 }, { 2, 0, 0, 4 }, { 0, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6a5c[] = {
    { 0, 0, 0, 4 }, { 1, 0, 0, 4 }, { 2, 0, 0, 4 }, { 3, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6a48[] = {
    { 10, 0, 0, 8 }, { 9, 0, 0, 8 }, { 11, 0, 0, 8 }, { 9, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6944[] = {
    { 4, 0, 0, 16 }, { 3, 0, 0, 16 }, { 5, 0, 0, 16 }, { 3, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6a20[] = {
    { 1, 0, 0, 16 }, { 0, 0, 0, 16 }, { 2, 0, 0, 16 }, { 0, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6a0c[] = {
    { 3, 0, 0, 16 }, { 2, 0, 0, 16 }, { 3, 1, 0, 16 }, { 2, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6980[] = {
    { 1, 0, 0, 8 }, { 0, 0, 0, 8 }, { 2, 0, 0, 8 }, { 0, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d69bc[] = {
    { 4, 0, 0, 8 }, { 3, 0, 0, 8 }, { 5, 0, 0, 8 }, { 3, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d69e4[] = {
    { 5, 0, 0, 8 }, { 4, 0, 0, 8 }, { 6, 0, 0, 8 }, { 4, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6a98[] = {
    { 19, 0, 0, 4 }, { 18, 0, 0, 4 }, { 20, 0, 0, 4 }, { 18, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6ad4[] = {
    { 13, 0, 0, 4 }, { 12, 0, 0, 4 }, { 14, 0, 0, 4 }, { 12, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6b74[] = {
    { 1, 0, 0, 3 }, { 0, 0, 0, 3 }, { 2, 0, 0, 3 }, { 0, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6bb0[] = {
    { 7, 0, 0, 2 }, { 6, 0, 0, 2 }, { 8, 0, 0, 2 }, { 6, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6bd8[] = {
    { 4, 0, 0, 16 }, { 3, 0, 0, 16 }, { 5, 0, 0, 16 }, { 3, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6c14[] = {
    { 1, 0, 0, 8 }, { 0, 0, 0, 8 }, { 2, 0, 0, 8 }, { 0, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6c8c[] = {
    { 4, 0, 0, 2 }, { 3, 0, 0, 2 }, { 5, 0, 0, 2 }, { 3, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6ca0[] = {
    { 8, 1, 0, 4 }, { 9, 1, 0, 4 }, { 10, 1, 0, 4 }, { 11, 1, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6cc8[] = {
    { 5, 0, 0, 4 }, { 4, 0, 0, 3 }, { 6, 0, 0, 4 }, { 4, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6cdc[] = {
    { 10, 0, 0, 2 }, { 9, 0, 0, 2 }, { 11, 0, 0, 2 }, { 9, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6d18[] = {
    { 5, 0, 0, 6 }, { 4, 0, 0, 6 }, { 6, 0, 0, 6 }, { 4, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6d2c[] = {
    { 4, 0, 0, 3 }, { 3, 0, 0, 3 }, { 5, 0, 0, 3 }, { 3, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6d54[] = {
    { 0, 0, 0, 4 }, { 1, 0, 0, 6 }, { 2, 0, 0, 6 }, { 3, 0, 0, 6 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6d7c[] = {
    { 1, 0, 0, 2 }, { 0, 0, 0, 1 }, { 2, 0, 0, 2 }, { 0, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6db8[] = {
    { 10, 0, 0, 3 }, { 9, 0, 0, 3 }, { 11, 0, 0, 3 }, { 9, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6dcc[] = {
    { 10, 0, 0, 2 }, { 9, 0, 0, 1 }, { 11, 0, 0, 2 }, { 9, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6df4[] = {
    { 10, 0, 0, 16 }, { 9, 0, 0, 16 }, { 11, 0, 0, 16 }, { 9, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6e1c[] = {
    { 1, 0, 0, 8 }, { 0, 0, 0, 8 }, { 2, 0, 0, 8 }, { 0, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6e58[] = {
    { 7, 0, 0, 8 }, { 6, 0, 0, 8 }, { 8, 0, 0, 8 }, { 6, 0, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6ee4[] = {
    { 4, 0, 0, 4 }, { 3, 0, 0, 4 }, { 5, 0, 0, 4 }, { 3, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6f34[] = {
    { 10, 0, 0, 4 }, { 9, 0, 0, 4 }, { 11, 0, 0, 4 }, { 9, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6fd4[] = {
    { 1, 0, 0, 4 }, { 0, 0, 0, 3 }, { 1, 1, 0, 4 }, { 0, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7074[] = {
    { 1, 0, 0, 3 }, { 0, 0, 0, 3 }, { 1, 1, 0, 3 }, { 0, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7114[] = {
    { 1, 0, 0, 2 }, { 0, 0, 0, 2 }, { 1, 1, 0, 2 }, { 0, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d718c[] = {
    { 7, 0, 0, 6 }, { 6, 0, 0, 6 }, { 8, 0, 0, 6 }, { 6, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7290[] = {
    { 1, 0, 0, 4 }, { 0, 0, 0, 4 }, { 2, 0, 0, 4 }, { 0, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d72cc[] = {
    { 7, 0, 0, 2 }, { 6, 0, 0, 1 }, { 8, 0, 0, 2 }, { 6, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7308[] = {
    { 16, 0, 0, 4 }, { 15, 0, 0, 4 }, { 17, 0, 0, 4 }, { 15, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7344[] = {
    { 1, 0, 0, 16 }, { 0, 0, 0, 16 }, { 2, 0, 0, 16 }, { 0, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7380[] = {
    { 10, 0, 0, 6 }, { 9, 0, 0, 6 }, { 11, 0, 0, 6 }, { 9, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d73a8[] = {
    { 4, 0, 0, 6 }, { 3, 0, 0, 6 }, { 5, 0, 0, 6 }, { 3, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d678c[] = {
    { 4, 0, 0, 6 }, { 3, 0, 0, 6 }, { 5, 0, 0, 6 }, { 3, 0, 0, 6 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d67b4[] = {
    { 7, 0, 0, 16 }, { 6, 0, 0, 16 }, { 8, 0, 0, 16 }, { 6, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d67c8[] = {
    { 1, 0, 0, 2 }, { 0, 0, 0, 2 }, { 2, 0, 0, 2 }, { 0, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6804[] = {
    { 4, 0, 0, 4 }, { 3, 0, 0, 3 }, { 5, 0, 0, 4 }, { 3, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6818[] = {
    { 4, 0, 0, 1 }, { 3, 0, 0, 1 }, { 5, 0, 0, 1 }, { 3, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6840[] = {
    { 10, 0, 0, 1 }, { 9, 0, 0, 1 }, { 11, 0, 0, 1 }, { 9, 0, 0, 1 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6854[] = {
    { 3, 0, 0, 10 }, { 21, 0, 0, 5 }, { 19, 0, 0, 2 }, { 20, 0, 0, 10 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6890[] = {
    { 10, 0, 0, 16 }, { 9, 0, 0, 16 }, { 11, 0, 0, 16 }, { 9, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d68a4[] = {
    { 4, 0, 0, 3 }, { 3, 0, 0, 3 }, { 5, 0, 0, 3 }, { 3, 0, 0, 3 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d68cc[] = {
    { 13, 0, 0, 4 }, { 12, 0, 0, 4 }, { 14, 0, 0, 4 }, { 12, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d68e0[] = {
    { 7, 0, 0, 4 }, { 6, 0, 0, 4 }, { 8, 0, 0, 4 }, { 6, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d691c[] = {
    { 10, 0, 0, 2 }, { 9, 0, 0, 2 }, { 11, 0, 0, 2 }, { 9, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6930[] = {
    { 5, 0, 0, 16 }, { 4, 0, 0, 16 }, { 6, 0, 0, 16 }, { 4, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d6958[] = {
    { 10, 0, 0, 16 }, { 9, 0, 0, 16 }, { 11, 0, 0, 16 }, { 9, 0, 0, 16 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d696c[] = {
    { 1, 0, 0, 4 }, { 0, 0, 0, 4 }, { 1, 1, 0, 4 }, { 0, 0, 0, 4 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d69d0[] = {
    { 5, 1, 0, 8 }, { 4, 1, 0, 8 }, { 6, 1, 0, 8 }, { 4, 1, 0, 8 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d73d0[] = {
    { 4, 0, 0, 7 }, { 10, 0, 0, 6 }, { 11, 0, 0, 13 }, { 12, 0, 0, 6 }, { 0, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d73e8[] = {
    { 3, 0, 0, 10 }, { 12, 0, 0, 8 }, { 13, 0, 0, 25 }, { 14, 0, 0, 3 }, { 3, 0, 0, 10 },
    { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7400[] = {
    { 8, 0, 0, 5 }, { 9, 0, 0, 5 }, { 10, 0, 0, 5 }, { 11, 0, 0, 5 }, { 7, 0, 0, 5 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7418[] = {
    { 0, 0, 0, 7 }, { 1, 0, 0, 7 }, { 2, 0, 0, 7 }, { 3, 0, 0, 7 }, { 1, 0, 0, 7 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7430[] = {
    { 2, 0, 0, 2 }, { 7, 0, 0, 4 }, { 8, 0, 0, 6 }, { 7, 0, 0, 4 }, { 2, 0, 0, 2 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7448[] = {
    { 4, 0, 0, 7 }, { 5, 0, 0, 7 }, { 6, 0, 0, 7 }, { 5, 0, 0, 7 }, { 7, 0, 0, 7 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7460[] = {
    { 0, 0, 0, 7 }, { 1, 0, 0, 7 }, { 2, 0, 0, 7 }, { 3, 0, 0, 7 }, { 1, 0, 0, 7 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7478[] = {
    { 4, 0, 0, 10 }, { 5, 0, 0, 10 }, { 6, 0, 0, 10 }, { 14, 0, 0, 10 }, { 6, 0, 0, 10 }, { 5, 0, 0, 10 },
    { BLACT_ANIM_CMD_GOTO, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7494[] = {
    { 8, 0, 0, 2 }, { 9, 0, 0, 2 }, { 13, 0, 0, 2 }, { 14, 0, 0, 2 }, { 13, 0, 0, 2 }, { 14, 0, 0, 2 },
    { BLACT_ANIM_CMD_GOTO, 0, 0, 4 },
};

static const BlActAnimStep data_ov021_021d74b0[] = {
    { 0, 0, 0, 5 }, { 1, 0, 0, 5 }, { 0, 0, 0, 5 }, { 1, 0, 0, 5 }, { 2, 0, 0, 10 }, { 3, 0, 0, 5 }, { 4, 0, 0, 5 },
    { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d74d0[] = {
    { 3, 0, 0, 10 }, { 12, 0, 0, 5 }, { 13, 0, 0, 2 }, { 15, 0, 0, 10 }, { 14, 0, 0, 10 }, { 13, 0, 0, 1 },
    { 3, 0, 0, 10 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d74f0[] = {
    { 3, 0, 0, 10 }, { 12, 0, 0, 5 }, { 13, 0, 0, 2 }, { 15, 0, 0, 10 }, { 14, 0, 0, 5 }, { 13, 0, 0, 2 },
    { 3, 0, 0, 10 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7510[] = {
    { 3, 0, 0, 2 }, { 12, 0, 0, 4 }, { 13, 0, 0, 4 }, { 12, 0, 0, 2 }, { 3, 0, 0, 2 }, { 13, 0, 0, 4 }, { 3, 0, 0, 2 },
    { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7530[] = {
    { 4, 0, 0, 5 }, { 3, 0, 0, 5 }, { 2, 0, 0, 10 }, { 1, 0, 0, 5 }, { 0, 0, 0, 5 }, { 1, 0, 0, 5 }, { 0, 0, 0, 5 },
    { 4, 0, 0, 1 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7554[] = {
    { 0, 0, 0, 5 }, { 1, 0, 0, 5 }, { 2, 0, 0, 5 }, { 3, 0, 0, 5 }, { 4, 0, 0, 5 }, { 5, 0, 0, 5 }, { 6, 0, 0, 5 },
    { 7, 0, 0, 5 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d7578[] = {
    { 8, 0, 0, 5 }, { 9, 0, 0, 5 }, { 10, 0, 0, 5 }, { 11, 0, 0, 5 }, { 12, 0, 0, 5 }, { 13, 0, 0, 5 }, { 14, 0, 0, 5 },
    { 15, 0, 0, 5 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d759c[] = {
    { 3, 0, 0, 4 }, { 16, 0, 0, 4 }, { 17, 0, 0, 4 }, { 16, 0, 0, 4 }, { 18, 0, 0, 4 }, { 16, 0, 0, 4 },
    { 17, 0, 0, 4 }, { 16, 0, 0, 4 }, { 3, 0, 0, 4 }, { BLACT_ANIM_CMD_END, 0, 0, 0 },
};

static const BlActAnimStep data_ov021_021d75dc[] = {
    { 0, 0, 0, 4 }, { 1, 0, 0, 4 }, { 0, 0, 0, 4 }, { 2, 0, 0, 4 }, { 3, 0, 0, 4 }, { 2, 0, 0, 4 }, { 0, 0, 0, 3 },
    { 1, 0, 0, 3 }, { 0, 0, 0, 3 }, { 2, 0, 0, 3 }, { 3, 0, 0, 3 }, { 2, 0, 0, 3 }, { 0, 0, 0, 2 }, { 1, 0, 0, 2 },
    { 0, 0, 0, 2 }, { 2, 0, 0, 2 }, { 3, 0, 0, 2 }, { 2, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 12 },
};

static const BlActAnimStep data_ov021_021d7628[] = {
    { 4, 0, 0, 4 }, { 5, 0, 0, 4 }, { 4, 0, 0, 4 }, { 6, 0, 0, 4 }, { 7, 0, 0, 4 }, { 6, 0, 0, 4 }, { 4, 0, 0, 3 },
    { 5, 0, 0, 3 }, { 4, 0, 0, 3 }, { 6, 0, 0, 3 }, { 7, 0, 0, 3 }, { 6, 0, 0, 3 }, { 4, 0, 0, 2 }, { 5, 0, 0, 2 },
    { 4, 0, 0, 2 }, { 6, 0, 0, 2 }, { 7, 0, 0, 2 }, { 6, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 12 },
};

static const BlActAnimStep data_ov021_021d7674[] = {
    { 8, 0, 0, 4 }, { 9, 0, 0, 4 }, { 8, 0, 0, 4 }, { 10, 0, 0, 4 }, { 11, 0, 0, 4 }, { 10, 0, 0, 4 }, { 8, 0, 0, 3 },
    { 9, 0, 0, 3 }, { 8, 0, 0, 3 }, { 10, 0, 0, 3 }, { 11, 0, 0, 3 }, { 10, 0, 0, 3 }, { 8, 0, 0, 2 }, { 9, 0, 0, 2 },
    { 8, 0, 0, 2 }, { 10, 0, 0, 2 }, { 11, 0, 0, 2 }, { 10, 0, 0, 2 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 12 },
};

static const BlActAnimStep data_ov021_021d76c0[] = {
    { 12, 0, 0, 4 }, { 13, 0, 0, 4 }, { 12, 0, 0, 4 }, { 14, 0, 0, 4 }, { 15, 0, 0, 4 }, { 14, 0, 0, 4 },
    { 12, 0, 0, 3 }, { 13, 0, 0, 3 }, { 12, 0, 0, 3 }, { 14, 0, 0, 3 }, { 15, 0, 0, 3 }, { 14, 0, 0, 3 },
    { 12, 0, 0, 2 }, { 13, 0, 0, 2 }, { 12, 0, 0, 2 }, { 14, 0, 0, 2 }, { 15, 0, 0, 2 }, { 14, 0, 0, 2 },
    { BLACT_ANIM_CMD_GOTO, 0, 0, 12 },
};

static const BlActAnimStep data_ov021_021d7760[] = {
    { 0, 0, 0, 7 }, { 1, 0, 0, 7 }, { 2, 0, 0, 7 }, { 3, 0, 0, 7 }, { 1, 0, 0, 7 }, { 0, 0, 0, 5 }, { 13, 0, 0, 5 },
    { 8, 0, 0, 5 }, { 9, 0, 0, 5 }, { 10, 0, 0, 5 }, { 11, 0, 0, 5 }, { 7, 0, 0, 5 }, { 8, 0, 0, 5 }, { 9, 0, 0, 5 },
    { 10, 0, 0, 5 }, { 11, 0, 0, 5 }, { 7, 0, 0, 5 }, { 8, 0, 0, 5 }, { 9, 0, 0, 5 }, { 10, 0, 0, 5 }, { 11, 0, 0, 5 },
    { 7, 0, 0, 5 }, { BLACT_ANIM_CMD_GOTO, 0, 0, 17 },
};

// Animation set 1
static const BlActAnimStep *data_ov021_021ddcb0[] = {
    data_ov021_021d65ec, data_ov021_021d654c, data_ov021_021d656c, data_ov021_021d658c, data_ov021_021d7344,
    data_ov021_021d6bd8, data_ov021_021d67b4, data_ov021_021d6890, data_ov021_021d6c14, data_ov021_021d6da4,
    data_ov021_021d6b9c, data_ov021_021d68b8, data_ov021_021d7290, data_ov021_021d7394, data_ov021_021d68e0,
    data_ov021_021d6908, data_ov021_021d67c8, data_ov021_021d682c, data_ov021_021d6bb0, data_ov021_021d691c,
    data_ov021_021d67dc, data_ov021_021d6818, data_ov021_021d6b88, data_ov021_021d6840, data_ov021_021d6b74,
    data_ov021_021d68a4, data_ov021_021d6b60, data_ov021_021d6db8, data_ov021_021d6b4c, data_ov021_021d73bc,
    data_ov021_021d6b38, data_ov021_021d6dcc, data_ov021_021d6b24, data_ov021_021d6804, data_ov021_021d6b10,
    data_ov021_021d6868, data_ov021_021d687c, data_ov021_021d73a8, data_ov021_021d6ae8, data_ov021_021d7380,
    data_ov021_021d6ad4, data_ov021_021d7358, data_ov021_021d6ac0, data_ov021_021d6aac, data_ov021_021d68cc,
    data_ov021_021d7308, data_ov021_021d6a98, data_ov021_021d68f4,
};

// Animation set 22
static const BlActAnimStep *data_ov021_021ddc60[] = {
    data_ov021_021d7448, data_ov021_021d7460, data_ov021_021d7494, data_ov021_021d73d0,
};

// Animation set 6
static const BlActAnimStep *const data_ov021_021d77bc[] = {
    data_ov021_021d655c, data_ov021_021d65f4, data_ov021_021d65fc, data_ov021_021d6554, data_ov021_021d6ea8,
    data_ov021_021d6e94, data_ov021_021d6e80, data_ov021_021d6df4, data_ov021_021d6e1c, data_ov021_021d6e30,
    data_ov021_021d6e58, data_ov021_021d6e6c, data_ov021_021d6ed0, data_ov021_021d6ee4, data_ov021_021d6f20,
    data_ov021_021d6f34, data_ov021_021d6f70, data_ov021_021d6fac, data_ov021_021d7010, data_ov021_021d713c,
    data_ov021_021d7164, data_ov021_021d7178, data_ov021_021d71dc, data_ov021_021d71f0, data_ov021_021d722c,
    data_ov021_021d7240, data_ov021_021d7254, data_ov021_021d7268, data_ov021_021d72a4, data_ov021_021d72b8,
    data_ov021_021d72cc, data_ov021_021d72e0, data_ov021_021d72f4, data_ov021_021d731c, data_ov021_021d7330,
    data_ov021_021d736c, data_ov021_021d6c28, data_ov021_021d678c, data_ov021_021d6c00, data_ov021_021d67a0,
};

// Animation set 5
static const BlActAnimStep *const data_ov021_021d785c[] = {
    data_ov021_021d6544, data_ov021_021d65e4, data_ov021_021d65dc, data_ov021_021d65a4, data_ov021_021d7204,
    data_ov021_021d6a0c, data_ov021_021d6930, data_ov021_021d71c8, data_ov021_021d71b4, data_ov021_021d71a0,
    data_ov021_021d69e4, data_ov021_021d69d0, data_ov021_021d696c, data_ov021_021d6994, data_ov021_021d69a8,
    data_ov021_021d7128, data_ov021_021d7114, data_ov021_021d7100, data_ov021_021d70ec, data_ov021_021d70d8,
    data_ov021_021d70c4, data_ov021_021d70b0, data_ov021_021d709c, data_ov021_021d7088, data_ov021_021d7074,
    data_ov021_021d7060, data_ov021_021d704c, data_ov021_021d7038, data_ov021_021d7024, data_ov021_021d6c64,
    data_ov021_021d6c78, data_ov021_021d6fe8, data_ov021_021d6fd4, data_ov021_021d6fc0, data_ov021_021d6cc8,
    data_ov021_021d6f98, data_ov021_021d6f84, data_ov021_021d6d04, data_ov021_021d6d18, data_ov021_021d6f48,
};

// Animation set 7
static const BlActAnimStep *const data_ov021_021d78fc[] = {
    data_ov021_021d657c, data_ov021_021d6604, data_ov021_021d65d4, data_ov021_021d6594, data_ov021_021d6a20,
    data_ov021_021d6944, data_ov021_021d69f8, data_ov021_021d6958, data_ov021_021d6980, data_ov021_021d69bc,
    data_ov021_021d6a34, data_ov021_021d6a48, data_ov021_021d6a70, data_ov021_021d6a84, data_ov021_021d6bc4,
    data_ov021_021d6bec, data_ov021_021d6c50, data_ov021_021d6c8c, data_ov021_021d6cb4, data_ov021_021d6cdc,
    data_ov021_021d6a70, data_ov021_021d6a84, data_ov021_021d6bc4, data_ov021_021d6bec, data_ov021_021d6cf0,
    data_ov021_021d6d2c, data_ov021_021d6d40, data_ov021_021d6f0c, data_ov021_021d6d7c, data_ov021_021d6d90,
    data_ov021_021d6de0, data_ov021_021d6e08, data_ov021_021d6e44, data_ov021_021d6ebc, data_ov021_021d6ef8,
    data_ov021_021d6f5c, data_ov021_021d6ffc, data_ov021_021d7150, data_ov021_021d718c, data_ov021_021d7218,
};

// Animation set 8
static const BlActAnimStep *const data_ov021_021d799c[] = {
    data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c,
    data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c,
    data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c,
    data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c,
    data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c,
    data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c,
    data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c,
    data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c, data_ov021_021d727c,
};

// Animation set 11
static const BlActAnimStep *const data_ov021_021d6540[] = { data_ov021_021d6650 };

// Animation set 28
static const BlActAnimStep *const data_ov021_021d7ccc[] = {
    data_ov021_021d655c, data_ov021_021d65f4, data_ov021_021d65fc, data_ov021_021d6554, data_ov021_021d6ea8,
    data_ov021_021d6e94, data_ov021_021d6e80, data_ov021_021d6df4, data_ov021_021d6e1c, data_ov021_021d6e30,
    data_ov021_021d6e58, data_ov021_021d6e6c, data_ov021_021d6ed0, data_ov021_021d6ee4, data_ov021_021d6f20,
    data_ov021_021d6f34, data_ov021_021d6f70, data_ov021_021d6fac, data_ov021_021d7010, data_ov021_021d713c,
    data_ov021_021d7164, data_ov021_021d7178, data_ov021_021d71dc, data_ov021_021d71f0, data_ov021_021d722c,
    data_ov021_021d7240, data_ov021_021d7254, data_ov021_021d7268, data_ov021_021d72a4, data_ov021_021d72b8,
    data_ov021_021d72cc, data_ov021_021d72e0, data_ov021_021d72f4, data_ov021_021d731c, data_ov021_021d7330,
    data_ov021_021d736c, data_ov021_021d6c28, data_ov021_021d678c, data_ov021_021d6c00, data_ov021_021d67a0,
    data_ov021_021d7510,
};

// Animation set 16
static const BlActAnimStep *data_ov021_021ddbf0[] = {
    data_ov021_021d67f0, data_ov021_021d7400, data_ov021_021d6584, data_ov021_021d6d54,
};

// Animation set 21
static const BlActAnimStep *data_ov021_021ddc50[] = {
    data_ov021_021d65cc, data_ov021_021d7554, data_ov021_021d7578, data_ov021_021d6d68,
};

// Animation set 15
static const BlActAnimStep *data_ov021_021ddc70[] = {
    data_ov021_021d6a5c, data_ov021_021d6afc, data_ov021_021d6c3c, data_ov021_021d6ca0, data_ov021_021d66bc,
    data_ov021_021d670c, data_ov021_021d671c, data_ov021_021d66fc, data_ov021_021d6620, data_ov021_021d6680,
    data_ov021_021d668c, data_ov021_021d6698, data_ov021_021d672c, data_ov021_021d673c, data_ov021_021d674c,
    data_ov021_021d676c,
};

// Animation set 10
static const BlActAnimStep *const data_ov021_021d6614[] = {
    data_ov021_021d65b4, data_ov021_021d74b0, data_ov021_021d7530,
};

// Animation set 4
static const BlActAnimStep *data_ov021_021ddc20[] = {
    data_ov021_021d65bc, data_ov021_021d659c, data_ov021_021d6564, data_ov021_021d65c4,
};

// Animation set 13
static const BlActAnimStep *data_ov021_021ddc10[] = {
    data_ov021_021d6674, data_ov021_021d66dc, data_ov021_021d66a4, data_ov021_021d6668,
};

// Animation set 14
static const BlActAnimStep *data_ov021_021ddc00[] = {
    data_ov021_021d6638, data_ov021_021d66cc, data_ov021_021d662c, data_ov021_021d66b0,
};

// Animation set 24
static const BlActAnimStep *const data_ov021_021d7b84[] = {
    data_ov021_021d655c, data_ov021_021d65f4, data_ov021_021d65fc, data_ov021_021d6554, data_ov021_021d6ea8,
    data_ov021_021d6e94, data_ov021_021d6e80, data_ov021_021d6df4, data_ov021_021d6e1c, data_ov021_021d6e30,
    data_ov021_021d6e58, data_ov021_021d6e6c, data_ov021_021d6ed0, data_ov021_021d6ee4, data_ov021_021d6f20,
    data_ov021_021d6f34, data_ov021_021d6f70, data_ov021_021d6fac, data_ov021_021d7010, data_ov021_021d713c,
    data_ov021_021d7164, data_ov021_021d7178, data_ov021_021d71dc, data_ov021_021d71f0, data_ov021_021d722c,
    data_ov021_021d7240, data_ov021_021d7254, data_ov021_021d7268, data_ov021_021d72a4, data_ov021_021d72b8,
    data_ov021_021d72cc, data_ov021_021d72e0, data_ov021_021d72f4, data_ov021_021d731c, data_ov021_021d7330,
    data_ov021_021d736c, data_ov021_021d6c28, data_ov021_021d678c, data_ov021_021d6c00, data_ov021_021d67a0,
    data_ov021_021d73e8,
};

// Animation set 18
static const BlActAnimStep *data_ov021_021ddc30[] = {
    data_ov021_021d7418, data_ov021_021d7478, data_ov021_021d7760, data_ov021_021d65ac,
};

// Animation set 17
static const BlActAnimStep *data_ov021_021ddc40[] = {
    data_ov021_021d75dc, data_ov021_021d7628, data_ov021_021d7674, data_ov021_021d76c0,
};

// Animation set 20
static const BlActAnimStep *const data_ov021_021d7a3c[] = {
    data_ov021_021d655c, data_ov021_021d65f4, data_ov021_021d65fc, data_ov021_021d6554, data_ov021_021d6ea8,
    data_ov021_021d6e94, data_ov021_021d6e80, data_ov021_021d6df4, data_ov021_021d6e1c, data_ov021_021d6e30,
    data_ov021_021d6e58, data_ov021_021d6e6c, data_ov021_021d6ed0, data_ov021_021d6ee4, data_ov021_021d6f20,
    data_ov021_021d6f34, data_ov021_021d6f70, data_ov021_021d6fac, data_ov021_021d7010, data_ov021_021d713c,
    data_ov021_021d7164, data_ov021_021d7178, data_ov021_021d71dc, data_ov021_021d71f0, data_ov021_021d722c,
    data_ov021_021d7240, data_ov021_021d7254, data_ov021_021d7268, data_ov021_021d72a4, data_ov021_021d72b8,
    data_ov021_021d72cc, data_ov021_021d72e0, data_ov021_021d72f4, data_ov021_021d731c, data_ov021_021d7330,
    data_ov021_021d736c, data_ov021_021d6c28, data_ov021_021d678c, data_ov021_021d6c00, data_ov021_021d67a0,
    data_ov021_021d66ec,
};

// Animation set 23
static const BlActAnimStep *const data_ov021_021d7ae0[] = {
    data_ov021_021d655c, data_ov021_021d65f4, data_ov021_021d65fc, data_ov021_021d6554, data_ov021_021d6ea8,
    data_ov021_021d6e94, data_ov021_021d6e80, data_ov021_021d6df4, data_ov021_021d6e1c, data_ov021_021d6e30,
    data_ov021_021d6e58, data_ov021_021d6e6c, data_ov021_021d6ed0, data_ov021_021d6ee4, data_ov021_021d6f20,
    data_ov021_021d6f34, data_ov021_021d6f70, data_ov021_021d6fac, data_ov021_021d7010, data_ov021_021d713c,
    data_ov021_021d7164, data_ov021_021d7178, data_ov021_021d71dc, data_ov021_021d71f0, data_ov021_021d722c,
    data_ov021_021d7240, data_ov021_021d7254, data_ov021_021d7268, data_ov021_021d72a4, data_ov021_021d72b8,
    data_ov021_021d72cc, data_ov021_021d72e0, data_ov021_021d72f4, data_ov021_021d731c, data_ov021_021d7330,
    data_ov021_021d736c, data_ov021_021d6c28, data_ov021_021d678c, data_ov021_021d6c00, data_ov021_021d67a0,
    data_ov021_021d665c,
};

// Animation set 25
static const BlActAnimStep *const data_ov021_021d7ebc[] = {
    data_ov021_021d655c, data_ov021_021d65f4, data_ov021_021d65fc, data_ov021_021d6554, data_ov021_021d6ea8,
    data_ov021_021d6e94, data_ov021_021d6e80, data_ov021_021d6df4, data_ov021_021d6e1c, data_ov021_021d6e30,
    data_ov021_021d6e58, data_ov021_021d6e6c, data_ov021_021d6ed0, data_ov021_021d6ee4, data_ov021_021d6f20,
    data_ov021_021d6f34, data_ov021_021d6f70, data_ov021_021d6fac, data_ov021_021d7010, data_ov021_021d713c,
    data_ov021_021d7164, data_ov021_021d7178, data_ov021_021d71dc, data_ov021_021d71f0, data_ov021_021d722c,
    data_ov021_021d7240, data_ov021_021d7254, data_ov021_021d7268, data_ov021_021d72a4, data_ov021_021d72b8,
    data_ov021_021d72cc, data_ov021_021d72e0, data_ov021_021d72f4, data_ov021_021d731c, data_ov021_021d7330,
    data_ov021_021d736c, data_ov021_021d6c28, data_ov021_021d678c, data_ov021_021d6c00, data_ov021_021d67a0,
    data_ov021_021d74d0, data_ov021_021d759c, data_ov021_021d6854,
};

// Animation set 12
static const BlActAnimStep *const data_ov021_021d7d70[] = {
    data_ov021_021d6544, data_ov021_021d65e4, data_ov021_021d65dc, data_ov021_021d65a4, data_ov021_021d7204,
    data_ov021_021d6a0c, data_ov021_021d6930, data_ov021_021d71c8, data_ov021_021d71b4, data_ov021_021d71a0,
    data_ov021_021d69e4, data_ov021_021d69d0, data_ov021_021d696c, data_ov021_021d6994, data_ov021_021d69a8,
    data_ov021_021d7128, data_ov021_021d7114, data_ov021_021d7100, data_ov021_021d70ec, data_ov021_021d70d8,
    data_ov021_021d70c4, data_ov021_021d70b0, data_ov021_021d709c, data_ov021_021d7088, data_ov021_021d7074,
    data_ov021_021d7060, data_ov021_021d704c, data_ov021_021d7038, data_ov021_021d7024, data_ov021_021d6c64,
    data_ov021_021d6c78, data_ov021_021d6fe8, data_ov021_021d6fd4, data_ov021_021d6fc0, data_ov021_021d6cc8,
    data_ov021_021d6f98, data_ov021_021d6f84, data_ov021_021d6d04, data_ov021_021d6d18, data_ov021_021d6f48,
    data_ov021_021d7430,
};

// Animation set 27
static const BlActAnimStep *const data_ov021_021d7e14[] = {
    data_ov021_021d655c, data_ov021_021d65f4, data_ov021_021d65fc, data_ov021_021d6554, data_ov021_021d6e1c,
    data_ov021_021d6e30, data_ov021_021d6e58, data_ov021_021d6e6c, data_ov021_021d6ed0, data_ov021_021d6ee4,
    data_ov021_021d6f20, data_ov021_021d6f34, data_ov021_021d6f70, data_ov021_021d6fac, data_ov021_021d7010,
    data_ov021_021d713c, data_ov021_021d7164, data_ov021_021d7178, data_ov021_021d71dc, data_ov021_021d71f0,
    data_ov021_021d7164, data_ov021_021d7178, data_ov021_021d71dc, data_ov021_021d71f0, data_ov021_021d722c,
    data_ov021_021d7240, data_ov021_021d7254, data_ov021_021d7268, data_ov021_021d72a4, data_ov021_021d72b8,
    data_ov021_021d72cc, data_ov021_021d72e0, data_ov021_021d72f4, data_ov021_021d731c, data_ov021_021d7330,
    data_ov021_021d736c, data_ov021_021d6c28, data_ov021_021d678c, data_ov021_021d6c00, data_ov021_021d67a0,
    data_ov021_021d677c, data_ov021_021d675c,
};

// Animation set 26
static const BlActAnimStep *const data_ov021_021d7c28[] = {
    data_ov021_021d655c, data_ov021_021d65f4, data_ov021_021d65fc, data_ov021_021d6554, data_ov021_021d6ea8,
    data_ov021_021d6e94, data_ov021_021d6e80, data_ov021_021d6df4, data_ov021_021d6e1c, data_ov021_021d6e30,
    data_ov021_021d6e58, data_ov021_021d6e6c, data_ov021_021d6ed0, data_ov021_021d6ee4, data_ov021_021d6f20,
    data_ov021_021d6f34, data_ov021_021d6f70, data_ov021_021d6fac, data_ov021_021d7010, data_ov021_021d713c,
    data_ov021_021d7164, data_ov021_021d7178, data_ov021_021d71dc, data_ov021_021d71f0, data_ov021_021d722c,
    data_ov021_021d7240, data_ov021_021d7254, data_ov021_021d7268, data_ov021_021d72a4, data_ov021_021d72b8,
    data_ov021_021d72cc, data_ov021_021d72e0, data_ov021_021d72f4, data_ov021_021d731c, data_ov021_021d7330,
    data_ov021_021d736c, data_ov021_021d6c28, data_ov021_021d678c, data_ov021_021d6c00, data_ov021_021d67a0,
    data_ov021_021d74f0,
};

// The animation sets, by the set number of an object code's record
const BlActDefaultAnim data_ov021_021d7f68[30] = {
    { NULL, 0 },
    { data_ov021_021ddcb0, NELEMS(data_ov021_021ddcb0) },
    { NULL, 0 },
    { NULL, 0 },
    { data_ov021_021ddc20, NELEMS(data_ov021_021ddc20) },
    { data_ov021_021d785c, NELEMS(data_ov021_021d785c) },
    { data_ov021_021d77bc, NELEMS(data_ov021_021d77bc) },
    { data_ov021_021d78fc, NELEMS(data_ov021_021d78fc) },
    { data_ov021_021d799c, NELEMS(data_ov021_021d799c) },
    { data_ov021_021d660c, NELEMS(data_ov021_021d660c) },
    { data_ov021_021d6614, NELEMS(data_ov021_021d6614) },
    { data_ov021_021d6540, NELEMS(data_ov021_021d6540) },
    { data_ov021_021d7d70, NELEMS(data_ov021_021d7d70) },
    { data_ov021_021ddc10, NELEMS(data_ov021_021ddc10) },
    { data_ov021_021ddc00, NELEMS(data_ov021_021ddc00) },
    { data_ov021_021ddc70, NELEMS(data_ov021_021ddc70) },
    { data_ov021_021ddbf0, NELEMS(data_ov021_021ddbf0) },
    { data_ov021_021ddc40, NELEMS(data_ov021_021ddc40) },
    { data_ov021_021ddc30, NELEMS(data_ov021_021ddc30) },
    { &data_ov021_021ddbf0[2], 2 },
    { data_ov021_021d7a3c, NELEMS(data_ov021_021d7a3c) },
    { data_ov021_021ddc50, NELEMS(data_ov021_021ddc50) },
    { data_ov021_021ddc60, NELEMS(data_ov021_021ddc60) },
    { data_ov021_021d7ae0, NELEMS(data_ov021_021d7ae0) },
    { data_ov021_021d7b84, NELEMS(data_ov021_021d7b84) },
    { data_ov021_021d7ebc, NELEMS(data_ov021_021d7ebc) },
    { data_ov021_021d7c28, NELEMS(data_ov021_021d7c28) },
    { data_ov021_021d7e14, NELEMS(data_ov021_021d7e14) },
    { data_ov021_021d7ccc, NELEMS(data_ov021_021d7ccc) },
    { &data_ov021_021ddbf0[1], 1 },
};
