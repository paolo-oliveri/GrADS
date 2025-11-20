#!/bin/bash
#-- location: /etc/profile.d/grads.sh
#
# GrADS env var configuration file
#
# 20251120 Paolo Oliveri

export PATH=GRADSPREFIX/bin:${PATH}
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:GRADSPREFIX/lib

export GABDIR="GRADSPREFIX/bin/"
export GAINC="GRADSPREFIX/include/"
export GADDIR="GRADSPREFIX/share/grads"
export GADSET="GRADSPREFIX/share/grads/datasets"
export GASHP="GRADSPREFIX/share/grads/shapefiles"
export GASCRP="GRADSPREFIX/share/grads/scripts ${HOME}/grads/scripts"
export GAUDPT="GRADSPREFIX/share/grads/udpt"
export GA2UDXT="GRADSPREFIX/share/grads/gex/udxt"
export GAGPY="GRADSPREFIX/lib/libgradspy.so"
