

# TOKI-ccminerARM

**TOKI-ccminerARM** is a specially optimized version of ccminer for ARM architecture devices. Initially compiled on an Ubuntu system, this software has been enhanced for performance on various ARM CPUs (such as those found in mobile devices and single-board computers) to deliver effective mining capabilities on ARM-based hardware.

### Pre-compiled Versions

To simplify usage, TOKI-ccminerARM offers pre-compiled versions for different ARM cores (e.g., Cortex-A53 and Cortex-A73). These versions allow you to get started quickly on compatible devices without the need to compile the software yourself.

### Compilation Steps

If you prefer to compile the software from source to suit your specific device architecture or system configuration, please follow the detailed steps below. Ensure that your device meets the necessary dependencies and that the required tools are installed.

---

## ccminer for ARM Compilation Steps

### Git and Build Process

```
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev libssl-dev libjansson-dev automake autotools-dev build-essential -y
sudo apt-get install -y libllvm-16-ocaml-dev libllvm16 llvm-16 llvm-16-dev llvm-16-doc llvm-16-examples llvm-16-runtime clang-16 clang-tools-16 clang-16-doc libclang-common-16-dev libclang-16-dev libclang1-16 clang-format-16 python3-clang-16 clangd-16 clang-tidy-16 libclang-rt-16-dev libpolly-16-dev libfuzzer-16-dev lldb-16 lld-16 libc++-16-dev libc++abi-16-dev libomp-16-dev libclc-16-dev libunwind-16-dev libmlir-16-dev mlir-16-tools flang-16 libclang-rt-16-dev-wasm32 libclang-rt-16-dev-wasm64 libclang-rt-16-dev-wasm32 libclang-rt-16-dev-wasm64 gcc-11 g++-11
git clone https://github.com/TokiZeng/TOKI-ccminerARM.git
cd TOKI-ccminerARM
chmod +x build.sh
chmod +x configure.sh
chmod +x autogen.sh
CXX=clang++ CC=clang build.sh
```

For specific details on installing clang-16 on your current OS, check: https://apt.llvm.org/

### Compilation Details

Before making any modifications to the compilation settings, it’s recommended to first compile the code as-is to ensure there are no errors and that the process completes correctly.

You can edit `configure.sh` and replace the `-march=` and `-mcpu=` flags according to your requirements. For example:

- `-march=armv8.2-a+crypto -mcpu=cortex-a76`
- `-march=armv8-a+crypto -mcpu=cortex-a53`

After making the modifications, run the following command to compile:

```
CXX=clang++ CC=clang ./build.sh
```

This will complete the compilation.
