#@TYPE: Distribution
#@NAME: Angstrom 
#@DESCRIPTION: Release independent distribution configuration for Angstrom

# Preserve original DISTRO value
USERDISTRO := "${DISTRO}"
DISTRO = "angstrom"
DISTRO_NAME = "Angstrom"

BUILDNAME = "Angstrom ${DISTRO_VERSION}"

TARGET_VENDOR = "-angstrom"
SDK_VENDOR = "-angstromsdk"

SOC_FAMILY ?= ""

# Yocto work around for missing ONLINE_PACKAGE_MANAGEMENT support
ONLINE_PACKAGE_MANAGEMENT = "full"

#Set the right arch for the feeds
#Alphabetically sorted

FEED_ARCH ?= "${TUNE_PKGARCH}"

#blackfin machines
FEED_ARCH_bfin          = "blackfin"

# Add FEED_ARCH to machine overrides so we get access to e.g. 'armv7a' and 'sh4'
# Hopefully we'll never see a machine or arch with 'all' as substring
MACHINEOVERRIDES .= ":${@d.getVar('FEED_ARCH', d,1).replace('all','noarch')}"

# Put links to sources in deploy/sources to make it easier for people to be GPL compliant
#INHERIT += "src_distribute_local"
SRC_DIST_LOCAL ?= "symlink"

# Can be "glibc", "musl" or "uclibc"
TCLIBC ?= "glibc"

# The things *libc can provide.
PREFERRED_PROVIDER_eglibc = "glibc"

PREFERRED_PROVIDER_virtual/gettext ??= "gettext"
PREFERRED_PROVIDER_linux-libc-headers-nativesdk = "linux-libc-headers-nativesdk"

require conf/distro/include/angstrom-${TCLIBC}.inc

# ARM920T and up can use thumb mode to decrease binary size at the expense of speed
# (the complete story is a bit more nuanced due to cache starvation)
# Angstrom turns on thumb for armv4t machine according to this RFC:
# http://lists.linuxtogo.org/pipermail/angstrom-distro-devel/2008-October/002714.html

ARM_INSTRUCTION_SET ??= "thumb"
ARM_INSTRUCTION_SET_armv4t ?= "arm"
ARM_INSTRUCTION_SET_armv5te ?= "arm"
ARM_INSTRUCTION_SET_armv6 ?= "arm"

# "arm" "thumb"
#    The instruction set the compiler should use when generating application
#    code.  The kernel is always compiled with arm code at present.  arm code
#    is the original 32 bit ARM instruction set, thumb code is the 16 bit
#    encoded RISC sub-set.  Thumb code is smaller (maybe 70% of the ARM size)
#    but requires more instructions (140% for 70% smaller code) so may be
#    slower.

#Use this variable in feeds and other parts that need a URI
ANGSTROM_URI ?= "http://feeds.angstrom-distribution.org"

#Use this variable to select which recipe you want to use to get feed configs (/etc/ipkg/, /etc/apt/sources.list)
# usefull for derivative distros and local testing
# As this is runtime we can't and won't use virtual/feed-configs
ANGSTROM_FEED_CONFIGS ??= "angstrom-feed-configs"

#Set the default maintainer to angstrom-dev
MAINTAINER = "Angstrom Developers <angstrom-distro-devel@linuxtogo.org>"

#use debian style naming
INHERIT += "debian"

#activate config checker
INHERIT += "sanity" 

#activate the blacklister
INHERIT += "blacklist"

#make devshell available as task
INHERIT += "devshell" 

#run QA tests on builds and packages and log them  
INHERIT += "insane"
QA_LOG = "1"

# Log information on images and packages
# https://github.com/Angstrom-distribution/buildhistory/
INHERIT += "buildhistory"
BUILDHISTORY_COMMIT = "1"

#run QA tests on recipes
#INHERIT += "recipe_sanity"

#have a shared state a.k.a package-staging2
INHERIT += "sstate"

ANGSTROM_PKG_FORMAT ?= "ipk"

#Generate locales on the buildsystem instead of on the target. Speeds up first boot, set to "1" to enable
ENABLE_BINARY_LOCALE_GENERATION ?= "1"

# We only want to build UTF8 locales
LOCALE_UTF8_ONLY = "1"

#qemu doesn't like this fp
ENABLE_BINARY_LOCALE_GENERATION_ep9312 = "0"

#qemu has taken a dislike to armeb as well
ENABLE_BINARY_LOCALE_GENERATION_armeb = "0"

# Prelink images
INHERIT += "image-prelink"

#Name the generated images in a sane way
IMAGE_NAME = "${DISTRO_NAME}-${IMAGE_BASENAME}-${TCLIBC}-${ANGSTROM_PKG_FORMAT}-${DISTRO_VERSION}-${MACHINE}"
DEPLOY_DIR_IMAGE = "${DEPLOY_DIR}/images/${MACHINE}"

# Prefer ext4 images where possible
SDIMG_ROOTFS_TYPE = "ext4"

# Angstrom *always* has some form of release config, so error out if someone thinks he knows better 
DISTRO_CHECK := "${@d.getVar("DISTRO_VERSION",d,1) or bb.fatal('Remove this line or set a dummy DISTRO_VERSION if you really want to build an unversioned distro')}"

# We want images supporting the following features (for packagegroup(-core)-base)
DISTRO_FEATURES = "alsa argp bluetooth ext2 ipv4 ipv6 irda largefile nfs pam pci pcmcia ppp smbfs systemd usbgadget usbhost vfat wifi xattr zeroconf gobject-introspection-data"

# Disable sysvinit for recipes with systemd support
DISTRO_FEATURES_BACKFILL_CONSIDERED += "sysvinit"
VIRTUAL-RUNTIME_init_manager = "systemd"
# Uncomment to completely disable support for sysv scripts:
#PACKAGECONFIG_pn-systemd = "xz"

# USE-flag like features
DISTRO_FEATURES += "tk"
DISTRO_FEATURES += "x11"
DISTRO_FEATURES += "3g"
DISTRO_FEATURES += "pulseaudio"
DISTRO_FEATURES += "ldconfig"

# OpenGL support
DISTRO_FEATURES += "opengl"

# wayland...
DISTRO_WAYLAND = "wayland"

# No wayland for vivante blobs
DISTRO_WAYLAND_mx6 = ""
DISTRO_WAYLAND_mx7 = ""
DISTRO_WAYLAND_mx8 = ""
DISTRO_WAYLAND_use-mainline-bsp = "wayland"

DISTRO_FEATURES += "${DISTRO_WAYLAND}"


# Inherit the default LIBC features superset from OE-core
DISTRO_FEATURES += "${DISTRO_FEATURES_LIBC}"

# Yocto tweaks
require conf/distro/include/angstrom-core-tweaks.inc
