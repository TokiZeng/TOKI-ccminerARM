# To change the cuda arch, edit Makefile.am and run ./build.sh

extracflags="-O3 -ffinite-loops -ffast-math -mfix-cortex-a53-835769 -D_REENTRANT -falign-functions=16 -fomit-frame-pointer -fpic -pthread -flto -fno-stack-protector -fslp-vectorize -fvectorize"

./configure CXXFLAGS="--gcc-toolchain=/usr -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize -funroll-loops -finline-functions $extracflags -march=armv8.2-a+crypto -mcpu=cortex-a55 -mtune=cortex-a55" \
CFLAGS="--gcc-toolchain=/usr -finline-functions -march=armv8.2-a+crypto -mcpu=cortex-a55 -mtune=cortex-a55 -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize $extracflags" \
CXX=clang++ CC=clang LDFLAGS="-v -Wl,-hugetlbfs-align -fuse-ld=lld"
