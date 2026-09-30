#!/usr/bin/env bash
#
# Использование:
#   ./run.sh              # собрать Release, запустить только сервер
#   ./run.sh server       # только сервер
#   ./run.sh client       # только клиент
#   ./run.sh both         # сервер + один клиент
#   ./run.sh server 3     # сервер + 3 клиента
#
set -euo pipefail

MODE="${1:-server}"
CLIENT_COUNT="${2:-1}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

"${SCRIPT_DIR}/build.sh" Release >/dev/null

find_bin() {
    local name="$1"
    local bin
    bin="$(find "${BUILD_DIR}" -maxdepth 3 -type f -executable -name "${name}" -print -quit 2>/dev/null || true)"
    if [[ -z "${bin}" ]]; then
        echo "Error: binary '${name}' not found in ${BUILD_DIR}" >&2
        echo "Check that the CMake target is named '${name}'." >&2
        exit 1
    fi
    echo "${bin}"
}

SERVER_BIN="$(find_bin server)"
CLIENT_BIN=""
if [[ "${MODE}" == "client" || "${MODE}" == "both" || "${MODE}" =~ ^[0-9]+$ ]]; then
    CLIENT_BIN="$(find_bin client)"
fi

declare -a PIDS=()

cleanup() {
    echo
    echo "==> Stopping..."
    for pid in "${PIDS[@]}"; do
        kill "${pid}" 2>/dev/null || true
    done
    wait 2>/dev/null || true
    echo "Done."
}
trap cleanup EXIT INT TERM

start_client() {
    local n="$1"
    stdbuf -oL -eL "${CLIENT_BIN}" 2>&1 \
        | sed "s/^/[client-${n}] /" &
    PIDS+=("$!")
    echo "==> Started client-${n} (pid $!)"
}

case "${MODE}" in
    server)
        echo "==> Running server: ${SERVER_BIN}"
        echo "    Ctrl+C to stop."
        "${SERVER_BIN}"
        ;;
    client)
        echo "==> Running client: ${CLIENT_BIN}"
        "${CLIENT_BIN}"
        ;;
    both)
        echo "==> Running server in background: ${SERVER_BIN}"
        "${SERVER_BIN}" &
        PIDS+=("$!")
        sleep 1
        for ((i = 1; i <= CLIENT_COUNT; ++i)); do
            start_client "${i}"
        done
        echo "==> Server and ${CLIENT_COUNT} client(s) started. Ctrl+C to stop."
        wait
        ;;
    *)
        if [[ "${MODE}" =~ ^[0-9]+$ ]]; then
            N="${MODE}"
            echo "==> Running server in background: ${SERVER_BIN}"
            "${SERVER_BIN}" &
            PIDS+=("$!")
            sleep 1
            for ((i = 1; i <= N; ++i)); do
                start_client "${i}"
            done
            echo "==> Server and ${N} client(s) started. Ctrl+C to stop."
            wait
        else
            echo "Unknown mode: ${MODE}" >&2
            echo "Usage: $0 [server|client|both|<N>] [client_count]" >&2
            exit 1
        fi
        ;;
esac