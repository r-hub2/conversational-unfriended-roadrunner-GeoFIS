# Installation

The external libraries [GMP](https://gmplib.org/), [MPFR](https://www.mpfr.org/), [GEOS](https://trac.osgeo.org/geos/), [PROJ](https://proj.org/) and [GDAL]( https://gdal.org/) are required on Unix systems. You must install them before installing the `GeoFIS` package.

- install Ubuntu/Debian packages: `apt install -y libgmp-dev libmpfr-dev libgeos++-dev libproj-dev libgdal-dev`
- or install Fedora packages: `yum install -y gmp-devel mpfr-devel geos-devel proj-devel sqlite-devel gdal-devel`
- or install Solaris packages: `pkgutil -y -i gcc5gfortran libgmp_dev libmpfr_dev geos_dev gdal proj_dev gdal_dev`
