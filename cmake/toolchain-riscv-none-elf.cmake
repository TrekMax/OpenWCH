set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv)

# Configure your toolchain root here or via environment variables.
set(TOOLCHAIN_PREFIX "riscv-wch-elf" CACHE STRING "Cross toolchain prefix")

# Allow environment override even if cache exists.
if(DEFINED ENV{TOOLCHAIN_PREFIX})
	set(TOOLCHAIN_PREFIX "$ENV{TOOLCHAIN_PREFIX}" CACHE STRING "Cross toolchain prefix" FORCE)
endif()

set(_toolchain_bin "$ENV{WCH_TOOLCHAIN_DIR}")

find_program(CMAKE_C_COMPILER NAMES ${TOOLCHAIN_PREFIX}-gcc
	HINTS ENV PATH
	PATHS "${_toolchain_bin}"
)
find_program(CMAKE_CXX_COMPILER NAMES ${TOOLCHAIN_PREFIX}-g++
	HINTS ENV PATH
	PATHS "${_toolchain_bin}"
)
find_program(CMAKE_ASM_COMPILER NAMES ${TOOLCHAIN_PREFIX}-gcc
	HINTS ENV PATH
	PATHS "${_toolchain_bin}"
)
find_program(SIZE_TOOL NAMES ${TOOLCHAIN_PREFIX}-size
	HINTS ENV PATH
	PATHS "${_toolchain_bin}"
)

if(NOT CMAKE_C_COMPILER OR NOT CMAKE_C_COMPILER MATCHES "riscv-wch-elf")
	message(FATAL_ERROR "${TOOLCHAIN_PREFIX}-gcc not found; install toolchain or set TOOLCHAIN_PREFIX and PATH (current='${TOOLCHAIN_PREFIX}')")
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
