Compilation of U-Boot:
1. Pre-requisite
2. Initialize cross-compilation via SDK
3. Prepare U-Boot source code
4. Manage of U-Boot source code with GIT
5. Compile U-Boot source code
6. Update software on board
7. Update starter package with U-Boot compilation outputs

----------------
1. Pre-requisite
----------------
OpenSTLinux SDK must be installed.

For U-Boot build you need to install:
* libncurses and libncursesw dev package
    - Ubuntu: sudo apt-get install libncurses5-dev libncursesw5-dev
    - Fedora: sudo yum install ncurses-devel
* git:
    - Ubuntu: sudo apt-get install git-core gitk
    - Fedora: sudo yum install git

If you have never configured you git configuration:
    $ git config --global user.name "your_name"
    $ git config --global user.email "your_email@example.com"

External device tree is extracted. If this is not the case, please follow the
README_HOW_TO.txt in ../external-dt.

---------------------------------------
2. Initialize cross-compilation via SDK
---------------------------------------
* Source SDK environment:
    $ source <path to SDK>/environment-setup

* To verify that your cross-compilation environment is set-up correctly:
    $ set | grep CROSS_COMPILE

  If the variable CROSS_COMPILE has a value:
   - arm-ostl-linux-gnueabi- for 32 bits architecture (for example STM32MP1)
   - aarch64-ostl-linux- for 64 bits architecture (for example STM32MP2)
  Then everything is set-up correctly

Warning: the environment are valid only on the shell session where you have
         sourced the sdk environment.

------------------------
3. Prepare U-Boot source
------------------------
If not already done, extract the sources from Developer Package tarball, for example:
    $ tar xf en.SOURCES-stm32mp*-*.tar.xz

