SUMMARY = "Trusted Firmware for Cortex-M"
DESCRIPTION = "Trusted Firmware-M"
HOMEPAGE = "https://git.trustedfirmware.org/trusted-firmware-m.git"
PROVIDES = "virtual/trusted-firmware-m"

LICENSE = "BSD-3-Clause & Apache-2.0"

LIC_FILES_CHKSUM = "file://license.rst;md5=07f368487da347f3c7bd0fc3085f3afa"

SRC_URI = "git://git.trustedfirmware.org/TF-M/trusted-firmware-m.git;protocol=https;nobranch=1;name=tfm"
SRC_URI:append = " git://git.trustedfirmware.org/TF-M/tf-m-tests.git;protocol=https;nobranch=1;name=tfm-tests;destsuffix=git/external/tf-m-tests"
SRC_URI:append = " git://github.com/ARMmbed/mbedtls.git;protocol=https;branch=master;name=mbedtls;destsuffix=git/external/mbedtls"
SRC_URI:append = " git://github.com/mcu-tools/mcuboot.git;protocol=https;branch=main;name=mcuboot;destsuffix=git/external/mcuboot"
SRC_URI:append = " git://github.com/laurencelundblade/QCBOR.git;protocol=https;branch=master;name=qcbor;destsuffix=git/external/qcbor"
SRC_URI:append = " git://github.com/STMicroelectronics/stm32-ddr-phy-binary;protocol=https;branch=main;name=ddr-phy;destsuffix=git/external/stm32-ddr-phy-binary"

SRC_URI:append = " git://github.com/STMicroelectronics/SCP-firmware;protocol=https;name=scp-firmware;destsuffix=git/external/scp-firmware;branch=v2.13.0-stm32mp-w2438"


# The required dependencies are documented in tf-m/config/config_base.cmake
# TF-Mv1.7.0
SRCREV_tfm = "b725a1346cdb9ec75b1adcdc4c84705881e8fd4e"
# mbedtls-3.2.1
SRCREV_mbedtls = "869298bffeea13b205343361b7a7daf2b210e33d"
# TF-Mv1.7.0++
SRCREV_tfm-tests = "4c4b58041c6c01670266690538a780b4a23d08b8"
# v1.9.0
SRCREV_mcuboot = "c657cbea75f2bb1faf1fceacf972a0537a8d26dd"

# v1.1++
SRCREV_qcbor = "b0e7033268e88c9f27146fa9a1415ef4c19ebaff"

SRCREV_ddr-phy = "77447cf214eadf128e487fcb10a4a78cd4ab6d56"

SRCREV_scp-firmware = "881f63ac7f5bc975973380b23f1a19af8c70f598"

SRCREV_FORMAT = "tfm"

UPSTREAM_CHECK_GITTAGREGEX = "^TF-Mv(?P<pver>\d+(\.\d+)+)$"

# ST Patches
SRC_URI += " \
    file://0001-v1.7.0-stm32mp25-r13.patch \
    \
    file://0001-Build-Add-stub-functions-of-system-calls.patch;patchdir=external/tf-m-tests \
    file://0002-app-start-cortexA35-in-ns-thread.patch;patchdir=external/tf-m-tests \
    file://0003-app-fix-a35-build-error-on-cpu-interface.patch;patchdir=external/tf-m-tests \
"
SRC_URI[dt-png.sha256sum] = "04d887fd02bfa6b2296f69cc5389fc4d699b6b42a9dd72e302b0369343533293"
SRC_URI[ddr-fwm.sha256sum] = "176647ca369fed92cf4f297dc4e84b4f93048f86bcebd81315656e600ce34ecd"

TF_M_VERSION = "v1.7.0"
TF_M_SUBVERSION = "stm32mp25"
TF_M_RELEASE = "r13"
PV = "${TF_M_VERSION}-${TF_M_SUBVERSION}-${TF_M_RELEASE}"

include tf-m-stm32mp-common.inc

# ---------------------------------
# Configure archiver use
# ---------------------------------
include ${@oe.utils.ifelse(d.getVar('ST_ARCHIVER_ENABLE') == '1', 'tf-m-stm32mp-archiver.inc','')}
