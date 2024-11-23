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

# Build debug mode
function build_debug() {
    build_target build_debug Debug
}

# Build release mode
function build_release() {
    build_target build_release Release
}

# Build documentation
function build_doc() {
    local doc_deployment_dir='/mnt/c/Users/benoit/Documents/tt_alliance'
    cd docs
    make html
    cp -r build/html ${doc_deployment_dir}/. 
    cd ..
}

# Build all
function build_all() {
    build_release
    build_debug
}

# Clean project
function clean_project() {
    rm -rf build_* debug release include
    rm -rf docs/build
}

# help function
function show_help() {
    echo "Usage: $0 {all|release|debug|doc|clean}"
    echo "Options:"
    echo "  all       Execute release, debug (default action)"
    echo "  release   Compile release version"
    echo "  debug     Compile debug version"
    echo "  doc       Compile documentation"
    echo "  clean     Clean the project"
}

# Check number of arguments
if [ $# -lt 1 ]; then
    build_all
    exit 0
fi

# Check if option
while [[ $# -gt 0 ]]; do
    mode=$1
    case "$mode" in
        all)
            build_all
            shift
            ;;
        release)
            build_release
            shift
            ;;
        debug)
            build_debug
            shift
            ;;
        doc)
            build_doc
            shift
            ;;
        clean)
            clean_project
            shift
            ;;        
        *)
            echo "Error: unknow option"
            show_help
            exit 1
            ;;
    esac
done