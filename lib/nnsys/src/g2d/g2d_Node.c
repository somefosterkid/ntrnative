// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), lib/nnsys/src/g2d/g2d_Node.c.
#include "nnsys/g2d.h"

// NitroSystem's multi-cell nodes. The file name is NitroSystem's: this one function sits alone between the SRT control
// and the cell animation, and only the multi-cell animation calls it

void NNSi_G2dInitializeNode(NNSG2dNode *node, NNSG2dNodeType type) {
    node->pContent = NULL;
    node->type = type;
    node->bVisible = TRUE;
    NNSi_G2dSrtcInitControl(&node->srtCtrl, NNS_G2D_SRTCONTROLTYPE_SRT);
}
