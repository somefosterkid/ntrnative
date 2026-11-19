#include "global.h"

#include "overlay_67_app.h"
#include "touchscreen.h"
#include "unk_02019BA4.h"

extern GridCallbacks ov67_021E6D6C;
extern TouchscreenHitbox ov67_021E6E60;
extern DpadMenuBox ov67_021E6EE8;

void ov67_021E6BB8(Ov67App *app) {
    ov67_021E6B6C(app, 1);
}

void ov67_021E6BC4(Ov67App *app) {
    app->unk_4A4 = GridInputHandler_Create(&ov67_021E6E60, &ov67_021E6EE8, &ov67_021E6D6C, app, 1, 0U, app->heapID);
    ov67_021E6C14(app, 0);
}
