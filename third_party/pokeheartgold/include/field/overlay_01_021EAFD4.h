#ifndef POKEHEARTGOLD_FIELD_OVERLAY_01_021EAFD4_H
#define POKEHEARTGOLD_FIELD_OVERLAY_01_021EAFD4_H

#include "field_types_def.h"

#define MAX_TEXTURE_KEYS 16

typedef struct {
    char name[16];
    u8 frames[18][2]; // {texture index, duration}; index 0xFF ends the animation
} TextureAnimationData;

typedef struct TextureSlot {
    u32 vramOffset; // an offset into texture VRAM, not a pointer
    u32 vramSize;
    NNSG3dResTex *textureResource;
    void *narcData;
    TextureAnimationData *animationData;
    u16 currentFrame;
    u16 frameCounter;
} TextureSlot;

typedef struct {
    u32 textureCount;
    TextureAnimationData animationData[0];
} TextureDataHeader;

struct FieldTextureManager {
    void *dataBuffer;
    TextureDataHeader *dataHeader;
    TextureSlot textureSlots[MAX_TEXTURE_KEYS];
    u32 keepTextures;
};

FieldTextureManager *FieldTextureManager_Init();
int FieldTextureManager_LoadTexture(FieldTextureManager *textureManager, NNSG3dResTex *textureResource);
void FieldTextureManager_Free(FieldTextureManager *textureManager);
void ov01_021EB18C(FieldTextureManager *textureManager, int slotIndex);
void FieldTextureManager_FreeAllSlots(FieldTextureManager *textureManager);
void FieldTextureManager_Destroy(FieldTextureManager *textureManager);

#endif // POKEHEARTGOLD_FIELD_OVERLAY_01_021EAFD4_H
