#include "global.h"

#include "filesystem.h"
#include "geonet_globe.h"
#include "gf_3d_render.h"
#include "heap.h"

void ov69_021E6E88(GeonetGlobe *globe, NARC *narc);

void ov69_021E6E88(GeonetGlobe *globe, NARC *narc) {
    globe->globeResource = NARC_AllocAndReadWholeMember(narc, 0U, globe->heapID);
    GF3dRender_InitObjFromHeader(&globe->globeRenderObj, &globe->globeModel, &globe->globeResource);
    globe->markerResources[3] = NARC_AllocAndReadWholeMember(narc, 1U, globe->heapID);
    GF3dRender_InitObjFromHeader(&globe->markerRenderObjs[3], &globe->markerModels[3], &globe->markerResources[3]);
    globe->markerResources[4] = NARC_AllocAndReadWholeMember(narc, 2U, globe->heapID);
    GF3dRender_InitObjFromHeader(&globe->markerRenderObjs[4], &globe->markerModels[4], &globe->markerResources[4]);
    globe->markerResources[1] = NARC_AllocAndReadWholeMember(narc, 3U, globe->heapID);
    GF3dRender_InitObjFromHeader(&globe->markerRenderObjs[1], &globe->markerModels[1], &globe->markerResources[1]);
    globe->markerResources[2] = NARC_AllocAndReadWholeMember(narc, 4U, globe->heapID);
    GF3dRender_InitObjFromHeader(&globe->markerRenderObjs[2], &globe->markerModels[2], &globe->markerResources[2]);
}

void ov69_021E6F48(GeonetGlobe *globe);

void ov69_021E6F48(GeonetGlobe *globe) {
    Heap_Free(globe->markerResources[2]);
    Heap_Free(globe->markerResources[1]);
    Heap_Free(globe->markerResources[4]);
    Heap_Free(globe->markerResources[3]);
    Heap_Free(globe->globeResource);
}

void ov69_021E6F8C(GeonetGlobe *globe);

void ov69_021E6F8C(GeonetGlobe *globe) {
    globe->renderOrigin.x = 0;
    globe->renderOrigin.y = 0;
    globe->renderOrigin.z = 0;
    globe->globeScale.x = 0x1000;
    globe->globeScale.y = 0x1000;
    globe->globeScale.z = 0x1000;
    globe->rotationAngle.x = 0x1A40;
    globe->rotationAngle.y = 0x7C00;
    globe->rotationAngle.z = 0;
    globe->markerScale.x = 0x1000;
    globe->markerScale.y = 0x1000;
    globe->markerScale.z = 0x1000;
}
