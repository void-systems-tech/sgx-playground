#!/bin/sh

# script path
get_script_path() {
  SCRIPT_PATH="${0}"
  # ensure SCRIPT_PATH is absolute
  case "${SCRIPT_PATH}" in
      /*) ;; # Already absolute
      *) SCRIPT_PATH="$(pwd)/${SCRIPT_PATH}" ;; # Make it absolute
  esac
  # while ${SCRIPT_PATH} is a symlink, resolve it
  while [ -h "${SCRIPT_PATH}" ]; do
      SCRIPT_DIR="$(cd -P "$(dirname "${SCRIPT_PATH}")" && pwd)"
      SCRIPT_PATH="$(readlink "${SCRIPT_PATH}")"
      # If ${SCRIPT_PATH} was a relative symlink, resolve it relative to the symlink base directory
      case "${SCRIPT_PATH}" in
          /*) ;; # Absolute path, do nothing
          *) SCRIPT_PATH="${SCRIPT_DIR}/${SCRIPT_PATH}" ;; # Relative path
      esac
  done
  cd -P "$(dirname "${SCRIPT_PATH}")" && pwd
}

SCRIPT_PATH="$(get_script_path)"
BUILD_PATH="${SCRIPT_PATH}/build"

OE_DOCKER_IMAGE=${OE_DOCKER_IMAGE:-"openenclavedockerregistry.azurecr.io/oetools-22.04:2025.03.13148"}
OE_INSTALL=$(realpath "${SCRIPT_PATH}/../../_openenclave_openenclave/work/build/install")


mkdir -p "${BUILD_PATH}"
set -x
docker run \
  -v "${OE_INSTALL}:/opt/openenclave" \
  -v "${SCRIPT_PATH}:/opt/work" \
  --workdir="/opt/work" \
  "${OE_DOCKER_IMAGE}" \
  /bin/bash -c " \
    source /opt/openenclave/share/openenclave/openenclaverc && \
    cd ./build && \
    cmake ../src && \
    make
    #make VERBOSE=1
    #make -j$(nproc) \
  "


# vim: ts=2 sw=2 et:


