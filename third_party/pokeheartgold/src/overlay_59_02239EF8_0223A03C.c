#include "global.h"
#include "overlay_59_apricorn_app.h"

s32 ov59_0223A3B0(Ov59ApricornApp *);

s32 ov59_0223A418(Ov59ApricornApp *, s32 *);

s32 ov59_0223A03C(Ov59ApricornApp *arg0);

s32 ov59_0223A03C(Ov59ApricornApp *arg0)
{
  s32 sp0;
#if defined(SDK_PORT) || defined(NONMATCHING)
  s32 ret;
#endif
  sp0 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
  ret = ov59_0223A418(arg0, &sp0);
  if (sp0 == 0)
  {
    ret = ov59_0223A3B0(arg0);
  }
  return ret; /* r0 on DS = result of the last call made */
#else
  ov59_0223A418(arg0, &sp0);
  if (sp0 == 0)
  {
    ov59_0223A3B0(arg0);
  }
#endif
}

s32 ov59_0223A58C(Ov59ApricornApp *);

s32 ov59_0223A614(Ov59ApricornApp *, s32 *);

int ov59_0223A05C(Ov59ApricornApp *arg0);

int ov59_0223A05C(Ov59ApricornApp *arg0)
{
  s32 sp0;
#if defined(SDK_PORT) || defined(NONMATCHING)
  s32 ret;
#endif
  sp0 = 0;
#if defined(SDK_PORT) || defined(NONMATCHING)
  ret = ov59_0223A614(arg0, &sp0);
  if (sp0 == 0)
  {
    ret = ov59_0223A58C(arg0);
  }
  return ret; /* r0 on DS = result of the last call made */
#else
  ov59_0223A614(arg0, &sp0);
  if (sp0 == 0)
  {
    ov59_0223A58C(arg0);
  }
#endif
}
