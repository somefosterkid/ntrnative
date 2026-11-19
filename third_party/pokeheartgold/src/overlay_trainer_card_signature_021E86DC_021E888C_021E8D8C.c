#include "global.h"
#include "signature_app.h"

void ov52_021E8D3C(SignatureApp *signatureApp);

void ov52_021E8D64(struct BgConfig *bgConfig, struct YesNoPrompt *menu);

int ov52_021E92E8(int printerID);

void ov52_021E9488(SignatureApp *signatureApp, s32 arg1);

int ov52_021E8D8C(SignatureApp *signatureApp, int state) {
    if (ov52_021E92E8(signatureApp->printerID) != 0) {
        ov52_021E8D64(signatureApp->bgConfig, signatureApp->yesNoMenu);
        ov52_021E9488(signatureApp, 1);
        signatureApp->subState = 3;
    }
    ov52_021E8D3C(signatureApp);
    return state;
}
