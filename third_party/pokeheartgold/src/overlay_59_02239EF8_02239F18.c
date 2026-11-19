#include "global.h"
#include "overlay_59_apricorn_app.h"

s32 ov59_0223A268(Ov59ApricornApp *);

s32 ov59_0223A2D0(Ov59ApricornApp *, s32 *);

unsigned int ov59_02239F18(Ov59ApricornApp *arg0);

unsigned int ov59_02239F18(Ov59ApricornApp *arg0)
{
  s32 sp0;
  sp0 = 0;
  ov59_0223A2D0(arg0, &sp0);
  if (sp0 == 0)
  {
    ov59_0223A268(arg0);
  }
}
