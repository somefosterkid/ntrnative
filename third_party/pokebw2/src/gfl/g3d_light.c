#include "types.h"
#include "gfl/g3d.h"
#include "gfl/heap.h"

#define G3D_LIGHT_MAX 4

struct G3DLight {
    Light lights[G3D_LIGHT_MAX];
};

G3DLight *GFL_G3DLightCreate(const LightSetupList *setup, HeapID heapId) {
    G3DLight *lights = GFL_HeapAllocate(heapId, sizeof(G3DLight), TRUE, "g3d_light.c", 43);
    int i;

    for (i = 0; i < setup->count; i++) {
        lights->lights[setup->lights[i].index] = setup->lights[i].light;
    }
    return lights;
}

void GFL_G3DLightFree(G3DLight *lights) {
    GFL_HeapFree(lights);
}

void GFL_G3DLightFlush(G3DLight *lights) {
    int i;

    for (i = 0; i < G3D_LIGHT_MAX; i++) {
        GFL_G3DSysLightSet(i, &lights->lights[i]);
    }
}

void GFL_G3DLightGetDirVector(G3DLight *lights, u8 lightId, VecFx16 *direction) {
    *direction = lights->lights[lightId].direction;
}

void GFL_G3DLightSetDirVector(G3DLight *lights, u8 lightId, const VecFx16 *direction) {
    lights->lights[lightId].direction = *direction;
}

void GFL_G3DLightGetColor(G3DLight *lights, u8 lightId, GXRgb *color) {
    *color = lights->lights[lightId].color;
}

void GFL_G3DLightSetColor(G3DLight *lights, u8 lightId, const GXRgb *color) {
    lights->lights[lightId].color = *color;
}
