# Adaptation from u-boot-common_${PV}.inc

HOMEPAGE = "http://www.denx.de/wiki/U-Boot/WebHome"
SECTION = "bootloaders"

LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://Licenses/README;md5=2ca5f2c35c8cc335f0a19756634782f1"

CVE_PRODUCT = "denx:u-boot"

DEPENDS += "dtc-native bc-native"
DEPENDS += "flex-native bison-native"
DEPENDS += "python3-setuptools-native"

SRC_URI = "git://github.com/u-boot/u-boot.git;protocol=https;branch=master"
SRCREV  = "4459ed60cb1e0562bc5b40405e2b4b9bbf766d57"

SRC_URI += "\
    file://0001-v2023.10-stm32mp-r1.patch \
    file://0099-Add-external-var-to-allow-build-of-new-devicetree-fi.patch \
    "

# debug and trace
SRC_URI += " ${@bb.utils.contains('ST_UBOOT_DEBUG_TRACE', '1', '', 'file://0098-silent_mode.patch', d)} "
SRC_URI += "${@bb.utils.contains('ST_UBOOT_DEBUG_TRACE', '1', '', 'file://fragment-01-silent_mode.cfg;subdir=fragments/features', d)}"

# optional config
SRC_URI += "${@bb.utils.contains('UBOOT_SIGN_ENABLE', '1', 'file://fragment-02-signature.cfg;subdir=fragments/features', '', d)}"

# efi config
SRC_URI += "${@bb.utils.contains('MACHINE_FEATURES', 'efi', 'file://fragment-03-efi.cfg;subdir=fragments/features', '', d)}"

U_BOOT_VERSION = "v2023.10"
U_BOOT_SUBVERSION = "stm32mp"
U_BOOT_RELEASE = "r1"

PV = "${U_BOOT_VERSION}-${U_BOOT_SUBVERSION}-${U_BOOT_RELEASE}"

ARCHIVER_ST_BRANCH = "${U_BOOT_VERSION}-${U_BOOT_SUBVERSION}"
ARCHIVER_ST_REVISION = "${PV}"
ARCHIVER_COMMUNITY_BRANCH = "master"
ARCHIVER_COMMUNITY_REVISION = "${U_BOOT_VERSION}"

S = "${WORKDIR}/git"

# ---------------------------------
# Configure devupstream class usage
# ---------------------------------
BBCLASSEXTEND = "devupstream:target"

SRC_URI:class-devupstream = "git://github.com/STMicroelectronics/u-boot.git;protocol=https;branch=${ARCHIVER_ST_BRANCH}"
SRCREV:class-devupstream = "96c47fdebde887c954f5aced90ac9e32b7787091"

# ---------------------------------
# Configure default preference to manage dynamic selection between tarball and github
# ---------------------------------
STM32MP_SOURCE_SELECTION ?= "tarball"

DEFAULT_PREFERENCE = "${@bb.utils.contains('STM32MP_SOURCE_SELECTION', 'github', '-1', '1', d)}"
