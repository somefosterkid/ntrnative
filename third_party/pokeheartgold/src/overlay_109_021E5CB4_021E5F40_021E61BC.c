#include "global.h"

#include "overlay_109_photo_album_app.h"

short ov109_021E61BC(PhotoAlbumApp *app) {
    s32 v0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    s32 ret;
#endif

    v0 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
    ret = ov109_021E638C(app, &v0);
    if (v0 == 0) {
        ret = ov109_021E63E8(app);
    }
    return ret; /* r0 on DS = result of the last call made */
#else
    ov109_021E638C(app, &v0);
    if (v0 == 0) {
        ov109_021E63E8(app);
    }
#endif
}
