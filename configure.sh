# To change the cuda arch, edit Makefile.am and run ./build.sh

extracflags="-O3 -ffinite-loops -ffast-math -mfix-cortex-a53-835769 -D_REENTRANT -falign-functions=16 -fomit-frame-pointer -fpic -pthread -flto -fuse-ld=lld -fno-stack-protector -fslp-vectorize -fvectorize"

./configure CXXFLAGS="--gcc-toolchain=/usr -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize -Wl,-hugetlbfs-align -funroll-loops -finline-functions $extracflags -march=armv8-a+crypto -mcpu=cortex-a73" \
CFLAGS="--gcc-toolchain=/usr -finline-functions -Wl,-hugetlbfs-align -march=armv8-a+crypto -mcpu=cortex-a73 -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize $extracflags" \
CXX=clang++ CC=clang LDFLAGS="-v -flto -Wl,-hugetlbfs-align"