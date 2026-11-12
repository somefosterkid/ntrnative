# tools/hwfit: the hardware fitter

A permuter for hardware arithmetic: the computer searches parameter combinations against the melonDS oracle (black box
only, never its source) instead of agents guessing.

    python3 tools/hwfit/hwfit.py all --n 3000 --wmode flat --dir D     gen + render (oracle, 3D layer png) + pack D/probes.bin
        --wmode flat|persp|mixed|vgrad   (vgrad: apex over a flat bottom, colour depends on the row only)
    gcc -O2 -fopenmp -o hwfit_raster tools/hwfit/hwfit_raster.c
    hwfit_raster D --steep --set base=0,xmaj=0 --top 8 --misses 12      (--list shows every parameter; MISSW=w filters misses by row width)
    fit1d / hyp1.py style one-off analyses live in tools/hwfit/analysis/

Needs build/vcore/vcore_render (meson setup build/vcore port/vcore; ninja -C build/vcore) for the screen vertices (--geo).
Coverage (each row's first/last pixel) is taken from the oracle: this tool fits colour arithmetic, not edge rules.
