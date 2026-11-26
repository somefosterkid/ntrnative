#!/bin/bash
# tools/boot_stress_sdlfail.sh [BOOTS]: boots of build/port/before/bwport (tools/boot_stress_ab.sh makes it) under gdb with SDL_Init forced
# to fail (tools/boot_stress_sdlfail.gdb): prints the `sdlfail:` lines (SDL_Init forced, SDL_OpenAudio entered, the window title's heap
# chunk size word at SDL_CreateWindow and at frame 40). A boot that hangs in SIM_Audio_Init ends by port_run's --hang.
cd "$(dirname "$0")/.." || exit 2
D=build/boot_stress/sdlfail
[ -d $D/w0 ] || bash tools/boot_stress.sh -n 1 -j 1 -o $D > /dev/null 2>&1
for ((i = 1; i <= ${1:-3}; i++)); do
    BWPORT_BUILD=build/port/before python3 tools/port_run.py --no-build --game black --frames 300 -t 50 --hang 20 --shots 0 --dir $D/w0 --out $D/out$i \
        --gdb "source $PWD/tools/boot_stress_sdlfail.gdb" 2>&1 | tail -1 | cut -c1-200
    grep -a "sdlfail" $D/out$i/log.txt | head -6 | cut -c1-200
done
rm -rf $D
