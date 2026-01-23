#!/usr/bin/env bash
# Export toolchain path for riscv-wch-elf gcc and make wdf.py callable
# Usage: . ./export_env.sh (run from repo root or provide full path)

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
WCH_TOOLCHAIN_DIR="/opt/MRS/Toolchain/RISC-V_Embedded_GCC12/bin"


export OPENWCH_SDK_PATH="${SCRIPT_DIR}"
export WCH_TOOLCHAIN_PREFIX="riscv-wch-elf"
export PATH="${WCH_TOOLCHAIN_DIR}:${OPENWCH_SDK_PATH}:${PATH}"

echo "[env] OPENWCH_SDK_PATH=${OPENWCH_SDK_PATH}"
echo "[env] TOOLCHAIN_PREFIX=${WCH_TOOLCHAIN_PREFIX}"
echo "[env] PATH updated with ${WCH_TOOLCHAIN_DIR}"
echo "[env] PATH updated with ${OPENWCH_SDK_PATH} (wdf.py)"