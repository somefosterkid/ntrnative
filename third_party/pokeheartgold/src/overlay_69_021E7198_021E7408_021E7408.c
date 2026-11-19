#include "global.h"

#include "camera.h"
#include "geonet_globe.h"
#include "gf_3d_render.h"
#include "unk_02026E30.h"

extern const MtxFx33 ov69_021E7728;

extern const MtxFx33 ov69_021E774C;

extern const MtxFx33 ov69_021E7794;


void ov69_021E70A8(MtxFx33 *param0, const VecFx32 *param1);

void ov69_021E7408(GeonetGlobe *globe);

void ov69_021E7408(GeonetGlobe *globe) {
    MtxFx33 v0;
    MtxFx33 v1;
    MtxFx33 v2;
    u32 i;

    v0 = ov69_021E7728;

    switch (globe->renderMode) {
    case 0:
        break;
    case 2:
        Thunk_G3X_Reset();
        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
        globe->renderMode = 0;
        break;
    case 1:
        Thunk_G3X_Reset();
        Camera_PushLookAtToNNSGlb();

        ov69_021E70A8(&v0, &globe->rotationAngle);
        GF3dRender_DrawModel(&globe->globeRenderObj, &globe->renderOrigin, &v0, &globe->globeScale);

        v1 = ov69_021E7794;
        GF3dRender_DrawModel(&globe->markerRenderObjs[4], &globe->renderOrigin, &v1, &globe->markerScale);

        v2 = ov69_021E774C;
        for (i = 0; i < globe->markers.count; i++) {
            MTX_Concat33(&globe->markers.markers[i].transform, &v0, &v2);

            if (globe->markers.markers[i].recency != 0) {
                GF3dRender_DrawModel(&globe->markerRenderObjs[globe->markers.markers[i].recency], &globe->renderOrigin, &v2, &globe->markerScale);
            }
        }

        RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_W);
        break;
    }
}
