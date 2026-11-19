#include "touchscreen.h"
#include "touchscreen.h"
extern u8 gSystem;
extern u8 gSystem;

u32 ov112_021F0EB4(u8 *data)
{
  if (data[0x13d] != 0) {
    TouchscreenHitbox hitbox = {{0xA0, 0xC0, 0x10, 0x30}};
    if (TouchscreenHitbox_TouchNewIsIn(&hitbox) || (((u32 *)&gSystem)[0x12] & 0x20) != 0) {
      return 1;
    }
  }
  return 0;
}