In the U-Boot source directory (sources/*/##BP##-##PR##),
you have one U-Boot source tarball, the patches and one Makefile:
   - ##BP##-##PR##.tar.xz
   - 00*.patch
   - Makefile.sdk

If you would like to have a git management for the source code move to
to section 4 [Management of U-Boot source code with GIT].

Otherwise, to manage U-Boot source code without git, you must extract the
tarball now and apply the patch:

    $> tar xf ##BP##-##PR##.tar.xz
    $> cd ##BP##
    $> for p in `ls -1 ../*.patch`; do patch -p1 < $p; done

You can now move to section 5 [Compile U-Boot source code].

-------------------------------------
4. Manage U-Boot source code with GIT
-------------------------------------
If you like to have a better management of change made on U-Boot source,
you have 3 solutions to use git

4.1 Get STMicroelectronics U-Boot source from GitHub
----------------------------------------------------
    URL: https://github.com/STMicroelectronics/u-boot.git
    Branch: ##ARCHIVER_ST_BRANCH##
    Revision: ##ARCHIVER_ST_REVISION##

    $ git clone https://github.com/STMicroelectronics/u-boot.git
    $ git checkout -b WORKING ##ARCHIVER_ST_REVISION##

4.2 Create Git from tarball
---------------------------
    $ tar xf ##BP##-##PR##.tar.xz
    $ cd ##BP##
    $ test -d .git || git init . && git add . && git commit -m "U-Boot source code" && git gc
    $ git checkout -b WORKING
    $ for p in `ls -1 ../*.patch`; do git am $p; done

4.3 Get Git from DENX community and apply STMicroelectronics patches
---------------------------------------------------------------
    URL: git://git.denx.de/u-boot.git
    Branch: ##ARCHIVER_COMMUNITY_BRANCH##
    Revision: ##ARCHIVER_COMMUNITY_REVISION##

    $ git clone git://git.denx.de/u-boot.git
or
    $ git clone http://git.denx.de/u-boot.git

    $ cd u-boot
    $ git checkout -b WORKING ##ARCHIVER_COMMUNITY_REVISION##
    $ for p in `ls -1 <path to patch>/*.patch`; do git am $p; done

-----------------------------
5. Compile U-Boot source code
-----------------------------
To compile U-Boot source code, first move to U-Boot source:
    $ cd ##BP##
    or
    $ cd u-boot

5.1 Compilation for one target
------------------------------
To use the external device tree feature, EXTDT_DIR variable must be set to the root location of external DT
as specified in the README.HOW_TO.txt of external-dt
    $> export EXTDT_DIR=<external DT location>
and add the following parameter to make command:
    "EXT_DTS=$EXTDT_DIR/u-boot"

    STM32MP15 series is selected by defconfig: stm32mp15_defconfig
    STM32MP13 series is selected by defconfig: stm32mp13_defconfig
    STM32MP25 series is selected by defconfig: stm32mp25_defconfig
    Board is selected by the device tree name to use

    see <U-Boot source>/doc/board/st/stm32mp1.rst for details

    $ make <target>_defconfig
    $ make DEVICE_TREE=<device tree> all
    or for external DT usage:
    $ make DEVICE_TREE=<device tree> all EXT_DTS=$EXTDT_DIR/u-boot

    example:

    a) trusted boot on STM32MP157F-EV1
      $ make stm32mp15_defconfig
      $ make DEVICE_TREE=stm32mp157f-ev1 all

    b) trusted boot on STM32MP135F-DK
      $ make stm32mp13_defconfig
      $ make DEVICE_TREE=stm32mp135f-dk all

    c) trusted boot on STM32MP257F-EV1
      $ make stm32mp25_defconfig
      $ make DEVICE_TREE=stm32mp257f-ev1 all

    d) trusted boot on STM32MP257F-EV1 with external DT
      $ make stm32mp25_defconfig
      $ make DEVICE_TREE=stm32mp257f-ev1-ca35tdcid-ostl all EXT_DTS=$EXTDT_DIR/u-boot


    then u-boot.dtb and u-boot-nodtb.bin can be added in the an existing FIP file with:
      $ fiptool --verbose update \
      --nt-fw u-boot-nodtb.bin \
      --hw-config u-boot.dtb \
      <FIP.bin>

    or used to create a FIP, see command in TF-A readme.

    warning: 'fiptool update' is not possible for signed FIP

* Configure on a dedicated build directory
    Here for example, build directory is located at the same level of U-Boot
    source code
    $ cd <directory to U-Boot source code>
    $ export OUTPUT_BUILD_DIR=$PWD/../build
    $ mkdir -p ${OUTPUT_BUILD_DIR}

    Then to compile, add O="${OUTPUT_BUILD_DIR}" to the command line
    $ make O="${OUTPUT_BUILD_DIR}" stm32mp25_defconfig
    $ make O="${OUTPUT_BUILD_DIR}" DEVICE_TREE=stm32mp257f-ev1 all

5.2 Compilation for several targets: use Makefile.sdk (with FIP)
----------------------------------------------------------------
Since OpenSTLinux activates FIP by default, FIP_artifacts directory path must be specified before launching compilation
  - In case of using SOURCES-xxxx.tar.gz of Developer package the FIP_DEPLOYDIR_ROOT must be set as below:
    $> export FIP_DEPLOYDIR_ROOT=$PWD/../../FIP_artifacts

The build results for this component are available in DEPLOYDIR (Default: $PWD/../deploy).
If needed, this deploy directory can be specified by adding "DEPLOYDIR=<your_deploy_dir_path>" compilation option to the build command line below.
The generated FIP images are available in $FIP_DEPLOYDIR_ROOT/fip

To list U-Boot source code compilation configurations:
    $ make -f $PWD/../Makefile.sdk help
To compile U-Boot source code:
    $ make -f $PWD/../Makefile.sdk all
To compile U-Boot source code for a specific config:
  - Compile default U-Boot configuration but applying specific devicetree(s):
    $ make -f $PWD/../Makefile.sdk DEVICE_TREE="<devicetree1> <devicetree2>" all
  - Compile for a specific U-Boot configuration:
    $ make -f $PWD/../Makefile.sdk UBOOT_CONFIG=default UBOOT_DEFCONFIG=stm32mp15_defconfig UBOOT_BINARY=u-boot.dtb DEVICE_TREE=stm32mp157f-dk2 all
To compile U-Boot source code and overwrite the default FIP artifacts with built artifacts:
    $> rm -rf $FIP_DEPLOYDIR_ROOT/u-boot/*
    $> make -f $PWD/../Makefile.sdk DEPLOYDIR=$FIP_DEPLOYDIR_ROOT/u-boot all

5.3 Compilation with support for fastboot support
-------------------------------------------------
Fastboot is a feature in U-Boot which can improve a lot the speed of binary downloading.
Fastboot is, for the moment, only supported on SD-Card and eMMC. And only one mode is
supported at a given time. It means that if your U-Boot supports fastboot on SD-Card
then it's not availabe on eMMC. To support it on eMMC, you must use another U-BOOT.

To enable fastboot on the right mass storage, simply apply the right fragment
into the right defconfig:
  - fastboot on SD-Card for stm32mp15:
    $ cat $PWD/../fragment-04-fastboot_mmc0.fb_cfg >> configs/stm32mp15_defconfig
  - fastboot on eMMC for stm32mp15:
    $ cat $PWD/../fragment-05-fastboot_mmc1.fb_cfg >> configs/stm32mp15_defconfig
  - fastboot on SD-Card for stm32mp13:
    $ cat $PWD/../fragment-04-fastboot_mmc0.fb_cfg >> configs/stm32mp13_defconfig
  - fastboot on eMMC for stm32mp13:
    $ cat $PWD/../fragment-05-fastboot_mmc1.fb_cfg >> configs/stm32mp13_defconfig

Then build U-Boot as explained in chapter 5.

---------------------------
6. Update software on board
---------------------------
Please use STM32CubeProgrammer and only tick the ssbl-boot and fip partition (more informations on the wiki website http://wiki.st.com/stm32mpu)

---------------------------
7. Generate new Starter Package with U-Boot compilation outputs
---------------------------
If not already done, extract the artifacts from Starter Package tarball, for example:
    # tar xf en.FLASH-stm32mp*-*.tar.xz

Move to Starter Package root folder,
    #> cd <your_starter_package_dir_path>
Cleanup Starter Package from original U-Boot artifacts first
    #> rm -rf images/stm32mp*/fip/*
Update Starter Package with new fip artifacts from <FIP_DEPLOYDIR_ROOT>/fip folder:
    #> cp -rvf $FIP_DEPLOYDIR_ROOT/fip/* images/stm32mp*/fip/

Then the new Starter Package is ready to use for "Image flashing" on board (more information on wiki website https://wiki.st.com/stm32mpu).
