#!/bin/bash

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

release_build_dir="${ROOT}/build"
debug_build_dir="${ROOT}/build/debug"

DP_FLAG_DEBUG=1
DP_FLAG_ERROR=2

BUILD_FLAG_STATIC=1

MODE_DEBUG=1
MODE_RELEASE=2

BUILD_TARGET_CLN=0x1
BUILD_TARGET_DEL=0x2
BUILD_TARGET_N2B=0x4
BUILD_TARGET_S2A=0x8
BUILD_TARGET_CTL=0x10

name=
pos_targets="-ctl, -n2b,-s2a"
def_target=0
target=0
build_mode=$MODE_RELEASE
build_flags=0
usage=0
help=0
verbose=0
debug_print=$DP_FLAG_ERROR

# Clean build directory from meta files
#
# @param $1 build directory
function clean() {
    local dir=$1
    local type=$2

    if [[ ${dir} != "${release_build_dir}" ]] && [[ ${dir} != "${debug_build_dir}" ]]; then
        echo [e] Invalid clean dir!
        return
    fi

    if (( ${type} == 1 )); then
        echo "cleaning build dir: $dir"
        rm -rf ${dir}/*.o 2> /dev/null
    elif (( ${type} == 2 )); then
        echo "deleting dir: $dir"
        rm -rf ${dir}/* 2> /dev/null
    fi

    return 0
}

# CMake build a target
#
# @param $1 cmake target
# @param $2 build directory
# @param $3 build mode
function buildTarget() {
    local target=$1
    local dir=$2
    local build_mode=$3
    local dp=$4
    local build_flags=$5
    local ep=0

    if [[ $verbose == 1 ]]; 
    then
        echo "buildTarget()"
        echo   "target="$target
        echo   "dir="$dir
        echo   "build_mode="$build_mode
        echo   "dp="$dp
        echo   "build_flags="$build_flags
    fi

    if ! mkdir -p ${dir}; then
        return 1
    fi

    if (( $((dp & $DP_FLAG_ERROR)) == $DP_FLAG_ERROR )); then
        ep=1
    fi
    dp=$((dp & ~$DP_FLAG_ERROR))

    local flags="-Wl,-z,relro,-z,now -D_FILE_OFFSET_BITS=64 -Wall -pedantic -Wextra -Werror=return-type -Werror=overflow -Werror=format"
    if (( ${build_mode} == $MODE_DEBUG )); then
        flags="${flags} -ggdb -O0"
    else
        flags="${flags} -Ofast"
    fi

    if (( $((build_flags & $BUILD_FLAG_STATIC)) == $BUILD_FLAG_STATIC )); then
        flags="${flags} -static"
    fi

    local dpf=
    if (( $dp > 0 )); then
        dpf=-DDEBUG_PRINT=$dp
    fi

    local epf=
    if (( $ep > 0 )); then
        epf=-DERROR_PRINT
    fi
    
    
    case $target in
    
        $((BUILD_TARGET_CTL)))
            local bin_name=ctlCode
            local src="src/ctlCode.c"
            gcc $flags $dpf $epf -o $dir/$bin_name $src
            ;;
    
        $((BUILD_TARGET_N2B)))
            local bin_name=num2bin
            local src="src/num2bin.c"
            gcc $flags $dpf $epf -o $dir/$bin_name $src
            ;;
    
        $((BUILD_TARGET_S2A)))
            local bin_name=string2array
            local src="src/string2Array.c"
            gcc $flags $dpf $epf -o $dir/$bin_name $src
            ;;
            
        *)
            echo "Unknown target: ${target}"
            ;;

    esac

    return $?
}

function printUsage() {
    echo "Usage: $0 [${pos_targets}] [-d|-r] [-h]"
    return 0;
}

function printHelp() {
    printUsage
    echo ""
    echo "Possible targets: ${pos_targets}"
    echo "  * -ctl: build ctlCode application"
    echo "  * -n2b: build num2bin application"
    echo "  * -s2a: build string2array application"
    echo "-d Build in debug mode"
    echo "-r Build in release mode"
    echo "-s Build statically linked binary"
    echo "-c Clean build dir from obj files"
    echo "-x Delete all files in build dir"
    echo "-h Print this."
    return 0;
}

while (("$#")); do
    case "$1" in
        -c | -cln | --clean)
            target=$(( target | BUILD_TARGET_CLN ))
            shift 1
            ;;
        -d | --debug)
            build_mode=$MODE_DEBUG
            shift 1
            ;;
        -h | --help)
            help=1
            break
            ;;
        #
        # targets
        #
        -ctl | --ctlCode)
            target=$(( target | BUILD_TARGET_CTL ))
            shift 1
            ;;
        -n2b | --num2bin)
            target=$(( target | BUILD_TARGET_N2B ))
            shift 1
            ;;
        -s2a | --string2array)
            target=$(( target | BUILD_TARGET_S2A ))
            shift 1
            ;;

        #
        # other
        #
        -p | -dp | --debug-print)
            debug_print=$2
            shift 2
            ;;
        -r | --release)
            build_mode=$MODE_RELEASE
            shift 1
            ;;
        -s | --static)
            build_flags=$((build_flags | $BUILD_FLAG_STATIC))
            shift 1
            ;;
        -v | --verbose)
            verbose=1
            shift 1
            ;;
        -x | --delete)
            target=$(( target | BUILD_TARGET_DEL ))
            shift 1
            ;;
        -* | --usage)
            usage=1
            break
            ;;
        *) # No more options
            break
            ;;
    esac
done

if (( $target == 0 && help != 1)); then
    usage=1
fi

if (( ${usage} == 1 )); then
    printUsage
    exit $?
fi

if (( ${help} == 1 )); then
    printHelp
    exit $?
fi

# set build dir
if (( $((build_mode & $MODE_RELEASE)) == $MODE_RELEASE )); then
    build_dir=${release_build_dir}
else
    build_dir=${debug_build_dir}
fi

if (( $verbose == 1 )); 
then
    echo "target: "${target}
    echo "  clean: "$(( (target & BUILD_TARGET_CLN) > 0 ))
    echo "  del: "$(( (target & BUILD_TARGET_DEL) > 0 ))
    echo "  ctl: "$(( (target & BUILD_TARGET_CTL) > 0 ))
    echo "  n2b: "$(( (target & BUILD_TARGET_N2B) > 0 ))
    echo "  s2a: "$(( (target & BUILD_TARGET_S2A) > 0 ))
    echo "build_mode: "${build_mode}
    echo "debug_print: "${debug_print}
    echo "build_dir: "${build_dir}
    echo -e
fi


#
# build set targets
#

if (( $((target & BUILD_TARGET_CLN)) == $BUILD_TARGET_CLN )); then
    target=$(( target & ~BUILD_TARGET_CLN ))
    clean ${build_dir} 1
fi
if (( $((target & BUILD_TARGET_DEL)) == $BUILD_TARGET_DEL )); then
    target=$(( target & ~BUILD_TARGET_DEL ))
    clean ${build_dir} 2
fi

if (( $target > $BUILD_TARGET_DEL )); 
then
    buildTarget ${target} ${build_dir} ${build_mode} ${debug_print} ${build_flags}
fi

exit $?
