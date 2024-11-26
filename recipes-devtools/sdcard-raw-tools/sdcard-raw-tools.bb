# Copyright (C) 2017, STMicroelectronics - All Rights Reserved
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Script for creating raw SDCARD image ready to flash"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://create_sdcard_from_flashlayout.sh"

S = "${WORKDIR}/sources"
UNPACKDIR = "${S}"

BBCLASSEXTEND = "native nativesdk"

RDEPENDS:${PN}:append = " bash "

RRECOMMENDS:${PN}:append:class-nativesdk = " nativesdk-gptfdisk "

inherit deploy

SCRIPT_DEPLOYDIR ?= "scripts"

do_configure() {
    if [ -e ${UNPACKDIR}/create_sdcard_from_flashlayout.sh ]; then
        bbnote "Update DEFAULT_ROOTFS_PARTITION_SIZE to ${STM32MP_ROOTFS_SIZE}"
        sed 's/^DEFAULT_ROOTFS_PARTITION_SIZE=\${ROOTFS_SIZE:.*$/DEFAULT_ROOTFS_PARTITION_SIZE=\${ROOTFS_SIZE:-'"${STM32MP_ROOTFS_SIZE}"'}/' -i ${UNPACKDIR}/create_sdcard_from_flashlayout.sh
        if [ ${STM32MP_ROOTFS_SIZE} -gt 1887232 ]; then
            # rootfs > 1.8GB then put sdcard raw size = STM32MP_ROOTFS_SIZE + 1.8GB
            raw_size=$(expr ${STM32MP_ROOTFS_SIZE} / 1024 )
            raw_size=$(expr $raw_size + 1843)
            sed 's/DEFAULT_RAW_SIZE=\${SDCARD_SIZE:.*}$/DEFAULT_RAW_SIZE=\${SDCARD_SIZE:-'"$raw_size"'}/' -i ${UNPACKDIR}/create_sdcard_from_flashlayout.sh
        fi

        bbnote "Update DEFAULT_SDCARD_PARTUUID to ${DEVICE_PARTUUID_ROOTFS:SDCARD}"
        sed 's/^DEFAULT_SDCARD_PARTUUID=.*$/DEFAULT_SDCARD_PARTUUID='"${DEVICE_PARTUUID_ROOTFS:SDCARD}"'/' -i ${UNPACKDIR}/create_sdcard_from_flashlayout.sh
        sed 's/^DEFAULT_FIP_TYPEUUID=.*$/DEFAULT_FIP_TYPEUUID='"${DEVICE_TYPEUUID_FIP}"'/' -i ${UNPACKDIR}/create_sdcard_from_flashlayout.sh
        sed 's/^DEFAULT_FIP_A_PARTUUID=.*$/DEFAULT_FIP_A_PARTUUID='"${DEVICE_PARTUUID_FIP_A}"'/' -i ${UNPACKDIR}/create_sdcard_from_flashlayout.sh
        sed 's/^DEFAULT_FIP_B_PARTUUID=.*$/DEFAULT_FIP_B_PARTUUID='"${DEVICE_PARTUUID_FIP_B}"'/' -i ${UNPACKDIR}/create_sdcard_from_flashlayout.sh
        sed 's/^DEFAULT_FWU_MDATA_TYPEUUID=.*$/DEFAULT_FWU_MDATA_TYPEUUID='"${DEVICE_PARTUUID_FWU_MDATA}"'/' -i ${UNPACKDIR}/create_sdcard_from_flashlayout.sh
        sed 's/^DEFAULT_UBOOT_ENV_TYPEUUID=.*$/DEFAULT_UBOOT_ENV_TYPEUUID='"${DEVICE_PARTUUID_UBOOT_ENV}"'/' -i ${UNPACKDIR}/create_sdcard_from_flashlayout.sh
    fi
}

do_install() {
    install -d ${D}/${bindir}
    install -m 0755 ${UNPACKDIR}/create_sdcard_from_flashlayout.sh ${D}/${bindir}
}

do_deploy() {
    :
}
do_deploy:class-native() {
    install -d ${DEPLOYDIR}/${SCRIPT_DEPLOYDIR}
    install -m 0755 ${UNPACKDIR}/create_sdcard_from_flashlayout.sh ${DEPLOYDIR}/${SCRIPT_DEPLOYDIR}/
}
addtask deploy before do_build after do_compile
