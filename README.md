# GrADS patched with OpenGrADS extensions

Repository for building the version of **GrADS** published in [Github](https://github.com/j-m-adams/GrADS) by *J.M. Adams*, by also patching it to use **OpenGrADS extensions**.

## Dependencies

Required X11 development level packages:
* libXext
* libXt
* libXmu
* neXtaw
* libXaw
* Xaw3d
* libXft
* libsx
* libXaw3dxft

Other required development level packages:

* jasper
* cairo
* readline
* hdf
* hdf5
* netcdf
* libtiff
* libgeotiff
* shapelib
* libpng
* libxml2
* gd
* g2clib
* udunits2
* libdap
* libtirpc
* gadap

Optional packages (for TCL module building):

* environment-modules

Some older packages that are not present in the repository (for example, libsx, libXaw3dxft and gadap for dnf repository) are bundled in this repository and staled.

The [GrADS](https://github.com/j-m-adams/GrADS) repository is added as a git submodule. Please BE AWARE before pulling down GrADS repository update that they can broke the patch process. Actually the patch works for commit **067f2e83fb5ede55920472714166384fb212a5aa** of branch *master*.

## Cloning this repository
Due to the git submodule presence, the command for cloning this repository must add the "--recurse-submodule" option (valid since git version 2.13)

## Building

There is/are some bash script(s) to pilot the build using different OS(es). All packages that can be acquired using the system package manager are installed with it.

The build phase is divided into several steps:

1. X11 system-wide dependent packages check and install
2. Other system-wide dependent packages check and install
3. Patch, run GNU Autotools, build and install GrADS with OpenGrADS extension activated
4. Build, test and install extensions, scripts, data and shapefiles
5. Install GrADS TCL modulefile (optional)

## Build example

Here it is a build command example for usage in Rocky Linux 8.

**grads_build_gcc_rocky.sh**

Input arguments:

1. GrADS installation directory (mandatory)
2. TCL module installation directory (optional)

Example:

**bash grads_build_gcc_rocky.sh $DESTDIR $MODULEDIR**

20220628, Paolo Oliveri
