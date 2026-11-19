#include "global.h"

void *sub_02020838(NNSG3dResTex *texture, const u8 index);

u32 sub_020208CC(NNSG3dResTex *texture, const NNSG3dResDictTexData *textureData);

void *sub_02020838(NNSG3dResTex *texture, const u8 index) {
    const NNSG3dResDictTexData *textureData = NNS_G3dGetTexDataByIdx(texture, index);

    if (textureData == NULL) {
        return NULL;
    }

    u32 address = (textureData->texImageParam & NNS_G3D_TEXIMAGE_PARAM_TEX_ADDR_MASK) + texture->texInfo.vramKey;
    return (void *)((u8 *)texture + texture->texInfo.ofsTex + (address << 3));
}

void *sub_02020888(NNSG3dResTex *texture, const u8 index);

void *sub_02020888(NNSG3dResTex *texture, const u8 index) {
    const NNSG3dResDictPlttData *plttData = NNS_G3dGetPlttDataByIdx(texture, index);

    if (plttData == NULL) {
        return NULL;
    }

    return (void *)((u8 *)texture + texture->plttInfo.ofsPlttData + (plttData->offset << 3));
}

u32 sub_020208CC(NNSG3dResTex *texture, const NNSG3dResDictTexData *textureData) {
    return ((textureData->texImageParam << 0x10) >> 0xD) + ((texture->texInfo.vramKey << 0x10) >> 0xD);
}
