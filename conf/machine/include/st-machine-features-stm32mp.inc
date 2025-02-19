#@DESCRIPTION: Machine features definitions for STM32 systems

include conf/machine/include/gpu_vivante.inc

# =========================================================================
# gpu
# =========================================================================
# To support gpu on a machine,
# please add "gpu" to MACHINE_FEATURES in machine conf file
GPU_LIST ?= "\
    gcnano-driver-stm32mp \
    ${GPU_USERLAND_LIBRARIES_INSTALL} \
    "
GPU_IMAGE_INSTALL = "${@bb.utils.contains('MACHINE_FEATURES', 'gpu', '${GPU_LIST}', '', d)} "

# =========================================================================
# alsa
# =========================================================================
ALSA_LIST ?= ""
ALSA_LIST:stm32mp1common ?= " alsa-state-stm32mp1"
ALSA_LIST:stm32mp2common ?= " alsa-state-stm32mp2"
ALSA_IMAGE_INSTALL = "${@bb.utils.contains('COMBINED_FEATURES', 'alsa', '${ALSA_LIST}', '', d)} "

# =========================================================================
# bluetooth
# =========================================================================
BLUETOOTH_LIST ?= ""
BLUETOOTH_IMAGE_INSTALL = "${@bb.utils.contains('MACHINE_FEATURES', 'bluetooth', '${BLUETOOTH_LIST}', '', d)}"

# =========================================================================
# m4copro
# =========================================================================
M4COPRO_LIST ?= "\
    m4fwcoredump \
    m4projects-stm32mp1 \
    "
M4COPRO_IMAGE_INSTALL = "${@bb.utils.contains('MACHINE_FEATURES', 'm4copro', '${M4COPRO_LIST}', '', d)}"

# =========================================================================
# m3copro
# =========================================================================
M33COPRO_LIST ?= "\
    m33projects-stm32mp2 \
    "
M33COPRO_IMAGE_INSTALL = "${@bb.utils.contains('MACHINE_FEATURES', 'm33copro', '${M33COPRO_LIST}', '', d)}"


# =========================================================================
# optee
# =========================================================================
OPTEE_LIST ?= "\
    virtual-optee-os \
    "
OPTEE_IMAGE_INSTALL = "${@bb.utils.contains('COMBINED_FEATURES', 'optee', '${OPTEE_LIST}', '', d)} "

OPTEE_BINARY ?= "\
    virtual-optee-os \
    "
OPTEE_BINARY_INSTALL = "${@bb.utils.contains('MACHINE_FEATURES', 'optee', '${OPTEE_BINARY}', '', d)} "

# =========================================================================
# pci
# =========================================================================
PCI_LIST ?= ""
PCI_IMAGE_INSTALL = "${@bb.utils.contains('MACHINE_FEATURES', 'pci', '${PCI_LIST}', '', d)} "

# =========================================================================
# wifi
# =========================================================================
WIFI_LIST ?= ""
WIFI_IMAGE_INSTALL = "${@bb.utils.contains('MACHINE_FEATURES', 'wifi', '${WIFI_LIST}', '', d)} "

# =========================================================================
# g0
# =========================================================================
G0_LIST ?= "\
    stm32mp-g0 \
    "
G0_IMAGE_INSTALL = "${@bb.utils.contains('MACHINE_FEATURES', 'usbg0', '${G0_LIST}', '', d)} "

# =========================================================================
# Image appends
# =========================================================================
EXTRA_IMAGEDEPENDS:append = " \
    ${OPTEE_BINARY_INSTALL} \
    "

MACHINE_EXTRA_RRECOMMENDS:append = " \
    ${GPU_IMAGE_INSTALL}        \
    ${ALSA_IMAGE_INSTALL}       \
    ${BLUETOOTH_IMAGE_INSTALL}  \
    ${M4COPRO_IMAGE_INSTALL}    \
    ${M33COPRO_IMAGE_INSTALL}   \
    ${OPTEE_IMAGE_INSTALL}      \
    ${PCI_IMAGE_INSTALL}        \
    ${WIFI_IMAGE_INSTALL}       \
    ${G0_IMAGE_INSTALL}         \
    "
