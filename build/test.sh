#!/bin/sh

#
# Copyright (c) 2017 Juniper Networks.  All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#

HERE=`realpath $(dirname $0)`
cd $HERE

mkdir -p ../bin
BIN=`realpath $HERE/../bin`
TESTS=`realpath $HERE/../tests`
SRC=`realpath $HERE/../src`
INC=`realpath $HERE/../include`
LIB=`realpath $HERE/../lib`

SOURCES=`find $TESTS -name '*_test.c'`
f=`ls $BIN | wc | awk '{ print $1 }'`
if [ $f != 0 ]; then
    rm $BIN/*
fi

cc --version | grep 'Free Software Foundation' > /dev/null
if [ $? = 0 ]; then
    IS_GCC=1
fi
cc --version | grep 'clang' > /dev/null
if [ $? = 0 ]; then
    IS_CLANG=1
fi

PUBLIC_INCLUDE=-I${INC}
GCC_WARNINGS="-Werror"
CLANG_WARNINGS="-Werror"

CFLAGS="$CFLAGS $PUBLIC_INCLUDE"
if [ x${IS_GCC} = x1 ]; then
    CFLAGS="$CFLAGS $GCC_WARNINGS"
fi
if [ x${IS_CLANG} = x1 ]; then
    CFLAGS="$CFLAGS $CLANG_WARNINGS"
fi

# For atf test 
LDFLAGS="$LDFLAGS -latf-c"
# For our library
LDFLAGS="$LDFLAGS -L${LIB} -lext"

echo "Building tests"
RUN_TESTS=
for s in $SOURCES; do
    c=`basename $s`
    b=${c%.*}
    o=$b
    echo "Building $o"
    cc $CFLAGS $s $LDFLAGS -o $BIN/$o
    if [ $? != 0 ]; then
	exit 1
    fi
    if [ ! -f $BIN/$o ]; then
	exit 1
    fi
    RUN_TESTS="$RUN_TESTS $BIN/$o"
done

# XXX where libatf-c.so is found
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

echo "Running tests"
for r in $RUN_TESTS; do
    c=`basename $r`
    echo "Running $c "
    L=`$r -l | grep ident | awk '{ print $2 }'`
    for l in $L ; do
	echo -n "  test $l "
	m=`$r $l 2>/dev/null`
	if [ $? != 0 ]; then
	    echo "failed"
	    exit 1
	else
	    echo $m
	fi
    done
done
