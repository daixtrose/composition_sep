#!/bin/bash

# Stolen from https://stackoverflow.com/a/246128/1528210
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
    DIR="$(cd -P "$(dirname "$SOURCE")" >/dev/null 2>&1 && pwd)"
    SOURCE="$(readlink "$SOURCE")"
    [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$(cd -P "$(dirname "$SOURCE")" >/dev/null 2>&1 && pwd)"

# Stolen from https://stackoverflow.com/a/48185201/1528210
PATH_TO_ROS_LIBS=/opt/ros/eloquent/lib

case :${LD_LIBRARY_PATH}: in # notice colons around the value
*:${PATH_TO_ROS_LIBS}:*) ;;  # do nothing, it's there
*) export LD_LIBRARY_PATH=${PATH_TO_ROS_LIBS}:${LD_LIBRARY_PATH} ;;
esac

echo "LD_LIBRARY_PATH = ${LD_LIBRARY_PATH}"

export LD_LIBRARY_PATH=${DIR}/../lib:${LD_LIBRARY_PATH}

echo "LD_LIBRARY_PATH = ${LD_LIBRARY_PATH}"

${DIR}/manual_composition
