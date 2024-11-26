FILESEXTRAPATHS:append := "${THISDIR}/files:"

DEPENDS += "zstd libcap-ng "

RDEPENDS:${PN}-tests =+ "bash"
PACKAGECONFIG:stm32mpcommon = "python tui libunwind coresight"
PACKAGECONFIG:class-native = "python tui libunwind coresight"
PACKAGECONFIG:class-nativesdk = "python tui libunwind coresight"

FILES:${PN}:append:class-nativesdk = " ${base_prefix}/usr/etc/bash_completion.d "
FILES:${PN}:append:class-native = " ${sysconfdir}/bash_completion.d "

BBCLASSEXTEND += "native nativesdk"
# Avoid QA Issue: contains reference to TMPDIR [buildpaths]
INSANE_SKIP:${PN}-python += "buildpaths"
INSANE_SKIP:${PN} += "buildpaths"
INSANE_SKIP:${PN}-dbg += "buildpaths"
