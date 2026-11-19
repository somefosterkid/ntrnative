#include "global.h"

void GF_SetG2dRendererSurface(NNSG2dRenderSurface *surface, NNSG2dViewRect *viewRect);

void GF_SetG2dRendererSurface(NNSG2dRenderSurface *surface, NNSG2dViewRect *viewRect) {
    surface->viewRect = *viewRect;
}
