#%Module1.0#####################################################################

proc ModulesHelp { } {

puts stderr " "
puts stderr "This module loads GrADS built with GCC"
puts stderr " "
puts stderr "Note that this build of GrADS leverages the NETCDF, HDF I/O library, GRIB2 etc. and requires adequate linkage"

puts stderr "\nVersion GRADSVERSION\n"

}
module-whatis "Name: GrADS built with GCC"
module-whatis "Version: GRADSVERSION"
module-whatis "Category: application"
module-whatis "Description: Grid Analysis and Display System (GrADS)"
module-whatis "http://cola.gmu.edu/grads"

set             version             GRADSVERSION

prepend-path    PATH                GRADSPREFIX/bin

append-path     LD_LIBRARY_PATH     GRADSPREFIX/lib:/usr/local/lib/

setenv          GABDIR              GRADSPREFIX/bin/
setenv          GAINC               GRADSPREFIX/include/
setenv          GADDIR              GRADSPREFIX/lib/grads
setenv          GADSET              GRADSPREFIX/datasets
setenv          GASHP               GRADSPREFIX/lib/grads/shapefiles
setenv          GASCRP              "$env(HOME)/grads/scripts GRADSPREFIX/lib/grads/scripts/"
setenv          GAUDPT              GRADSPREFIX/lib/grads/udpt
setenv          GA2UDXT             GRADSPREFIX/lib/grads/gex/udxt
