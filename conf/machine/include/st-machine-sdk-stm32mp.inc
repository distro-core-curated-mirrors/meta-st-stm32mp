#@DESCRIPTION: definition of sdk content for STM32 systems

# Make sure to provide all expected tools in SDK
ST_TOOLS_FOR_SDK = " \
    nativesdk-gcc-arm-none-eabi \
    nativesdk-binutils \
    nativesdk-openocd-stm32mp \
    nativesdk-stm32wrapper4dbg \
    nativesdk-sdcard-raw-tools \
    nativesdk-ncurses-libncursesw \
    nativesdk-perf \
    "

# Add tools to build native application with SDK
# Currently mandatory for kernel and u-boot
ST_TOOLS_FOR_SDK:append = " \
    nativesdk-cpp-symlinks \
    nativesdk-gcc-symlinks \
    "

# For support of perl ansicolor
ST_TOOLS_FOR_SDK_PERL = " \
    nativesdk-perl-module-term-ansicolor \
    nativesdk-perl-module-encode \
    nativesdk-perl-module-encode-encoding \
    nativesdk-perl-module-encode-mime-header \
"

# For some scripts in kernel source code
ST_TOOLS_FOR_SDK_PERL:append = " \
    nativesdk-perl-module-file-spec-functions \
    nativesdk-perl-module-integer \
    nativesdk-perl-module-pod-text \
"

ST_TOOLS_FOR_SDK:append = " ${ST_TOOLS_FOR_SDK_PERL} "

# For support of python module for u-boot
ST_TOOLS_FOR_SDK:append = " \
    nativesdk-python3-dev \
    nativesdk-python3-setuptools \
    nativesdk-swig \
    nativesdk-dtc \
    nativesdk-flex \
    "

# For support of python module for optee-os
ST_TOOLS_FOR_SDK:append = " \
    nativesdk-python3-pyelftools \
    nativesdk-python3-pycryptodomex \
    nativesdk-python3-pillow \
    nativesdk-python3-cffi \
    nativesdk-python3-pycparser \
    nativesdk-python3-cryptography \
"

# For support of python module for tf-m
ST_TOOLS_FOR_SDK:append = " \
    nativesdk-python3-pyyaml \
    nativesdk-ninja \
"

# For support of string convertion (iconv) in SDK
ST_TOOLS_FOR_SDK:append = " \
    nativesdk-glibc-gconv-utf-16 \
    nativesdk-glibc-gconv-utf-32 \
    "
# For support wayland-scanner in SDK
ST_TOOLS_FOR_SDK:append = " \
    nativesdk-wayland \
    "

# For Kernel yaml dependency
ST_TOOLS_FOR_SDK:remove = " \
    nativesdk-libyaml \
    nativesdk-libyaml-dev \
    "

# For CMSIS-SVD file parser with GDB
ST_TOOLS_FOR_SDK:append = " \
    nativesdk-svd-tools \
    "

# For Trusted Application development using OP-TEE TA SDK
ST_TOOLS_FOR_SDK:append = " \
    nativesdk-optee-sdk \
    "

# Fip tool + metadata_v2 generation
ST_TOOLS_FOR_SDK:append = " \
    nativesdk-tf-a-tools \
    nativesdk-u-boot-tools-stm32mp \
    "

# for igt-gpu-tools
ST_TOOLS_FOR_SDK:append:stm32mp2common = " \
    nativesdk-libpciaccess \
    "

# Make sure to append mkimage to SDK for kernel uImage build
ST_DEPENDENCIES_BUILD_FOR_SDK = " \
    ${@bb.utils.contains('KERNEL_IMAGETYPE', 'uImage', 'nativesdk-u-boot-mkimage', '', d)} \
    ${@bb.utils.contains('KERNEL_ALT_IMAGETYPE', 'uImage', 'nativesdk-u-boot-mkimage', '', d)} \
  "
# Make sure to append openssl to SDK for kernel-module and scripts build
ST_DEPENDENCIES_BUILD_FOR_SDK:append = " nativesdk-openssl-dev "

# Make sure to append bison to SDK for u-boot build
ST_DEPENDENCIES_BUILD_FOR_SDK:append = " \
    ${@bb.utils.contains('EXTRA_IMAGEDEPENDS', 'u-boot-stm32mp', 'nativesdk-bison', '', d)} \
  "

# for populate_sdk, we will have all the tools
TOOLCHAIN_HOST_TASK:append = " ${ST_TOOLS_FOR_SDK} "
TOOLCHAIN_HOST_TASK:append = " ${ST_DEPENDENCIES_BUILD_FOR_SDK} "
# for populate_sdk_ext, the tools are not desired as mandatory tools (aka basic
# tools for devtool)
TOOLCHAIN_HOST_TASK:remove:task-populate-sdk-ext = " ${ST_TOOLS_FOR_SDK} "
TOOLCHAIN_HOST_TASK:remove:task-populate-sdk-ext = " ${ST_DEPENDENCIES_BUILD_FOR_SDK} "
# buildtools is used only by populate_sdk_ext
# populate_sdk_ext = buildtools + uninative + layer
# buildtools: sdk part of esdk (like sdk generated by populate_sdk)
# uninative: basic tools for devtool
TOOLCHAIN_HOST_TASK:remove:pn-buildtools-tarball = " ${ST_TOOLS_FOR_SDK_PERL} "

# kernel headers
ST_TOOLS_FOR_TARGET_SDK = " \
    kernel-headers \
    "


TOOLCHAIN_TARGET_TASK += " bash-dev libgomp-dev ${ST_TOOLS_FOR_TARGET_SDK}"
TOOLCHAIN_TARGET_TASK:remove:pn-buildtools-tarball = " bash-dev libgomp-dev ${ST_TOOLS_FOR_TARGET_SDK}"
