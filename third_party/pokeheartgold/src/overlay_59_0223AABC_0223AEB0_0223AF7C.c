#include "global.h"
#include "overlay_59_juice_feed_app.h"

s32 ov59_0223B358(Ov59JuiceFeedApp *);

s32 ov59_0223B3B0(Ov59JuiceFeedApp *, s32 *);

s32 ov59_0223AF7C(Ov59JuiceFeedApp *arg0);

s32 ov59_0223AF7C(Ov59JuiceFeedApp *arg0)
{
  s32 sp0;
#if defined(SDK_PORT) || defined(NONMATCHING)
  s32 ret;
#endif
  sp0 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
  ret = ov59_0223B3B0(arg0, &sp0);
  if (sp0 == 0)
  {
    ret = ov59_0223B358(arg0);
  }
  return ret; /* r0 on DS = result of the last call made */
#else
  ov59_0223B3B0(arg0, &sp0);
  if (sp0 == 0)
  {
    ov59_0223B358(arg0);
  }
#endif
}
