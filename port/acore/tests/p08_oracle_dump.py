import sys, struct
m = open(sys.argv[1], 'rb').read()
b = 0x0237D000 - 0x02000000
w = m[b:b + 0x1180]
for ch in range(16):
    c = w[ch * 0x54:(ch + 1) * 0x54]
    if not (c[3] & 1): continue
    (mn, ty, es, fl, pr, ok, ud2, key, vel, ip, up, ud, upit) = struct.unpack('<BBBBBBhBBbbhh', c[:16])
    print('ch', ch, 'flags', fl, 'pan_range', pr, 'key', key, 'vel', vel, 'init_pan', ip, 'user_pan', up,
          'user_decay', ud, 'user_decay2', ud2, 'user_pitch', upit, 'lfo', c[0x28:0x32].hex(), 'prio', c[0x22], 'pan', c[0x23])
t = w[0x780:0x780 + 0x40]
print('track0', t.hex())
print('player0', w[0x540:0x540 + 0x24].hex())
