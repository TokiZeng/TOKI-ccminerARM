

# TOKI-ccminerARM  #verus coin

Mining software designed specifically for mining Verus on ARM.

This version is an extended modification of Oink70's work: https://github.com/Oink70/CCminer-ARM-optimized.

If the original version runs perfectly on your device, you can simply replace the ccminer file. Download the appropriate core version from Releases, extract it, and overwrite the original file.

**TOKI-ccminerARM** is a specially optimized version of ccminer for ARM architecture devices. Initially compiled on an Ubuntu system, this software has been enhanced for performance on various ARM CPUs (such as those found in mobile devices and single-board computers) to deliver effective mining capabilities on ARM-based hardware. This version is specifically optimized for the Verus algorithm, leveraging ARM's unique architecture for improved efficiency.

For a quick installation guide, please refer to https://github.com/TokiZeng/TOKI-ccminerARM/blob/main/miner-setup-guide.md. Follow the steps to complete the installation and update process, and start using TOKI-ccminerARM with ease!


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

# UserLAnd Installation Guide

## Introduction
This is a simple method to install Ubuntu on an Android device. By using the **UserLAnd** app, you can easily set up a full Linux environment without modifying the native Android system. UserLAnd supports multiple Linux distributions (including Ubuntu and Debian) and allows access through SSH or VNC. Another straightforward way to install UserLAnd is via **Google Play**, which is more intuitive, so it won’t be explained here. Below are two alternative methods for installing UserLAnd: one directly on the phone and another using ADB from a computer.

---

## Method 1: Direct Installation on the Phone

1. **Download the APK file**:
   - Use your phone's browser to visit the [UserLAnd GitHub Releases page](https://github.com/CypherpunkArmory/UserLAnd/releases).
   - Find the latest version of **app-release.apk**, then download it to the phone's Downloads folder.

2. **Allow installation from unknown sources**:
   - If you haven't enabled the installation of apps from unknown sources, go to **Settings** > **Security** > **Unknown Sources** or **App Management** > **Special App Access** > **Install Unknown Apps**.
   - Enable this setting for your browser or file manager.

3. **Install the APK file**:
   - Open a file manager and navigate to the **Downloads** folder, then locate **app-release.apk**.
   - Tap the APK file and select **Install**.
   - Once installation is complete, you can find and open UserLAnd in your app list.

---

## Method 2: Installing via ADB from a Computer

1. **Download the APK file**:
   - Use your computer’s browser to visit the [UserLAnd GitHub Releases page](https://github.com/CypherpunkArmory/UserLAnd/releases).
   - Find the latest version of **app-release.apk**, then download it and save it to your computer.

2. **Enable USB debugging**:
   - On your phone, go to **Settings > About phone** and tap on **Build Number** 7 times to enable **Developer Options**.
   - Go back to **Settings**, open **Developer Options**, and enable **USB Debugging**.

3. **Connect the phone and install the APK**:
   - Use a USB cable to connect your phone to the computer.
   - Open a Command Prompt or Terminal window and enter the following command to confirm the device connection:
     ```bash
     adb devices
     ```
   - If your device’s serial number appears, the connection is successful.

4. **Install the APK file**:
   - Run the following command to install the APK file on the phone (replace `/path/to/app-release.apk` with the actual path to the APK file):
     ```bash
     adb install /path/to/app-release.apk
     ```
   - Once the installation is complete, you will find UserLAnd in your app list.

---

After completing these steps, you’ll be able to use UserLAnd on your phone to install and use Ubuntu or other Linux systems. The first method is ideal for downloading and installing directly from your phone, while the second method is convenient for installing remotely through ADB after downloading on a computer.


