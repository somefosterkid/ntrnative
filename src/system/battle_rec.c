// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/battle_rec.c, ported by tools/port_file.py.
#include "types.h"
#include "save/battle_rec.h"
#include "constants/species.h"
#include "gfl/heap.h"
#include "gfl/overlay.h"
#include "gfl/std.h"
#include "save/save_control.h"
#include "save/tournament_block.h"
#include "system/game_data.h"
#include "nitro/mi.h"

// battle_rec.c (the name its allocation passes): the battle video that is loaded, and loading it from and saving it
// to the extra save blocks of the four slots. Its body is encrypted with its CRC as the key.

static BattleRecVideo *func_0200b3dc(HeapID heapId);
static BOOL func_0200b4e0(SaveControl *save, BattleRecVideo *video, HeapID heapId, u32 *result, u32 block);
static void func_0200b7d8(SaveControl *save, BattleRecHeader *header, BattleRecBody *body, u32 modeIndex, u32 counter);
static BOOL func_0200b888(SaveControl *save, BattleRecVideo *video);
static BOOL func_0200b8ac(SaveControl *save, BattleRecVideo *video);
static void __decryptData(void *data, u32 size, u32 seed);

// The battle modes that a header's mode can be. A video's mode is given as an index into them
static const u16 data_0209ddcc[BATTLE_REC_MODE_MAX] = {
#include "asset/battle_rec_data_0209ddcc.inc"
};

BattleRecVideo *data_0214616c;

// The key that a body is encrypted with, of its CRC
#define BATTLE_REC_KEY(body) ((body)->crc + (((body)->crc ^ 0xffff) << 16))

static BattleRecVideo *func_0200b3dc(HeapID heapId) {
    BattleRecVideo *video = GFL_HeapAllocate(heapId, BATTLE_REC_BLOCK_SIZE, TRUE, "battle_rec.c", 103);

    clearVSPlayerBlk(video);
    return video;
}

void func_0200b400(HeapID heapId) {
    if (data_0214616c != NULL) {
        GFL_HeapFree(data_0214616c);
        data_0214616c = NULL;
    }
    data_0214616c = func_0200b3dc(heapId);
}

void freeVSPlayerBlkClearPtr(void) {
    GFL_HeapFree(data_0214616c);
    data_0214616c = NULL;
}

void func_0200b438(BattleRecVideo *video) {
    GFL_HeapFree(video);
}

u32 getSizeOfVSPlayer(void) {
    return sizeof(BattleRecVideo);
}

void clearVSPlayerBlk(BattleRecVideo *video) {
    MIi_CpuClear32(0, video, sizeof(BattleRecVideo));
}

BOOL func_0200b45c(void) {
    if (addUpDataStream((u8 *)&data_0214616c->body, sizeof(BattleRecBody)) != 0) {
        return TRUE;
    }
    return FALSE;
}

void clearVSPlayer(void) {
    clearVSPlayerBlk(data_0214616c);
}

void func_0200b490(void) {
    __decryptData(&data_0214616c->body, BATTLE_REC_BODY_CRC_SIZE, BATTLE_REC_KEY(&data_0214616c->body));
}

void func_0200b4b8(void) {
    decryptVSPlayer(&data_0214616c->body, BATTLE_REC_BODY_CRC_SIZE, BATTLE_REC_KEY(&data_0214616c->body));
}

static BOOL func_0200b4e0(SaveControl *save, BattleRecVideo *video, HeapID heapId, u32 *result, u32 block) {
    BattleRecBody *body;

    *result = func_020072cc(save, block, heapId, video, BATTLE_REC_BLOCK_SIZE);
    freeIntermediateSaveExtraBlksAfterLoad(save, block);
    if (*result == 0) {
        *result = 0;
        return TRUE;
    }
    if (*result != 1) {
        *result = 3;
        return TRUE;
    }

    body = &video->body;
    __decryptData(body, BATTLE_REC_BODY_CRC_SIZE, BATTLE_REC_KEY(body));
    if (func_0200b888(save, video) == TRUE) {
        *result = 0;
        return TRUE;
    }
    if (!func_0200b8ac(save, video)) {
        *result = 2;
        return TRUE;
    }
    *result = 1;
    return TRUE;
}


void func_0200b570(SaveControl *save, HeapID heapId, u32 *result, u32 slot) {
    if (data_0214616c != NULL) {
        clearVSPlayerBlk(data_0214616c);
    } else {
        func_0200b400(heapId);
    }
    func_0200b4e0(save, data_0214616c, heapId, result, slot);
}

