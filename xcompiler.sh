#!/bin/sh

if [ -z ${WICAMERA_ROOT} ]; then
    echo "Please set environment WICAMERA_ROOT first"
    exit 1
fi

if [ -e ${WICAMERA_ROOT} ]; then
    if ! [ -d ${WICAMERA_ROOT} ]; then
        echo "WHISPER_PREFIX:${WICAMERA_ROOT} is not a path"
        exit 1
    fi
else
    mkdir -p ${WICAMERA_ROOT}
fi

echo  "--->${WICAMERA_ROOT}"

make clean &&
   make libvutils.a &&
      make install INSTALL_PREFIX=${WICAMERA_ROOT}
