#!/bin/bash

# settings
CPP_VERSION=11
BUILD_DIR_DEBUG=./build/debug
BUILD_DIR_RELEASE=./build/release

RELEASE_PARAMS="-fPIC -shared -Wl,--whole-archive -Wl,--no-whole-archive -std=c++$CPP_VERSION -O2 -s"
DEBUG_PARAMS="-fPIC -shared -Wl,--whole-archive -Wl,--no-whole-archive -std=c++$CPP_VERSION -Wall -Wextra -pedantic"

COMPILER_LINUX="g++"
COMPILER_WINDOWS="x86_64-w64-mingw32-g++.exe"

SOURCE_FILES="api/engine.cpp api/script.cpp compiler/parsing/token.cpp compiler/parsing/operators.cpp compiler/parsing/tokenizer.cpp compiler/parsing/parser.cpp compiler/error_handler.cpp"

# prepare folders
mkdir -p $BUILD_DIR_DEBUG
mkdir -p $BUILD_DIR_RELEASE


PLATFORMS=(${2//#/ })

if [[ "$1" = "debug" ]]; then
    printf "Configuring the debug build mode"
    DIR="$BUILD_DIR_DEBUG"
    PARAMS="$DEBUG_PARAMS"
elif [[ "$1" = "release" ]]; then
    printf "Configuring the release build mode"
    DIR="$BUILD_DIR_RELEASE"
    PARAMS="$RELEASE_PARAMS"
else
    echo "Usage: ./build.sh [debug|release] [platforms; ...]"
    exit 1
fi

for platform in "${PLATFORMS[@]}"; do
    printf "\n\nPreparing to compile code targeting $platform...\n"

    if [[ "$platform" = "windows" ]]; then
        COMPILER="$COMPILER_WINDOWS"
        FILENAME="$DIR/win_olcl"
    elif [[ "$platform" = "linux" ]]; then
        COMPILER="$COMPILER_LINUX"
        FILENAME="$DIR/lin_olcl"
    else
        echo "Supported target platforms: windows, linux"
        exit 2
    fi

    printf "Running command: $COMPILER $SOURCE_FILES $PARAMS -o $FILENAME.so\n\n"
    eval "$COMPILER $SOURCE_FILES $PARAMS -o $FILENAME.so"
    # eval "ar rvs $FILENAME.a $FILENAME.so"
done