BattleRecVideo *func_0200b5a4(SaveControl *save, HeapID heapId, u32 *result, u32 slot) {
    BattleRecVideo *video = func_0200b3dc(heapId);

    func_0200b4e0(save, video, heapId, result, slot);
    return video;
}

// Whether a slot has a valid video, without keeping it. result is the u32 result of func_0200b570
u32 func_0200b5cc(SaveControl *save, HeapID heapId, u32 *result, u32 block) {
    BattleRecVideo *video;
    BattleRecBody *body;

    *result = func_0200725c(save, block, heapId);
    video = getAddressOfExtraSaveBlk(save, block, 0);
    if (*result == 0) {
        *result = 0;
        freeIntermediateSaveExtraBlksAfterLoad(save, block);
        return FALSE;
    }
    if (*result != 1) {
        *result = 3;
        freeIntermediateSaveExtraBlksAfterLoad(save, block);
        return FALSE;
    }

    body = &video->body;
    __decryptData(body, BATTLE_REC_BODY_CRC_SIZE, BATTLE_REC_KEY(body));
    if (func_0200b888(save, video) == TRUE) {
        *result = 0;
        freeIntermediateSaveExtraBlksAfterLoad(save, block);
        return FALSE;
    }
    if (!func_0200b8ac(save, video)) {
        *result = 2;
        freeIntermediateSaveExtraBlksAfterLoad(save, block);
        return FALSE;
    }
    *result = 1;
    freeIntermediateSaveExtraBlksAfterLoad(save, block);
    return TRUE;
}

u32 func_0200b67c(GameData *gameData, BattleRecVideo *video, u32 block, u16 *seq, HeapID heapId) {
    SaveControl *save = GameData_GetSaveControl(gameData);
    u32 result;

    switch (*seq) {
    case 0:
        func_02007324(save, block, heapId, data_0214616c, BATTLE_REC_BLOCK_SIZE);
        func_02012e4c(gameData, block);
        (*seq)++;
        break;
    case 1:
        result = func_02012e7c(gameData, block);
        if (result == 2 || result == 3) {
            freeIntermediateSaveExtraBlksAfterLoad2(save, block);
            return result;
        }
        break;
    }
    return 0;
}


// Makes the loaded video's header, signs and encrypts the video and saves it to slot a4, a step each call like
// func_0200b67c. a1 is the heap, a2 the index of the battle mode and a3 the header's counter. 3 if no video is loaded
u32 func_0200b6e0(GameData *gameData, HeapID a1, u32 a2, u32 a3, u32 a4, u16 *a5, u16 *a6) {
    SaveControl *save = GameData_GetSaveControl(gameData);
    BattleRecHeader *header;
    BattleRecBody *body;

    switch (*a5) {
    case 0:
        if (data_0214616c == NULL) {
            return 3;
        }
        header = &data_0214616c->header;
        body = &data_0214616c->body;
        func_0200b7d8(save, header, body, a2, a3);
        header->magic = BATTLE_REC_MAGIC;
        header->crc = getCRC16(header, BATTLE_REC_HEADER_CRC_SIZE);
        body->magic = BATTLE_REC_MAGIC;
        func_0200ba44(body);
        decryptVSPlayer(body, BATTLE_REC_BODY_CRC_SIZE, BATTLE_REC_KEY(body));
        *a6 = 0;
        (*a5)++;
        break;
    case 1:
        return func_0200b67c(gameData, data_0214616c, a4, a6, a1);
    }
    return 0;
}

void func_0200b784(u16 mode, s32 *clientCount, s32 *pkmCount) {
    s32 i;

    for (i = 0; i < BATTLE_REC_MODE_MAX; i++) {
        if (mode == data_0209ddcc[i]) {
            switch (i) {
            case 16:
            case 17:
            case 18:
            case 19:
            case 22:
                *clientCount = 4;
                *pkmCount = 3;
                return;
            }
        }
    }
    *clientCount = 2;
    *pkmCount = 6;
}

