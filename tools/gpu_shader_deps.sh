#!/bin/bash
# tools/gpu_shader_deps.sh [--mali]: the offline shader compilers into /opt/gpu-tools (outside the repo), once per machine.
#   glslang 16.6.0 (Khronos' reference GLSL / ESSL front end; the version port/gpu/tools/gpu_msl.py names, so the same binary
#   also regenerates port/gpu/msl) and SPIRV-Cross vulkan-sdk-1.4.363.0 (built with cmake): ~1 min, 60 MB.
#   --mali: Arm's Mali Offline Compiler (malioc), from Arm Performance Studio 2026.6 for Linux (a plain download, no login;
#   800 MB fetched, only mali_offline_compiler/ kept, ~250 MB). Arm Performance Studio is free to use under Arm's EULA
#   (developer.arm.com/tools-and-software/arm-performance-studio, "EULA" beside the download): running this with --mali
#   downloads it under those terms.
# The Android emulator's ANGLE and SwiftShader (tools/android_deps.sh) and Mesa are the other compilers
# port/gpu/tools/gpu_glsl_check.py runs; Qualcomm's Adreno offline compiler needs a Qualcomm ID (docs/gpu-compat.md).
set -e
D=/opt/gpu-tools
APS=2026.6
mkdir -p $D
cd $D
if [ ! -x glslang/bin/glslang ]; then
  curl -fsSL -o g.tgz https://github.com/KhronosGroup/glslang/releases/download/16.6.0/glslang-16.6.0-linux-x86_64-release.tar.gz
  mkdir -p glslang && tar xzf g.tgz -C glslang && rm g.tgz
fi
glslang/bin/glslang --version | head -1
if [ ! -x spirv-cross/build/spirv-cross ]; then
  [ -d spirv-cross ] || git clone -q --depth 1 -b vulkan-sdk-1.4.363.0 https://github.com/KhronosGroup/SPIRV-Cross spirv-cross
  cmake -S spirv-cross -B spirv-cross/build -DCMAKE_BUILD_TYPE=Release -DSPIRV_CROSS_ENABLE_TESTS=OFF > /dev/null
  cmake --build spirv-cross/build -j "${BW_JOBS:-8}" --target spirv-cross > /dev/null
fi
echo "spirv-cross: $D/spirv-cross/build/spirv-cross"
if [ "${1:-}" = --mali ] && ! ls arm-ps/*/mali_offline_compiler/malioc > /dev/null 2>&1; then
  curl -fsSL -o aps.tgz https://artifacts.tools.arm.com/arm-performance-studio/$APS/Arm_Performance_Studio_${APS}_linux_x86-64.tgz
  mkdir -p arm-ps
  tar xzf aps.tgz -C arm-ps Arm_Performance_Studio_$APS/mali_offline_compiler Arm_Performance_Studio_$APS/readme.txt
  rm aps.tgz
fi
ls arm-ps/*/mali_offline_compiler/malioc 2> /dev/null && arm-ps/*/mali_offline_compiler/malioc --version | head -1 || true
