#!/usr/bin/env bash

set -euo pipefail

# --- Аргументы и значения по умолчанию --------------------------------------

if [[ $# -lt 1 || $# -gt 2 ]]; then
    echo "Usage: $0 <count> [client_binary]" >&2
    echo "  count          — количество клиентов (>= 1)" >&2
    echo "  client_binary  — путь к исполняемому файлу (по умолчанию ./build/client)" >&2
    exit 1
fi

readonly COUNT="$1"
readonly CLIENT_BIN="${2:-${CLIENT_BIN:-./build/client}}"
readonly LOG_DIR="${LOG_DIR:-./client-logs}"

# --- Проверки ---------------------------------------------------------------

if ! [[ "$COUNT" =~ ^[0-9]+$ ]] || (( COUNT < 1 )); then
    echo "Error: count must be a positive integer, got '$COUNT'" >&2
    exit 1
fi

if [[ ! -x "$CLIENT_BIN" ]]; then
    echo "Error: client binary not found or not executable: $CLIENT_BIN" >&2
    echo "Build the project first, e.g.:" >&2
    echo "  cmake -S . -B build && cmake --build build -j" >&2
    exit 1
fi

mkdir -p "$LOG_DIR"

# --- Запуск -----------------------------------------------------------------

declare -a PIDS=()

# Убираем за собой: при Ctrl+C и при выходе скрипта глушим всех клиентов.
cleanup() {
    echo
    echo "Stopping ${#PIDS[@]} client(s)..."
    for pid in "${PIDS[@]}"; do
        if kill -0 "$pid" 2>/dev/null; then
            kill "$pid" 2>/dev/null || true
        fi
    done
    wait 2>/dev/null || true
    echo "All clients stopped. Logs in: $LOG_DIR"
}
trap cleanup EXIT INT TERM

echo "Starting $COUNT client(s) from: $CLIENT_BIN"
echo "Logs: $LOG_DIR"
echo

for (( i = 1; i <= COUNT; i++ )); do
    log_file="$LOG_DIR/client-$i.log"

    # stdbuf — чтобы qInfo() из Qt не буферизовал вывод при перенаправлении
    # в файл. Без этого строки появляются в логе с задержкой.
    stdbuf -oL -eL "$CLIENT_BIN" > >(sed "s/^/[client-$i] /" | tee "$log_file") 2>&1 &

    PIDS+=("$!")
    echo "  client-$i -> PID $! -> $log_file"
done

echo
echo "Press Ctrl+C to stop all clients."

# Ждём завершения всех клиентов. Если один упадёт — ждать остальных всё равно
# полезно, поэтому не используем `wait -n`.
wait