static void func_0200b7d8(SaveControl *save, BattleRecHeader *header, BattleRecBody *body, u32 modeIndex, u32 counter) {
    s32 client;
    s32 i;
    s32 n = 0;
    s32 clientCount;
    s32 pkmCount;
    u16 mode;

    MI_CpuFill8(header, 0, sizeof(BattleRecHeader));
    mode = data_0209ddcc[modeIndex];
    func_0200b784(mode, &clientCount, &pkmCount);
    for (client = 0; client < clientCount; client++) {
        for (i = 0; i < pkmCount; i++) {
            BattleRecPkm *pkm = &body->parties[client].pkm[i];

            if (!pkm->isEgg && !pkm->badEgg) {
                header->species[n] = pkm->species;
                header->formGender[n] = (pkm->form & 0x7f) | (pkm->gender << 7);
            }
            n++;
        }
    }
    header->counter = counter;
    header->mode = mode;
    header->version = BATTLE_REC_VERSION;
}

// Whether a video is not one: a slot that was never saved to
static BOOL func_0200b888(SaveControl *save, BattleRecVideo *video) {
    BattleRecHeader *header = &video->header;

    if (video->body.magic != BATTLE_REC_MAGIC || header->magic != BATTLE_REC_MAGIC) {
        return TRUE;
    }
    return FALSE;
}

// Whether a video's magic numbers and CRCs are right
static BOOL func_0200b8ac(SaveControl *save, BattleRecVideo *video) {
    BattleRecBody *body = &video->body;
    BattleRecHeader *header = &video->header;
    u16 crc;

    if (body->magic != BATTLE_REC_MAGIC || header->magic != BATTLE_REC_MAGIC) {
        return FALSE;
    }
    crc = getCRC16(header, BATTLE_REC_HEADER_CRC_SIZE);
    if (crc != header->crc) {
        return FALSE;
    }
    crc = getCRC16(body, BATTLE_REC_BODY_CRC_SIZE);
    if (crc == body->crc) {
        return TRUE;
    }
    return FALSE;
}

static void __decryptData(void *data, u32 size, u32 seed) {
    _decryptData(data, size, seed);
}

void decryptVSPlayer(void *data, u32 size, u32 seed) {
    decryptData(data, size, seed);
}

GdsProfile *getVSPlayerAllocation(void) {
    return (GdsProfile *)data_0214616c->profile;
}

BattleRecBody *func_0200b91c(void) {
    return &data_0214616c->body;
}

BattleRecHeader *func_0200b928(void) {
    return &data_0214616c->header;
}

BattleRecHeader *func_0200b934(BattleRecVideo *video) {
    return &video->header;
}

void func_0200b938(const GdsProfile *profile, const BattleRecHeader *header, const BattleRecBody *body, SaveControl *save) {
    MI_CpuCopy8(header, &data_0214616c->header, sizeof(BattleRecHeader));
    MI_CpuCopy8(body, &data_0214616c->body, sizeof(BattleRecBody));
    MI_CpuCopy8(profile, data_0214616c->profile, sizeof(data_0214616c->profile));
    __decryptData(&data_0214616c->body, BATTLE_REC_BODY_CRC_SIZE, BATTLE_REC_KEY(&data_0214616c->body));
}

u64 func_0200b98c(BattleRecHeader *header, u32 id, u32 index) {
    s32 i;

    switch (id) {
    case BATTLE_REC_HEADER_SPECIES:
        if (header->species[index] > SPECIES_GENESECT) {
            return 0;
        }
        return header->species[index];
    case BATTLE_REC_HEADER_FORM:
        return header->formGender[index] & 0x7f;
    case BATTLE_REC_HEADER_COUNTER:
        if (header->counter > 9999) {
            return 9999;
        }
        return header->counter;
    case BATTLE_REC_HEADER_MODE:
        for (i = 0; i < BATTLE_REC_MODE_MAX; i++) {
            if (header->mode == data_0209ddcc[i]) {
                return i;
            }
        }
        return 0;
    case BATTLE_REC_HEADER_DATA_NUMBER:
        return header->dataNumber;
    case BATTLE_REC_HEADER_UNK2A:
        return header->unk2A;
    case BATTLE_REC_HEADER_GENDER:
        return (header->formGender[index] & 0x80) >> 7;
    case BATTLE_REC_HEADER_VERSION:
        return header->version;
    }
    return 0;
}

// Whether a video of this version can be played
u32 func_0200ba38(u8 a0) {
    if (a0 <= BATTLE_REC_VERSION) {
        return TRUE;
    }
    return FALSE;
}

void func_0200ba44(BattleRecBody *body) {
    body->crc = getCRC16(body, BATTLE_REC_BODY_CRC_SIZE);
}

void func_0200ba58(void) {
    GFL_OvlLoad(OVERLAY_ID(188));
}

void func_0200ba68(void) {
    GFL_OvlUnload(OVERLAY_ID(188));
}


