# GrADS
The Grid Analysis and Display System (GrADS) is a tool for fast and easy access, manipulation, analysis, and visualization of Earth science data. GrADS has two data models for handling both gridded and station data, and supports all of the standard data file formats. GrADS uses a 5-Dimensional data environment: the four conventional dimensions (longitude, latitude, vertical level, and time) plus an optional fifth dimension for grids that is generally implemented but designed to be used for ensembles. Analysis operations are executed via algebraic expressions, which are evaluated recursively so that expressions may be nested. A rich set of built-in functions are provided, but users may also add their own functions as external plug-ins that may be written in any programming language. GrADS has a programmable interface (scripting language) that allows for sophisticated analysis and display applications. GrADS can be run interactively or in batch mode.

The repository is a fork of **GrADS** published in [Github](https://github.com/j-m-adams/GrADS) by *J.M. Adams*, by also optionally use [OpenGrADS extensions](https://sourceforge.net/projects/opengrads/).

## Build dependencies

* GCC
* GNU autotools & libtool
* GNU make & which
* PCRE 1
* HDF4, HDF5, NetCDF-C, libxml, UDUNITS 2
* Readline, Ncurses, DAP lib, transport-independent RPC,
* Jasper, Shapefile C library, TIFF & GeoTIFF libraries, JPEG and PNG libraries, LibZ
* Cairo, GD
* X11 libraries: Xext, Xt, Xmu, Xaw, Xaw3d, Xft, Xaw3dXft
* COLA shared libraries: libsx & gadap
* G2CLIB modern grib decoder as shared library v2.0.0+ 
* environment modules (optional)

## Run dependencies

* HDF4, HDF5, NetCDF-C, UDUNITS 2
* Readline, DAP lib, transport-independent RPC,
* Jasper, Adaptive Entropy Coding library (AEC), PROJ, HarfBuzz, Graphite 2, Shapefile C library, WebP, TIFF & GeoTIFF libraries, JPEG & PNG libraries, JBIG-KIT
* Cairo
* X11 libraries: Xext, Xpm, SM, ICE, Xft Xcb Xrender, Xau
* environment modules (optional)

## Compilation

There are two different spec files for RHEL distributions:

. GrADS classic version (grads.spec)
. OpenGrADS with optional extensions and scripts (from Antonio Da Silva & Mike Fiorino and from Centro Funzionale Meteo-Idrologico della Regione Liguria, Italy (subdir arpal)).

Please use RPM build tools and produce RPM for installing.

The build phase is divided into several steps:

. Run GNU Autotools, build and install GrADS with or without OpenGrADS extensions
. Build, test and install extensions, scripts, data and shapefiles
. Install env var file or GrADS TCL modulefile (optional)

## Build example

Here it is a build command example for usage in Rocky Linux 8.

**grads.spec**

Input arguments:

. __install_path: GrADS installation directory (default: /usr)
. module_dir: TCL module installation directory (default: empty)

Example:

**rpmbuild -ba grads.spec --define "__install_path /opt/grads" --define "module_dir /usr/share/Modules/modulefiles"

20251120, Paolo Oliveri
