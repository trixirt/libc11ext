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

mkdir -p ../obj
mkdir -p ../lib
OBJ=`realpath $HERE/../obj`
LIB=`realpath $HERE/../lib`
SRC=`realpath $HERE/../src`
INC=`realpath $HERE/../include`

SOURCES=`find $SRC -name '*.c'`
f=`ls $OBJ | wc | awk '{ print $1 }'`
if [ $f != 0 ]; then
    rm $OBJ/*
fi
f=`ls $LIB | wc | awk '{ print $1 }'`
if [ $f != 0 ]; then
    rm $LIB/*
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
PRIVATE_INCLUDE=-I${SRC}/private/include
GCC_WARNINGS="-Wall -Wextra -Werror"
CLANG_WARNINGS="-Weverything -Werror"

CFLAGS="$CFLAGS $PRIVATE_INCLUDE"
CFLAGS="$CFLAGS $PUBLIC_INCLUDE"
if [ x${IS_GCC} = x1 ]; then
    CFLAGS="$CFLAGS $GCC_WARNINGS"
fi
if [ x${IS_CLANG} = x1 ]; then
    CFLAGS="$CFLAGS $CLANG_WARNINGS"
fi

for s in $SOURCES; do
    c=`basename $s`
    b=${c%.*}
    o=$b.o
    echo $o

    cc -c $CFLAGS $s -o $OBJ/$o
    if [ $? != 0 ]; then
	exit 1
    fi
    if [ ! -f $OBJ/$o ]; then
	exit 1
    fi
done
