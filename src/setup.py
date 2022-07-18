from distutils.core import setup, Extension
import numpy

setup(name='gradspy', version='1.1',  \
      ext_modules=[Extension('gradspy', ['gradspy.c'], include_dirs=[numpy.get_include()])])
