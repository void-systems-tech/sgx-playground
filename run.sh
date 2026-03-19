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

OE_SIMULATE="${OE_SIMULATE:-"1"}"

set -x
export OE_SIMULATE
"${BUILD_PATH}/host/host_hello" "${BUILD_PATH}/enclave/hello/enclave_hello.signed"
"${BUILD_PATH}/host/host_datasealing" "${BUILD_PATH}/enclave/datasealing/a_v1/enclave_datasealing_a_v1.signed" "${BUILD_PATH}/enclave/datasealing/a_v2/enclave_datasealing_a_v2.signed" "${BUILD_PATH}/enclave/datasealing/b/enclave_datasealing_b.signed"

# vim: ts=2 sw=2 et:


