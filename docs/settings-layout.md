# Settings layout (one tree on every platform)

PC (libntr sim_gui.cpp), Android (SettingsScreen.java) and iOS (ios_settings.m) show the same groups, in this order.
Keys are `SIM_UISettingGet/Set` keys (phones) and sim_config.ini keys (all). Old configs still load.

| Group | Row | Key (ini) | Values | Default |
|---|---|---|---|---|
| Speed | Fast-forward speed | ffspeed (FFSpeed) | 2x 3x 4x Max | 4x |
| | Battle speed | battlespeed | Off 2x 3x 4x | Off |
| | Text speed | textspeed | Game Faster Fastest Instant | Game |
| | Walk / Run / Surf speed | walkspeed runspeed surfspeed | Normal 2x 4x | Normal |
| | Transitions | fadespeed | Normal 2x 4x Instant | Normal |
| | Saving | savespeed | Off 2x 4x | Off |
| | Music at normal speed | musicnormal | on/off | on |
| Rendering | Renderer | renderer | Auto CPU GPU | Auto |
| | Resolution | resolution | 1x..8x | 1x |
| | Antialiasing | antialias | Off, 4 samples, SMAA | Off |
| | 3D renderer | renderer3d | Exact Fast | Exact |
| | 2D filter | filter2d (Filter2D=off/scale2x/mmpx) | 0 Off, 1 Scale2x, 2 MMPX | Scale2x |
| | Texture smoothing | texsmooth (TextureSmoothing=off/bilinear/mips) | 0 Off, 1 Bilinear, 2 Mipmaps | Mipmaps |
| | Smooth motion, Skip frames when slow | smooth, skipslow | on/off | off |
| | Advanced: Lines, Edge marking, Dots, Fog (fog), Colour (color), Graphics API (present) | line edge fog color present | | DS look |
| Sound | Volume, Sound quality | volume, soundquality | | 100, Enhanced |
| Controls | Hide touch controls with a controller | hidetouch | on/off | on |
| Display and metrics | Metrics Off/Compact/Detailed (one row; today overlay + detail), position (corner) | overlay, detail, corner | | Compact, left |
| Game / About | Pause, Restart, Games, Quit; Debug last | | | |

Texture smoothing is for the map only (ic2, 11-22): billboard polygons (field characters, battle Pokemon, reflections: every vertex at one w,
port/gpu/src/g02_spans.c gpu_poly_is_sprite, GPU_PF_SPRITE) keep the DS's texels in Bilinear and Mipmaps; `BWPORT_GPU_SPRITE=0` is the old look.
Mipmaps is now the texture cache's trilinear program (it was the Bilinear one: both set opt.filter); on Metal and with the fast 3D renderer, which
have no texture cache yet, Mipmaps stays the Bilinear look (map-only bilinear).
Notes: 2D filter, Texture smoothing, Fog and Colour act at Resolution 2x and above (1x stays the DS picture). The old `Filter=bilinear`
key loads as Texture smoothing = Bilinear (`Filter=nearest` was the default and is ignored, so old configs get the new default);
`SIM_UISettingGet/Set("filter")` still means "bilinear or more". Environment variables (BWPORT_FILTER2D, BWPORT_GPU_TEXCACHE/
TEXMIPS/TEXMAG, BWPORT_GPU_FOG, BWPORT_GPU_COLOR) override the settings. A value is edited in one place per platform; rows with no
meaning on the platform are hidden, not dimmed.

## Display and metrics (the metrics group, for iOS)

One row, **Metrics: Off / Compact / Detailed** (UISetting key `metrics` 0/1/2; the old `overlay` on/off + `detail` rows are gone), and
**Corner** (`corner`, Top left / Top right) that is greyed (Android) or hidden (iOS) while Metrics is Off. PC: menu "Display and metrics"
(Metrics, corner, controller panel); the cfgscript key `metrics=off|compact|detailed`. Android (SettingsScreen.java) root page, in order:
Speed >, Rendering > (two sub-pages with a Back button next to Done and the system Back key), Sound (Volume, Sound quality), Controller
(Hide touch controls...), Metrics (Metrics, Corner), About. Speed page: Fast-forward, Battle, Text, Transitions, Walk, Run, Surf, Save speed,
Music at normal speed. Rendering page: Renderer, Resolution, Antialiasing, 3D renderer, Texture smoothing (Off/Bilinear/Mipmaps), 2D filter
(Off/Scale2x/MMPX), Lines, Edge marking, Fog, Colour, Smooth motion, Skip frames when slow. Texture smoothing, 2D filter, Lines, Edge marking,
Fog and Colour are greyed at Resolution 1x (footer says so). iOS (ios_settings.m, as built 2026-11-21): the
same root and pages, Speed and Rendering pushed as table controllers with Done on each; the Rendering page in sections Display (Renderer,
Resolution, Antialiasing, 3D renderer), Look at 2x and above (Texture smoothing, 2D filter, Lines, Edge marking, Fog DS / Smooth, Colour
DS / Full: greyed at 1x, the footer says so), Timing (Smooth motion, Skip frames when slow) and Advanced (Graphics API); the old Texture
filter row is gone (`filter` still reads/writes "bilinear or more"). Fog and Colour are two-choice menus on iOS (switches on Android).
The settings test hook takes page=Speed|Rendering|back.
