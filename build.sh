#!/usr/bin/env bash
set -euo pipefail
#
# Использование:
#   ./build.sh              # Release-сборка (по умолчанию)
#   ./build.sh Debug        # Debug-сборка
#   ./build.sh Release -j8  # с явным числом параллельных задач
#
BUILD_TYPE="${1:-Release}"
JOBS="${2:--j$(nproc 2>/dev/null || echo 4)}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

if ! command -v cmake >/dev/null 2>&1; then
    echo "Error: cmake not found in PATH" >&2
    echo "Install it with: sudo apt install cmake" >&2
    exit 1
fi

if [[ ! -f "${SCRIPT_DIR}/CMakeLists.txt" ]]; then
    echo "Error: CMakeLists.txt not found in ${SCRIPT_DIR}" >&2
    exit 1
fi

echo "==> Configuring (${BUILD_TYPE})"
cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" \
      -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"

echo "==> Building (${JOBS})"
cmake --build "${BUILD_DIR}" ${JOBS}

echo
echo "==> Build complete. Artifacts in: ${BUILD_DIR}"
echo

shopt -s nullglob
found=0
for target in server client; do
    while IFS= read -r -d '' bin; do
        echo "    ${bin}"
        found=1
    done < <(find "${BUILD_DIR}" -maxdepth 3 -type f -executable -name "${target}" -print0 2>/dev/null)
done
shopt -u nullglob

if [[ "${found}" -eq 0 ]]; then
    echo "    (no server/client binaries found — check CMake target names)"
fi