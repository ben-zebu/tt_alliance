#!/bin/bash

function build_target() {
    local folder=${1}
    local target=${2} 

    mkdir -p ${folder} && cd ${folder}
    cmake -DCMAKE_BUILD_TYPE=${target} ..
    make
    make install

    cd ..
}

# Build en mode Debug
function build_debug() {
    build_target build_debug Debug
}

# Build en mode Release
function build_release() {
    build_target build_release Release
}

build_release
build_debug

