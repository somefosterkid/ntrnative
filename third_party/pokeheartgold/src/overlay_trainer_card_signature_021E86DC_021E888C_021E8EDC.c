#include "global.h"
#include "signature_app.h"

void ov52_021E8D3C(SignatureApp *signatureApp);

void ov52_021E927C(SignatureApp *signatureApp, int bankEntry);

int ov52_021E8EDC(SignatureApp *signatureApp, int state) {
    ov52_021E927C(signatureApp, 0xb);
    signatureApp->subState = 5;
    ov52_021E8D3C(signatureApp);
    return state;
}
