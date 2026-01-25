#!/usr/bin/env bash
# Export toolchain path for riscv-wch-elf gcc and make wdf.py callable
# Usage: . ./export_env.sh (run from repo root or provide full path)

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
WCH_TOOLCHAIN_DIR="/opt/MRS/Toolchain/RISC-V_Embedded_GCC12/bin"
VENV_DIR="${SCRIPT_DIR}/.venv"
REQ_FILE="${SCRIPT_DIR}/requirements.txt"


export OPENWCH_SDK_PATH="${SCRIPT_DIR}"
export WCH_TOOLCHAIN_PREFIX="riscv-wch-elf"
export PATH="${WCH_TOOLCHAIN_DIR}:${OPENWCH_SDK_PATH}:${PATH}"

if [[ -z "${VIRTUAL_ENV}" || "${VIRTUAL_ENV}" != "${VENV_DIR}" ]]; then
	if [[ ! -d "${VENV_DIR}" ]]; then
		if ! command -v python3 >/dev/null 2>&1; then
			echo "[env] python3 not found; cannot create venv" >&2
		else
			echo "[env] creating venv at ${VENV_DIR}"
			python3 -m venv "${VENV_DIR}"
		fi
	fi

	if [[ -f "${VENV_DIR}/bin/activate" ]]; then
		# shellcheck disable=SC1090
		. "${VENV_DIR}/bin/activate"
		echo "[env] venv activated: ${VENV_DIR}"
		if [[ -f "${REQ_FILE}" ]]; then
			python -m pip install --upgrade pip >/dev/null 2>&1 || true
			python -m pip install -r "${REQ_FILE}" >/dev/null 2>&1 || true
			echo "[env] requirements installed"
		fi
	fi
fi

echo "[env] OPENWCH_SDK_PATH=${OPENWCH_SDK_PATH}"
echo "[env] TOOLCHAIN_PREFIX=${WCH_TOOLCHAIN_PREFIX}"
echo "[env] PATH updated with ${WCH_TOOLCHAIN_DIR}"
echo "[env] PATH updated with ${OPENWCH_SDK_PATH} (wdf.py)"